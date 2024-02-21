// TODO: Define DOCTEST_CONFIG_DISABLE for release builds.
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

int
main(int argc, char* argv[])
{
    doctest::Context context;
    context.applyCommandLine(argc, argv);
    int doctestResult = context.run();
    if (context.shouldExit()) {
        return doctestResult;
    }

    return doctestResult;
}
