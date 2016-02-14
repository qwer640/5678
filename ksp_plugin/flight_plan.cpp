﻿
#include "ksp_plugin/flight_plan.hpp"

#include "testing_utilities/make_not_null.hpp"

namespace principia {
namespace ksp_plugin {

FlightPlan::FlightPlan(
    not_null<DiscreteTrajectory<Barycentric>*> const root,
    Instant const& initial_time,
    Instant const& final_time,
    Mass const& initial_mass,
    not_null<Ephemeris<Barycentric>*> const ephemeris,
    AdaptiveStepSizeIntegrator<
        Ephemeris<Barycentric>::NewtonianMotionEquation> const& integrator,
    Length const& length_integration_tolerance,
    Speed const& speed_integration_tolerance)
    : initial_time_(initial_time),
      final_time_(final_time),
      initial_mass_(initial_mass),
      ephemeris_(ephemeris),
      integrator_(integrator),
      length_integration_tolerance_(length_integration_tolerance),
      speed_integration_tolerance_(speed_integration_tolerance) {
  CHECK(final_time_ >= initial_time_);
  auto it = root->LowerBound(initial_time_);
  if (it.time() != initial_time_) {
    --it;
    initial_time_ = it.time();
  }

  // Create a fork for the first coasting trajectory.  We set a callback to be
  // notified when it goes away.  This is not necessary for correctness, but it
  // helps to detect bugs
  segments_.emplace_back(root->NewForkWithCopy(it.time()));
  segments_.front()->set_on_destroy(
      [this](not_null<DiscreteTrajectory<Barycentric> const*> trajectory) {
        LOG(FATAL) << "Destroying the first segment of flight plan " << this;
      });

  ResetLastSegment();  // TODO(phl): A NewForkWithoutCopy would be nicer.
  CoastLastSegment(final_time_);
}

FlightPlan::~FlightPlan() {
  // |segments_| is empty for a mock object.
  if (!segments_.empty()) {
    // Deleting the first fork deletes everything.
    DiscreteTrajectory<Barycentric>* trajectory = segments_.front();
    CHECK(!trajectory->is_root());
    trajectory->set_on_destroy(nullptr);
    trajectory->parent()->DeleteFork(&trajectory);
  }
}

Instant FlightPlan::initial_time() const {
  return initial_time_;
}

Instant FlightPlan::final_time() const {
  return final_time_;
}

int FlightPlan::number_of_manœuvres() const {
  return manœuvres_.size();
}

NavigationManœuvre const& FlightPlan::GetManœuvre(int const index) const {
  CHECK_LE(0, index);
  CHECK_LT(index, number_of_manœuvres());
  return manœuvres_[index];
}

bool FlightPlan::Append(Burn burn) {
  auto manœuvre =
      MakeNavigationManœuvre(
          std::move(burn),
          manœuvres_.empty() ? initial_mass_ : manœuvres_.back().final_mass());
  if (manœuvre.FitsBetween(start_of_last_coast(), final_time_)) {
    Append(std::move(manœuvre));
    return true;
  } else {
    return false;
  }
}

void FlightPlan::RemoveLast() {
  CHECK(!manœuvres_.empty());
  manœuvres_.pop_back();
  PopLastSegment();  // Last coast.
  PopLastSegment();  // Last burn.
  ResetLastSegment();
  CoastLastSegment(final_time_);
}

bool FlightPlan::ReplaceLast(Burn burn) {
  CHECK(!manœuvres_.empty());
  auto manœuvre =
      MakeNavigationManœuvre(std::move(burn), manœuvres_.back().initial_mass());
  if (manœuvre.FitsBetween(start_of_penultimate_coast(), final_time_)) {
    manœuvres_.pop_back();
    PopLastSegment();  // Last coast.
    PopLastSegment();  // Last burn.
    Append(std::move(manœuvre));
    return true;
  } else {
    return false;
  }
}

bool FlightPlan::SetFinalTime(Instant const& final_time) {
  if (start_of_last_coast() > final_time) {
    return false;
  } else {
    final_time_ = final_time;
    ResetLastSegment();
    CoastLastSegment(final_time_);
    return true;
  }
}

void FlightPlan::SetTolerances(
    Length const& length_integration_tolerance,
    Speed const& speed_integration_tolerance) {
  length_integration_tolerance_ = length_integration_tolerance;
  speed_integration_tolerance_ = speed_integration_tolerance;
  RecomputeSegments();
}

int FlightPlan::number_of_segments() const {
  return segments_.size();
}

void FlightPlan::GetSegment(
    int const index,
    not_null<DiscreteTrajectory<Barycentric>::Iterator*> begin,
    not_null<DiscreteTrajectory<Barycentric>::Iterator*> end) const {
  CHECK_LE(0, index);
  CHECK_LT(index, number_of_segments());
  *begin = segments_[index]->Fork();
  *end = segments_[index]->End();
}

void FlightPlan::WriteToMessage(
    not_null<serialization::FlightPlan*> const message) const {
  initial_mass_.WriteToMessage(message->mutable_initial_mass());
  initial_time_.WriteToMessage(message->mutable_initial_time());
  final_time_.WriteToMessage(message->mutable_final_time());
  length_integration_tolerance_.WriteToMessage(
      message->mutable_length_integration_tolerance());
  speed_integration_tolerance_.WriteToMessage(
      message->mutable_speed_integration_tolerance());
  integrator_.WriteToMessage(message->mutable_integrator());
  for (auto const& segment : segments_) {
    segment->WritePointerToMessage(message->add_segment());
  }
  for (auto const& manœuvre : manœuvres_) {
    manœuvre.WriteToMessage(message->add_manoeuvre());
  }
}

std::unique_ptr<FlightPlan> FlightPlan::ReadFromMessage(
    serialization::FlightPlan const& message,
    not_null<DiscreteTrajectory<Barycentric>*> const root,
    not_null<Ephemeris<Barycentric>*> const ephemeris) {
  auto flight_plan = std::make_unique<FlightPlan>(
      root,
      Instant::ReadFromMessage(message.initial_time()),
      Instant::ReadFromMessage(message.final_time()),
      Mass::ReadFromMessage(message.initial_mass()),
      ephemeris,
      AdaptiveStepSizeIntegrator<
          Ephemeris<Barycentric>::NewtonianMotionEquation>::
          ReadFromMessage(message.integrator()),
      Length::ReadFromMessage(message.length_integration_tolerance()),
      Speed::ReadFromMessage(message.speed_integration_tolerance()));
  // The constructor has forked a segment.  Remove it.
  flight_plan->segments_.front()->set_on_destroy(nullptr);
  flight_plan->PopLastSegment();
  for (auto const& segment : message.segment()) {
    flight_plan->segments_.emplace_back(
        DiscreteTrajectory<Barycentric>::ReadPointerFromMessage(segment, root));
  }
  for (int i = 0; i < message.manoeuvre_size(); ++i) {
    auto const& manoeuvre = message.manoeuvre(i);
    flight_plan->manœuvres_.push_back(
        NavigationManœuvre::ReadFromMessage(manoeuvre, ephemeris));
    flight_plan->manœuvres_[i].set_coasting_trajectory(
        flight_plan->segments_[2 * i]);
  }
  return std::move(flight_plan);
}

FlightPlan::FlightPlan()
    : ephemeris_(testing_utilities::make_not_null<Ephemeris<Barycentric>*>()),
      integrator_(
          *testing_utilities::make_not_null<
              AdaptiveStepSizeIntegrator<
                  Ephemeris<Barycentric>::NewtonianMotionEquation>*>()) {}

void FlightPlan::Append(NavigationManœuvre manœuvre) {
  manœuvres_.emplace_back(std::move(manœuvre));
  {
    // Hide the moved-from |manœuvre|.
    NavigationManœuvre& manœuvre = manœuvres_.back();
    ResetLastSegment();
    CoastLastSegment(manœuvre.initial_time());
    manœuvre.set_coasting_trajectory(segments_.back());
    AddSegment();
    BurnLastSegment(manœuvre);
    AddSegment();
    CoastLastSegment(final_time_);
  }
}

void FlightPlan::RecomputeSegments() {
  // It is important that the segments be destroyed in (reverse chronological)
  // order of the forks.
  while (segments_.size() > 1) {
    PopLastSegment();
  }
  ResetLastSegment();
  for (auto& manœuvre : manœuvres_) {
    CoastLastSegment(manœuvre.initial_time());
    manœuvre.set_coasting_trajectory(segments_.back());
    AddSegment();
    BurnLastSegment(manœuvre);
    AddSegment();
  }
  CoastLastSegment(final_time_);
}

void FlightPlan::BurnLastSegment(NavigationManœuvre const& manœuvre) {
  if (manœuvre.initial_time() < manœuvre.final_time()) {
    ephemeris_->FlowWithAdaptiveStep(segments_.back(),
                                     manœuvre.IntrinsicAcceleration(),
                                     length_integration_tolerance_,
                                     speed_integration_tolerance_,
                                     integrator_,
                                     manœuvre.final_time());
  }
}

void FlightPlan::CoastLastSegment(Instant const& final_time) {
  ephemeris_->FlowWithAdaptiveStep(
      segments_.back(),
      Ephemeris<Barycentric>::kNoIntrinsicAcceleration,
      length_integration_tolerance_,
      speed_integration_tolerance_,
      integrator_,
      final_time);
}

void FlightPlan::AddSegment() {
  segments_.emplace_back(segments_.back()->NewForkAtLast());
}

void FlightPlan::ResetLastSegment() {
  segments_.back()->ForgetAfter(segments_.back()->Fork().time());
}

void FlightPlan::PopLastSegment() {
  DiscreteTrajectory<Barycentric>* trajectory = segments_.back();
  CHECK(!trajectory->is_root());
  trajectory->parent()->DeleteFork(&trajectory);
  segments_.pop_back();
}

Instant FlightPlan::start_of_last_coast() const {
  return manœuvres_.empty() ? initial_time_ : manœuvres_.back().final_time();
}

Instant FlightPlan::start_of_penultimate_coast() const {
  return manœuvres_.size() == 1
             ? initial_time_
             : manœuvres_[manœuvres_.size() - 2].final_time();
}

}  // namespace ksp_plugin
}  // namespace principia