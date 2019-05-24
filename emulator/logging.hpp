#pragma once

class logging
{
    private:
        //private constructor means no instances of this object can be created
        logging() {}
    public:
        static void log(std::string toLog);
        static void logerr(std::string toLog, bool fatal=false);
};