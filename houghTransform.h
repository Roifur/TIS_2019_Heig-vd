#ifndef _HOUGH_TRANSFORM_H
#define _HOUGH_TRANSFORM_H

#include "image_param.h"


#define houghDTheta    1 //degrés
#define houghDRho      1 //pixel

#define houghThetaMax  89
#define houghThetaMin  (-houghThetaMax-1)
#define houghThetaSize ((houghThetaMax-houghThetaMin+1)/houghDTheta)

#define houghRhoMax   363 //pixel
#define houghRhoMin   (-houghRhoMax) //pixel
#define houghRhoSize  ((houghRhoMax-houghRhoMin+1)/houghDRho)




void houghTransform_line	(IMAGE *houghSpace, IMAGE *imf, int seuil);
int lookforlines			(IMAGE *imf, IMAGE *img, int n, int thetas[], int rhos[]);


void houghTransform_circle	(IMAGE *houghSpace_circle, IMAGE *imf, int seuil, int r);
int lookforcircles(IMAGE *imf, IMAGE *img, int n, int r, int xs[], int ys[]);


#ifdef DEBUG_OUTPUT
#define DEBUG_HOUGH(houghSpace) numero_test++; \
								sprintf(titre, "test_%03d-HOUGH.tif", numero_test);\
								MESSAGE(titre);\
								houghTransform_debug(houghSpace, titre);
void houghTransform_debug(IMAGE *houghSpace, char *file);
#else
#define DEBUG_HOUGH(houghSpace)
#endif


#endif //_HOUGH_TRANSFORM_H
