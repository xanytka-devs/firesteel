#ifndef FS_UTIL_RANDOM_H
#define FS_UTIL_RANDOM_H

#include <iostream>

namespace Random {
    void setSeed(const unsigned int& tSeed=0) {
#ifdef FS_UTIL_DATE_TIME_H
        if(tSeed==0) srand(static_cast<unsigned int>(std::hash<std::string>()(DateTime::formatted())));
        else
#endif // ~FS_UTIL_DATE_TIME_H
        srand(static_cast<unsigned int>(tSeed));
    }
    bool get() {return rand()%1;}
    int get(const int& tMax=INT_MAX) {return rand()%tMax;}
    float get(const float& tMax) {return static_cast<float>(rand())/static_cast<float>(RAND_MAX/tMax);}
    int get(const int& tMin=INT_MIN, const int& tMax=INT_MAX) {return tMin+(rand()%(tMax-tMin));}
    float get(const float& tMin, const float& tMax) {return tMin+static_cast<float>(rand())/static_cast<float>(RAND_MAX/(tMax-tMin));}
}

#endif // !FS_UTIL_RANDOM_H