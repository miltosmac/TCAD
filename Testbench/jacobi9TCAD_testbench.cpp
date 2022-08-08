#include "jacobi9TCAD.h"


using namespace std;

int main(int argc, char **argv)
{
	const mat coeff=1.0f/9.0f;
	mat temp;
	int err_cnt = 0;
	mat mult;
	/*
	HEIGHT=rand() % 2159+1; //For a 4K image the resolution is 4096x2160
	WIDTH=rand() % 4095+1;
	*/
	auto A = new mat[HEIGHT][WIDTH];
	auto sw_result= new mat[HEIGHT][WIDTH];

	ofstream txtfile;
	txtfile.open ("out_mat.txt");

	/*DYNAMIC ALLOCATION
	mat** A=new mat*[HEIGHT];
	mat** sw_result=new mat*[HEIGHT];
	for (int k; k <HEIGHT; k++){
		A[k]=new mat[WIDTH];
		sw_result[k]=new mat[WIDTH];
	}
	*/
	hls::stream<mat> testream_in ("instream");
	hls::stream<mat> testream_out ("outstream");
	/*mat A[HEIGHT][WIDTH]={
			{867,	644,	218,	961,	997,	869,	968,	811,	827,	769,	298,	36,		567,	984,	743},
			{721,	122,	959,	333,	421,	467,	974,	113,	826,	272,	464,	143,	396,	372,	459},
			{993,	886,	417,	464,	153,	686,	412,	329,	768,	136,	567,	979,	598,	701,	187},
			{244,	553,	346,	882,	424,	278,	766,	487,	899,	538,	566,	477,	274,	788,	736},
			{917,	144,	543,	61,		987,	574,	783,	576,	117	,	804,	23,		446,	882,	419,	133},
			{746,	552,	184,	413,	491,	892,	724,	953,	21,		787,	579,	938,	231,	92,		734},
			{578,	771,	617,	139,	413,	476,	112,	664,	148,	531,	757,	9,		838,	236,	587},
			{899,	533,	421,	457,	339,	73,		956,	842,	214,	993,	881,	927,	444,	218,	971},
			{632,	254,	838,	241,	732,	919,	468,	136,	582,	759,	658,	506,	677,	27,		778},
			{446,	877,	274,	113,	811,	492,	59,		338,	906,	342,	889,	238,	891,	567,	974},
			{108,	681,	252,	529,	428,	271,	297,	529,	443,	51,		357,	679,	933,	81,		927},
			{434,	583,	931,	417,	479,	933,	91,		217,	819,	798,	361,	667,	974,	103,	191},
			{232,	594,	713,	146,	877,	944,	893,	216,	642,	549,	368,	476,	97,		884,	743},
			{726,	612,	979,	268,	481,	592,	49,		43,		376,	452,	179,	273,	616,	977,	944},
			{888,	726,	277,	614,	653,	351,	22,		299,	193,	891,	552,	194,	48,		531,	762}
	};
*/
	for (int i=0; i<HEIGHT;i++){
		for(int j=0; j<WIDTH; j++){
			A[i][j]= mat(rand())/mat(RAND_MAX/100.0f); //Initialize A matrix as random floats
			//A[i][j] = static_cast <mat>(i * WIDTH + j);
			testream_in.write(A[i][j]);
		}
	}
	//Generate the expected results
	//Iterate over the rows of the 2D input matrix
	for (int t=0; t<TIMESTEPS; t++){
		for (int i=0; i<HEIGHT; i++){
			//Iterate the columns of the 2D input matrix
			for (int j=0; j<WIDTH; j++){
				if (i>0 && i < HEIGHT-1 && j>0 && j < WIDTH-1){

					temp= A[i-1][j-1]+A[i-1][j]+A[i-1][j+1]+ // First Line
							A[i][j-1]+A[i][j]+A[i][j+1]+      // Second Line
							A[i+1][j-1]+A[i+1][j]+A[i+1][j+1];   // Third Line
					sw_result[i][j]=coeff*temp; //Create SW result
				}else {
					sw_result[i][j]=A[i][j]; //Results for the HALO
				}
			}
		}
		for (int i=0; i<HEIGHT; i++){
					for (int j=0; j<WIDTH; j++){
						A[i][j]=sw_result[i][j];
					}
		}
	}
	delete []A;
		/*DYNAMIC ALLOCATION/DEALLOCATION
		for (int k=0; k<HEIGHT; k++){
				delete []A[k];
			}
		delete[] A; //Delete the block of allocated memory
		mat** hw_result=new mat*[HEIGHT];
		for (int k=0; k<HEIGHT; k++){
					hw_result[k]= new mat[WIDTH];
					}
		*/
	// Calculate TCAD Result
	tcad_temporal(testream_in,testream_out);

	auto hw_result= new mat[HEIGHT][WIDTH];
	for (int i=0; i<HEIGHT; i++){
		for (int j=0; j<WIDTH; j++){
		// Check HW result against SW
			testream_out.read(hw_result[i][j]); //Values read from stream and into the HW result variable for checking
			txtfile << hw_result[i][j]<<" ";
			if (fabs(float(hw_result[i][j] - sw_result[i][j]))>1e-2f) {
				cout << "At iteration i:" << i << " and j:" <<j << endl;
				err_cnt++;
			}
		}
		txtfile <<"\n";
	}
	txtfile.close();
	/*DYNAMIC DEALLOCATION
	 for (int k=0; k<HEIGHT; k++){
					delete []sw_result[k];
					delete []hw_result[k];
				}
	delete[] sw_result;
	delete[] hw_result;
	*/
	delete []sw_result;
	delete []hw_result;

	if (err_cnt){
		cout << "ERROR: " << err_cnt << " mismatches detected!" << endl;
	}else{
		cout << "Test passes." << endl;
	}
}
