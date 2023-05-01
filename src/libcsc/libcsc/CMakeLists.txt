set(target_name libcsc)

add_library(${target_name} STATIC
  libcsc/time.cpp
  libcsc/time.hpp)

include(CompileOptions)
set_compile_options(${target_name})

target_include_directories(
  ${target_name}
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)
