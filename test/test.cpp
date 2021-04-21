/* 
* MIT License
* 
* Copyright (c) 2021 juliokscesar
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

#include <iostream>
#include "../src/griffinLog/griffinLog.h"

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

    return 0;
}
