#include "jacobi9TCAD.h"

#define PRAGMA_SUB(x) _Pragma (#x)
#define DO_PRAGMA(x) PRAGMA_SUB(x)

void jacobi9d(
		hls::stream<mat> &in, //When you use an array as an argument to the top-level function. V. HLS assumes Memory is off-chip
		hls::stream<mat> &out//Streams must be passed in and out of functions by-reference
		)
{
	const mat coeff=1.0f/9.0f;
	mat temp;
	mat linebuf[9]; //Registers the data for the kernel computation
#pragma HLS ARRAY_PARTITION variable=linebuf complete dim=1 //Breakdown into registers
	hls::stream<mat> fifo_2("fifo_2"),fifo_5("fifo_5"); // In C simulation, streams have infinite size. The size is defined only during RTL sim
	DO_PRAGMA(HLS stream depth=FIFO_DEPTH variable=fifo_2)
	DO_PRAGMA(HLS stream depth=FIFO_DEPTH variable=fifo_5)
//#ifndef __SYNTHESIS__
	for (int i = 0; i < FIFO_DEPTH; i++) {
		fifo_2.write_nb((mat)0);
		fifo_5.write_nb((mat)0);
	}
//#endif
	assert(HEIGHT < MAX_ROWS);
	assert(WIDTH < MAX_COLUMNS);
	//[COLS-3] The intermediate buffers should be implemented as FIFOs to ensure streaming access
	//Iterate over the rows of the 2D input matrix
	Row: for (int i=0; i<HEIGHT+1; i++){
		//Iterate the columns of the 2D input matrix
		Col: for (int j=0; j<WIDTH; j++){
#pragma HLS PIPELINE //Pipeline at this spot to sample one input per clock
			//We need to shift the values of the registers every clock cycle
			linebuf[8]=linebuf[7];
			linebuf[7]=linebuf[6];
			//We need to make sure that the FIFO is filled before reading from its head.
			fifo_5.read_nb(linebuf[6]);
			if (i<HEIGHT){
				fifo_5.write_nb(linebuf[5]);// Last Element of the FIFO buffer that contains all of the lines
			}
			linebuf[5]=linebuf[4];
			linebuf[4]=linebuf[3];
			fifo_2.read_nb(linebuf[3]);
			if (i*WIDTH+j<WIDTH*HEIGHT+3){
				fifo_2.write_nb(linebuf[2]);
			}
			linebuf[2]=linebuf[1];
			linebuf[1]=linebuf[0];
			if (i<HEIGHT){
				in.read(linebuf[0]);//Non-Blocking read from the head of the input stream
			}else {linebuf[0]=0.0f;}
			/*Kernel Computation

			The first Calculation should take place when the first 2 Rows and the first 3 elements of the 3rd Row are read (i==2 && j==2).
			Before that the HALO should be filled with the original values
			*/
#ifndef __SYNTHESIS__
			if (i*WIDTH+j>WIDTH){
#else
			if (i*WIDTH+j>WIDTH-1){
#endif
				if (i == 1 || i == HEIGHT || j < 2) {
					out.write_nb(linebuf[4]);
				}
				else{
					temp=0;
					Kernel:for (int k = 0; k < 9; k++) {
#pragma HLS UNROLL
						temp += linebuf[k];
					}
					out.write_nb(coeff * temp);
				}
			}
		}
	}
	out.write_nb(linebuf[3]);
}
