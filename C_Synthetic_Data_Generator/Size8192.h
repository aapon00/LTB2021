#ifndef Size8192_H
#define Size8192_H
#include "Sensor.h"

// using namespace std;

class Size8192 : public Sensor {
    public:
        Size8192(string d, double i, double _shape, double t, int buf, bool bur, int s, string c, string f, string p, int _msgcount) : Sensor(d, i, _shape, t, buf, bur, s, c, f, p, _msgcount) {};
        void GenerateTemplateFieldTypes();
};

#endif
