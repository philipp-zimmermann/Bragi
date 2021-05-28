#ifndef _ML_UTILITY_MACROS_H_
#define _ML_UTILITY_MACROS_H_

#define _ML_INIT_LOCAL(sourceClass, localConfig)                                \
  template <marsLogging::LogLevel messageLevel>                                 \
  constexpr static inline auto log_message() noexcept                           \
  {                                                                             \
    return marsLogging::Logger<messageLevel, sourceClass, localConfig.logLevel, \
                               localConfig.enable>{};                           \
  }

#define _ML_INIT_GLOBAL(sourceClass)                         \
  template <marsLogging::LogLevel messageLevel>              \
  constexpr static inline auto log_message() noexcept        \
  {                                                          \
    return marsLogging::Logger<messageLevel, sourceClass>{}; \
  }

#define _ML_INIT_DEFAULT()                            \
  template <marsLogging::LogLevel messageLevel>       \
  constexpr static inline auto log_message() noexcept \
  {                                                   \
    return marsLogging::Logger<messageLevel>{};       \
  }

#define _ML_FUNC_CHOOSER(_f1, _f2, _f3, ...) _f3
#define _ML_FUNC_RECOMPOSER(argsWithParentheses) _ML_FUNC_CHOOSER argsWithParentheses
#define _ML_CHOOSE_FROM_ARG_COUNT(...) \
  _ML_FUNC_RECOMPOSER((__VA_ARGS__, _ML_INIT_LOCAL, _ML_INIT_GLOBAL, ))
#define _ML_NO_ARG_EXPANDER() , , _ML_INIT_DEFAULT
#define _ML_MACRO_CHOOSER(...) \
  _ML_CHOOSE_FROM_ARG_COUNT(_ML_NO_ARG_EXPANDER __VA_ARGS__())

#endif
