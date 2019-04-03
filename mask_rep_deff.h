

#define MAX_MASK 81

#define MASK_croix3 {-W, -1, 0, 1, W},\
						  5,\
						  3,\
						  3,\
						  2,\
						  2

#define MASK_carre3 {-1*W-1, -1*W, -1*W+1,\
						  0*W-1,  0*W,  0*W+1, \
						  1*W-1,  1*W,  1*W+1},\
						  9,\
						  3,\
						  3,\
						  2,\
						  2

#define MASK_carre5 {-2*W-2, -2*W-1, -2*W, -2*W+1, -2*W+2, \
						 -1*W-2, -1*W-1, -1*W, -1*W+1, -1*W+2, \
						  0*W-2,  0*W-1,  0*W,  0*W+1,  0*W+2, \
						  1*W-2,  1*W-1,  1*W,  1*W+1,  1*W+2, \
						  2*W-2,  2*W-1,  2*W,  2*W+1,  2*W+2},\
						  25,\
						  5,\
						  5,\
						  3,\
						  3

#define MASK_carre7 {-3*W-3, -3*W-2, -3*W-1, -3*W, -3*W+1, -3*W+2, -3*W+3, \
						 -2*W-3, -2*W-2, -2*W-1, -2*W, -2*W+1, -2*W+2, -2*W+3, \
						 -1*W-3, -1*W-2, -1*W-1, -1*W, -1*W+1, -1*W+2, -1*W+3, \
						  0*W-3,  0*W-2,  0*W-1,  0*W,  0*W+1,  0*W+2,  0*W+3, \
						  1*W-3,  1*W-2,  1*W-1,  1*W,  1*W+1,  1*W+2,  1*W+3, \
						  2*W-3,  2*W-2,  2*W-1,  2*W,  2*W+1,  2*W+2,  2*W+3, \
						  3*W-3,  3*W-2,  3*W-1,  3*W,  3*W+1,  3*W+2,  3*W+3},\
						  49,\
						  7,\
						  7,\
						  4,\
						  4

#define MASK_carre9 {-4*W-4, -4*W-3, -4*W-2, -4*W-1, -4*W, -4*W+1, -4*W+2, -4*W+3, -4*W+4, \
						 -3*W-4, -3*W-3, -3*W-2, -3*W-1, -3*W, -3*W+1, -3*W+2, -3*W+3, -3*W+4, \
						 -2*W-4, -2*W-3, -2*W-2, -2*W-1, -2*W, -2*W+1, -2*W+2, -2*W+3, -2*W+4, \
						 -1*W-4, -1*W-3, -1*W-2, -1*W-1, -1*W, -1*W+1, -1*W+2, -1*W+3, -1*W+4, \
						  0*W-4,  0*W-3,  0*W-2,  0*W-1,  0*W,  0*W+1,  0*W+2,  0*W+3,  0*W+4, \
						  1*W-4,  1*W-3,  1*W-2,  1*W-1,  1*W,  1*W+1,  1*W+2,  1*W+3,  1*W+4, \
						  2*W-4,  2*W-3,  2*W-2,  2*W-1,  2*W,  2*W+1,  2*W+2,  2*W+3,  2*W+4, \
						  3*W-4,  3*W-3,  3*W-2,  3*W-1,  3*W,  3*W+1,  3*W+2,  3*W+3,  3*W+4, \
						  4*W-4,  4*W-3,  4*W-2,  4*W-1,  4*W,  4*W+1,  4*W+2,  4*W+3,  4*W+4},\
						  81,\
						  9,\
						  9,\
						  5,\
						  5




