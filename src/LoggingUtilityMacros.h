#ifndef _BRAGI_UTILITY_MACROS_H_
#define _BRAGI_UTILITY_MACROS_H_

#define _BRAGI_INIT_LOCAL(sourceClass, localConfig)                       \
  template <bragi::LogLevel messageLevel>                                 \
  constexpr static inline auto log_message() noexcept {                   \
    return bragi::Logger<messageLevel, sourceClass, localConfig.logLevel, \
                         localConfig.enable>{};                           \
  }

#define _BRAGI_INIT_GLOBAL(sourceClass)                 \
  template <bragi::LogLevel messageLevel>               \
  constexpr static inline auto log_message() noexcept { \
    return bragi::Logger<messageLevel, sourceClass>{};  \
  }

#define _BRAGI_INIT_DEFAULT()                           \
  template <bragi::LogLevel messageLevel>               \
  constexpr static inline auto log_message() noexcept { \
    return bragi::Logger<messageLevel>{};               \
  }

#define _BRAGI_FUNC_CHOOSER(_f1, _f2, _f3, ...) _f3
#define _BRAGI_FUNC_RECOMPOSER(argsWithParentheses) \
  _BRAGI_FUNC_CHOOSER argsWithParentheses
#define _BRAGI_CHOOSE_FROM_ARG_COUNT(...) \
  _BRAGI_FUNC_RECOMPOSER((__VA_ARGS__, _BRAGI_INIT_LOCAL, _BRAGI_INIT_GLOBAL, ))
#define _BRAGI_NO_ARG_EXPANDER() , , _BRAGI_INIT_DEFAULT
#define _BRAGI_MACRO_CHOOSER(...) \
  _BRAGI_CHOOSE_FROM_ARG_COUNT(_BRAGI_NO_ARG_EXPANDER __VA_ARGS__())

#endif
