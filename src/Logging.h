/**
 * @file Logging.h
 * @author Philipp Zimmermann (philipp.zimmermann@sci-track.com, zimmermp@cs.uni-kl.de)
 * @brief marsLogging framework: This header provides the complete interface.
 * See the README and examples for detailled explanations and guidance.
 *
 * Note that the Header "LoggingImpl.h" must be accessible from here, since it provides
 * the underlying implementation.
 *
 * @version 1.0
 * @date 2020-06-24
 */


#ifndef _LOGGING_H_
#define _LOGGING_H_


#include "Logging.tcc"

//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
// log messages
//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
namespace marsLogging {
/**
 * @brief This is the masLogging interface, with which a message is logged.
 *
 * Message behaviour is defined by the template parameter
 * Message body is provided to the system via constructor argument or stream operator<<.
 *
 * @tparam logLevel             The log Level for this message
 * @tparam sourceclass          defines the source prefix
 * @tparam localLogLevelCutoff  the local cutoff level, defaults to the global cutoff.
 * @tparam inEnabled            enables/disables printing for this message
 */
template <LogLevel logLevel = marsLogging::LogLevel::error,
          class sourceClass = NO_SOURCE_DEFINED,
          LogLevel localLogLevelCutoff = MARSLOGGING_GLOBAL_LEVEL,
          bool isEnabled = true>
class Log
{
 public:
  constexpr Log() noexcept : logBuffer_{} {}
  constexpr Log(Log&& other) = default;

  Log (const Log& other) = delete;
  Log& operator=(const Log& other) = delete;
  Log& operator=(Log&& other) = delete;

  template <typename msgType>
  constexpr explicit Log(msgType&& message) : logBuffer_{}
  {
    logBuffer_ << std::forward<msgType>(message);
  }

  template <typename msgType>
  constexpr Log<logLevel, sourceClass, localLogLevelCutoff, isEnabled>& operator<<(
      msgType&& message)
  {
    logBuffer_ << std::forward<msgType>(message);
    return *this;
  }


 private:
  // decide if this logged message is to be printed.
  constexpr static bool isPrinted() noexcept
  {
    return(MARSLOGGING_GLOBAL_ENABLE && isEnabled && logLevel >= localLogLevelCutoff);
  }
  typename std::conditional<(!isPrinted()), EmptyLogBuffer,
                            LogBuffer<logLevel, sourceClass>>::type logBuffer_;
};


}  // namespace marsLogging
#endif  // include guard
