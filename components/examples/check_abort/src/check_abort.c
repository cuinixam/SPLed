#include "check_abort.h"

// Example function to demonstrate the use of MC/DC (code coverage metric).
// See this youtube video for more information:
// https://www.youtube.com/watch?v=k0_PF8MtEEo
boolean CheckAbort(boolean off_course, boolean abort_commanded, boolean valid_abort_command)
{
    // This decision is tree-like, .i.e., in the BDD (Binary Decision Diagram) every condition is a leaf node hanging on a single branch.
    if (off_course || (abort_commanded && valid_abort_command))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

boolean CheckAbort_NotTreeLike(boolean abort_commanded, boolean valid_abort_command, boolean off_course)
{
    // This decision is not tree-like, .i.e., in the BDD (Binary Decision Diagram) some conditions are hanging on a muliple branches.
    if ((abort_commanded && valid_abort_command) || off_course)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
