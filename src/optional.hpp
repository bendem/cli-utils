#ifndef CLI_UTILS_OPTIONAL_HPP
#define CLI_UTILS_OPTIONAL_HPP

#if __has_include(<optional>)
#include <optional>
template<typename T>
using optional = std::optional<T>;
#else
#include <experimental/optional>
template<typename T>
using optional = std::experimental::optional<T>;
#endif

#endif
