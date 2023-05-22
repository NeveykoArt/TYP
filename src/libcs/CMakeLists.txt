set(lib_name csharp)

add_library(${lib_name})

include(CompileOptions)
json_target_set_compile_options(${lib_name})

target_include_directories(${lib_name} PUBLIC . ${CMAKE_BINARY_DIR}/src/grammar/antlr4cpp_generated_src)

target_sources(
  ${lib_name}
  PUBLIC
    libcs/ast/Ast.hpp
    libcs/ast/Visitor.hpp
    libcs/ast/Serializer.hpp
    libcs/ast/SymbolTable.hpp
    libcs/ast/CodeGenerator.hpp
    libcs/dump_tokens.hpp
    libcs/parser.hpp
  PRIVATE
    libcs/ast/detail/Builder.cpp
    libcs/ast/detail/Builder.hpp
    libcs/ast/SymbolTable.cpp
    libcs/ast/CodeGenerator.cpp
    libcs/ast/Ast.cpp
    libcs/ast/Serializer.cpp
    libcs/dump_tokens.cpp
    libcs/parser.cpp
)

target_link_libraries(
  ${lib_name}
  PRIVATE
    fmt
    pugixml
  PUBLIC
    CSharp
)

set(test_name libcs_test)

add_executable(${test_name})

add_test(NAME ${test_name} COMMAND ${test_name})

json_target_set_compile_options(${test_name})

target_sources(
  ${test_name}
  PRIVATE
    test/lexer.cpp
    test/parser.cpp
)

target_link_libraries(
  ${test_name}
  PRIVATE
    ${lib_name}
    gtest_main
)
