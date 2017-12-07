#include <gtest/gtest.h>

#include <iostream>
#include <include/CodeGenerator.h>
#include <include/TestData.h>

using namespace std;

namespace CPPParser {
namespace Test {

class ASTBuildTest
    : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }
};

TEST_F(ASTBuildTest, EmptyAST)
{
    AST ast;

    EXPECT_FALSE(ast.IsValid());
}

} // namespace Test
} // namespace CPPASTVisitor
