#include "image_param.h"
#include "mask_rep_deff.h"

typedef struct Masque
{
	uint8 	(*type)();		// FOURBERIE ceci est un pointeur sur une fonction.
							// on attend logiquement une des fonctions filles
							// listées ci dessous
							
	int 	m[MAX_MASK];    // valeur du mask
							//(pas toujours implementé)
	
							// tout ces élements peuvent être definis d'un
							// coup par une macro de mask_rep_deff.h
							//
	int 	rep[MAX_MASK];  // adresse relative
	int 	sizeMasque;		//
	int 	hMasque;		// taille du masque
	int 	wMasque;		//
	int 	xMasque;		//position du centre du masque
	int 	yMasque;		//
	
	
	float 	gain;			//gain statique en fin de fonction 
							//(pas toujours implementé)
}MASQUE;

/****************************************************************************************
****                         fonction Mère                                           ****
****************************************************************************************/
//application d'un masque
void 			filtreImage	(IMAGE *imf, IMAGE *img, MASQUE *masque);

//detection d'une zone blanche (retourne le centre de gravité ainsi que le poid)
unsigned long 	detectObj		(IMAGE *imf, IMAGE *img, IMAGE *imBuffer, 
								 float *gravX, float *gravY);

/****************************************************************************************
****                         fonction Fille                                          ****
****************************************************************************************/
uint8 filtre_erosion		(IMAGE *imf, int ligne, int colonne, MASQUE *masque);
uint8 filtre_dilatation		(IMAGE *imf, int ligne, int colonne, MASQUE *masque);
uint8 filtre_median			(IMAGE *imf, int ligne, int colonne, MASQUE *masque);
uint8 filtre_convolution	(IMAGE *imf, int ligne, int colonne, MASQUE *masque);

/****************************************************************************************
****                         fonction Cousines                                       ****
****************************************************************************************/
void 			maskInverse		(MASQUE *masque);
int 			tvScan			(IMAGE *imf, IMAGE *img, uint8 seuil);
void 			setImage		(IMAGE *imf, uint8 setVal);
void			multImage		(IMAGE *imf, uint8 gain);
void			divImage		(IMAGE *imf, uint8 gain);
void			offsetImage		(IMAGE *imf, uint8 offst);
void 			copyImage		(IMAGE *imf, IMAGE *img);

void 			intersectImage	(IMAGE *imf, IMAGE *img, IMAGE *imh);

void			addImage		(IMAGE *imf, IMAGE *img, IMAGE *imh, int8 kf, int8 kg);

unsigned long 	poidImage		(IMAGE *imf);
unsigned long	centreGravImage	(IMAGE *imf, float *gravX, float *gravY);

void coloriseImage				(IMAGE *imf, 
								 IMAGE *imR, 
								 IMAGE *imG, 
								 IMAGE *imB, uint8 couleur[][3]);
								 
void labeliseImage(IMAGE *imf, IMAGE *img, int i, int seuil);

int detectAllObj(IMAGE *imf, IMAGE *img, IMAGE *imh, IMAGE *imi, float *gravX, float *gravY, unsigned long *poid);

int drawFiledRectancle(IMAGE *imf, int x0, int y0, int W, int H, uint8 val);

