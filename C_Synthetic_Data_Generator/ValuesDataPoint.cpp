#include "ValuesDataPoint.h"

ValuesDataPoint::ValuesDataPoint(string t, vector<string> v) : DataPoint(t) {
    values = v;
}

vector<string> ValuesDataPoint::getValues() {
    return values;
}