#include "IntegerDataPoint.h"

IntegerDataPoint::IntegerDataPoint(string t, int start, int end) : DataPoint(t) {
    range = make_pair(start, end);
}

pair<int, int> IntegerDataPoint::getRange() {
    return range;
}