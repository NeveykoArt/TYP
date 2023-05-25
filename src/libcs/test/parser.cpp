#include <gtest/gtest.h>

#include <libcs/parser.hpp>

#include <CSharpLexer.h>
#include <CSharpParser.h>

#include <filesystem>
#include <algorithm>
#include <iterator>
#include <sstream>
#include <string>

std::filesystem::path core_path = "../../../../examples";

namespace csharp::test {

TEST(Parser, ValidProgram1) {
    std::ifstream example_file(std::filesystem::current_path() / core_path / "example_assig1.cs");
    auto parse_result = csharp::parse(example_file);
    EXPECT_TRUE(parse_result.errors_.empty());
    std::stringstream out;
    csharp::dump_ast(parse_result.program_, out);
    EXPECT_EQ(
        out.str(),
        "<?xml version=\"1.0\"?>\n<csharp>\n  <class>\n    <id>example_assig1</id>\n    <assign>\n      <leftside>\n        <var_def>\n          <type>int</type>\n"
        "          <id>a</id>\n        </var_def>\n      </leftside>\n      <rightside>\n        <arg>\n          <literal>2</literal>\n        </arg>\n      </rightside>\n"
        "    </assign>\n    <assign>\n      <leftside>\n        <var_def>\n          <type>float</type>\n          <id>b</id>\n        </var_def>\n      </leftside>\n"
        "      <rightside>\n        <arg>\n          <literal>2.0</literal>\n        </arg>\n      </rightside>\n    </assign>\n    <assign>\n      <leftside>\n"
        "        <var_def>\n          <type>char</type>\n          <id>c</id>\n        </var_def>\n      </leftside>\n      <rightside>\n        <arg>\n"
        "          <literal>'a'</literal>\n        </arg>\n      </rightside>\n    </assign>\n    <assign>\n      <leftside>\n        <var_def>\n          <type>string</type>\n"
        "          <id>d</id>\n        </var_def>\n      </leftside>\n      <rightside>\n        <arg>\n          <literal>\"hello\"</literal>\n        </arg>\n      </rightside>\n"
        "    </assign>\n    <assign>\n      <leftside>\n        <var_def>\n          <type>bool</type>\n          <id>e</id>\n        </var_def>\n      </leftside>\n"
        "      <rightside>\n        <arg>\n          <literal>true</literal>\n        </arg>\n      </rightside>\n    </assign>\n    <assign>\n      <leftside>\n"
        "        <var_def>\n          <type>bool</type>\n          <id>f</id>\n        </var_def>\n      </leftside>\n      <rightside>\n        <arg>\n"
        "          <literal>false</literal>\n        </arg>\n      </rightside>\n    </assign>\n  </class>\n</csharp>\n");
}

TEST(Parser, ValidProgram2) {
    std::ifstream example_file(std::filesystem::current_path() / core_path / "example_assig2.cs");
    auto parse_result = csharp::parse(example_file);
    EXPECT_TRUE(parse_result.errors_.empty());
    std::stringstream out;
    csharp::dump_ast(parse_result.program_, out);
    EXPECT_EQ(
        out.str(),
        "<?xml version=\"1.0\"?>\n<csharp>\n  <class>\n    <id>example_assig2</id>\n    <mas_assign>\n      <leftside>\n        <var_def>\n          "
        "<type>int[]</type>\n          <id>h</id>\n        </var_def>\n      </leftside>\n    </mas_assign>\n    <assign>\n      <leftside>\n        "
        "<arg>\n          <id>h</id>\n        </arg>\n      </leftside>\n      <rightside>\n        <new>\n          <type>int<sq_brakets>2</sq_brakets>\n          "
        "</type>\n          <braces>\n            <literal>1</literal>\n            <literal>2</literal>\n          </braces>\n        "
        "</new>\n      </rightside>\n    </assign>\n    <assign>\n      <leftside>\n        <var_def>\n          <type>int</type>\n          "
        "<id>g</id>\n        </var_def>\n      </leftside>\n      <rightside>\n        <arg>\n          <id>h</id>\n          <at>\n            "
        "<sq_brakets>2</sq_brakets>\n          </at>\n        </arg>\n      </rightside>\n    </assign>\n  </class>\n</csharp>\n");
}

TEST(Parser, ValidProgram3) {
    std::ifstream example_file(std::filesystem::current_path() / core_path / "example_assig4.cs");
    auto parse_result = csharp::parse(example_file);
    EXPECT_TRUE(parse_result.errors_.empty());
    std::stringstream out;
    csharp::dump_ast(parse_result.program_, out);
    EXPECT_EQ(
        out.str(),
        "<?xml version=\"1.0\"?>\n<csharp>\n  <class>\n    <id>example_assig4</id>\n    <mas_assign>\n      <leftside>\n        <var_def>\n          "
        "<type>int[]</type>\n          <id>h</id>\n        </var_def>\n      </leftside>\n      <rightside>\n        <new>\n          "
        "<type>int<sq_brakets>2</sq_brakets>\n          </type>\n          <braces>\n            <literal>1</literal>\n            <literal>2</literal>\n          "
        "</braces>\n        </new>\n      </rightside>\n    </mas_assign>\n    <assign>\n      <leftside>\n        <var_def>\n          <type>int</type>\n          "
        "<id>g</id>\n        </var_def>\n      </leftside>\n      <rightside>\n        <arg>\n          <id>h</id>\n          <at>\n            "
        "<sq_brakets>2</sq_brakets>\n          </at>\n        </arg>\n      </rightside>\n    </assign>\n  </class>\n</csharp>\n");
}

TEST(Parser, ValidProgram4) {
    std::ifstream example_file(std::filesystem::current_path() / core_path / "example_assig5.cs");
    auto parse_result = csharp::parse(example_file);
    EXPECT_TRUE(parse_result.errors_.empty());
    std::stringstream out;
    csharp::dump_ast(parse_result.program_, out);
    EXPECT_EQ(
        out.str(),
        "<?xml version=\"1.0\"?>\n<csharp>\n  <class>\n    <id>example_assig5</id>\n    <function>\n      <function_type>int</function_type>\n      "
        "<id>sum</id>\n      <pars>\n        <var_def>\n          <type>int</type>\n          <id>a</id>\n        </var_def>\n        <var_def>\n          "
        "<type>int</type>\n          <id>b</id>\n        </var_def>\n      </pars>\n      <scope>\n        <assign>\n          <leftside>\n            "
        "<var_def>\n              <type>int</type>\n              <id>c</id>\n            </var_def>\n          </leftside>\n          <rightside>\n            "
        "<binary_op>\n              <op_type>+</op_type>\n              <arg>\n                <id>a</id>\n              </arg>\n              <arg>\n                "
        "<id>b</id>\n              </arg>\n            </binary_op>\n          </rightside>\n        </assign>\n      </scope>\n      <return>\n        "
        "<arg>\n          <id>c</id>\n        </arg>\n      </return>\n    </function>\n    <assign>\n      <leftside>\n        <var_def>\n          "
        "<type>int</type>\n          <id>g</id>\n        </var_def>\n      </leftside>\n      <rightside>\n        <func_call>\n          "
        "<id>sum</id>\n          <roundbrackets>\n            <arg>\n              <literal>1</literal>\n            </arg>\n            <arg>\n              "
        "<literal>2</literal>\n            </arg>\n          </roundbrackets>\n        </func_call>\n      </rightside>\n    </assign>\n  </class>\n</csharp>\n");
}

TEST(Parser, ValidProgram5) {
    std::ifstream example_file(std::filesystem::current_path() / core_path / "example_func1.cs");
    auto parse_result = csharp::parse(example_file);
    EXPECT_TRUE(parse_result.errors_.empty());
    std::stringstream out;
    csharp::dump_ast(parse_result.program_, out);
    EXPECT_EQ(
        out.str(),
        "<?xml version=\"1.0\"?>\n<csharp>\n  <class>\n    <id>example_func1</id>\n    <function>\n      <function_type>int</function_type>\n      "
        "<id>sum</id>\n      <pars>\n        <var_def>\n          <type>int</type>\n          <id>a</id>\n        </var_def>\n        <var_def>\n          "
        "<type>int</type>\n          <id>b</id>\n        </var_def>\n      </pars>\n      <scope>\n        <assign>\n          <leftside>\n            "
        "<var_def>\n              <type>int</type>\n              <id>c</id>\n            </var_def>\n          </leftside>\n          <rightside>\n            "
        "<binary_op>\n              <op_type>+</op_type>\n              <arg>\n                <id>a</id>\n              </arg>\n              "
        "<arg>\n                <id>b</id>\n              </arg>\n            </binary_op>\n          </rightside>\n        </assign>\n      "
        "</scope>\n      <return>\n        <arg>\n          <id>c</id>\n        </arg>\n      </return>\n    </function>\n  </class>\n</csharp>\n");
}

TEST(Parser, ValidProgram6) {
    std::ifstream example_file(std::filesystem::current_path() / core_path / "example_func2.cs");
    auto parse_result = csharp::parse(example_file);
    EXPECT_TRUE(parse_result.errors_.empty());
    std::stringstream out;
    csharp::dump_ast(parse_result.program_, out);
    EXPECT_EQ(
        out.str(),
        "<?xml version=\"1.0\"?>\n<csharp>\n  <class>\n    <id>example_func2</id>\n    <function>\n      <function_type>static int</function_type>\n      "
        "<id>sum</id>\n      <pars>\n        <var_def>\n          <type>int</type>\n          <id>a</id>\n        </var_def>\n        <var_def>\n          "
        "<type>int</type>\n          <id>b</id>\n        </var_def>\n      </pars>\n      <scope>\n        <assign>\n          <leftside>\n            "
        "<var_def>\n              <type>int</type>\n              <id>c</id>\n            </var_def>\n          </leftside>\n          <rightside>\n            "
        "<binary_op>\n              <op_type>+</op_type>\n              <arg>\n                <id>a</id>\n              </arg>\n              "
        "<arg>\n                <id>b</id>\n              </arg>\n            </binary_op>\n          </rightside>\n        </assign>\n      "
        "</scope>\n      <return>\n        <arg>\n          <id>c</id>\n        </arg>\n      </return>\n    </function>\n  </class>\n</csharp>\n");
}

TEST(Parser, ValidProgram7) {
    std::ifstream example_file(std::filesystem::current_path() / core_path / "example_func3.cs");
    auto parse_result = csharp::parse(example_file);
    EXPECT_TRUE(parse_result.errors_.empty());
    std::stringstream out;
    csharp::dump_ast(parse_result.program_, out);
    EXPECT_EQ(
        out.str(),
        "<?xml version=\"1.0\"?>\n<csharp>\n  <class>\n    <id>example_func3</id>\n    <function>\n      <function_type>static int</function_type>\n      "
        "<id>sum</id>\n      <pars>\n        <var_def>\n          <type>int</type>\n          <id>a</id>\n        </var_def>\n        <var_def>\n          "
        "<type>int</type>\n          <id>b</id>\n        </var_def>\n      </pars>\n      <scope>\n        <assign>\n          <leftside>\n            "
        "<var_def>\n              <type>int</type>\n              <id>c</id>\n            </var_def>\n          </leftside>\n          "
        "<rightside>\n            <binary_op>\n              <op_type>+</op_type>\n              <arg>\n                <id>a</id>\n              "
        "</arg>\n              <arg>\n                <id>b</id>\n              </arg>\n            </binary_op>\n          </rightside>\n        "
        "</assign>\n      </scope>\n      <return>\n        <arg>\n          <id>c</id>\n        </arg>\n      </return>\n    </function>\n    "
        "<function>\n      <function_type>static void</function_type>\n      <id>Main</id>\n      <pars>\n        <var_def>\n          "
        "<type>string[]</type>\n          <id>args</id>\n        </var_def>\n      </pars>\n      <scope>\n        <assign>\n          <leftside>\n            "
        "<var_def>\n              <type>int</type>\n              <id>c</id>\n            </var_def>\n          </leftside>\n          <rightside>\n            "
        "<arg>\n              <literal>0</literal>\n            </arg>\n          </rightside>\n        </assign>\n        <assign>\n          "
        "<leftside>\n            <arg>\n              <id>c</id>\n            </arg>\n          </leftside>\n          <rightside>\n            "
        "<func_call>\n              <id>sum</id>\n              <roundbrackets>\n                <arg>\n                  <id>a</id>\n                "
        "</arg>\n                <arg>\n                  <id>b</id>\n                </arg>\n              </roundbrackets>\n            </func_call>\n          "
        "</rightside>\n        </assign>\n        <println>\n          <id>Console.WriteLine<arg>\n              <id>c</id>\n            </arg>\n          "
        "</id>\n        </println>\n      </scope>\n      <return />\n    </function>\n  </class>\n</csharp>\n");
}

}  // namespace csharp::test
