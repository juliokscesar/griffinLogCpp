#include <iostream>
#include "../griffinLog.h"

int main()
{
    std::cout << "Console Test\n";

    grflog::info("This is an %s message in %d.", "info", 2021);
    grflog::debug("This is a %s message in %d.", "debug", 2021);
    grflog::warn("This is a %s message in %d.", "warn", 2021);
    grflog::critical("This is a %s message in %d.", "critical", 2021);
    grflog::fatal("This is a %s message in %d.", "fatal", 2021);


    grflog::add_file_logger(grflog::file_logger("test.log"));

    grflog::info("This is a file writing test %s message in %d.", "info", 2021);
    grflog::debug("This is a file writing test %s message in %d.", "debug", 2021);
    grflog::warn("This is a file writing test %s message in %d.", "warn", 2021);
    grflog::critical("This is a file writing test %s message in %d.", "critical", 2021);
    grflog::fatal("This is a file writing test %s message in %d.", "fatal", 2021);

    grflog::add_file_logger(grflog::file_logger("test1.log"));

    grflog::info("This is a file writing test %s message in %d.", "info", 2021);
    grflog::debug("This is a file writing test %s message in %d.", "debug", 2021);
    grflog::warn("This is a file writing test %s message in %d.", "warn", 2021);
    grflog::critical("This is a file writing test %s message in %d.", "critical", 2021);
    grflog::fatal("This is a file writing test %s message in %d.", "fatal", 2021);

    return 0;
}
