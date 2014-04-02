// Dimensionless.h

#pragma once

#include<string>

namespace Principia {
namespace Quantities {
// A double by any other name...
class Dimensionless {
public:
  Dimensionless(double value);
  double Value() const;
  Dimensionless Pow(int const) const;
  // This function calls Pow(Exponent), its purpose is to provide consistency
  // with Quantity<D>.Pow<Exponent>();
  template<int Exponent>
  Dimensionless Pow() const;
private:
  double value_;
};

Dimensionless operator+(Dimensionless const&);
Dimensionless operator-(Dimensionless const&);
Dimensionless operator+(Dimensionless const&, Dimensionless const&);
Dimensionless operator-(Dimensionless const&, Dimensionless const&);
Dimensionless operator*(Dimensionless const&, Dimensionless const&);
Dimensionless operator/(Dimensionless const&, Dimensionless const&);

void operator+=(Dimensionless&, Dimensionless const&);
void operator-=(Dimensionless&, Dimensionless const&);
void operator*=(Dimensionless&, Dimensionless const&);
void operator/=(Dimensionless&, Dimensionless const&);

bool operator>(Dimensionless const&, Dimensionless const&);
bool operator<(Dimensionless const&, Dimensionless const&);
bool operator>=(Dimensionless const&, Dimensionless const&);
bool operator<=(Dimensionless const&, Dimensionless const&);
bool operator==(Dimensionless const&, Dimensionless const&);
bool operator!=(Dimensionless const&, Dimensionless const&);

Dimensionless Abs(Dimensionless const&);

std::wstring ToString(Dimensionless const& number, 
                      unsigned char const precision = 16);
}
}

#include "Dimensionless-body.hpp"