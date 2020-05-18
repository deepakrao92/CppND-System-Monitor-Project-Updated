#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
    int hh = seconds/3600;
    int mm = (seconds % 3600) / 60;
    int ss = seconds%60;
  
    std::string format_time = 
    ((hh < 10) ? ("0" + std::to_string(hh)) : std::to_string(hh)) 
    + ":" + 
    ((mm < 10) ? ("0" + std::to_string(mm)) : std::to_string(mm)) 
    + ":" +
    ((ss < 10) ? ("0" + std::to_string(ss)) : std::to_string(ss)) ;
    
    return format_time; 
}

string Format::kB2MB(long kB ){
    return std::to_string(kB/1024);
}