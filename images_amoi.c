#include <stdio.h>
#include <stdlib.h>

#include "image_param.h"
#include "dsp_types.h"
#include "debug.h"

void load_image(IMAGE *imf, char *file_name){
	
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
			imf->datas[i+0]=d;
			imf->datas[i+1]=c;// ! FOURBERIE ! 
			imf->datas[i+2]=b;// les bytes sont distribués à l'envers, LSByte first
			imf->datas[i+3]=a;
			i+=4;
			
		}
		printf("Bien lus!\n");
		fclose(fichier);
    }
    else printf("Impossible d'ouvrir le fichier test.txt");
}

#ifdef DEBUG_OUTPUT

void save_image(IMAGE *R, IMAGE *G, IMAGE *B, char file_name[255]){
	
	int x=0;
	int y=0;
	
	PIXEL *image = malloc(sizeof(PIXEL)*R->Width*R->Height);
	
	for(y=0;y<R->Height;y++){
		for(x=0;x<R->Width;x++){
			image[(y*R->Width+x)][ROUGE]= R->datas[y*R->Width+x];
			image[(y*R->Width+x)][BLEU] = B->datas[y*R->Width+x];
			image[(y*R->Width+x)][VERT] = G->datas[y*R->Width+x];
		}
	}
	creer_image_tif_rgb(file_name, (uint32_t)R->Width, (uint32_t)R->Height, image);
	free(image);
}
#endif //DEBUG_OUTPUT
