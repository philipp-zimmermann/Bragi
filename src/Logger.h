/**
 * @file Logger.h
 * @author Philipp Zimmermann (philipp.zimmermann@sci-track.com, zimmermp@cs.uni-kl.de)
 * @brief marsLogging framework: This header provides the implementation. It should not be
 * included directly, use the header `marsLogging` instead.
 *
 * marsLogging is logically structured as followes:
 * 1. Messages are logged with instances of class Logger. Messages can be passed to Logger
 *    via its constructor or operator<<
 * 2. Each Logger instance buffers all messages passed to it with its member logBuffer_
 * 3. Logger::isPrinted() decides if buffered messages to this instance should be printed
 * 4. If the messages are to be printed it is done, when the respective Logger instance is
 *    destroyed (via the destructor of logBuffer_)
 * 5. The LogBuffer appends all buffered messages to create a single message
 *    (let's call it "print_message" for this explanation)
 * 6. The LogBuffer destructor prints the buffered messages by passing the print_message
 *    to the LogWriter singleton object.
 * 7. By configuring the LogWriter instance (via configureLogging()) the destination
 *    (file or console) and style (colored or uncolored) can be selected.
 *
 * @version 2.0.0
 * @date 28th May 2021
 */

#ifndef _LOGGING_H_
#define _LOGGING_H_

#include <type_traits>  // for validating types of global LEVEL and ENABLE

#include "LoggingTypes.h"
//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
// type checks or setting of default values
//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
namespace marsLogging {
#if defined(MARSLOGGING_GLOBAL_LEVEL)
static_assert(std::is_same<decltype(MARSLOGGING_GLOBAL_LEVEL), LogLevel>::value,
              "MARSLOGGING_GLOBAL_LEVEL is not of type marsLogging::Loglevel.");
#else
#define MARSLOGGING_GLOBAL_LEVEL LogLevel::info
#endif

#if defined(MARSLOGGING_GLOBAL_ENABLE)
static_assert(std::is_same<decltype(MARSLOGGING_GLOBAL_ENABLE), bool>::value,
              "MARSLOGGING_GLOBAL_ENABLE is not of type bool.");
#else
#define MARSLOGGING_GLOBAL_ENABLE true
#endif
}  // namespace marsLogging

// clang-format off
#include "LogWriter.h"
#include "LogBuffer.h"
// clang-format on

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
 * @tparam msgLevel          The log Level for this message
 * @tparam sourceclass       defines the source prefix
 * @tparam localCutoffLevel  the local cutoff level, defaults to the global cutoff.
 * @tparam localEnable       enables/disables printing for this message
 */
template <LogLevel msgLevel = marsLogging::LogLevel::error,
          class sourceClass = NO_SOURCE_DEFINED,
          LogLevel localCutoffLevel = MARSLOGGING_GLOBAL_LEVEL, bool localEnable = true>
class Logger
{
 public:
  constexpr Logger() noexcept : logBuffer_{} {}
  constexpr Logger(Logger&& other) = default;
  Logger(const Logger& other) = delete;
  Logger& operator=(const Logger& other) = delete;
  Logger& operator=(Logger&& other) = delete;

  template <typename msgType>
  constexpr explicit Logger(msgType&& message) : logBuffer_{}
  {
    logBuffer_ << std::forward<msgType>(message);
  }

  template <typename msgType>
  constexpr Logger<msgLevel, sourceClass, localCutoffLevel, localEnable>& operator<<(
      msgType&& message)
  {
    logBuffer_ << std::forward<msgType>(message);
    return *this;
  }


 private:
  constexpr static bool isPrinted() noexcept
  {
    return (MARSLOGGING_GLOBAL_ENABLE && localEnable && msgLevel >= localCutoffLevel);
  }
  // determines if this logged message is to be printed, if so the type of logBuffer_ is
  // LogBuffer, otherwise is is an EmptyLogBuffer this enables zero computation cost with
  // O3 for not printed messages
  typename std::conditional<isPrinted(), LogBuffer<msgLevel, sourceClass>,
                            EmptyLogBuffer>::type logBuffer_;
};


}  // namespace marsLogging
#endif  // include guard
