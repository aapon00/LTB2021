#ifndef Size4096_H
#define Size4096_H
#include "Sensor.h"

// using namespace std;

class Size4096 : public Sensor {
    public:
        Size4096(string d, double i, double _shape, double t, int buf, bool bur, int s, string c, string f, string p, int _msgcount) : Sensor(d, i, _shape, t, buf, bur, s, c, f, p, _msgcount) {};
        void GenerateTemplateFieldTypes();
};

#endif
