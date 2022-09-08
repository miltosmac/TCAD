#include <hls_stream.h>
#include "Jacobi9TCAD.h"

void tcad_temporal(
		hls::stream <mat> &in,
		hls::stream <mat> &out
) {
	hls::stream<mat> stage0, stage1, stage2, stage3;
#pragma HLS DATAFLOW
	jacobi9d(in, stage0);
	jacobi9d(stage0,stage1);
	jacobi9d(stage1,stage2);
	jacobi9d(stage2,stage3);
	jacobi9d(stage3,out);
}
