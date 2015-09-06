#pragma once

#include <functional>
#include <list>
#include <map>
#include <memory>

#include "base/not_null.hpp"
#include "geometry/grassmann.hpp"
#include "geometry/named_quantities.hpp"
#include "physics/degrees_of_freedom.hpp"
#include "physics/forkable.hpp"
#include "quantities/named_quantities.hpp"
#include "serialization/physics.pb.h"

namespace principia {

using base::not_null;
using geometry::Instant;
using geometry::Vector;
using geometry::Velocity;
using quantities::Acceleration;
using quantities::Length;
using quantities::Speed;

namespace physics {

template<typename Frame>
class DiscreteTrajectory;

template<typename Frame>
struct ForkableTraits<DiscreteTrajectory<Frame>> {
  using TimelineConstIterator =
      typename std::map<Instant, DegreesOfFreedom<Frame>>::const_iterator;
  static Instant const& time(TimelineConstIterator const it);
};

template<typename Frame>
class DiscreteTrajectory : public Forkable<DiscreteTrajectory<Frame>> {
  using Timeline = std::map<Instant, DegreesOfFreedom<Frame>>;

 public:
  class NativeIterator;
  template<typename ToFrame>
  class TransformingIterator;

  // A function that transforms the coordinates to a different frame.
  template<typename ToFrame>
  using Transform = std::function<DegreesOfFreedom<ToFrame>(
                        Instant const&,
                        DegreesOfFreedom<Frame> const&,
                        not_null<DiscreteTrajectory<Frame> const*> const)>;

  DiscreteTrajectory() = default;
  ~DiscreteTrajectory() override;

  DiscreteTrajectory(DiscreteTrajectory const&) = delete;
  DiscreteTrajectory(DiscreteTrajectory&&) = delete;
  DiscreteTrajectory& operator=(DiscreteTrajectory const&) = delete;
  DiscreteTrajectory& operator=(DiscreteTrajectory&&) = delete;

  // Sets a callback to be run before this trajectory gets destroyed.
  void set_on_destroy(
      std::function<void(not_null<DiscreteTrajectory<Frame>const*> const)>
          on_destroy);

  // TODO(phl): Many/most of the iterator functions are obsolete.  Remove them
  // and use the ones from Forkable.

  // Returns an iterator at the first point of the trajectory.  Complexity is
  // O(|depth|).  The result may be at end if the trajectory is empty.
  NativeIterator first() const;

  // Returns at the first point of the trajectory which is on or after |time|.
  // Complexity is O(|depth| + Ln(|length|)).  The result may be at end if the
  // |time| is after the end of the trajectory.
  NativeIterator on_or_after(Instant const& time) const;

  // Returns an iterator at the last point of the trajectory.  Complexity is
  // O(1).  The trajectory must not be empty.
  NativeIterator last() const;

  // Same as |first| above, but returns an iterator that performs a coordinate
  // tranformation to ToFrame.
  template<typename ToFrame>
  TransformingIterator<ToFrame> first_with_transform(
      Transform<ToFrame> const& transform) const;

  // Returns at the first point of the trajectory which is on or after |time|.
  // Complexity is O(|depth| + Ln(|length|)).  The result may be at end if the
  // |time| is after the end of the trajectory.
  template<typename ToFrame>
  TransformingIterator<ToFrame> on_or_after_with_transform(
      Instant const& time,
      Transform<ToFrame> const& transform) const;

  // Same as |last| above, but returns an iterator that performs a coordinate
  // tranformation to ToFrame.
  template<typename ToFrame>
  TransformingIterator<ToFrame> last_with_transform(
      Transform<ToFrame> const& transform) const;

  // These functions return the series of positions/velocities/times for the
  // trajectory.  All three containers are guaranteed to have the same size.
  // These functions are O(|depth| + |length|).
  std::map<Instant, Position<Frame>> Positions() const;
  std::map<Instant, Velocity<Frame>> Velocities() const;
  std::list<Instant> Times() const;

  // Creates a new child trajectory forked at time |time|, and returns it.  The
  // child trajectory shares its data with the current trajectory for times less
  // than or equal to |time|, and is an exact copy of the current trajectory for
  // times greater than |time|.  It may be changed independently from the
  // parent trajectory for any time (strictly) greater than |time|.  The child
  // trajectory is owned by its parent trajectory.  Deleting the parent
  // trajectory deletes all child trajectories.  |time| must be one of the times
  // of this trajectory, and must be at or after the fork time, if any.
  not_null<DiscreteTrajectory<Frame>*> NewForkWithCopy(Instant const& time);

