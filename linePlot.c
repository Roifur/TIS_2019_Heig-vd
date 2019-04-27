#include "sincostableData.h"
#include "image_param.h"
#include "linePlot.h"
#include "debug.h"


#define A (1<<0)
#define B (1<<1)
#define C (1<<2)
#define D (1<<3)

//img = image to draw
void setPixel(IMAGE *img, int x, int y, int value){
	img->datas[y*img->Width+x] = value;
}

uint8 interceptY(int rho, int theta, int x, int* y, int yMin, int yMax) {
	int32 cosTheta = tcos[theta + BASE_ANGLE];
	int32 sinTheta = tsin[theta + BASE_ANGLE];
	if(sinTheta!=0){
		*y = rho * sinTheta/ANGLE_DIVISION + (cosTheta * (rho * cosTheta/ANGLE_DIVISION - x)/ sinTheta);
	}else{
		*y = (int32)1<<30;//bonne aproximation de l'infini
		MESSAGE("tentative de division par 0");
	}
	return (*y <= yMax && *y >= yMin);
}

uint8 interceptX(int rho, int theta, int* x, int y, int xMin, int xMax){
	int32 cosTheta = tcos[theta + BASE_ANGLE];
	int32 sinTheta = tsin[theta + BASE_ANGLE];
	if(cosTheta!=0){
		*x = rho * cosTheta /ANGLE_DIVISION- (sinTheta * (y - rho * sinTheta/ANGLE_DIVISION)/cosTheta);
	}else{
		*x = (int32)1<<30;//bonne aproximation de l'infini
		MESSAGE("tentative de division par 0");
	}
	return (*x <= xMax && *x >= xMin);
}

void drawLine(IMAGE *img, int x0, int y0, int x1, int y1){

	
	int32  dx = 0;
	int32  dy = 0;
	int32  sx = 1;
	int32  sy = 1;
	int32 err = 0;
	int32   e2;

	dx  = x1 > x0 ? x1-x0 : x0-x1 ;
	dy  = y1 > y0 ? y1-y0 : y0-y1 ;

	err = dx - dy;
	sx = x0 < x1 ? 1 : -1;
	sy = y0 < y1 ? 1 : -1;

	while(x0 != x1 || y0 != y1){
		setPixel(img, x0, y0, 255);
		e2 = err << 1;
		if(e2 > -dy) {
			err -= dy;
			x0  += sx;
		}
		if(e2 < dx) {
			err += dx;
			y0  += sy;
		}
	}
}


//img = image to draw
void plotLine(IMAGE *img, int rho, int theta) {
	int xUp, xDown, yRight, yLeft;
	//calcul of intercept A,B,C,
	uint8 intA = interceptX(rho, theta, &xUp           , 0              , 1, img->Width -1);
	uint8 intB = interceptY(rho, theta, img->Width - 1, &yRight         , 1, img->Height-1);
	uint8 intC = interceptX(rho, theta, &xDown		   , img->Height - 1, 1, img->Width -1);
	uint8 intD = interceptY(rho, theta, 0			   , &yLeft          ,1, img->Height -1);

	int result = A*intA+B*intB+C*intC+D*intD;
	
	switch (result){
	case (A|B): drawLine(img,            xUp,               0, img->Width - 1,          yRight); break;
	case (B|C): drawLine(img, img->Width - 1,          yRight,          xDown, img->Height - 1); break;
	case (C|D): drawLine(img,          xDown, img->Height - 1,              0,           yLeft); break;
	case (D|A): drawLine(img,              0,           yLeft,            xUp,               0); break;
	case (A|C): drawLine(img,            xUp,               0,          xDown, img->Height - 1); break;
	case (D|B): drawLine(img,              0,           yLeft, img->Width - 1,          yRight); break;
	default: MESSAGE("ERREUR jeune padawan!! ce lieu etait sacré!");VAR_DUMP(result); 
	}
}

