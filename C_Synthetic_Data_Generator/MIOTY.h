#ifndef MIOTY_H
#define MIOTY_H
#include "Sensor.h"

// using namespace std;

class MIOTY : public Sensor {
    public:
        MIOTY(string d, double i, double _shape, double t, int buf, bool bur, int s, string c, string f, string p, int _msgcount) : Sensor(d, i, _shape, t, buf, bur, s, c, f, p, _msgcount) {};
        void GenerateTemplateFieldTypes();
};

#endif
