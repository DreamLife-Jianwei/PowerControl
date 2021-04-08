# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\PowerControl_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\PowerControl_autogen.dir\\ParseCache.txt"
  "PowerControl_autogen"
  )
endif()
