#include <stdio.h>
#include <stdlib.h>

#include "image_param.h"
#include "minitifRGB.h"

void load_image(uint8_t f[][W], char *file_name){
	
	FILE* fichier;
	fichier=fopen(file_name, "r");
	char s[255];
	
	if (fichier != NULL)
    {

        // On peut lire et écrire dans le fichier	
		int d;
		int c;
		int b;
		int a;
		
		// les deux premières ligne ne servent à rien
		fscanf(fichier, "%d %d %d %d\n", &a, &b, &c, &d);
		fscanf(fichier, "%s", &s);
		
		int i=0;
		while(fscanf(fichier, "%2s%2X%2X%2X%2X", &s, &a, &b, &c, &d)==5){
			f[0][i+0]=d;
			f[0][i+1]=c;// ! FOURBERIE ! 
			f[0][i+2]=b;// les bytes sont distribués à l'envers LSByte first
			f[0][i+3]=a;
			i+=4;
			
		}
		printf("Bien lus!\n");
		fclose(fichier);
    }
    else printf("Impossible d'ouvrir le fichier test.txt");
}

void save_image(uint8_t R[][W], uint8_t G[][W], uint8_t B[][W], char file_name[255]){
	int x=0;
	int y=0;
	
	PIXEL image[H][W];
	
	for(y=0;y<H;y++){
		for(x=0;x<W;x++){
			image[y][x][ROUGE]= R[y][x];
			image[y][x][BLEU] = B[y][x];
			image[y][x][VERT] = G[y][x];
		}
	}
	creer_image_tif_rgb(file_name, (uint32_t)W, (uint32_t)H, image);
}
