#include "FloatDataPoint.h"

FloatDataPoint::FloatDataPoint(string t, double start, double end) : DataPoint(t) {
    range = make_pair(start, end);
}

pair<double, double> FloatDataPoint::getRange() {
    return range;
}