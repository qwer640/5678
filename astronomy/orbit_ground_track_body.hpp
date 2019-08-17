﻿#pragma once

#include "astronomy/orbit_ground_track.hpp"

#include "physics/apsides.hpp"

namespace principia {
namespace astronomy {
namespace internal_orbit_ground_track {

using geometry::Vector;
using physics::ComputeNodes;

template<typename PrimaryCentred, typename Inertial>
OrbitGroundTrack OrbitGroundTrack::ForTrajectory(
    DiscreteTrajectory<PrimaryCentred> const& trajectory,
    RotatingBody<Inertial> const& primary,
    std::optional<OrbitRecurrence> const& nominal_recurrence) {
  DiscreteTrajectory<PrimaryCentred> ascending_nodes;
  DiscreteTrajectory<PrimaryCentred> descending_nodes;
  ComputeNodes(trajectory.Begin(),
               trajectory.End(),
               Vector<double, PrimaryCentred>({0, 0, 1}),
               /*max_points=*/std::numeric_limits<int>::max(),
               ascending_nodes,
               descending_nodes);
  OrbitGroundTrack ground_track;
  if (nominal_recurrence.has_value()) {
    int n = 0;
    Angle previous_reduced_longitude;
    for (auto ascending_node = ascending_nodes.Begin();
         ascending_node != ascending_nodes.End();
         ++ascending_node) {
      Angle const celestial_longitude =
          (ascending_node.degrees_of_freedom().position() -
           PrimaryCentred::origin)
              .coordinates()
              .ToSpherical()
              .longitude;
      Angle const planetocentric_longitude =
          celestial_longitude - primary.AngleAt(ascending_node.time()) -
          π / 2 * Radian;
      planetocentric_longitude - n * nominal_recurrence->equatorial_shift();
    }
  }
  return ground_track;
}

}  // namespace internal_orbit_ground_track
}  // namespace astronomy
}  // namespace principia
