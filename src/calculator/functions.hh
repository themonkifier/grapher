#ifndef FUNCTIONS_HH
#define FUNCTIONS_HH

#include <string>
#include <complex>
#include <optional>
#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <functional>

using std::unordered_set, std::unordered_map, std::optional, std::vector, std::string, std::unique_ptr;

typedef std::complex<double> complex;
typedef std::pair<string, std::function<complex(const complex &)>> FuncInfo;

extern unordered_set<char> numbers;
extern unordered_map<string, complex> constants;
extern unordered_map<string, std::function<complex(const complex &)>> functions;

#endif
