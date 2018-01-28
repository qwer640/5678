
#pragma once

#include <tuple>
#include <utility>

#include "base/not_null.hpp"
#include "geometry/point.hpp"
#include "quantities/named_quantities.hpp"
#include "serialization/numerics.pb.h"

namespace principia {
namespace numerics {
namespace internal_polynomial {

using base::not_null;
using geometry::Point;
using quantities::Derivative;

// TODO(phl): We would like to define NthDerivative in named_quantities.hpp
// thus:
//
//   template<typename Value, typename Argument, int order>
//   using NthDerivative = typename std::conditional_t<
//       order == 0,
//       Value,
//       Quotient<Difference<Value>,
//                Exponentiation<Difference<Argument>, order>>>;
//
//   template<typename Value, typename Argument>
//   using Derivative = NthDerivative<Value, Argument, 1>;
//
// Unfortunately VS2015 is buggy and this interacts poorly with the
// std::integer_sequence below (we get the wrong types).  Revisit once MSFT has
// fixed their bugs.

template<typename Value, typename Argument, int order>
struct NthDerivativeGenerator {
  using Type = Derivative<
      typename NthDerivativeGenerator<Value, Argument, order - 1>::Type,
      Argument>;
};
template<typename Value, typename Argument>
struct NthDerivativeGenerator<Value, Argument, 0> {
  using Type = Value;
};

template<typename Value, typename Argument, int order>
using NthDerivative =
    typename NthDerivativeGenerator<Value, Argument, order>::Type;

template<typename Value, typename Argument, typename>
struct NthDerivativesGenerator;
template<typename Value, typename Argument, int... orders>
struct NthDerivativesGenerator<Value,
                               Argument,
                               std::integer_sequence<int, orders...>> {
  using Type = std::tuple<NthDerivative<Value, Argument, orders>...>;
};

template<typename Value, typename Argument, typename Sequence>
using NthDerivatives =
    typename NthDerivativesGenerator<Value, Argument, Sequence>::Type;

// |Value| must belong to an affine space.  |Argument| must belong to a ring or
// to Point based on a ring.
// TODO(phl): We would like the base case to be the affine case (not limited to
// Point) and the specialized case to check for the existence of Sum and Product
// for Argument, and that works with Clang but not with VS2015.  Revisit once
// MSFT has fixed their bugs.
template<typename Value, typename Argument>
class Polynomial {
 public:
  virtual ~Polynomial() = default;

  virtual Value Evaluate(Argument const& argument) const = 0;
  virtual Derivative<Value, Argument> EvaluateDerivative(
      Argument const& argument) const = 0;

  // Only useful for benchmarking or analyzing performance.  Do not use in real
  // code.
  virtual int degree() const = 0;

  virtual void WriteToMessage(
      not_null<serialization::Polynomial*> message) const = 0;
};

template<typename Value, typename Argument, int degree_,
         template<typename, typename, int> class Evaluator>
class PolynomialInMonomialBasis : public Polynomial<Value, Argument> {
 public:
  // Equivalent to:
  //   std::tuple<Value,
  //              Derivative<Value, Argument>,
  //              Derivative<Derivative<Value, Argument>>...>
  using Coefficients =
      NthDerivatives<Value,
                     Argument,
                     std::make_integer_sequence<int, degree_ + 1>>;

  // The coefficients are applied to powers of argument.
  explicit PolynomialInMonomialBasis(Coefficients const& coefficients);

  FORCE_INLINE(inline) Value
  Evaluate(Argument const& argument) const override;
  FORCE_INLINE(inline) Derivative<Value, Argument>
  EvaluateDerivative(Argument const& argument) const override;

  constexpr int degree() const override;

  void WriteToMessage(
      not_null<serialization::Polynomial*> message) const override;
  static not_null<std::unique_ptr<Polynomial>> ReadFromMessage(
      serialization::Polynomial const& message);

 private:
  Coefficients coefficients_;
};

template<typename Value, typename Argument, int degree_,
         template<typename, typename, int> class Evaluator>
class PolynomialInMonomialBasis<Value, Point<Argument>, degree_, Evaluator>
    : public Polynomial<Value, Point<Argument>> {
 public:
  // Equivalent to:
  //   std::tuple<Value,
  //              Derivative<Value, Argument>,
  //              Derivative<Derivative<Value, Argument>>...>
  using Coefficients =
      NthDerivatives<Value,
                     Argument,
                     std::make_integer_sequence<int, degree_ + 1>>;

  // The coefficients are relative to origin; in other words they are applied to
  // powers of (argument - origin).
  PolynomialInMonomialBasis(Coefficients const& coefficients,
                            Point<Argument> const& origin);

  FORCE_INLINE(inline) Value
  Evaluate(Point<Argument> const& argument) const override;
  FORCE_INLINE(inline) Derivative<Value, Argument>
  EvaluateDerivative(Point<Argument> const& argument) const override;

  constexpr int degree() const override;

  void WriteToMessage(
      not_null<serialization::Polynomial*> message) const override;
  static not_null<std::unique_ptr<Polynomial>> ReadFromMessage(
      serialization::Polynomial const& message);

 private:
  Coefficients coefficients_;
  Point<Argument> origin_;
};

}  // namespace internal_polynomial

using internal_polynomial::NthDerivative;
using internal_polynomial::Polynomial;
using internal_polynomial::PolynomialInMonomialBasis;

}  // namespace numerics
}  // namespace principia

#include "numerics/polynomial_body.hpp"