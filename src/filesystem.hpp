#ifndef CLI_UTILS_FILESYSTEM_HPP_HPP
#define CLI_UTILS_FILESYSTEM_HPP_HPP

#if __has_include(<filesystem>)
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

#endif
