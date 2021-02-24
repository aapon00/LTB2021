#ifndef INTEGERDATAPOINT_H
#define INTEGERDATAPOINT_H

#include "DataPoint.h"

class IntegerDataPoint : public DataPoint {
    private:
        pair<int, int> range;
    public:
        IntegerDataPoint(string t, int start, int end);
        pair<int, int> getRange();
};

#endif