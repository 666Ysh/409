#ifndef __TDOA_H
#define	__TDOA_H
#include <math.h>

typedef struct
{
	double x;
	double y;
	double r21;
	double r31;
	double x31;
	double x21;
	double y2;
	double y3;
	double L;
	double y31;
	double y21;
	double x1;
	double x2;
	double x3;
	double y1;
	double dx;
	double dy;
	double q1;
	double p1;
	double q2;
	double p2;
	double r1;
	double r2;
	double r3;
	double K1;
	double K2;
	double K3;
	double a;
	double b;
	double c;

	double Angle;
}_TDOA;


extern _TDOA TDOA_S;
void TDOA_Init(_TDOA *TDOA_S);
void TDOA_Get_distanceANDangle(_TDOA *TDOA_S);

#endif
