#include <hls_stream.h>
#include "Jacobi9TCAD.h"

void tcad_temporal(
		hls::stream <mat> &in,
		hls::stream <mat> &out
) {
	hls::stream<mat> stage0,stage1,stage2,stage3,stage4,stage5,stage6,stage7,stage8,stage9;
	hls::stream<mat> stage10,stage11,stage12,stage13,stage14,stage15,stage16,stage17,stage18,stage19;
	hls::stream<mat> stage20,stage21,stage22,stage23,stage24,stage25,stage26,stage27,stage28,stage29;
	hls::stream<mat> stage30,stage31,stage32,stage33,stage34,stage35,stage36,stage37,stage38,stage39;
	hls::stream<mat> stage40,stage41,stage42,stage43,stage44,stage45,stage46,stage47,stage48,stage49;

#pragma HLS DATAFLOW
	jacobi9d(in, stage0);
	jacobi9d(stage0, stage1);
	jacobi9d(stage1, stage2);
	jacobi9d(stage2, stage3);
	jacobi9d(stage3, stage4);
	jacobi9d(stage4, stage5);
	jacobi9d(stage5, stage6);
	jacobi9d(stage6, stage7);
	jacobi9d(stage7, stage8);
	jacobi9d(stage8, stage9);
	jacobi9d(stage9, stage10);
	jacobi9d(stage10, stage11);
	jacobi9d(stage11, stage12);
	jacobi9d(stage12, stage13);
	jacobi9d(stage13, stage14);
	jacobi9d(stage14, stage15);
	jacobi9d(stage15, stage16);
	jacobi9d(stage16, stage17);
	jacobi9d(stage17, stage18);
	jacobi9d(stage18, stage19);
	jacobi9d(stage19, stage20);
	jacobi9d(stage20, stage21);
	jacobi9d(stage21, stage22);
	jacobi9d(stage22, stage23);
	jacobi9d(stage23, stage24);
	jacobi9d(stage24, stage25);
	jacobi9d(stage25, stage26);
	jacobi9d(stage26, stage27);
	jacobi9d(stage27, stage28);
	jacobi9d(stage28, stage29);
	jacobi9d(stage29, stage30);
	jacobi9d(stage30, stage31);
	jacobi9d(stage31, stage32);
	jacobi9d(stage32, stage33);
	jacobi9d(stage33, stage34);
	jacobi9d(stage34, stage35);
	jacobi9d(stage35, stage36);
	jacobi9d(stage36, stage37);
	jacobi9d(stage37, stage38);
	jacobi9d(stage38, stage39);
	jacobi9d(stage39, stage40);
	jacobi9d(stage40, stage41);
	jacobi9d(stage41, stage42);
	jacobi9d(stage42, stage43);
	jacobi9d(stage43, stage44);
	jacobi9d(stage44, stage45);
	jacobi9d(stage45, stage46);
	jacobi9d(stage46, stage47);
	jacobi9d(stage47, stage48);
	jacobi9d(stage48, out);
}
