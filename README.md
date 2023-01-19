# Griffin Log - C++ Version
A very basic C++ single header only logging library.

## Introduction
Griffin Log is a very basic logging library which, for now, includes only the basics: colored logging info, debug, warnings, critical and fatal, also having a file logger implementation to export the output. 

## Attention
Griffin Log is a basic logging library built as a practicing project, it needs to be improved a lot and needs refactoring.

## Building
Before building, clone the repository into your project folder or any other place. Open a terminal where you want to clone to and execute `git clone https://github.com/juliokscesar/griffinLogCpp`

### Requirements
* Linux:
    * GCC and G++
    * CMake >=  3.9 (*if you want to build the static library*)

* Windows:
    * MinGW or MSVC (Visual Studio)
    * CMake >= 3.9

* macOS - **Currently not supported**


### Linux
* Static Library
    * Run `chmod +x ./build.sh` to allow build.sh execution.
    * Run `./build.sh` in the repo root folder, a folder called "build" will be created and inside it will be the static library built.
    * Include `griffinLog.h` and link the build library to your project.

* Compiling
    * Add `griffinLog.cpp` to build with your other source files and include `griffingLog.h` where you want to use Griffin Log functionalities.

### Windows
* Static Library
    * Make sure CMake is in your PATH environment.
    * Open PowerShell in the repo folder.
    * Execute `mkdir build; cd build` 

    * MinGW
        * Execute `cmake -G "MinGW Makefiles" ..; cmake --build .`

    * Visual Studio (MSVC)
        * Execute `cmake -G "Visual Studio [version]" ..; cmake --build .`

* Compiling
    * Add `griffinLog.cpp` to build with your source files and include `griffinLog.h` where you want to use it.

## Usage
Griffin Log has 5 different colored levels: Info (blue), Debug (green), Warn (yellow), Critical (red), Fatal (black with red background) and it uses C printf style formatting, with '%' placeholders for different types. See the placeholder table [here](https://www.cplusplus.com/reference/cstdio/printf/) as a reference.

**ATTENTION**: For now, when printing C++ exclusive types (like std::string), you'll need to convert it to a C type (like const char*), because of variardic arguments. One way to do that with `std::string` is using the macro `GRIFFIN_STR(str)`, defined in griffinLog.h, as we do in the example below with the file's name.

### Example
```c++
#include <griffinLog/griffinLog.h>

int main()
{
    grflog::info("Hello World!");

    grflog::debug("We are %s in %d", "debugging", 2021);

    // If the 'file' gets destructed in your scope, it will not be finished... 
    // ...until you set a new file or until you specify it with grflog::stop_file_logging();
    grflog::file_logger file("filename.log");
    grflog::set_file_logger(file);

    grflog::warn("We now set the output file to %s", GRIFFIN_STR(file_logger.get_name()));
    
    grflog::info("Stopping file logging...");
    grflog::stop_file_logging();

    grflog::critical("Critical logging");

    grflog::fatal("Bye World!");

    return 0;
}

```
