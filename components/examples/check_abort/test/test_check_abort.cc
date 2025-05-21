#include <gtest/gtest.h>
#include <iostream>
using namespace testing;

extern "C"
{
#include "check_abort.h"
}

struct CheckAbortParam
{
    boolean off_course;
    boolean abort_commanded;
    boolean valid_abort_command;
    boolean expected_result;
    const char *description;
};

inline std::ostream &operator<<(std::ostream &os, const CheckAbortParam &param)
{
    os << param.description;
    return os;
}

class CheckAbortParamTest : public ::testing::TestWithParam<CheckAbortParam>
{
};

INSTANTIATE_TEST_SUITE_P(
    CheckAbortCases,
    CheckAbortParamTest,
    ::testing::Values(
        CheckAbortParam{true, false, false, true, "Off course triggers abort"},
        CheckAbortParam{false, true, true, true, "Abort commanded and valid"},
        CheckAbortParam{false, true, false, false, "Abort commanded but not valid"},
        CheckAbortParam{false, false, false, false, "No abort, not off course"}));

TEST_P(CheckAbortParamTest, ReturnsExpectedResult)
{
    const auto &param = GetParam();
    EXPECT_EQ(CheckAbort(param.off_course, param.abort_commanded, param.valid_abort_command), param.expected_result);
}

TEST(check_abort_not_tree_like, test_mcdc)
{
    ASSERT_EQ(TRUE, CheckAbort_NotTreeLike(TRUE, TRUE, FALSE));
    ASSERT_EQ(TRUE, CheckAbort_NotTreeLike(TRUE, FALSE, TRUE));
    ASSERT_EQ(FALSE, CheckAbort_NotTreeLike(FALSE, FALSE, FALSE));
    // ASSERT_EQ(TRUE, CheckAbort_NotTreeLike(FALSE, FALSE, TRUE));
}
