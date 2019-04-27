#ifndef _DEBUG_H_
#define _DEBUG_H_



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
	#include "dsp_types.h"
	#include <stdlib.h>
	#include <stdio.h>

	#include "../minitifRGB.h"
	#include "images_amoi.h"
	
	#ifdef MAIN
		int numero_test=0;
	#undef MAIN
	#else
		extern 	int numero_test;
	#endif

	char titre[256];

	#define LOAD_IMAGE(imf, file) load_image(imf, file);
	#define FLAG		printf("%20s  L %d FLG\n", __FILE__, __LINE__);
	#define VAR_DUMP(a) printf("%20s  L %d VAR (%s) %d\n", __FILE__, __LINE__, #a, a);
	#define MESSAGE(a)  printf("%20s  L %d MSG %s\n", __FILE__, __LINE__, a);
	#define DEBUG_IMAGE_NB(im) numero_test++; \
							sprintf(titre, "test_%03d-NB.tif", numero_test);\
							MESSAGE(titre);\
							save_image(im, im, im, titre);
							
	#define DEBUG_IMAGE_COULEUR(R, G, B) numero_test++; \
							sprintf(titre, "test_%03d-COUL.tif", numero_test);\
							MESSAGE(titre);\
							save_image(R, G, B, titre);
							

#else
	#define DEBUG_IMAGE_NB(im)
	#define DEBUG_IMAGE_COULEUR(R, G, B)
	#define LOAD_IMAGE(imf, file)
	#define FLAG
	#define VAR_DUMP(a)
	#define MESSAGE(a)
#endif



#endif //_DEBUG_H_
