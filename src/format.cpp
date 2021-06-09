#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function

string Format::ElapsedTime(long int seconds) { 
    
    string HH;
    string MM;
    string SS;

    HH = std::to_string((seconds / 3600));
    MM = std::to_string((seconds / 60) % 60);
    SS = std::to_string(seconds % 60);

    if ((seconds / 3600)*0.1<1) {
        HH = "0"+HH;
    }
    if (((seconds / 60) % 60)*0.1<1){
        MM = "0"+MM;
    }
    if ((seconds % 60)*0.1<1){
        SS = "0"+SS;
    }
    return HH+":"+MM+":"+SS; 
    }