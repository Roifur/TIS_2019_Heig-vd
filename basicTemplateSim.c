/*
 * File     : basicTemplaceSim.c
 * Author   : Pierre BRESSY
 * Company  : HEIG-VD
 * Created  : Thu Feb 7 11:00:12 2011
 * Purpose  :

 |   Date   | By  | Description of changes in basicTemplaceSim.c
 +----------+-----+-----------------------------------------------------------+
 |          |     |

*/
#include "basicTemplateSim.h"


/* runtime include files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/////////////////////// librairies pour le debuggage, chargement et sauvegarde image
#define MAIN
#include "debug.h"

/////////////////////// Librairies necesaires à la carte DSP
#ifndef SIMULATION
/* DSP/BIOS include files */
#include <std.h>
#include <log.h>
#include <sys.h>
#include <gio.h>
#include <iom.h>
/* DSP/BIOS objects include files (from .tcf file) */
#include "basicTemplateSimcfg.h"
#endif


/////////////////////// Librairies accessoires 
/* Application include files */
#include "dsp_types.h"
#include "mask.h"
#include "histogram.h"
#include "houghTransform.h"
#include "color.h"
#include "circlePlot.h"



#include "linePlot.h"





////////////////////////////////////////////////////////////////////////////////////////
////							prototypes de fonctions								////
////////////////////////////////////////////////////////////////////////////////////////

void processingTask(void);

////////////////////////////////////////////////////////////////////////////////////////
////							fonctions											////
////////////////////////////////////////////////////////////////////////////////////////

void main(void) {
	printf("BASIC TEMPLATE FOR SIMULATION\n");
	processingTask();
	return;
}


// allocation de la mémoire



#define W_lena 256
#define H_lena 256

#define W_loto 360
#define H_loto 250


int W = W_lena;
int H = H_lena;
uint8 f[W_lena*H_lena]={0};
uint8 g[W_lena*H_lena]={0};
uint8 h[W_lena*H_lena]={0};
uint8 R[W_lena*H_lena]={0};
uint8 G[W_lena*H_lena]={0};
uint8 B[W_lena*H_lena]={0};
//uint8 z[W_lena*H_lena]={0};
//uint8 q[W_lena*H_lena]={0};
//uint8 r[W_lena*H_lena]={0};

uint32 houghSpace_circle[W_lena*H_lena]={0};
IMAGE hough_circle={4, W_lena, H_lena, (uint8*) houghSpace_circle};


