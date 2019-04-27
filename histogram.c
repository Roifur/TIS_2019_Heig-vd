#include <stdio.h>
#include <stdlib.h>
#include "image_param.h"
#include "dsp_types.h"
#include "histogram.h"



void calculHistogram(IMAGE *imf, HISTOGRAM hist){
	int x;
	int y;
	for(y=0; y<imf->Height; y++){
		for(x=0; x<imf->Width; x++){
			hist[imf->datas[y*imf->Width+x]]++;
		}
	}
}

#ifdef DEBUG_OUTPUT
#include "../minitifRGB.h"
void traceHistogram(HISTOGRAM hist, char fichier[255]){
	int i;
	int maxHist=0;
	for(i=0; i<256; i++){
		if(maxHist<hist[i])
			maxHist=hist[i];
	}
	float corr = 255./maxHist;
	
	PIXEL *image = malloc(sizeof(PIXEL)*256*256);
	for(i=0; i<256*256; i++){
			image[i][ROUGE]= 0;
			image[i][BLEU] = 0;
			image[i][VERT] = 0;
		}
	
	uint8 height;
	int y;
	int x;
	
	
	
	for(x=0; x<256; x++){
		height=corr*hist[x];
		for(y=256-height; y<256;y++){
			image[(y*256+x)][ROUGE]= 255;
			image[(y*256+x)][BLEU] = 64+x/2;
			image[(y*256+x)][VERT] = 64+x/2;
		}
	}
	creer_image_tif_rgb(fichier, 256, 256, image);
}

#endif

