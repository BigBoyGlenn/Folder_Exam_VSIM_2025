# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\QtVulkan_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\QtVulkan_autogen.dir\\ParseCache.txt"
  "QtVulkan_autogen"
  )
endif()
