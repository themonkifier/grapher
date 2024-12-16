#ifndef NUMERICAL_H
#define NUMERICAL_H

#include <string>
#include <bitset>
#include <vector>
#include <complex>
#include <variant>

using integer = long long int;
using complex = std::complex<long double>;
using std::vector;

class Matrix;
using Variable = std::variant<complex, Matrix>;

complex operator%=(complex& lhs, const complex& rhs);
complex operator%(complex a, complex b);
std::string to_string(const complex& num);

class Numerical {
public:
    Numerical(integer num);

    vector<integer> distinct_prime_factors(integer num);
    vector<integer> prime_factors(integer num);

private:
    vector<integer> primes;
};  // class Numerical

#endif  // NUMERICAL_H
