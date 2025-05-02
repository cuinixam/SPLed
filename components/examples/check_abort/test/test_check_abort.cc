#include <gtest/gtest.h>
using namespace testing;

extern "C"
{
#include "check_abort.h"
}

#include "mockup_components_examples_check_abort.h"

// Just provide a simple test case for the function CheckAbort
// to reach 100% branch and 100% condition coverage (MC/DC).
TEST(check_abort, test_mcdc)
{
    CREATE_MOCK(mymock);
    ASSERT_EQ(FALSE, CheckAbort(FALSE, FALSE, FALSE));
    ASSERT_EQ(FALSE, CheckAbort(FALSE, TRUE, FALSE));
    ASSERT_EQ(TRUE, CheckAbort(FALSE, TRUE, TRUE));
    ASSERT_EQ(TRUE, CheckAbort(TRUE, FALSE, FALSE));
}

TEST(check_abort_not_tree_like, test_mcdc)
{
    CREATE_MOCK(mymock);
    ASSERT_EQ(TRUE, CheckAbort_NotTreeLike(TRUE, TRUE, FALSE));
    ASSERT_EQ(TRUE, CheckAbort_NotTreeLike(TRUE, FALSE, TRUE));
    ASSERT_EQ(FALSE, CheckAbort_NotTreeLike(FALSE, FALSE, FALSE));
    // ASSERT_EQ(TRUE, CheckAbort_NotTreeLike(FALSE, FALSE, TRUE));
}
