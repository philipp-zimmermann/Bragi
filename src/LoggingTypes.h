#ifndef _ML_LOGGING_TYPES_H_
#define _ML_LOGGING_TYPES_H_

#include <unordered_map>  // log level prefixes (un-/colored) and LoggingConfig

namespace marsLogging {

enum class LogLevel
{
  trace = 100,  // more detailed information for debugging
  debug = 101,  // general debugging messages
  eval = 102,   // for evaluation messages that might be included in release versions
  info = 103,
  warn = 104,
  error = 105,
  dev = 106  // for development convenience, this should not be in any released version
};


// Passed to MARSLOGGINING_INIT to configure which logged messages within this scope
// (component) are to be printed
struct ComponentConfig
{
  const bool enable;  // enables/disables printing of logged messages for this component
  const LogLevel logLevel;  // The cutoff level below which no messages are printed
};


// Passed configureLogging() to configure the output and style of printed messages
using LoggingConfig = std::unordered_map<std::string, std::string>;


// Hasher for Prefix maps
struct EnumHasher
{
  template <typename T>
  std::size_t operator()(T t) const
  {
    return static_cast<std::size_t>(t);
  }
};
// clang-format off
const std::unordered_map<LogLevel, std::string, EnumHasher> coloredPrefixes{
    {LogLevel::dev,   "\x1b[35;1m[DEV]\x1b[0m   "},
    {LogLevel::error, "\x1b[31;1m[ERROR]\x1b[0m "},
    {LogLevel::warn,  "\x1b[33;1m[WARN]\x1b[0m  "},
    {LogLevel::info,  "\x1b[32;1m[INFO]\x1b[0m  "},
    {LogLevel::eval,  "\x1b[36;1m[EVAL]\x1b[0m  "},
    {LogLevel::debug, "\x1b[34;1m[DEBUG]\x1b[0m "},
    {LogLevel::trace, "\x1b[37;1m[TRACE]\x1b[0m "}};

const std::unordered_map<LogLevel, std::string, EnumHasher> uncoloredPrefixes{
    {LogLevel::dev,   "[DEV]   "},
    {LogLevel::error, "[ERROR] "},
    {LogLevel::warn,  "[WARN]  "},
    {LogLevel::info,  "[INFO]  "},
    {LogLevel::eval,  "[EVAL]  "},
    {LogLevel::debug, "[DEBUG] "},
    {LogLevel::trace, "[TRACE] "}};
// clang-format on


struct NO_SOURCE_DEFINED
{};  // default for mars::Logger template argument


constexpr const char* DEFAULT_LOG_FILE_PATH = "marsLOG.txt";  // default for FileLogWriter

}  // namespace marsLogging
#endif  // _ML_LOG_LEVEL_H_
