#include "numerical.h"
#include <algorithm>

complex operator%=(complex& lhs, const complex& rhs) {
    complex x = lhs / rhs;
    x = complex(std::floor(x.real()), std::floor(x.imag()));
    complex z = x * rhs;
    return lhs -= z;
}

complex operator%(complex lhs, complex rhs) {
    lhs %= rhs;
    return lhs;
}

std::string to_string(const complex& num) {
    if (num.imag() == 0) {
        return std::to_string(num.real());
    } else if (num.real() == 0) {
        return (num.imag() < 0 ? "-" : "+") +
            std::to_string(std::abs(num.imag())) + "i";
    }
    return std::to_string(num.real()) +
        (num.imag() < 0 ? "-" : "+") +
        std::to_string(std::abs(num.imag())) + "i";
}

Numerical::Numerical(integer num) {
    vector<bool> is_prime(num, true);
    is_prime[0] = is_prime[1] = false;

    int sqrt_num = std::sqrt(num);

    for (int i = 2; i <= sqrt_num; i++) {
        if (is_prime[i]) {
            primes.push_back(i);
            for (int j = i * i; j <= sqrt_num; j += i) {
                is_prime[j] = false;
            }
        }
    }
}

vector<integer> Numerical::distinct_prime_factors(integer num) {
    vector<integer> factors = prime_factors(num);
    factors.erase(
        std::unique(
            factors.begin(),
            factors.end()
        ),
        factors.end()
    );

    return factors;
}

vector<integer> Numerical::prime_factors(integer num) {
    vector<integer> factors;
    for (integer prime : primes) {
        if (prime * prime > num)
            break;
        while (num % prime == 0) {
            factors.push_back(prime);
            num /= prime;
        }
    }
    if (num > 1) factors.push_back(num);

    return factors;
}

/*
int main(void) {
    int num;
    Numerical n{1000};
    while (std::cin >> num) {
        for (auto factor : n.prime_factors(num)) {
            std::cout << factor << " ";
        }
        std::cout << "\n" << std::endl;
    }

    std::cout << "done" << std::endl;
}
*/
