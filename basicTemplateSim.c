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

/* runtime include files */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


/////////////////////// definition de l'environnement
#ifdef __unix__
	#define SIMULATION
	#define DEBUG_OUTPUT
#elif defined _WIN32 || _WIN64
	#define SIMULATION
	#define DEBUG_OUTPUT
#endif


/////////////////////// fonctions de debuggage: save_image
#ifdef DEBUG_OUTPUT
	char titre[256];
	int numero_test=0;
	#define SAVE_IMAGE_NB(im) numero_test++; \
							sprintf(titre, "test_%03d-NB.tif", numero_test);\
							save_image(im, im, im, titre);
	#define SAVE_IMAGE_COULEUR(R, G, B) numero_test++; \
							sprintf(titre, "test_%03d-COUL.tif", numero_test);\
							save_image(R, G, B, titre);
#else
	#define SAVE_IMAGE_NB(im)
	#define SAVE_IMAGE_COULEUR(R, G, B)
#endif



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
#include "basicTemplateSim.h"

#include "mask.h"
#include "histogram.h"

/////////////////////// librairies pour le debuggage, chargement et sauvegarde image
#ifdef SIMULATION
#include "image_param.h"
#include "images_amoi.h"
#endif



/////////////////////// 
/////////////////////// 
/////////////////////// Fin des dirrectives de préprocesseur
/////////////////////// 
/////////////////////// 

// affectation memoires: images
uint8 f[H][W];
uint8 g[H][W];
uint8 h[H][W];
uint8 i[H][W];

uint8 R[H][W];
uint8 G[H][W];
uint8 B[H][W];

// création d'un système à 16 couleurs
//					  R  G  B
uint8 couleur[16][3]={ 
	{0x00, 0x00, 0xFF}, //blue bleu
	{0x00, 0xFF, 0x00}, //lime vert citron
	{0x80, 0x00, 0x80}, //purple violet
	{0xFF, 0xFF, 0xFF}, //white blanc
	{0xFF, 0x00, 0xFF}, //fuchsia / magenta fuchsia (couleur), //fuchsia
	{0x80, 0x00, 0x00}, //maroon Bordeaux (couleur), //bordeaux
	{0xFF, 0x00, 0x00}, //red rouge
	{0xFF, 0xFF, 0x00}, //yellow jaune
	{0x00, 0xFF, 0xFF}, //aqua / cyan vert d'eau, //bleu-vert eau
	{0x00, 0x80, 0x00}, //green vert
	{0x00, 0x00, 0x80}, //navy bleu marine
	{0xc0, 0xc0, 0xc0}, //silver argent (couleur), //argent
	{0x00, 0x00, 0x00}, //black noir
	{0x80, 0x80, 0x80}, //gray gris
	{0x80, 0x80, 0x00}, //olive olive (couleur), //jaune olive
	{0x00, 0x80, 0x80}, //teal sarcelle (couleur), //sarcelle
};


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

void processingTask(void)
{
	//MASQUE median = {filtre_erosion,{0}, MASK_croix3, 1};

	//filtre : contraste 	-1, -4, -1, -4, 26, -4, -1, -4, -1
			// vertical 	1, 2, 1, 0, 0, 0, -1, -2, -1
			// horizontal 	1, 0, -1, 2, 0, -2, 1, 0, -1
			// vert+horiz	2, 4, 2, 4, 8, 4, 2, 4, 2


	//traitementImage(f, g, &median);

	
	//load_image(f, "lena256x256.dat");
	
	//SAVE_IMAGE_NB(f);
	
	//HISTOGRAM hist;
	//calculHistogram(f, hist);
	//traceHistogram(hist, "fichier.tif");
	
	
	
	
	
	load_image(f, "form-1.dat");
	
	SAVE_IMAGE_NB(f);

	MASQUE masque = {filtre_erosion, {0}, MASK_carre9, 1};
	traitementImage(f, g, &masque);
	
	SAVE_IMAGE_NB(g);


	maskInverse(&masque); //erosion devient dilatation et le mask est inversé
	
	setImage(f, 0);//effacement image f (
	traitementImage(g, f, &masque);

	SAVE_IMAGE_NB(f);

	float graviteX=0;
	float graviteY=0;
	unsigned long poid;
	int numobj=0;
	do{// boucle sur tous les objets
		numobj++;
		poid = detectObj(f, g, h, &graviteX, &graviteY);
		
		if(poid>0){ //affichage du rapport dans la console
			printf("Element n° %2d \t poid: %ldpx \t X: %f\t Y: %f\n",
					numobj, 
					poid/255, 
					graviteX, 
					graviteY);
					SAVE_IMAGE_COULEUR(f, g, g);
					
					// affectation de la couleur sur tous les points de g, et addition sur RGB
					coloriseImage(g, R, G, B, couleur[numobj-1]); 
		}
	}while(poid);
	
	SAVE_IMAGE_COULEUR(R, G, B);
	

	printf("Programme fini\n");

	#ifndef SIMULATION
	while(1) {
		TSK_sleep(10);
	}
	#endif
}
