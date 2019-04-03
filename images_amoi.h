#include <stdio.h>
#include <stdlib.h>

#include "image_param.h"
#include "minitifRGB.h"

void load_image(uint8_t f[][W], 
				char *file_name);

void save_image(uint8_t R[][W], 
				uint8_t G[][W], 
				uint8_t B[][W], 
				char *file_name);

