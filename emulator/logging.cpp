#include "includes.hpp"
#include "logging.hpp"

//Prints out what you want with a newline
void logging::log(std::string toLog)
{
    std::cout << toLog << std::endl;
}

void logging::logerr(std::string toLog, bool fatal)
{
    //clog is same as cerr, but buffered
    std::clog << toLog << std::endl;
    //if the error is fatal, we need to exit
    if (fatal)
    {
        exit(1);
    }
}