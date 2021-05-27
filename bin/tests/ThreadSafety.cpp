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
  return marsLogging::Log<static_cast<marsLogging::LogLevel>(lvl), AsyncTest>{};
}


void work()
{
  for(size_t i  = 0; i < 2; ++i)  // run all of this twice
  {
    asyncLog<0>() << "Thread_ID[" << std::this_thread::get_id() << "] I " << "am "
                  << "a " << "thread " << "and like " << "to print " << "stuff "
                  << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    asyncLog<1>() << "Thread_ID[" << std::this_thread::get_id() << "] I " << "am "
                  << "a " << "thread " << "and like " << "to print " << "stuff "
                  << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    asyncLog<2>() << "Thread_ID[" << std::this_thread::get_id() << "] I " << "am "
                  << "a " << "thread " << "and like " << "to print " << "stuff "
                  << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    asyncLog<3>() << "Thread_ID[" << std::this_thread::get_id() << "] I " << "am "
                  << "a " << "thread " << "and like " << "to print " << "stuff "
                  << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    asyncLog<4>() << "Thread_ID[" << std::this_thread::get_id() << "] I " << "am "
                  << "a " << "thread " << "and like " << "to print " << "stuff "
                  << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    asyncLog<5>() << "Thread_ID[" << std::this_thread::get_id() << "] I " << "am "
                  << "a " << "thread " << "and like " << "to print " << "stuff "
                  << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    asyncLog<42>() << "Thread_ID[" << std::this_thread::get_id() << "] I " << "am "
                   << "a " << "thread " << "and like " << "to print " << "stuff "
                   << 5 << " gubbl gubbl gubbl " << 65.78f;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}

int main()
{
  marsLogging::Log<marsLogging::LogLevel::trace>()
      << "––––––––––––––––––––––––––Async–Test––––––––––––––––––––––––––\n";
  std::vector<std::future<void>> results;
  for (size_t i = 0; i < 5; ++i)  // starts 5 threads
    results.emplace_back(std::async(std::launch::async, work));

  return 0;
}
