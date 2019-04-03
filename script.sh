#!/bin/bash
rm ./test*
cc basicTemplateSim.c mask.c images_amoi.c minitifRGB.c histogram.c -o test
./test

