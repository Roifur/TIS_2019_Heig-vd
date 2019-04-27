// automatic generated file

// cos table from -90 to 90 degres
// sin table from -90 to 90 degres
// representation in Q32.15 format

#ifndef _SINCOSTABLEDATA_H_
#define _SINCOSTABLEDATA_H_

#include "dsp_types.h"

#define THETA_MIN (-90)
#define THETA_MAX (+90)
#define THETA_STEP (1)

#define SINCOSTABLE_SIZE ((THETA_MAX-THETA_MIN+1)/THETA_STEP)

extern int32 tcos[SINCOSTABLE_SIZE];
extern int32 tsin[SINCOSTABLE_SIZE];

#endif // _SINCOSTABLEDATA_H_

