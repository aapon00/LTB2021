#ifndef RASPBERRY_PI_ACCELEROMETER_H
#define RASPBERRY_PI_ACCELEROMETER_H
#include "Sensor.h"

// using namespace std;

class Raspberry_Pi_Accelerometer : public Sensor {
    public:
        Raspberry_Pi_Accelerometer(string d, double i, double _shape, double t, int buf, bool bur, int s, string c, string f, string p, int _msgcount) : Sensor(d, i, _shape, t, buf, bur, s, c, f, p, _msgcount) {};
        void GenerateTemplateFieldTypes();
};

#endif
