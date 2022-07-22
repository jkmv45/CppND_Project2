#include <string>
#include "format.h"


// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
std::string Format::ElapsedTime(long seconds) { 
    int ssi = seconds % 60;
    int mmi = ((seconds-ssi)/60) % 60;
    int hhi = ((seconds-ssi)/3600) % 3600;
    std::string ss = std::to_string(ssi);
    std::string mm = std::to_string(mmi);
    std::string hh = std::to_string(hhi);
    return hh + ":" + mm + ":" + ss; 
    }