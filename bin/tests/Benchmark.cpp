#include <chrono>
#include <iostream>

#include <bragi>


//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––


struct Timer
{
  inline void start(){ startPoint_ = std::chrono::high_resolution_clock::now(); }
  inline auto stop()
  {
    const auto endPoint = std::chrono::high_resolution_clock::now();
    const auto start =
        std::chrono::time_point_cast<std::chrono::microseconds>(startPoint_)
            .time_since_epoch()
            .count();

    const auto end =
        std::chrono::time_point_cast<std::chrono::microseconds>(endPoint)
            .time_since_epoch()
            .count();

    const auto duration = end - start;
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
  return bragi::Logger<static_cast<bragi::LogLevel>(lvl), Benchmark>{};
}

// using statements:
using using_TRACE = bragi::Logger<bragi::LogLevel::trace, Benchmark>;
using using_WARN = bragi::Logger<bragi::LogLevel::warn, Benchmark>;

// define macro:
#define macro_LOG(lvl) bragi::Logger<static_cast<bragi::LogLevel>(lvl), Benchmark>{}

// function wrapper with constructor
template <uint8_t lvl, typename msgType = std::string>
constexpr inline auto ctor_LOG(msgType&& msg = "")
{
  return bragi::Logger<static_cast<bragi::LogLevel>(lvl), Benchmark>{
      std::forward<msgType>(msg)};
}

BRAGI_INIT(Benchmark, compConfig_benchmark)
//––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––


int main()
{
  // CONFIGURATION AT RUNTIME
  bragi::configureLogging({{"type", "std_cerr"},{"color",""}});
  // bragi::configureLogging({{"type", "file"}, {"path", "benchmarkResults.txt"}});


  bragi::Logger<bragi::LogLevel::info, Benchmark> result;
  Timer timer;
  std::cout << "\n\n";
  result << "____________________RESULTS____________________\n";

  //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  result << "\nBaseline: timing an empty loop:\n";
  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {}
  auto duration = timer.stop();
  result << "BASELINE: " << duration << "µs (" << duration*0.001l << "ms)\n";

  //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  result << "\nTiming heap allocated objects: logging of long strings"
            " with function wrapper:\n";
  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    function_LOG<103>() << "this is a message with a long string. I am afraid...";
  }
  duration = timer.stop();
  result << "HEAP LOGGING: " << duration << "µs (" << duration*0.001l << "ms)\n";

  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    function_LOG<100>() << "this is a message with a long string. I am afraid...";
  }
  duration = timer.stop();
  result << "HEAP CUTOFF:  " << duration << "µs (" << duration*0.001l << "ms)\n";

  //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  result << "\nTiming stack allocated objects: logging of integers"
            " with function wrapper:\n";
  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    function_LOG<103>() << i << 5;
  }
  duration = timer.stop();
  result << "STACK LOGGING: " << duration << "µs (" << duration*0.001l << "ms)\n";

  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    function_LOG<100>() << i << 5;
  }
  duration = timer.stop();
  result << "STACK CUTOFF:  " << duration << "µs (" << duration*0.001l << "ms)\n";

  //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  result << "\nTiming heap allocated objects: logging of long strings"
            " with function wrapper in constructor:\n";
  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    ctor_LOG<103>("this is a message with a long string. I am afraid...");
  }
  duration = timer.stop();
  result << "CTOR LOGGING: " << duration << "µs (" << duration*0.001l << "ms)\n";

  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    ctor_LOG<100>("this is a message with a long string. I am afraid...");
  }
  duration = timer.stop();
  result << "CTOR CUTOFF:  " << duration << "µs (" << duration*0.001l << "ms)\n";

  //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  result << "\nTiming heap allocated objects: logging of long strings"
            " with using statements:\n";
  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    using_WARN() << "this is a message with a long string. I am afraid...";
  }
  duration = timer.stop();
  result << "using LOGGING: " << duration << "µs (" << duration*0.001l << "ms)\n";

  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    using_TRACE() << "this is a message with a long string. I am afraid...";
  }
  duration = timer.stop();
  result << "using CUTOFF:  " << duration << "µs (" << duration*0.001l << "ms)\n";

  //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  result << "\nTiming heap allocated objects: logging of long strings"
            " with #define macro:\n";
  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    macro_LOG(103) << "this is a message with a long string. I am afraid...";
  }
  duration = timer.stop();
  result << "#define LOGGING: " << duration << "µs (" << duration*0.001l << "ms)\n";

  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    macro_LOG(100) << "this is a message with a long string. I am afraid...";
  }
  duration = timer.stop();
  result << "#define CUTOFF:  " << duration << "µs (" << duration*0.001l << "ms)\n";

  //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  result << "\nNEW: Timing heap allocated objects: logging of long strings"
            " with macros defined in `bragi`:\n";
  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    LOG_FUNC_DETAIL(eval) << "this is a message with a long string. I am afraid...";
  }
  duration = timer.stop();
  result << "HEAP LOGGING: " << duration << "µs (" << duration*0.001l << "ms)\n";

  timer.start();
  for ( uint32_t i = 0; i < 10000; ++i) {
    LOG_DEBUG << "this is a message with a long string. I am afraid...";
  }
  duration = timer.stop();
  result << "HEAP CUTOFF:  " << duration << "µs (" << duration*0.001l << "ms)\n";

  //––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––––

  return 0;
}
