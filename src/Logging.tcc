/**
 * @file Logging.tcc
 * @author Philipp Zimmermann (philipp.zimmermann@sci-track.com, zimmermp@cs.uni-kl.de)
 * @brief The underlying implementation for the marsLogging framework
 * @version 1.0
 * @date 2020-06-24
 */

#ifndef _LOGGING_TCC_
#define _LOGGING_TCC_

#include <boost/core/demangle.hpp>  // demabglig of typeinfo (For other implementation: https://stackoverflow.com/questions/281818/unmangling-the-result-of-stdtype-infoname)
#include <fstream>                  // FileLogWriter
#include <iostream>                 // CerrLogWriter
#include <memory>                   // static LogWriter object
#include <mutex>                    // ensure threadsafety in LogWriter
#include <sstream>                  // Log::buffer_
#include <typeinfo>                 // message prefixes from calling class
#include <unordered_map>            // log level prefixes (un-/colored) and LoggingConfig
#include <type_traits>


//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
// for disabling warnings
//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

#if defined(_MSC_VER)
    #define DISABLE_WARNING_PUSH           __pragma(warning( push ))
    #define DISABLE_WARNING_POP            __pragma(warning( pop ))
    #define DISABLE_WARNING(warningNumber) __pragma(warning( disable : warningNumber ))
    #define DISABLE_WARNING_TYPE_LIMITS
#elif defined(__GNUC__) || defined(__clang__)
    #define DO_PRAGMA(X) _Pragma(#X)
    #define DISABLE_WARNING_PUSH           DO_PRAGMA(GCC diagnostic push)
    #define DISABLE_WARNING_POP            DO_PRAGMA(GCC diagnostic pop)
    #define DISABLE_WARNING(warningName)   DO_PRAGMA(GCC diagnostic ignored #warningName)

    #define DISABLE_WARNING_TYPE_LIMITS    DISABLE_WARNING(-Wtype-limits)
#else
    #define DISABLE_WARNING_PUSH
    #define DISABLE_WARNING_POP
    #define DISABLE_WARNING_UNREFERENCED_FORMAL_PARAMETER
    #define DISABLE_WARNING_TYPE_LIMITS
#endif



namespace marsLogging {

//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
// types and constants
//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

using LoggingConfig = std::unordered_map<std::string, std::string>;

#if defined(MARSLOGGING_GLOBAL_LEVEL)
static_assert(std::is_same<decltype(MARSLOGGING_GLOBAL_LEVEL), LogLevel>::value,
              "MARSLOGGING_GLOBAL_LEVEL not of type marsLogging::Loglevel.");
#else
#define MARSLOGGING_GLOBAL_LEVEL LogLevel::info
#endif


// log level prefixes:
struct EnumHasher
{
  template <typename T>
  std::size_t operator()(T t) const
  {
    return static_cast<std::size_t>(t);
  }
};

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


// default for mars::Log template argument
struct NO_SOURCE_DEFINED{};
// default filepath  for FileLogWriter
constexpr const char* DEFAULT_LOG_FILE_PATH = "marsLOG.txt";



//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
// LogWriter implementations
//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

class LogWriter
{
 public:
  virtual ~LogWriter() = default;  // for unique_ptr dtor in clang
  LogWriter() = delete;
  LogWriter(const LogWriter& other) = delete;
  LogWriter(LogWriter&& other) = delete;
  LogWriter operator=(LogWriter&& other) = delete;
  LogWriter operator=(const LogWriter& other) = delete;

  explicit LogWriter(const LoggingConfig& config)
      : logPrefixes_{config.find("color") != config.end() ? coloredPrefixes
                                                          : uncoloredPrefixes}
  {}
 protected:
  virtual inline void log(const std::string&&, const LogLevel) {}

  std::mutex logMutex_;
  const std::unordered_map<LogLevel, std::string, EnumHasher> logPrefixes_;

  template <LogLevel logLevel, class sourceClass> friend class LogBuffer;
  friend inline std::unique_ptr<LogWriter> createLogWriter(const LoggingConfig& config);
};


class CerrLogWriter : public LogWriter
{
 public:
  ~CerrLogWriter() = default;
 private:
  CerrLogWriter() = delete;
  CerrLogWriter(const CerrLogWriter& other) = delete;
  CerrLogWriter(CerrLogWriter&& other) = delete;
  CerrLogWriter operator=(CerrLogWriter&& other) = delete;
  CerrLogWriter operator=(const CerrLogWriter& other) = delete;

  explicit CerrLogWriter(const LoggingConfig& config)
      : LogWriter{config}
  {}

  inline void log(const std::string&& message, const LogLevel level) override
  {
    std::lock_guard<std::mutex> lock(logMutex_);

    const auto prefixSearch = logPrefixes_.find(level);
    if (prefixSearch != logPrefixes_.end()) {
      std::cerr << prefixSearch->second << message << '\n';
    }
    else
    {
      if (logPrefixes_ == coloredPrefixes) {
        std::cerr << "\x1b[35;1m[CUSTOM:\x1b[0m" << "\x1b[35;1m"
                  << std::to_string(static_cast<uint8_t>(level)) << "]\x1b[0m "
                  << message << '\n';
      } else {
        std::cerr << "[CUSTOM:" << std::to_string(static_cast<uint8_t>(level))
                  << message << '\n';
      }
    }
  }

