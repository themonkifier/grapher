#include "functions.hh"

unordered_set<char> numbers = {'.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

unordered_map<string, complex> constants = {
    { "pi", std::numbers::pi_v<double> }, { "e", std::numbers::e_v<double> },
    { "c", 299792548 }, { "h", 6.62607015e-34}, { "hbar", 1.05457182e-34},
    {"N_A", 6.02214076e+23}, { "R", 8.31446262e+00}, { "k", 1.38064900e-23}
};

unordered_map<string, std::function<complex(const complex &)>> functions = {
    {"sin", [](const complex& c){ return std::sin(c); } },
    {"cos", [](const complex& c){ return std::cos(c); } },
    {"tan", [](const complex& c){ return std::tan(c); } },
    {"csc", [](const complex& c){ return 1.0 / std::sin(c); } },
    {"sec", [](const complex& c){ return 1.0 / std::cos(c); } },
    {"cot", [](const complex& c){ return 1.0 / std::tan(c); } },

    {"asin", [](const complex& c){ return std::asin(c); } },
    {"acos", [](const complex& c){ return std::acos(c); } },
    {"atan", [](const complex& c){ return std::atan(c); } },
    {"acsc", [](const complex& c){ return std::sin(1.0 / c); } },
    {"asec", [](const complex& c){ return std::cos(1.0 / c); } },
    {"acot", [](const complex& c){ return std::tan(1.0 / c); } },

    {"arcsin", [](const complex& c){ return std::asin(c); } },
    {"arccos", [](const complex& c){ return std::acos(c); } },
    {"arctan", [](const complex& c){ return std::atan(c); } },
    {"arccsc", [](const complex& c){ return std::sin(1.0 / c); } },
    {"arcsec", [](const complex& c){ return std::cos(1.0 / c); } },
    {"arccot", [](const complex& c){ return std::tan(1.0 / c); } },

    {"sinh", [](const complex& c){ return std::sinh(c); } },
    {"cosh", [](const complex& c){ return std::cosh(c); } },
    {"tanh", [](const complex& c){ return std::tanh(c); } },
    {"csch", [](const complex& c){ return 1.0 / std::sinh(c); } },
    {"sech", [](const complex& c){ return 1.0 / std::cosh(c); } },
    {"coth", [](const complex& c){ return 1.0 / std::tanh(c); } },

    {"asinh", [](const complex& c){ return std::asinh(c); } },
    {"acosh", [](const complex& c){ return std::acosh(c); } },
    {"atanh", [](const complex& c){ return std::atanh(c); } },
    {"acsch", [](const complex& c){ return std::asin(1.0 / c); } },
    {"asech", [](const complex& c){ return std::acos(1.0 / c); } },
    {"acoth", [](const complex& c){ return std::atan(1.0 / c); } },

    {"arcsinh", [](const complex& c){ return std::asinh(c); } },
    {"arccosh", [](const complex& c){ return std::acosh(c); } },
    {"arctanh", [](const complex& c){ return std::atanh(c); } },
    {"arccsch", [](const complex& c){ return std::asin(1.0 / c); } },
    {"arcsech", [](const complex& c){ return std::acos(1.0 / c); } },
    {"arccoth", [](const complex& c){ return std::atan(1.0 / c); } },

    {"arsinh", [](const complex& c){ return std::asinh(c); } },
    {"arcosh", [](const complex& c){ return std::acosh(c); } },
    {"artanh", [](const complex& c){ return std::atanh(c); } },
    {"arcsch", [](const complex& c){ return std::asin(1.0 / c); } },
    {"arsech", [](const complex& c){ return std::acos(1.0 / c); } },
    {"arcoth", [](const complex& c){ return std::atan(1.0 / c); } },

    {"exp", [](const complex& c){ return std::exp(c); } },
    {"sqrt", [](const complex& c){ return std::sqrt(c); } },
    {"cbrt", [](const complex& c){ return std::pow(c, 1.0 / 3.0); } },
    {"log", [](const complex& c){ return std::log10(c); } },
    {"ln", [](const complex& c){ return std::log(c); } } };
