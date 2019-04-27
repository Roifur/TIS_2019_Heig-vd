#!/bin/bash
rm ./test*
cc basicTemplateSim.c mask.c images_amoi.c ../minitifRGB.c histogram.c houghTransform.c sincostableData.c -o test
./test

