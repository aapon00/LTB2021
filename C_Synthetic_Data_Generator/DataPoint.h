#ifndef DATAPOINT_H
#define DATAPOINT_H

#include <string>
#include <vector>

using namespace std;

class DataPoint {
    private:
        string type;
    public:
        DataPoint() : type("undefined") {};
        DataPoint(string t) : type(t) {};
        virtual ~DataPoint() = default;
        string getType();
};

#endif