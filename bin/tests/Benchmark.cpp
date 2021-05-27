#include <chrono>
#include <iostream>

#include "utils/logging/Logging.h"


//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––


struct Timer
{
  inline void start(){ startPoint_ = std::chrono::high_resolution_clock::now(); }
  inline uint64_t stop()
  {
    const auto endPoint = std::chrono::high_resolution_clock::now();
    const uint64_t start =
        std::chrono::time_point_cast<std::chrono::microseconds>(startPoint_)
            .time_since_epoch()
            .count();

    const uint64_t end =
        std::chrono::time_point_cast<std::chrono::microseconds>(endPoint)
            .time_since_epoch()
            .count();

    const uint64_t duration = end - start;
    //double ms = duration * 0.001;
    //std::cout << duration << "µs (" << ms << ")\n";
    return duration;
  }

  std::chrono::time_point<std::chrono::high_resolution_clock> startPoint_;
};


//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––


struct Benchmark{}; // for the output prefix

// function wrapper_
template <uint8_t lvl>
constexpr static auto function_LOG() noexcept
{
  return marsUtils::Log<static_cast<marsUtils::LogLevel>(lvl), Benchmark>{};
}

// using statements:
using using_TRACE = marsUtils::Log<marsUtils::LogLevel::TRACE, Benchmark>;
using using_WARN = marsUtils::Log<marsUtils::LogLevel::WARN, Benchmark>;

// define macro:
#define macro_LOG(lvl) marsUtils::Log<static_cast<marsUtils::LogLevel>(lvl), Benchmark>{}

// function wrapper with constructor
template <uint8_t lvl, typename msgType = std::string>
constexpr inline auto ctor_LOG(msgType&& msg = "")
{
  return marsUtils::Log<static_cast<marsUtils::LogLevel>(lvl), Benchmark>{
      std::forward<msgType>(msg)};
}
//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––


int main()
{
  // CONFIGURATION AT RUNTIME
  marsUtils::configureLogging({{"type", "std_cerr"},{"color",""}});
  // marsUtils::configureLogging({{"type", "file"}, {"path", "benchmarkResults.txt"}});


  marsUtils::Log<marsUtils::LogLevel::INFO, Benchmark, 0> result;
  Timer timer;
  std::cout << "\n\n";
  result << "____________________RESULTS____________________\n";

  //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  result << "\nBaseline: timing an empty loop:\n";
  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {}
  uint64_t duration = timer.stop();
  result << "BASELINE: " << duration << "µs (" << duration*0.001 << "ms)\n";

  //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  result << "\nTiming heap allocated objects: logging of long strings"
            " with function wrapper:\n";
  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    function_LOG<3>() << "this is a message with a long string. I am afraid...";
  }
  duration = timer.stop();
  result << "HEAP LOGGING: " << duration << "µs (" << duration*0.001 << "ms)\n";

  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    function_LOG<0>() << "this is a message with a long string. I am afraid...";
  }
  duration = timer.stop();
  result << "HEAP CUTOFF:  " << duration << "µs (" << duration*0.001 << "ms)\n";

  //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  result << "\nTiming stack allocated objects: logging of integers"
            " with function wrapper:\n";
  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    function_LOG<3>() << i << 5;
  }
  duration = timer.stop();
  result << "STACK LOGGING: " << duration << "µs (" << duration*0.001 << "ms)\n";

  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    function_LOG<0>() << i << 5;
  }
  duration = timer.stop();
  result << "STACK CUTOFF:  " << duration << "µs (" << duration*0.001 << "ms)\n";

  //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  result << "\nTiming heap allocated objects: logging of long strings"
            " with function wrapper in constructor:\n";
  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    ctor_LOG<3>("this is a message with a long string. I am afraid...");
  }
  duration = timer.stop();
  result << "CTOR LOGGING: " << duration << "µs (" << duration*0.001 << "ms)\n";

  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    ctor_LOG<0>("this is a message with a long string. I am afraid...");
  }
  duration = timer.stop();
  result << "CTOR CUTOFF:  " << duration << "µs (" << duration*0.001 << "ms)\n";

  //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  result << "\nTiming heap allocated objects: logging of long strings"
            " with using statements:\n";
  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    using_WARN() << "this is a message with a long string. I am afraid...";
  }
  duration = timer.stop();
  result << "using LOGGING: " << duration << "µs (" << duration*0.001 << "ms)\n";

  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    using_TRACE() << "this is a message with a long string. I am afraid...";
  }
  duration = timer.stop();
  result << "using CUTOFF:  " << duration << "µs (" << duration*0.001 << "ms)\n";

  //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  result << "\nTiming heap allocated objects: logging of long strings"
            " with #define macro:\n";
  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    macro_LOG(3) << "this is a message with a long string. I am afraid...";
  }
  duration = timer.stop();
  result << "#define LOGGING: " << duration << "µs (" << duration*0.001 << "ms)\n";

  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    macro_LOG(0) << "this is a message with a long string. I am afraid...";
  }
  duration = timer.stop();
  result << "#define CUTOFF:  " << duration << "µs (" << duration*0.001 << "ms)\n";

  //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  return 0;
}