  // Appends one point to the trajectory.
  void Append(Instant const& time,
              DegreesOfFreedom<Frame> const& degrees_of_freedom);

  // Removes all data for times (strictly) greater than |time|, as well as all
  // child trajectories forked at times (strictly) greater than |time|.  |time|
  // must be at or after the fork time, if any.
  void ForgetAfter(Instant const& time);

  // Removes all data for times less than or equal to |time|, as well as all
  // child trajectories forked at times less than or equal to |time|.  This
  // trajectory must be a root.
  void ForgetBefore(Instant const& time);

  // This function represents the intrinsic acceleration of a body, irrespective
  // of any external field.  It can be due e.g., to an engine burn.
  using IntrinsicAcceleration =
      std::function<Vector<Acceleration, Frame>(Instant const& time)>;

  // Sets the intrinsic acceleration for the trajectory of a massless body.
  // For a nonroot trajectory the intrinsic acceleration only applies to times
  // (strictly) greater than |fork_time()|.  In other words, the function
  // |acceleration| is never called with times less than or equal to
  // |fork_time()|.  It may, however, be called with times beyond |last_time()|.
  // For a root trajectory the intrinsic acceleration applies to times greater
  // than or equal to the first time of the trajectory.  Again, it may apply
  // beyond |last_time()|.
  // It is an error to call this function for a trajectory that already has an
  // intrinsic acceleration, or for the trajectory of a massive body.
  void set_intrinsic_acceleration(IntrinsicAcceleration const acceleration);

  // Removes any intrinsic acceleration for the trajectory.
  void clear_intrinsic_acceleration();

  // Returns true if this trajectory has an intrinsic acceleration.
  bool has_intrinsic_acceleration() const;

  // Computes the intrinsic acceleration for this trajectory at time |time|.  If
  // |has_intrinsic_acceleration()| return false, or if |time| is before the
  // |fork_time()| (or initial time) of this trajectory, the returned
  // acceleration is zero.
  Vector<Acceleration, Frame> evaluate_intrinsic_acceleration(
      Instant const& time) const;

  // This trajectory must be a root.  The intrinsic acceleration is not
  // serialized.
  void WriteToMessage(not_null<serialization::Trajectory*> const message) const;

  // NOTE(egg): This should return a |not_null|, but we can't do that until
  // |not_null<std::unique_ptr<T>>| is convertible to |std::unique_ptr<T>|, and
  // that requires a VS 2015 feature (rvalue references for |*this|).
  static std::unique_ptr<DiscreteTrajectory> ReadFromMessage(
      serialization::Trajectory const& message);

  // An iterator which returns the coordinates in the native frame of the
  // trajectory, i.e., |Frame|.
  class NativeIterator : public Iterator {
   public:
    bool at_end() const;
    Instant const& time() const;
    DegreesOfFreedom<Frame> const& degrees_of_freedom() const;

   private:
    explicit NativeIterator(Iterator it);
    friend class DiscreteTrajectory;
  };

  // An iterator which returns the coordinates in another frame, |ToFrame|.
  template<typename ToFrame>
  class TransformingIterator : public Iterator {
   public:
    bool at_end() const;
    Instant const& time() const;
    DegreesOfFreedom<ToFrame> degrees_of_freedom() const;

   private:
    TransformingIterator(Iterator it, Transform<ToFrame> transform);
    Transform<ToFrame> transform_;
    friend class DiscreteTrajectory;
  };

 protected:
  // The API inherited from Forkable.
  not_null<DiscreteTrajectory*> that() override;
  not_null<DiscreteTrajectory const*> that() const override;

  TimelineConstIterator timeline_begin() const override;
  TimelineConstIterator timeline_end() const override;
  TimelineConstIterator timeline_find(Instant const& time) const override;
  TimelineConstIterator timeline_lower_bound(
                            Instant const& time) const override;
  TimelineConstIterator timeline_upper_bound(
                            Instant const& time) const override;
  bool timeline_empty() const override;

 private:
  // This trajectory need not be a root.
  void WriteSubTreeToMessage(
      not_null<serialization::Trajectory*> const message) const;

  void FillSubTreeFromMessage(serialization::Trajectory const& message);

  Timeline timeline_;

  std::unique_ptr<IntrinsicAcceleration> intrinsic_acceleration_;

  std::function<void(not_null<DiscreteTrajectory<Frame>const *> const)>
      on_destroy_;

  template<typename Tr4jectory>
  friend class Forkable;

  // For using the private constructor in maps.
  template<typename, typename>
  friend struct std::pair;
};

}  // namespace physics
}  // namespace principia

#include "discrete_trajectory_body.hpp"