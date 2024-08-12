# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\WidgetModel_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\WidgetModel_autogen.dir\\ParseCache.txt"
  "WidgetModel_autogen"
  )
endif()
