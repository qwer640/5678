#include "physics/continuous_trajectory.hpp"

#include <functional>

#include "geometry/frame.hpp"
#include "geometry/named_quantities.hpp"
#include "gtest/gtest.h"
#include "physics/degrees_of_freedom.hpp"
#include "quantities/quantities.hpp"
#include "quantities/si.hpp"
#include "serialization/geometry.pb.h"
#include "testing_utilities/almost_equals.hpp"

namespace principia {

using geometry::Displacement;
using geometry::Frame;
using geometry::Velocity;
using quantities::Time;
using si::Metre;
using si::Second;
using testing_utilities::AlmostEquals;

namespace physics {

class ContinuousTrajectoryTest : public testing::Test {
 protected:
  static Time const step_;

  using World = Frame<serialization::Frame::TestTag,
                      serialization::Frame::TEST1, true>;

  ContinuousTrajectoryTest()
    : trajectory_(0.01 * Second,
                  0.05 * Metre /*low_tolerance*/,
                  0.1 * Metre /*high_tolerance*/) {}

  void FillTrajectory(
      int const number_of_steps,
      std::function<Position<World>(Instant const)> position_function,
      std::function<Velocity<World>(Instant const)> velocity_function) {
    Instant time;
    for (int i = 0; i < number_of_steps; ++i) {
      time += step_;
      trajectory_.Append(time,
                         DegreesOfFreedom<World>(position_function(time),
                                                 velocity_function(time)));
    }
  }

  ContinuousTrajectory<World> trajectory_;
};

Time const ContinuousTrajectoryTest::step_ = 0.01 * Second;

TEST_F(ContinuousTrajectoryTest, Test) {
  int const kNumberOfSteps = 20;
  int const kNumberOfSubsteps = 50;
  Instant t0;

  auto position_function =
      [t0](Instant const t) {
        return World::origin +
            Displacement<World>({(t - t0) * 3 * Metre / Second,
                                (t - t0) * 5 * Metre / Second,
                                (t - t0) * (-2) * Metre / Second});
      };
  auto velocity_function =
      [t0](Instant const time) {
        return Velocity<World>({3 * Metre / Second,
                                5 * Metre / Second,
                                -2 * Metre / Second});
      };

  EXPECT_TRUE(trajectory_.empty());
  FillTrajectory(kNumberOfSteps, position_function, velocity_function);
  EXPECT_FALSE(trajectory_.empty());
  EXPECT_EQ(t0 + step_, trajectory_.t_min());
  EXPECT_EQ(t0 + ((kNumberOfSteps / 8) * 8 + 1) * step_, trajectory_.t_max());
  
  ContinuousTrajectory<World>::Hint hint;
  for (Instant time = trajectory_.t_min();
       time <= trajectory_.t_max();
       time += step_ / kNumberOfSubsteps) {
    EXPECT_THAT(trajectory_.EvaluatePosition(time, &hint) - World::origin,
                AlmostEquals(position_function(time) - World::origin, 0, 7));
    EXPECT_THAT(trajectory_.EvaluateVelocity(time, &hint),
                AlmostEquals(velocity_function(time), 1, 3));
  }
}

}  // namespace physics
}  // namespace principia
