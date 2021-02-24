#ifndef SizeCustom_H
#define SizeCustom_H
#include "Sensor.h"

// using namespace std;

class SizeCustom : public Sensor {
	private:
	int buffer_size;

    public:
        SizeCustom(string d, double i, double _shape, double t, int buf, bool bur, int s, string c, string f, string p, int _msgcount, int _size) : Sensor(d, i, _shape, t, buf, bur, s, c, f, p, _msgcount), buffer_size(_size) {};
        void GenerateTemplateFieldTypes();
};

#endif
