#ifndef VALUESDATAPOINT_H
#define VALUESDATAPOINT_H

#include "DataPoint.h"

class ValuesDataPoint : public DataPoint {
    private:
        vector<string> values;
    public:
        ValuesDataPoint(string t, vector<string> v);
        vector<string> getValues();
};

#endif