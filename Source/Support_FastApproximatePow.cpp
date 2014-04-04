/**
A fast implementation of an approximized pow function by Martin Ankerl

http://martin.ankerl.com/2007/10/04/optimized-pow-approximation-for-java-and-c-c/
*/

#include "Support_FastApproximatePow.hpp"

double fastPow(double a, double b) {

	union {
		double d;
		int x[2];
	} u = { a };
	u.x[1] = (int)(b * (u.x[1] - 1072632447) + 1072632447);
	u.x[0] = 0;

	return u.d;

}