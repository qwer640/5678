#pragma once

#include "geometry/grassmann.hpp"
#include "geometry/linear_map.hpp"
#include "geometry/orthogonal_map.hpp"
#include "geometry/r3_element.hpp"
#include "geometry/sign.hpp"

namespace principia {
namespace geometry {

template<typename FromFrame, typename ToFrame>
OrthogonalMap<FromFrame, ToFrame>::OrthogonalMap()
    : determinant_(Sign(1)),
      rotation_(Rotation<FromFrame, ToFrame>::Identity()) {}

template<typename FromFrame, typename ToFrame>
OrthogonalMap<FromFrame, ToFrame>::OrthogonalMap(
    Sign const& determinant,
    Rotation<FromFrame, ToFrame> const& rotation)
    : determinant_(determinant),
      rotation_(rotation) {}

template<typename FromFrame, typename ToFrame>
Sign OrthogonalMap<FromFrame, ToFrame>::Determinant() const {
  return determinant_;
}

template<typename FromFrame, typename ToFrame>
OrthogonalMap<ToFrame, FromFrame>
OrthogonalMap<FromFrame, ToFrame>::Inverse() const {
  return OrthogonalMap(determinant_, rotation_.Inverse());
}

template<typename FromFrame, typename ToFrame>
template<typename Scalar>
Vector<Scalar, ToFrame> OrthogonalMap<FromFrame, ToFrame>::operator()(
    Vector<Scalar, FromFrame> const& vector) const {
  return Vector<Scalar, ToFrame>(determinant_ * rotation_(vector));
}

template<typename FromFrame, typename ToFrame>
template<typename Scalar>
Bivector<Scalar, ToFrame> OrthogonalMap<FromFrame, ToFrame>::operator()(
    Bivector<Scalar, FromFrame> const& bivector) const {
  return Bivector<Scalar, ToFrame>(rotation_(bivector));
}

template<typename FromFrame, typename ToFrame>
template<typename Scalar>
Trivector<Scalar, ToFrame> OrthogonalMap<FromFrame, ToFrame>::operator()(
    Trivector<Scalar, FromFrame> const& trivector) const {
  return determinant_ * trivector;
}

template<typename FromFrame, typename ToFrame>
OrthogonalMap<FromFrame, ToFrame>
OrthogonalMap<FromFrame, ToFrame>::Identity() {
  return OrthogonalMap(Sign(1), Rotation<FromFrame, ToFrame>::Identity());
}

template<typename FromFrame, typename ThroughFrame, typename ToFrame>
OrthogonalMap<FromFrame, ToFrame> operator*(
    OrthogonalMap<ThroughFrame, ToFrame> const& left,
    OrthogonalMap<FromFrame, ThroughFrame> const& right) {
  return OrthogonalMap<FromFrame, ToFrame>(
             left.determinant_ * right.determinant_,
             left.rotation_ * right.rotation_);
}

}  // namespace geometry
}  // namespace principia