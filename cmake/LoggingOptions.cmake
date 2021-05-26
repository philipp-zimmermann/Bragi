set(_ML_VALID_LEVEL_NAMES  "trace;debug;eval;info;warn;error;dev" CACHE INTERNAL
                           "The list of valid strings for log level names")
set(_ML_VALID_ENABLE_NAMES  "true;false" CACHE INTERNAL
                            "The list of valid strings for component_enable")

set(_ML_COMPONENT_LIST "" CACHE INTERNAL "")
set(_ML_COMPONENT_CONFIG_TEMP ${marsLogging_BINARY_DIR}/LoggingComponentConfig.h.in
                              CACHE INTERNAL "")
set(_ML_COMPONENT_CONFIG_HEADER ${marsLogging_BINARY_DIR}/LoggingComponentConfig.h
                                CACHE INTERNAL "")

string(CONCAT _file_preamble
    "// This file is automatically generated by marsLogging.cmake\n"
    "// For each <component_name> passed to marsLogging_add_component() the following\n"
    "//   three variables are defined:\n"
    "//   * MARSLOGGING_<upper_case(<component_name>)>_ENABLE\n"
    "//   * MARSLOGGING_<upper_case(<component_name>)>_LEVEL\n"
    "//   * compConfig_<lower_case(<component_name>)>\n\n"
    "#ifndef _ML_PRINT_CONFIG_H_\n#define _ML_PRINT_CONFIG_H_\n")


function(generate_temp_file)
  file(WRITE ${_ML_COMPONENT_CONFIG_TEMP} ${_file_preamble})
  foreach(component_name IN LISTS _ML_COMPONENT_LIST)
    string(TOLOWER ${component_name} comp_lower)
    string(CONCAT content
      "\n#define MARSLOGGING_${component_name}_ENABLE "
      "@MARSLOGGING_${component_name}_ENABLE@\n"

      "#define MARSLOGGING_${component_name}_LEVEL "
      "marsLogging::LogLevel::@MARSLOGGING_${component_name}_LEVEL@\n"

      "constexpr marsLogging::ComponentConfig compConfig_${comp_lower}"
      "{MARSLOGGING_${component_name}_ENABLE,\n    "
      "MARSLOGGING_${component_name}_LEVEL}\;\n")
    file(APPEND ${_ML_COMPONENT_CONFIG_TEMP} ${content})
  endforeach()

  file(APPEND ${_ML_COMPONENT_CONFIG_TEMP} "\n#endif  // _ML_PRINT_CONFIG_H_")
  configure_file(${_ML_COMPONENT_CONFIG_TEMP} ${_ML_COMPONENT_CONFIG_HEADER})
endfunction()


function(marsLogging_add_component component_name)
  string(TOUPPER ${component_name} comp_upper)
  set(_ML_COMPONENT_LIST ${_ML_COMPONENT_LIST};${comp_upper} CACHE INTERNAL "")

  # add the two corresponding options
  set(MARSLOGGING_${comp_upper}_ENABLE "true" CACHE STRING "")
  set_property(CACHE MARSLOGGING_${comp_upper}_ENABLE PROPERTY
                                                      STRINGS ${_ML_VALID_ENABLE_NAMES})
  set(MARSLOGGING_${comp_upper}_LEVEL "info" CACHE STRING "")
  set_property(CACHE MARSLOGGING_${comp_upper}_LEVEL PROPERTY
                                                     STRINGS ${_ML_VALID_LEVEL_NAMES})

  # generate the temporary file, which is the source for ComponentConfig.h
  generate_temp_file()
endfunction()



