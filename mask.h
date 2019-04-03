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
void 			traitementImage	(uint8 f[][W], uint8 g[][W], MASQUE *masque);

//detection d'une zone blanche (retourne le centre de gravité ainsi que le poid)
unsigned long 	detectObj		(uint8 f[][W], uint8 g[][W], uint8 buffer[][W], 
								 float *gravX, float *gravY);

/****************************************************************************************
****                         fonction Fille                                          ****
****************************************************************************************/
uint8 filtre_erosion		(uint8 f[][W], int ligne, int colonne, MASQUE *masque);
uint8 filtre_dilatation		(uint8 f[][W], int ligne, int colonne, MASQUE *masque);
uint8 filtre_median			(uint8 f[][W], int ligne, int colonne, MASQUE *masque);
uint8 filtre_convolution	(uint8 f[][W], int ligne, int colonne, MASQUE *masque);

/****************************************************************************************
****                         fonction Cousines                                       ****
****************************************************************************************/
void 			maskInverse		(MASQUE *masque);
int 			tvScan			(uint8 f[][W], uint8 g[][W], uint8 seuil);
void 			setImage		(uint8 f[][W], uint8 setVal);
void 			copyImage		(uint8 f[][W], uint8 g[][W]);
void 			intersectImage	(uint8 f[][W], uint8 g[][W], uint8 h[][W]);
void			substractImage	(uint8 f[][W], uint8 g[][W], uint8 h[][W]);
unsigned long 	poidImage		(uint8 f[][W]);
unsigned long	centreGravImage	(uint8 f[][W], float *gravX, float *gravY);

unsigned long	coloriseImage	(uint8 f[][W], 
								 uint8 R[][W], 
								 uint8 G[][W], 
								 uint8 B[][W], 
								 uint8 couleur[]);





