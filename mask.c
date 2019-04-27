#include "dsp_types.h"
#include "image_param.h"
#include "mask.h"
#include <stdio.h>
#include <stdlib.h>
#include "debug.h"


#define BOUCLE_SUR_IMAGE(imf)\
	int W = imf->Width;\
	int H = imf->Height;\
	int x;\
	int y;\
	for(y=0; y<H; y++)\
		for(x=0; x<W; x++)
		
#define CHECK_OVERFlOW(a)\
	if (a > 255)\
		a = 255;\
	else if (a < 0)\
		a = 0;



//fonction de comparaison pour l'utilisation de qsort
int fonction_comparaison (const void * a, const void * b){ return ( *(uint8*)a - *(uint8*)b );}


/****************************************************************************************
****                         fonction Mère                                           ****
****************************************************************************************/

void traitementImage(IMAGE *imf, IMAGE *img, MASQUE *masque){
	int ligneImage, colonneImage;
	//parcours chaques pixels de l'image
	int W		    = imf->Width;
	int H		    = imf->Height;
	int MaskW	    = masque->wMasque;
	int MaskH	    = masque->hMasque;
	int yCentreMask = masque->yMasque;
	int xCentreMask = masque->xMasque;
	
	for (ligneImage = yCentreMask;ligneImage < H - MaskH + yCentreMask; ligneImage++){
	  for (colonneImage = xCentreMask; colonneImage < W - MaskW + xCentreMask; colonneImage++){
		  //traitement de l'image
		  // on appele la fonction se trouvant sur le pointeur masque->type.,
		  // on lui donne la position du pixel et le masque
		  img->datas[ligneImage*W+colonneImage] = (*masque->type)(imf, ligneImage, colonneImage, masque);
	  }
	}
}

unsigned long detectObj(IMAGE *imf, IMAGE *img, IMAGE *imBuffer, float *gravX, float *gravY){
	int W		    = imf->Width;
	int H		    = imf->Height;
	
	MASQUE dilat_croix = {filtre_dilatation, {0}, MASK_croix3(W), 1};

	setImage(img,      0);
	setImage(imBuffer, 0);

	if(!tvScan(imf, img, 127))
		return 0;

	unsigned long poidInit=0;
	unsigned long poidFinal=poidImage(img);

	int i=0;

	while(poidInit!=poidFinal){
		i++;

		traitementImage(img, imBuffer, &dilat_croix);
		intersectImage (imf, imBuffer, img);

		poidInit=poidFinal;
		poidFinal=poidImage(img);
	}
	addImage(imf, img, imf, 1, -1);
	return centreGravImage(img, gravX, gravY);
}

int detectAllObj(IMAGE *imf, IMAGE *img, IMAGE *imh, IMAGE *imi, float gravX[], float gravY[], unsigned long poid[]){
	int W		    = imf->Width;
	int H		    = imf->Height;
	int numobj=-1;
	
	do{// boucle sur tous les objets
		numobj++;
		poid[numobj] = detectObj(imf, img, imh, &gravX[numobj], &gravY[numobj]);
		
		if(poid[numobj]>0){
			putchar('.');
			putchar('\n');
			DEBUG_IMAGE_COULEUR(imf, img, img);
			
			labeliseImage(img, imi, numobj+1, 127); 
		}
	}while(poid[numobj]);
	return numobj;
}


/****************************************************************************************
****                         fonctions Filles                                        ****
*****************************************************************************************
* ces fonctions sont appelées une foix par pixel, et retourne la valeur qu'il devra prendre.
* 
* 
*/

uint8 filtre_erosion(IMAGE *imf, int ligne, int colonne, MASQUE *masque){
	int W = imf->Width;
	int H = imf->Height;

	uint8 pixVal=255;
	int i;
	if(imf->datas[ligne*W+colonne]==0)
		return 0;
	for(i=0; i < masque->sizeMasque; i++){
		if(pixVal>   imf->datas[ligne*W+colonne+ masque->rep[i]]){
			pixVal=  imf->datas[ligne*W+colonne+ masque->rep[i]];
			//printf("%d\t%d\t%d\t%d\n", ligne, colonne, i, pixVal);
		}
	}
	return pixVal;
}

uint8 filtre_dilatation(IMAGE *imf, int ligne, int colonne, MASQUE *masque){
	int W = imf->Width;
	uint8 pixVal=0;
	int i;
	if(imf->datas[ligne*W+colonne]==255)
		return 255;
	for(i=0; i < masque->sizeMasque; i++){
		if(pixVal<  imf->datas[ligne*W+colonne + masque->rep[i]]){
			pixVal= imf->datas[ligne*W+colonne + masque->rep[i]];
		}
	}
	return pixVal;
}

