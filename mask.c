#include "dsp_types.h"
#include "image_param.h"
#include "mask.h"
#include <stdio.h>
#include <stdlib.h>


//fonction de comparaison pour l'utilisation de qsort
int fonction_comparaison (const void * a, const void * b){ return ( *(uint8*)a - *(uint8*)b );}


/****************************************************************************************
****                         fonction Mère                                           ****
****************************************************************************************/

void traitementImage(uint8 f[][W], uint8 g[][W], MASQUE *masque){
	int ligneImage, colonneImage;
	//parcours chaques pixels de l'image
	for (ligneImage = masque->yMasque; ligneImage < H - masque->hMasque + masque->yMasque; ligneImage++){
	  for (colonneImage = masque->xMasque; colonneImage < W - masque->wMasque + masque->xMasque; colonneImage++){
		  //traitement de l'image
		  // on appele la fonction se trouvant sur le pointeur masque->type.,
		  // on lui donne la position du pixel et le masque
		  g[ligneImage][colonneImage] = (*masque->type)(f, ligneImage, colonneImage, masque);
	  }
	}
}

unsigned long detectObj(uint8 f[][W], uint8 g[][W], uint8 buffer[][W], float *gravX, float *gravY){
	MASQUE dilat_croix = {filtre_dilatation, {0}, MASK_croix3, 1};

	setImage(g,      0);
	setImage(buffer, 0);

	if(!tvScan(f, g, 127))
		return 0;

	unsigned long poidInit=0;
	unsigned long poidFinal=poidImage(g);

	int i=0;

	while(poidInit!=poidFinal){
		i++;

		traitementImage(g, buffer, &dilat_croix);
		intersectImage (f, buffer, g);

		poidInit=poidFinal;
		poidFinal=poidImage(g);
	}
	substractImage(f, g, f);
	return centreGravImage(g, gravX, gravY);
}

/****************************************************************************************
****                         fonctions Filles                                        ****
*****************************************************************************************
* ces fonctions sont appelées une foix par pixel, et retourne la valeur qu'il devra prendre.
* 
* 
*/

uint8 filtre_erosion(uint8 f[][W], int ligne, int colonne, MASQUE *masque){

	uint8 pixVal=255;
	int i;
	if(f[ligne][colonne]==0)
		return 0;
	for(i=0; i < masque->sizeMasque; i++){
		if(pixVal>*(&f[ligne][colonne] + masque->rep[i])){
			pixVal=*(&f[ligne][colonne] + masque->rep[i]);
		}
	}
	return pixVal;
}

uint8 filtre_dilatation(uint8 f[][W], int ligne, int colonne, MASQUE *masque){
	uint8 pixVal=0;
	int i;
	if(f[ligne][colonne]==255)
		return 255;
	for(i=0; i < masque->sizeMasque; i++){
		if(pixVal<*(&f[ligne][colonne] + masque->rep[i])){
			pixVal=*(&f[ligne][colonne] + masque->rep[i]);
		}
	}
	return pixVal;
}

uint8 filtre_median(uint8 f[][W], int ligne, int colonne, MASQUE *masque){
	uint8 buffer[MAX_MASK];
	int i;
	for(i=0; i < masque->sizeMasque; i++){
		buffer[i] = *(&f[ligne][colonne] + masque->rep[i]);
	}
	qsort(buffer, masque->sizeMasque, sizeof(uint8), fonction_comparaison);
	return buffer[masque->sizeMasque/2];
}
uint8 filtre_convolution(uint8 f[][W], int ligne, int colonne, MASQUE *masque){
	int i;
	int pixVal= 0;
	for(i=0; i < masque->sizeMasque; i++){
		pixVal += *(&f[ligne][colonne] + masque->rep[i]) * masque->m[i];
	}
	pixVal*=masque->gain;

	if (pixVal > 255)	//vérifier l'overflow
		pixVal = 255;
	else if (pixVal < 0)
		pixVal = 0;
	return (uint8)pixVal;
}


/****************************************************************************************
****                         fonctions Cousines                                      ****
****************************************************************************************/
void maskInverse(MASQUE *masque){
	  if(masque->type==filtre_erosion)
		  masque->type=filtre_dilatation;
	  else if(masque->type==filtre_dilatation)
		  masque->type=filtre_erosion;
	  else
		  printf("ERREUR type de masque inconnus (maskInverse)");

	  int i;
	  for(i=0; i< masque->sizeMasque; i++){
		  masque->rep[i]*=-1;
	  }
}
int tvScan(uint8 f[][W], uint8 g[][W], uint8 seuil){
	int x;
	int y;
	for(y=0; y<H; y++){
		for(x=0; x<W; x++){
			if(f[y][x]>seuil){
				g[y][x]=f[y][x];
				return 1;
			}
		}
	}
	return 0;
}
void setImage(uint8 f[][W], uint8 setVal){
	int x;
	int y;
	for(y=0; y<H; y++){
		for(x=0; x<W; x++){
			f[y][x]=0;
		}
	}
}
void copyImage(uint8 f[][W], uint8 g[][W]){
	int x;
	int y;
	for(y=0; y<H; y++){
		for(x=0; x<W; x++){
			g[y][x]=f[y][x];
		}
	}
}
void intersectImage(uint8 f[][W], uint8 g[][W], uint8 h[][W]){
	int x;
	int y;
	for(y=0; y<H; y++){
		for(x=0; x<W; x++){
			h[y][x]=f[y][x] & g[y][x];
		}
	}
}
void substractImage(uint8 f[][W], uint8 g[][W], uint8 h[][W]){
	int x;
	int y;
	for(y=0; y<H; y++){
		for(x=0; x<W; x++){
			h[y][x]=f[y][x] - g[y][x];
		}
	}
}
unsigned long poidImage(uint8 f[][W]){
	unsigned long poid=0;
	int x;
	int y;
	for(y=0; y<H; y++){
		for(x=0; x<W; x++){
			poid+=f[y][x];
		}
	}
	return poid;
}
unsigned long centreGravImage(uint8 f[][W], float *gravX, float *gravY){
	unsigned long poidPondX=0;
	unsigned long poidPondY=0;
	unsigned long poid     =0;
	int x;
	int y;
	for(y=0; y<H; y++){
		for(x=0; x<W; x++){
			poidPondX+=f[y][x]*x;
			poidPondY+=f[y][x]*y;
			poid     +=f[y][x];
		}
	}
	*gravX=(float)poidPondX/(float)poid;
	*gravY=(float)poidPondY/(float)poid;
	return poid;
}
unsigned long	coloriseImage	(uint8 f[][W], uint8 R[][W], uint8 G[][W], 
								 uint8 B[][W], uint8 couleur[]){
	int x;
	int y;
	for(y=0; y<H; y++){
		for(x=0; x<W; x++){								 
			R[y][x]+=(int)f[y][x]*couleur[0]>>8;				 
			G[y][x]+=(int)f[y][x]*couleur[1]>>8;				 
			B[y][x]+=(int)f[y][x]*couleur[2]>>8;
		}
	}
}
