/**
 * @file BragiUtils.h
 * @author Philipp Zimmermann (zimmermann.philipp89@gmail.com)
 * @brief Implements Utility Functions
 * @version 3.0.0
 * @date 12th May 2023
 */

#ifndef _BRAGI_UTILS_H_
#define _BRAGI_UTILS_H_


#include <source_location>
#include <string_view>


namespace bragi
{

/**
 * @brief returns a demangled name of the class given as the template argument.
 *
 * @tparam ToBeDemangled
 * @returns std::string_view
*/
template <class ToBeDemangled>
[[nodiscard]] consteval auto demangle()
    -> std::string_view
{
  auto name = std::string_view{std::source_location::current().function_name()};
  name.remove_prefix(67);
  name.remove_suffix(50);
  return name;
}


} // namespace bragi
#endif  // _BRAGI_UTILS_H_
