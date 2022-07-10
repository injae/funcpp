cmake_minimum_required(VERSION 3.12)

set(CPPM_VERSION 0.0.13)
include(cmake/cppm_loader.cmake)
cppm_project()

project(funcpp VERSION 0.0.1 LANGUAGES C CXX)
cppm_setting()
cppm_cxx_standard(17)
cppm_compiler_option(DEFAULT)


if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR (SERDEPP_BUILD_EXAMPLES OR (SERDEPP_BUILD_TESTING)))
find_cppkg(fmt 8.0.1  MODULE fmt::fmt-header-only TYPE lib)
endif()


cppm_target_define(funcpp INTERFACE)

cppm_examples_area()
if(FUNCPP_BUILD_EXAMPLES)

cppm_target_define(basic BINARY
SOURCES
    examples/basic.cpp
)

endif()


set(funcpp_global_deps )
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR (SERDEPP_BUILD_EXAMPLES OR (SERDEPP_BUILD_TESTING)))
   list(APPEND funcpp_global_deps PRIVATE fmt)
endif()
cppm_target_dependencies(funcpp
   ${funcpp_global_deps})

cppm_target_dependencies(basic
   ${funcpp_global_deps}
   funcpp)


cppm_target_install(funcpp)
cppm_target_install(basic)

