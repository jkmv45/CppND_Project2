#include "format.h"


// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// Reference: https://stackoverflow.com/questions/26486419/c-save-int-with-leading-zeros-to-string-not-displaying-them
std::string Format::ElapsedTime(long seconds) { 
    // Calculate seconds, minutes, and hours as ints
    long ssi = seconds % 60;
    long mmi = ((seconds-ssi)/60) % 60;
    long hhi = ((seconds-ssi)/3600) % 3600;
    // Format each to always be two characters
    std::ostringstream sss;
    std::ostringstream mms;
    std::ostringstream hhs;
    sss << std::setw(2) << std::setfill('0') << ssi;
    mms << std::setw(2) << std::setfill('0') << mmi;
    hhs << std::setw(2) << std::setfill('0') << hhi;
    // Extract strings from string stream
    std::string ss = sss.str();
    std::string mm = mms.str();
    std::string hh = hhs.str();
    return hh + ":" + mm + ":" + ss; 
}