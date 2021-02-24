#ifndef Size512_H
#define Size512_H
#include "Sensor.h"

// using namespace std;

class Size512 : public Sensor {
    public:
        Size512(string d, double i, double _shape, double t, int buf, bool bur, int s, string c, string f, string p, int _msgcount) : Sensor(d, i, _shape, t, buf, bur, s, c, f, p, _msgcount) {};
        void GenerateTemplateFieldTypes();
};

#endif