  template <LogLevel logLevel, class sourceClass> friend class LogBuffer;
  friend inline std::unique_ptr<LogWriter> createLogWriter(const LoggingConfig& config);
};


class FileLogWriter : public LogWriter
{
 public:
  explicit FileLogWriter(const LoggingConfig& config)
      : LogWriter{config}
      , fileName_{config.find("path") != config.end()
                      ? config.find("path")->second.c_str()
                      : DEFAULT_LOG_FILE_PATH}
      , file_{}
  {
    file_.open(fileName_, std::ofstream::out | std::ofstream::trunc);
  }
  ~FileLogWriter() { file_.close(); }

  FileLogWriter() = delete;
  FileLogWriter(const FileLogWriter& other) = delete;
  FileLogWriter(FileLogWriter&& other) = delete;
  FileLogWriter operator=(FileLogWriter&& other) = delete;
  FileLogWriter operator=(const FileLogWriter& other) = delete;




  inline void log(const std::string&& message, const LogLevel level) override
  {
    std::lock_guard<std::mutex> lock(logMutex_);

    const auto prefixSearch = logPrefixes_.find(level);
    if (prefixSearch != logPrefixes_.end()) {
      file_ << prefixSearch->second << message << '\n';
    } else {
      file_ << "[CUSTOM:" << std::to_string(static_cast<uint8_t>(level))
            << message << '\n';
    }
    file_.flush();
  }
 private:

  const char* fileName_;
  std::ofstream file_;
};



//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
// get at the static LogWriter object
//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

inline std::unique_ptr<LogWriter> createLogWriter(const LoggingConfig& config)
{
  using namespace std::string_literals;
  const auto logLevelPrefix{uncoloredPrefixes.find(MARSLOGGING_GLOBAL_LEVEL)};
  const std::string creationInfo =
      "Global log level = "s +
      (logLevelPrefix != uncoloredPrefixes.end()
           ? logLevelPrefix->second
           : "[" + std::to_string(static_cast<uint8_t>(MARSLOGGING_GLOBAL_LEVEL)) + "]");

  auto printConfigError = [] {
    if (MARSLOGGING_GLOBAL_LEVEL <= LogLevel::error)
    {
      std::cerr << "\x1b[31;1m[ERROR]\x1b[0m "
                << "[configureLogging] found no valid type for LogWriter,"
                << " no messages will be logged!\n";
    }
  };


#ifdef MARSLOGGING_GLOBAL_ENABLE
  const auto type = config.find("type");
  if (type->second == "std_cerr")
  {
    std::unique_ptr<CerrLogWriter> cerrLogWriter(new CerrLogWriter{config});
    if(MARSLOGGING_GLOBAL_LEVEL <= LogLevel::debug) {
      cerrLogWriter->log(std::move(creationInfo), LogLevel::debug); }
    return cerrLogWriter;
  }
  else if (type->second == "file")
  {
    auto fileLogWriter = std::make_unique<FileLogWriter>(config);
    if(MARSLOGGING_GLOBAL_LEVEL <= LogLevel::debug) {
      fileLogWriter->log(std::move(creationInfo), LogLevel::debug); }
    return fileLogWriter;
  }
  else
  {
    printConfigError();
    return std::make_unique<LogWriter>(config);
  }
#else
  return std::make_unique<LogWriter>(config);
#endif
}


inline LogWriter& getLogWriter(const LoggingConfig& config = {{"type", "std_cerr"},
                                                              {"color", ""}})
{
  static std::unique_ptr<LogWriter> logger{createLogWriter(config)};
  return *logger;
}


//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––
// LogBuffer
//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

class EmptyLogBuffer
{
  constexpr EmptyLogBuffer() noexcept {}

  EmptyLogBuffer(const EmptyLogBuffer& other) = delete;
  EmptyLogBuffer operator=(EmptyLogBuffer&& other) = delete;
  EmptyLogBuffer operator=(const EmptyLogBuffer& other) = delete;
  ~EmptyLogBuffer() = default;

  template <typename T>
  constexpr EmptyLogBuffer(T&&){}

  template <typename T>
  constexpr EmptyLogBuffer& operator<<(T&&)
  {
    return *this;
  }

  template <LogLevel, class, LogLevel, bool> friend class Log;
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
  ~LogBuffer()
  {
    getLogWriter().log(buffer_.str(), logLevel);
  }

  template <typename msgType>
  constexpr LogBuffer<logLevel, sourceClass>& operator<<(msgType&& message)
  {
    buffer_ << std::forward<msgType>(message);
    return *this;
  }

  template <LogLevel, class, LogLevel, bool> friend class Log;
  std::ostringstream buffer_;
};



}  // namespace mars
#endif
