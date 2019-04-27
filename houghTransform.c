#include "dsp_types.h"
#include "sincostableData.h"
#include <stdlib.h>
#include <stdio.h>
#include "debug.h"
#include "circlePlot.h"
#include "linePlot.h"
#include "mask.h"

#include "houghTransform.h"

void houghTransform_line(IMAGE *houghSpace, IMAGE *imf, int seuil){
	int x;
	int y;
	int theta;
	int rho;
	int W = imf->Width;
	int H = imf->Height;
		
	uint32* hough = (uint32*)houghSpace->datas;
	
	//parcours l'image
	for(y=0; y<H; y++){
		for(x=0; x<W; x++){
			if(imf->datas[y*imf->Width+x]>seuil){
				
				//dessine le cosinus dans Hough
				for(theta = 0; theta<houghThetaSize; theta+=houghDTheta){
					
					rho = (x*tcos[theta]+y*tsin[theta])/32768;
					rho-=houghRhoMin;
					
					hough[rho*houghSpace->Width+theta]+=1;
					
				}
			}
		}
	}
}

void houghTransform_circle(IMAGE *houghSpace_circle, IMAGE *imf, int seuil, int r){
	int x;
	int y;
	int W = imf->Width;
	int H = imf->Height;
		
	for(y=0; y<H; y++){
		for(x=0; x<W; x++){
			if(imf->datas[y*imf->Width+x]>seuil){
				plotCircle(houghSpace_circle, x, y, r, 1);
			}
		}
	}
}

int lookforlines(IMAGE *imf, IMAGE *img, int n, int thetas[], int rhos[]){


	uint32 houghSpace_line[houghRhoSize*houghThetaSize]={0};
	IMAGE hough_line={4, houghThetaSize, houghRhoSize, (uint8*) houghSpace_line};

	houghTransform_line(&hough_line, imf, 127);
	DEBUG_HOUGH(&hough_line)
	FLAG;
	int rho=0;
	int theta=0;
	int max=0;
	int rho_max=0;
	int theta_max=0;
	
	int z;
	for(z=0;z<n;z++){
		for(rho=0; rho<hough_line.Height; rho++){
			for(theta=0; theta<hough_line.Width; theta++){
				if((      (uint32*)hough_line.datas)[rho*hough_line.Width+theta]>max){
					max= ((uint32*)hough_line.datas)[(rho*hough_line.Width+theta)];
					theta_max=theta;
					rho_max=rho;
				}
			}
		}
		plotLine(img, rho_max+houghRhoMin, theta_max+houghThetaMin);
		
		drawFiledRectancle(&hough_line, theta_max-5, rho_max-5, 10, 10, 0);
		
		
		MESSAGE("");
		MESSAGE("Ligne Trouvée!")
		VAR_DUMP(rho_max+houghRhoMin)
		VAR_DUMP(theta_max+houghThetaMin)
		DEBUG_IMAGE_NB(img);
		
		thetas[z]=theta_max+houghThetaMin;
		rhos[z]=rho_max+houghRhoMin;
		
		max=0;
	}
}



	

int lookforcircles(IMAGE *imf, IMAGE *img, int n, int r, int xs[], int ys[]){
	
	uint32 houghSpace_circle[houghRhoSize*houghThetaSize]={0};
	IMAGE  hough_circle;
	hough_circle.type=4;
	hough_circle.Width=imf->Width;
	hough_circle.Height=imf->Height;
	hough_circle.datas=(uint8*) houghSpace_circle;

	houghTransform_circle(&hough_circle, imf, 127, r);
	DEBUG_HOUGH(&hough_circle)
	int y=0;
	int x=0;
	int max=0;
	int y_max=0;
	int x_max=0;
	
	int z;
	for(z=0;z<n;z++){
		for(y=0; y<hough_circle.Height; y++){
			for(x=0; x<hough_circle.Width; x++){
				if((      (uint32*)hough_circle.datas)[y*hough_circle.Width+x]>max){
					max= ((uint32*)hough_circle.datas)[y*hough_circle.Width+x];
					x_max=x;
					y_max=y;
				}
			}
		}
		
		
		plotCircle(img, y_max, x_max, r, 255);
		
		
		drawFiledRectancle(&hough_circle, x_max-5, y_max-5, 10, 10, 0);
		
		MESSAGE("")
		MESSAGE("Cercle Trouvé!")
		VAR_DUMP(x_max)
		VAR_DUMP(y_max)
		DEBUG_IMAGE_NB(img);
		xs[z]=x_max;
		ys[z]=y_max;
		
		max=0;
	}
}



#ifdef DEBUG_OUTPUT
void houghTransform_debug(IMAGE *houghSpace, char *file){
	int x=0;
	int y=0;
	
	PIXEL *image = malloc(sizeof(PIXEL)*(houghSpace->Width)*(houghSpace->Height));
	
	uint32 max=0;
	int xmax=0;
	int ymax=0;
	for(y=0;y<houghSpace->Height;y++){
		for(x=0;x<houghSpace->Width;x++){
			if(((uint32*)houghSpace->datas)[(y*houghSpace->Width+x)]>max){
				max=((uint32*)houghSpace->datas)[(y*houghSpace->Width+x)];
				xmax=x;
				ymax=y;
			}
		}
	}
	
	for(y=0;y<houghSpace->Height;y++){
		for(x=0;x<houghSpace->Width;x++){
			image[(y*houghSpace->Width+x)][ROUGE]= (char)((uint32*)houghSpace->datas)[(y*houghSpace->Width+x)]*255/max;
			image[(y*houghSpace->Width+x)][BLEU] = (char)((uint32*)houghSpace->datas)[(y*houghSpace->Width+x)]*255/max;
			image[(y*houghSpace->Width+x)][VERT] = (char)((uint32*)houghSpace->datas)[(y*houghSpace->Width+x)]*255/max;
		}
	}
	image[(ymax*houghSpace->Width+xmax)][BLEU]=0;
	image[(ymax*houghSpace->Width+xmax)][VERT]=0;
	
	
	creer_image_tif_rgb(file, (uint32_t)houghSpace->Width, (uint32_t)houghSpace->Height, image);
}
#endif //DEBUG_OUTPUT

