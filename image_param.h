#ifndef _IMAGE_PARAM_H_
#define _IMAGE_PARAM_H_

#include "dsp_types.h"



#define INIT_IMAGE(a, W, H) IMAGE im ## a={0}; im ## a.type=1; im ## a.datas=a; im ## a.Width=W;  im ## a.Height=H

typedef struct Image
{
	int		type;			//type d'image
	int		Width;				//largeur
	int		Height;				//hauteur
	uint8   *datas;				//pointeur sur les données
}IMAGE;

#endif // _IMAGE_PARAM_H_