uint8 filtre_median(IMAGE *imf, int ligne, int colonne, MASQUE *masque){
	int W = imf->Width;
	uint8 buffer[MAX_MASK];
	int i;
	for(i=0; i < masque->sizeMasque; i++){
		buffer[i] = imf->datas[ligne*W+colonne+ masque->rep[i]];
	}
	qsort(buffer, masque->sizeMasque, sizeof(uint8), fonction_comparaison);
	return buffer[masque->sizeMasque/2];
}
uint8 filtre_convolution(IMAGE *imf, int ligne, int colonne, MASQUE *masque){
	int W = imf->Width;
	int i;
	int pixVal= 0;
	for(i=0; i < masque->sizeMasque; i++){
		pixVal +=imf->datas[ligne*W+colonne + masque->rep[i]] * masque->m[i];
	}
	pixVal*=masque->gain;

	CHECK_OVERFlOW(pixVal)
	return (uint8)pixVal;
}


/****************************************************************************************
****                         fonctions Cousines                                      ****
****************************************************************************************/
void maskInverse(MASQUE *masque){
	if(	masque->type==filtre_erosion)
		masque->type= filtre_dilatation;
		
	else if(masque->type==filtre_dilatation)
		    masque->type= filtre_erosion;
	else
		printf("ERREUR type de masque inconnus (maskInverse)");

	int i;
	for(i=0; i< masque->sizeMasque; i++){
		masque->rep[i]*=-1;
	}
}
int tvScan(IMAGE *imf, IMAGE *img, uint8 seuil){
	BOUCLE_SUR_IMAGE(imf){
		if(imf->datas[y*W+x]>seuil){
			img->datas[y*W+x]=imf->datas[y*W+x];
			return 1;
		}
	}
	return 0;
}
void setImage(IMAGE *imf, uint8 setVal){
	BOUCLE_SUR_IMAGE(imf){
		imf->datas[y*W+x]=0;
	}
}
void copyImage(IMAGE *imf, IMAGE *img){
	BOUCLE_SUR_IMAGE(imf){
			img->datas[y*W+x]=imf->datas[y*W+x];
		}
}
void intersectImage(IMAGE *imf, IMAGE *img, IMAGE *imh){
	BOUCLE_SUR_IMAGE(imf){
		imh->datas[y*W+x]=imf->datas[y*W+x] & img->datas[y*W+x];
	}
}
void addImage(IMAGE *imf, IMAGE *img, IMAGE *imh, float kf, float kg){
	int pixVal;
	BOUCLE_SUR_IMAGE(imf){
		pixVal=imf->datas[y*W+x]*kf + img->datas[y*W+x]*kg;
		CHECK_OVERFlOW(pixVal);
		imh->datas[y*W+x]=pixVal;
	}
}
unsigned long poidImage(IMAGE *imf){
	unsigned long poid=0;
	BOUCLE_SUR_IMAGE(imf){
			poid+=imf->datas[y*W+x];
		}
	return poid;
}
unsigned long centreGravImage(IMAGE *imf, float *gravX, float *gravY){
	unsigned long poidPondX=0;
	unsigned long poidPondY=0;
	unsigned long poid     =0;
	BOUCLE_SUR_IMAGE(imf){
		poidPondX+=imf->datas[y*W+x]*x;
		poidPondY+=imf->datas[y*W+x]*y;
		poid     +=imf->datas[y*W+x];
	}
	*gravX=(float)poidPondX/(float)poid;
	*gravY=(float)poidPondY/(float)poid;
	return poid;
}
void coloriseImage(IMAGE *imf, IMAGE *imR, IMAGE *imG, 
								 IMAGE *imB, uint8 couleur[][3]){
	BOUCLE_SUR_IMAGE(imf){								 
		imR->datas[y*W+x]=couleur[imf->datas[y*W+x]][0];
		imG->datas[y*W+x]=couleur[imf->datas[y*W+x]][1];
		imB->datas[y*W+x]=couleur[imf->datas[y*W+x]][2];
	}
}
void labeliseImage(IMAGE *imf, IMAGE *img, int i, int seuil){
	BOUCLE_SUR_IMAGE(imf){
		if(imf->datas[y*W+x]>seuil){
			img->datas[y*W+x]=i;
		}
	}
}

