

#include "dsp_types.h"

int32 mySqrt(int32 a) {

	int32 ax=1;
	int32 ax1=0;
	int32 loop=20;
	while(!(ax==ax1 || loop==0)) {
		loop--;
		ax1=ax;
		ax=(ax+a/ax)>>1;
	}
	return ax1;
}
