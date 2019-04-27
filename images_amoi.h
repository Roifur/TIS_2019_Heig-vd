#include <stdio.h>
#include <stdlib.h>

#include "image_param.h"
#include "../minitifRGB.h"

void load_image(IMAGE *imf, 
				char *file_name);

void save_image(IMAGE *R, 
				IMAGE *G, 
				IMAGE *B, 
				char *file_name);
