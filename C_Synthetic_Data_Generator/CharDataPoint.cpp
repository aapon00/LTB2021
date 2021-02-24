#include "CharDataPoint.h"

CharDataPoint::CharDataPoint(string t, CharacterType ct, int len) : DataPoint(t) {
    charType = ct;
    length = len;
}

CharacterType CharDataPoint::getCharType() {
    return charType;
}

int CharDataPoint::getLength() {
    return length;
}