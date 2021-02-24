#ifndef CHARDATAPOINT_H
#define CHARDATAPOINT_H

#include "DataPoint.h"

enum class CharacterType { 
    alpha,
    alphanumeric
};

class CharDataPoint : public DataPoint {
    private:
        CharacterType charType;
        int length;
    public:
        CharDataPoint(string t, CharacterType ct, int len);
        CharacterType getCharType();
        int getLength();
};

#endif