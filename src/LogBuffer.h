/**
 * @file LogWBuffer.tcc
 * @author Philipp Zimmermann (philipp.zimmermann@sci-track.com, zimmermp@cs.uni-kl.de)
 * @brief Implements LogBuffer
 * @version 2.0.0
 * @date 28th May 2021
 */

#ifndef _BRAGI_LOG_BUFFER_H_
#define _BRAGI_LOG_BUFFER_H_

#include <boost/core/demangle.hpp>  // demabglig of typeinfo (For other implementation: https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname)
#include <sstream>                  // Logger::logBuffer_
#include <typeinfo>                 // message prefixes from calling class

#include "LoggingTypes.h"

namespace bragi {

class EmptyLogBuffer
{
  constexpr EmptyLogBuffer() noexcept {}
  EmptyLogBuffer(const EmptyLogBuffer& other) = delete;
  EmptyLogBuffer operator=(EmptyLogBuffer&& other) = delete;
  EmptyLogBuffer operator=(const EmptyLogBuffer& other) = delete;
  ~EmptyLogBuffer() = default;

  template <typename T>
  constexpr EmptyLogBuffer(T&&)
  {}

  template <typename T>
  constexpr EmptyLogBuffer& operator<<(T&&)
  {
    return *this;
  }

  template <LogLevel, class, LogLevel, bool>
  friend class Logger;
};


template <LogLevel logLevel, class sourceClass>
class LogBuffer
{
  LogBuffer()
  {
    if (typeid(sourceClass) != typeid(NO_SOURCE_DEFINED))
      buffer_ << '[' << boost::core::demangle(typeid(sourceClass).name()) << "] ";
  }
  LogBuffer(LogBuffer&& other) = default;

  LogBuffer(const LogBuffer& other) = delete;
  LogBuffer operator=(LogBuffer&& other) = delete;
  LogBuffer operator=(const LogBuffer& other) = delete;

  template <typename msgType>
  explicit LogBuffer(msgType&& message) : LogBuffer{}
  {
    buffer_ << std::forward<msgType>(message);
  }

  // The logged message is printed, when LogBuffer ist destroyed:
  ~LogBuffer() { getLogWriter().log(buffer_.str(), logLevel); }

  template <typename msgType>
  constexpr LogBuffer<logLevel, sourceClass>& operator<<(msgType&& message)
  {
    buffer_ << std::forward<msgType>(message);
    return *this;
  }

  template <LogLevel, class, LogLevel, bool>
  friend class Logger;
  std::ostringstream buffer_;
};

}  // namespace bragi
#endif  // _BRAGI_LOG_BUFFER_H_
