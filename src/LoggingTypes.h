#ifndef _ML_LOGGING_TYPES_H_
#define _ML_LOGGING_TYPES_H_

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

struct ComponentConfig{
  const bool enable;  // enables/disables printing of logged messages for this component
  const LogLevel logLevel;  // The cutoff level below which no messages are printed
};


}  // namespace marsLogging
#endif  // _ML_LOG_LEVEL_H_
