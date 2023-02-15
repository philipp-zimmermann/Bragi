#include <bragi>

// this defines the function template for log_message()
// for the global scope with global enable and level definitions
BRAGI_INIT()


struct SomeClass
{
  // this defines the same function template for log_message()
  // but within this scope (SomeClass::)
  // and with the configurations defined by compConfig_example_comp
  // = BRAGI_EXAMPLE_COMP_ENABLE, BRAGI_EXAMPLE_COMP_LEVEL
  BRAGI_INIT(SomeClass, compConfig_example_comp)

  static void print_something()
  {
    auto persistent_log = LOG_ERROR;
    persistent_log << "This is a persistent log object.";

    LOG_TRACE << "For when you want to tell a lot of stuff";
    LOG_DEBUG << "This Debug message is printed, because BRAGI_EXAMPLE_COMP_LEVEL"
              << " allows it";
    LOG_WARN << "warnings are printed like this.";

    persistent_log << " It might be \"filled\" troughout its lifetime";

    LOG_FUNC_DETAIL(eval) << "This message prepends info about the calling function";
    LOG_CUSTOM(42) << "This message will not be printed because the custom level 42 is "
                      "below the component wide cutoff of debug (=101)";
    LOG_CUSTOM(102) << "This is a eval message, because bragi::LogLevel::eval=102";
    LOG_CUSTOM(155) << "This finally prints a message with a custom log level";

    persistent_log << "The logged message is printed, once the object is destroyed.";
    LOG_DEV << "For the fancy developer messages, while you are building stuff :)";
  }
};

int main()
{
  LOG_INFO << "Hello World\n";
  LOG_DEBUG << "this message doesn't print, because the global log level is info";
  SomeClass::print_something();
}
