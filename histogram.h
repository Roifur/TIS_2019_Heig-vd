#ifndef _HISTOGRAM_H_
#define _HISTOGRAM_H_

#include "debug.h"

typedef uint8 HISTOGRAM[256];


void calculHistogram(IMAGE *imf,	 HISTOGRAM hist);


#ifdef DEBUG_OUTPUT
#define DEBUG_HIST(hist) numero_test++; \
								sprintf(titre, "test_%03d-HISTO.tif", numero_test);\
								traceHistogram(hist, titre);
void traceHistogram	(HISTOGRAM hist, char fichier[255]);
#else
#define DEBUG_HIST(hist)
#endif
#endif //_HISTOGRAM_H_
