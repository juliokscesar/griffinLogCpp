@echo off
g++ -g -DGRIFFIN_LOG_DEBUG -Wall -Wextra -o test test.cpp ../src/griffinLog/griffinLog.cpp
