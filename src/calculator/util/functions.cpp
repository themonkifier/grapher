#include "functions.h"
#include "../matrix/matrix.h"

FuncInfo::FuncInfo(std::string name, fptr func)
    : name{name}, func{func}
{}

Variable FuncInfo::operator()(Variable v) const {
    if (v.index() == 0) {
        if (func.index() != 0) {
            throw errors::malformed_expression{};
        } else {
            complex num = std::get<complex>(v);
            complex_fptr f = std::get<complex_fptr>(func);
            return f(num);
        }
    } else {
        switch (func.index()) {
        case 0:
            throw errors::malformed_expression("can't call scalar function with matrix input");
        case 1: {
            Matrix mat = std::get<Matrix>(v);
            mat_to_complex_fptr f = std::get<mat_to_complex_fptr>(func);
            return f(&mat);
        } case 2: {
            Matrix mat = std::get<Matrix>(v);
            mat_to_mat_fptr f = std::get<mat_to_mat_fptr>(func);
            return f(&mat);
        } default:
            throw errors::malformed_expression{};
        }
    }
}

std::unordered_set<char> numbers = { '.', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

std::unordered_map<std::string, complex> constants = {
    {  "i", complex(0, 1) }, { "pi", std::numbers::pi_v<long double> },
    {  "e", std::numbers::e_v<long double> }, { "c", 299792548 },
    {  "h", 6.62607015e-34}, { "hbar", 1.05457182e-34},
    {"N_A", 6.02214076e+23}, { "R", 8.31446262e+00},
    {  "k", 1.38064900e-23}, { "e_", 1.60217663e-19} 
};

std::unordered_map<std::string, FuncInfo::complex_fptr> functions = {
    {"sin", [](const complex c){ return std::sin(c); } },
    {"cos", [](const complex c){ return std::cos(c); } },
    {"tan", [](const complex c){ return std::tan(c); } },
    {"csc", [](const complex c){ return 1.0l / std::sin(c); } },
    {"sec", [](const complex c){ return 1.0l / std::cos(c); } },
    {"cot", [](const complex c){ return 1.0l / std::tan(c); } },

    {"asin", [](const complex c){ return std::asin(c); } },
    {"acos", [](const complex c){ return std::acos(c); } },
    {"atan", [](const complex c){ return std::atan(c); } },
    {"acsc", [](const complex c){ return std::sin(1.0l / c); } },
    {"asec", [](const complex c){ return std::cos(1.0l / c); } },
    {"acot", [](const complex c){ return std::tan(1.0l / c); } },

    {"arcsin", [](const complex c){ return std::asin(c); } },
    {"arccos", [](const complex c){ return std::acos(c); } },
    {"arctan", [](const complex c){ return std::atan(c); } },
    {"arccsc", [](const complex c){ return std::sin(1.0l / c); } },
    {"arcsec", [](const complex c){ return std::cos(1.0l / c); } },
    {"arccot", [](const complex c){ return std::tan(1.0l / c); } },

    {"sinh", [](const complex c){ return std::sinh(c); } },
    {"cosh", [](const complex c){ return std::cosh(c); } },
    {"tanh", [](const complex c){ return std::tanh(c); } },
    {"csch", [](const complex c){ return 1.0l / std::sinh(c); } },
    {"sech", [](const complex c){ return 1.0l / std::cosh(c); } },
    {"coth", [](const complex c){ return 1.0l / std::tanh(c); } },

    {"asinh", [](const complex c){ return std::asinh(c); } },
    {"acosh", [](const complex c){ return std::acosh(c); } },
    {"atanh", [](const complex c){ return std::atanh(c); } },
    {"acsch", [](const complex c){ return std::asin(1.0l / c); } },
    {"asech", [](const complex c){ return std::acos(1.0l / c); } },
    {"acoth", [](const complex c){ return std::atan(1.0l / c); } },

    {"arcsinh", [](const complex c){ return std::asinh(c); } },
    {"arccosh", [](const complex c){ return std::acosh(c); } },
    {"arctanh", [](const complex c){ return std::atanh(c); } },
    {"arccsch", [](const complex c){ return std::asin(1.0l / c); } },
    {"arcsech", [](const complex c){ return std::acos(1.0l / c); } },
    {"arccoth", [](const complex c){ return std::atan(1.0l / c); } },

    {"arsinh", [](const complex c){ return std::asinh(c); } },
    {"arcosh", [](const complex c){ return std::acosh(c); } },
    {"artanh", [](const complex c){ return std::atanh(c); } },
    {"arcsch", [](const complex c){ return std::asin(1.0l / c); } },
    {"arsech", [](const complex c){ return std::acos(1.0l / c); } },
    {"arcoth", [](const complex c){ return std::atan(1.0l / c); } },

    {"exp", [](const complex c){ return std::exp(c); } },
    {"sqrt", [](const complex c){ return std::sqrt(c); } },
    {"cbrt", [](const complex c){ return std::pow(c, 1.0l / 3.0l); } },
    {"log", [](const complex c){ return std::log10(c); } },
    {"ln", [](const complex c){ return std::log(c); } }
};

std::unordered_map<std::string, FuncInfo::mat_to_complex_fptr> matrix_ops = {
    {"det", [](Matrix* m){ return m->determinant(); } },
    {"tr", [](Matrix* m){ return m->trace(); } },

    {"determinant", [](Matrix* m){ return m->determinant(); } },
    {"trace", [](Matrix* m){ return m->trace(); } },
};

std::unordered_map<std::string, FuncInfo::mat_to_mat_fptr> matrix_ops_ = {
    {"inv", [](Matrix* m){ return *m->inverse(); } },
    {"rref", [](Matrix* m){ return *m->rref(); } },
    //{"adj", [](Matrix* m){ return *m->adjoint(); } },

    {"inverse", [](Matrix* m){ return *m->inverse(); } },
    //{"adjoint", [](Matrix* m){ return *m->adjoint(); } },

    //{"eigen", [](Matrix* m){ return m->eigenvalue_decomp(); } },
    //{"qr", [](Matrix* m){ return m->qr_decomp(); } },
    //{"lu", [](Matrix* m){ return m->lu_decomp(); } },
    //{"svd", [](Matrix* m){ return m->sv_decomp(); } },
};
