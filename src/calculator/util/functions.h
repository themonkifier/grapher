#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "numerical.h"

#include <string>
#include <unordered_set>
#include <unordered_map>
#include <functional>

class FuncInfo {
public:
    using complex_fptr = std::function<const complex(complex)>;
    using mat_to_complex_fptr = std::function<complex(Matrix*)>;
    using mat_to_mat_fptr = std::function<Matrix(Matrix*)>;
    using fptr = std::variant<complex_fptr, mat_to_complex_fptr, mat_to_mat_fptr>;

    std::string name;
    Variable operator()(Variable v) const;
    FuncInfo(std::string name, fptr func);
private:
    fptr func;
};

extern std::unordered_set<char> numbers;
extern std::unordered_map<std::string, complex> constants;
extern std::unordered_map<std::string, FuncInfo::complex_fptr> functions;
extern std::unordered_map<std::string, FuncInfo::mat_to_complex_fptr> matrix_ops;
extern std::unordered_map<std::string, FuncInfo::mat_to_mat_fptr> matrix_ops_;

#endif  // FUNCTIONS_H
