# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\StandardModel_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\StandardModel_autogen.dir\\ParseCache.txt"
  "StandardModel_autogen"
  )
endif()
