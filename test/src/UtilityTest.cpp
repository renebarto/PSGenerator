#include <gtest/gtest.h>
#include <include/Utility.h>

namespace Utility {
namespace Test {

class UtilityTest : public ::testing::Test {
protected:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST_F(UtilityTest, Trim)
{
    std::string expected = "ABC";
    EXPECT_EQ(expected, Trim("ABC"));
    EXPECT_EQ(expected, Trim(" ABC"));
    EXPECT_EQ(expected, Trim("ABC "));
    EXPECT_EQ(expected, Trim(" ABC "));
}

TEST_F(UtilityTest, SplitSingle)
{
    std::string input = "ABC";
    std::vector<std::string> expected = { "ABC" };
    std::vector<std::string> actual;
    Split(input, ',', actual);
    EXPECT_EQ(expected, actual);
}

TEST_F(UtilityTest, SplitTwoParts)
{
    std::string input = "ABC,DEF";
    std::vector<std::string> expected = { "ABC", "DEF" };
    std::vector<std::string> actual;
    Split(input, ',', actual);
    EXPECT_EQ(expected, actual);
}

TEST_F(UtilityTest, SplitTwoPartsWithSpaces)
{
    std::string input = "ABC , DEF";
    std::vector<std::string> expected = { "ABC", "DEF" };
    std::vector<std::string> actual;
    Split(input, ',', actual);
    EXPECT_EQ(expected, actual);
}

TEST_F(UtilityTest, SplitPathsNoDirsNoExtension)
{
    std::string input = "Filename";
    std::string expectedDirectory = {};
    std::string expectedFilename = "Filename";
    std::string expectedExtension = {};
    std::string actualDirectory;
    std::string actualFileName;
    std::string actualExtension;
    SplitPath(input, actualDirectory, actualFileName, actualExtension);
    EXPECT_EQ(expectedDirectory, actualDirectory);
    EXPECT_EQ(expectedFilename, actualFileName);
    EXPECT_EQ(expectedExtension, actualExtension);
}

TEST_F(UtilityTest, SplitPathNoExtension)
{
    std::string input = "/Path/Dir/Filename";
    std::string expectedDirectory = "/Path/Dir";
    std::string expectedFilename = "Filename";
    std::string expectedExtension = {};
    std::string actualDirectory;
    std::string actualFileName;
    std::string actualExtension;
    SplitPath(input, actualDirectory, actualFileName, actualExtension);
    EXPECT_EQ(expectedDirectory, actualDirectory);
    EXPECT_EQ(expectedFilename, actualFileName);
    EXPECT_EQ(expectedExtension, actualExtension);
}

TEST_F(UtilityTest, SplitPathNoDirs)
{
    std::string input = "Filename.Extension";
    std::string expectedDirectory = {};
    std::string expectedFilename = "Filename";
    std::string expectedExtension = "Extension";
    std::string actualDirectory;
    std::string actualFileName;
    std::string actualExtension;
    SplitPath(input, actualDirectory, actualFileName, actualExtension);
    EXPECT_EQ(expectedDirectory, actualDirectory);
    EXPECT_EQ(expectedFilename, actualFileName);
    EXPECT_EQ(expectedExtension, actualExtension);
}

TEST_F(UtilityTest, SplitPath)
{
    std::string input = "/Path/Dir/Filename.Extension";
    std::string expectedDirectory = "/Path/Dir";
    std::string expectedFilename = "Filename";
    std::string expectedExtension = "Extension";
    std::string actualDirectory;
    std::string actualFileName;
    std::string actualExtension;
    SplitPath(input, actualDirectory, actualFileName, actualExtension);
    EXPECT_EQ(expectedDirectory, actualDirectory);
    EXPECT_EQ(expectedFilename, actualFileName);
    EXPECT_EQ(expectedExtension, actualExtension);
}

} // namespace Test
} // namespace Utility