void processingTask(void)
{


	INIT_IMAGE(f, W, H);
	INIT_IMAGE(g, W, H);
	INIT_IMAGE(h, W, H);
	INIT_IMAGE(R, W, H);
	INIT_IMAGE(G, W, H);
	INIT_IMAGE(B, W, H);

	LOAD_IMAGE(&imf, "lena256x256.dat");

	// detourage
	MASQUE sobel1=SOBEL_Hori(W_lena);
	MASQUE sobel2=SOBEL_Vert(W_lena);
	MASQUE ero3={filtre_erosion,{0}, MASK_croix3(W), 1};
	
	filtreImage(&imf, &img, &sobel1);
	filtreImage(&imf, &imh, &sobel2);
	
	divImage(&img, 2);
	divImage(&imh, 2);
	
	addImage(&img, &imh, &imf, 1, 1);
	
	
	//filtreImage(&imf, &img, &ero3);
	
	
	
	//copyImage(&img, &imf);
	
	

	plotCircle(&imf, 50, 50, 30, 255);
	
	
	plotLine(&imf, -10, -45);
	
	plotLine(&imf, 40, 0);
	
	plotLine(&imf, -40, -90);
	plotLine(&imf, -50, -80);
	
	
	
	copyImage(&imf, &img);
	setImage(&imh, 0);
	setImage(&img, 0);
	
	//DEBUG_IMAGE_NB(&imf);
	
	//LOAD_IMAGE(&imf, "lena256x256.dat");
	//LOAD_IMAGE(&img, "lena256x256.dat");
	
	
	//LOAD_IMAGE(f, "lena256x256.dat");
	DEBUG_IMAGE_NB(&imf);
	
	
	#define t 10
	int xs[t]={0};
	int ys[t]={0};
	
	int thetas[t]={0};
	int rhos[t]={0};
	
	lookforcircles(&imf, &img, 1, 30, xs, ys);
	
	lookforlines(&imf, &img, 4, thetas, rhos);
	
	int i;
	for(i=0;i<t; i++){
		
		printf("ligne n %2d: theta = %4d, rho = %4d\n", i, thetas[i], rhos[i]);
	}
	for(i=0;i<t; i++){
		
		printf("cercle n %2d: X = %4d, Y = %4d\n", i, xs[i], ys[i]);
	}
	
	
	
	
	//addImage(&imf, &img, &imf, 1, 1);
	DEBUG_IMAGE_COULEUR(&imf, &img, &imf);
	
	
	//printf("\nhough_line\t%d"

	/*
	MASQUE sobel1=SOBEL_Hori(W_lena);
	MASQUE sobel2=SOBEL_Vert(W_lena);

	
	filtreImage(&imf, &img, &sobel1);
	filtreImage(&imf, &imh, &sobel2);
	addImage(&img, &imh, &imh, 1, 1);
	DEBUG_IMAGE_NB(&imh);
	
	filtreImage(&imf, &img, &sobel1);
	
	DEBUG_IMAGE_NB(&img);
	
	
	filtreImage(&imf, &img, &sobel2);
	
	DEBUG_IMAGE_NB(&img);
	*/
	
	
	
	///////////////////////////////////////////////////////////////7
	
	// loto
	
	/*
	LOAD_IMAGE(&imf, "form-1.dat");
	DEBUG_IMAGE_NB(&imf);
	
	MASQUE masque = {filtre_erosion, {0}, MASK_carre9(W_lena), 1};
	filtreImage(&imf, &img, &masque);
	
	DEBUG_IMAGE_NB(&img);


	maskInverse(&masque); //érosion devient dilatation et le mask est inversé
	
	setImage(&imf, 0);//effacement image f (pour eviter les effets des bords de l'image)
	
	filtreImage(&img, &imf, &masque);

	DEBUG_IMAGE_NB(&imf);



	
	float 			gravX[255]	= {0};
	float 			gravY[255]	= {0};
	unsigned long 	poid[255]	= {0};
	detectAllObj(&imf, &img, &imh, &imi, gravX, gravY, poid);
	
	
	//int numobj=0;
	//printf("%d\n", poid[numobj]);
	//while(poid[numobj]){
		//printf("Element n %2d \t poid: %ld px \t X: %f\t Y: %f\n",
			//numobj, 
			//poid[numobj]/255, 
			//gravX[numobj], 
			//gravY[numobj]);
		//numobj++;
	//}
	
	//coloriseImage(&imi, &imR, &imG, &imB, couleur); 
	//DEBUG_IMAGE_COULEUR(&imR, &imG, &imB);
	
	
	
	
	////////////////////////////////////////////////////////// 
	
	//Lena
	
	
	//LOAD_IMAGE(&imz, "lena256x256.dat");
	//DEBUG_IMAGE_NB(&imz)
	//HISTOGRAM hist={0};
	//calculHistogram(&imz, hist);
	//DEBUG_HIST(hist);
	//MASQUE sobel1=SOBEL_Hori(256);
	//MASQUE sobel2=SOBEL_Vert(256);
	
	
	
	//filtreImage(&imz, &imq, &sobel1);
	//filtreImage(&imz, &imr, &sobel2);
	//DEBUG_IMAGE_NB(&imq)
	//DEBUG_IMAGE_NB(&imr)
	//addImage(&imq, &imr, &imr, 1, 1);
	//DEBUG_IMAGE_NB(&imr)
	
	*/
	
	

	printf("Programme fini\n");

	#ifndef SIMULATION
	while(1) {
		TSK_sleep(10);
	}
	#endif
}
