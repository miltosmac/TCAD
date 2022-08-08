/***********************************
9-Point Jacobi 2-D  TCAD Header File
***********************************/
#include <stdio.h>
#include <iostream>
#include "hls_stream.h"
#include <fstream>
#include <assert.h>
//#include "hls_math.h"
#include <ap_fixed.h>

// Compare TB vs HW C-model and/or RTL
#ifndef __JACOBI9TCAD__
#define __JACOBI9TCAD__

#define HW_COSIM

#define WIDTH 2560
#define HEIGHT 2560

#define MAX_ROWS 2160
#define MAX_COLUMNS 4096

#define FIFO_DEPTH WIDTH-2

#define TIMESTEPS 50

typedef ap_fixed<36,18> mat;
//typedef float mat;

void tcad_temporal(
		hls::stream <mat> &in,
		hls::stream <mat> &out
	);

void jacobi9d(
		hls::stream<mat> &in,
		hls::stream<mat> &out
		);
#endif // __JACOBI9TCAD__ not defined
