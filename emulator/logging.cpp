#include "includes.hpp"
#include "logging.hpp"

/*
template <typename T>
std::string NumberToString ( T Number )
{
    std::ostringstream ss;
    ss << std::hex << Number;
    return ss.str();
}
*/

//converts byte to hex string ex. 255 becomes "FF"
std::string logging::byteToString(byte num)
{
    std::ostringstream ss;
    ss.width(2);
    ss.fill('0');
    ss << std::hex << std::uppercase << num;
    return ss.str();
}

//converts short to hex string ex. 65535 becomes "FFFF"
std::string logging::ushortToString(ushort num)
{
    std::ostringstream ss;
    ss.width(4);
    ss.fill('0');
    ss << std::hex << std::uppercase << num;
    return ss.str();
}

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