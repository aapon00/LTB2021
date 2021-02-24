#ifndef ADVANTECH_ACCELEROMETER_H
#define ADVANTECH_ACCELEROMETER_H
#include "Sensor.h"

// using namespace std;

class Advantech_Accelerometer : public Sensor {
    public:
        Advantech_Accelerometer(string d, double i, double _shape, double t, int buf, bool bur, int s, string c, string f, string p, int _msgcount) : Sensor(d, i, _shape, t, buf, bur, s, c, f, p, _msgcount) {};
        void GenerateTemplateFieldTypes();
};

#endif
