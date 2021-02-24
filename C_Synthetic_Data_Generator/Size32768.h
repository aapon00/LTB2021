#ifndef Size32768_H
#define Size32768_H
#include "Sensor.h"

// using namespace std;

class Size32768 : public Sensor {
    public:
        Size32768(string d, double i, double _shape, double t, int buf, bool bur, int s, string c, string f, string p, int _msgcount) : Sensor(d, i, _shape, t, buf, bur, s, c, f, p, _msgcount) {};
        void GenerateTemplateFieldTypes();
};

#endif
