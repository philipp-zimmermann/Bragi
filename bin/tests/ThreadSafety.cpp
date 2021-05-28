#include <MarsLogging>
#include <string>
#include <iostream>
#include <thread>
#include <future>
#include <vector>


struct AsyncTest{};
template <uint8_t lvl>
constexpr auto asyncLog()
{
  return marsLogging::Logger<static_cast<marsLogging::LogLevel>(lvl), AsyncTest>{};
}

MARSLOGGINING_INIT(AsyncTest, compConfig_threadsafety)

void work()
{
  for(size_t i  = 0; i < 2; ++i)  // run all of this twice
  {
    ///_oldscool_stuff____________________________________________________________________
    // note: this asserts that global level is "info" (=103)
    asyncLog<100>() << "Thread_ID[" << std::this_thread::get_id() << "] I " << "am "
                  << "a " << "thread " << "and like " << "to print " << "stuff "
                  << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    asyncLog<101>() << "Thread_ID[" << std::this_thread::get_id() << "] I " << "am "
                  << "a " << "thread " << "and like " << "to print " << "stuff "
                  << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    asyncLog<102>() << "Thread_ID[" << std::this_thread::get_id() << "] I " << "am "
                  << "a " << "thread " << "and like " << "to print " << "stuff "
                  << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    asyncLog<103>() << "Thread_ID[" << std::this_thread::get_id() << "] I " << "am "
                  << "a " << "thread " << "and like " << "to print " << "stuff "
                  << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    asyncLog<104>() << "Thread_ID[" << std::this_thread::get_id() << "] I " << "am "
                  << "a " << "thread " << "and like " << "to print " << "stuff "
                  << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    asyncLog<105>() << "Thread_ID[" << std::this_thread::get_id() << "] I " << "am "
                  << "a " << "thread " << "and like " << "to print " << "stuff "
                  << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    asyncLog<42>() << "Thread_ID[" << std::this_thread::get_id() << "] I " << "am "
                   << "a " << "thread " << "and like " << "to print " << "stuff "
                   << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    ///_fancy_new_stuff___________________________________________________________________
    // note: the bahaviour here is dependent on compConfig_threadsafety.
    LOG_TRACE << "Thread_ID[" << std::this_thread::get_id() << "] LOG_TRACE: I " << "am "
                   << "a " << "thread " << "and like " << "to print " << "stuff "
                   << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    LOG_DEBUG << "Thread_ID[" << std::this_thread::get_id() << "] LOG_DEBUG: I " << "am "
                   << "a " << "thread " << "and like " << "to print " << "stuff "
                   << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    LOG_INFO << "Thread_ID[" << std::this_thread::get_id() << "] LOG_INFO: I " << "am "
                   << "a " << "thread " << "and like " << "to print " << "stuff "
                   << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    LOG_WARN << "Thread_ID[" << std::this_thread::get_id() << "] LOG_WARN: I " << "am "
                   << "a " << "thread " << "and like " << "to print " << "stuff "
                   << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    LOG_ERROR << "Thread_ID[" << std::this_thread::get_id() << "] LOG_ERROR: I " << "am "
                   << "a " << "thread " << "and like " << "to print " << "stuff "
                   << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

int main()
{
  marsLogging::Logger<marsLogging::LogLevel::info>()
      << "––––––––––––––––––––––––––Async–Test––––––––––––––––––––––––––\n";
  std::vector<std::future<void>> results;
  for (size_t i = 0; i < 5; ++i)  // starts 5 threads
    results.emplace_back(std::async(std::launch::async, work));
}
