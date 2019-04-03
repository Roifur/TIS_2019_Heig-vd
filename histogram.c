#include <stdio.h>
#include <stdlib.h>
#include "image_param.h"
#include "minitifRGB.h"
#include "dsp_types.h"
#include "histogram.h"



void calculHistogram(uint8 f[][W], HISTOGRAM hist){
	int x;
	int y;
	for(y=0; y<H; y++){
		for(x=0; x<W; x++){
			hist[f[y][x]]++;
		}
	}
}


void traceHistogram(HISTOGRAM hist, char fichier[255]){
	int i;
	int maxHist=0;
	for(i=0; i<256; i++){
		if(maxHist<hist[i])
			maxHist=hist[i];
	}
	float corr = 255./maxHist;
	PIXEL image[256][256]={0};
	
	uint8 height;
	int y;
	int x;
	
	for(x=0; x<256; x++){
		height=corr*hist[x];
		for(y=256-height; y<256;y++){
			image[y][x][ROUGE]= 255;
			image[y][x][BLEU] = 64+x/2;
			image[y][x][VERT] = 64+x/2;
		}
	}
	creer_image_tif_rgb(fichier, (uint32_t)W, (uint32_t)H, image);
}



