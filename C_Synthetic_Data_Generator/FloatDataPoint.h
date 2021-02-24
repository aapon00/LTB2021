#ifndef FLOATDATAPOINT_H
#define FLOATDATAPOINT_H

#include "DataPoint.h"

class FloatDataPoint : public DataPoint {
    private:
        pair<double, double> range;
    public:
        FloatDataPoint(string t, double start, double end);
        pair<double, double> getRange();
};

#endif