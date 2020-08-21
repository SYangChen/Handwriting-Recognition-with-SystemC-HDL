#include "systemc.h"
#include "run_mode.h"
#include<iostream>
#include<fstream>



using namespace std;

SC_MODULE(lenet)
{	
	sc_in_clk clock;
	sc_in<bool> reset;
	sc_out<bool> rom_rd;
	sc_out<bool> ram_wr;
	sc_out<sc_uint<16> > rom_addr;
	sc_out<sc_uint<16> > ram_addr;
	sc_in<TYPE > rom_data_in;		// TYPE
	sc_in<TYPE > ram_data_in;		// TYPE
	sc_out<TYPE > ram_data_out;		// TYPE
	sc_out<TYPE > result;			// TYPE
	sc_out<bool> valid;
	
	void le();
	
	ifstream fin;
	int steps, times, i_ker, i_rom ;
	bool readKernel, getBias ;
	TYPE pic[28*28] ;
	TYPE kernel5x5[5*5] ;
	TYPE bias ;
	int dir[25][2] ;
	TYPE temp ;
	int cnt ;
	int curPixel ;
	int i, j ;
	int ramCnt ;
	
	TYPE tempMaxPool ;
	int dir2x2[4] ;
	
	TYPE kernel5x5x6[5*5*6] ;
	int depth ;
	
	TYPE neuralSum ;
	
	SC_CTOR(lenet)
	{
		readKernel = true ;
		int p = 0 ;
		dir2x2[0] = 0 ;
		dir2x2[1] = 1 ;
		dir2x2[2] = 24 ;
		dir2x2[3] = 25 ;
		for( p = 0 ; p < 5 ; ++p ) {
			dir[p][0] = -2 ;
			dir[p][1] = p-2 ;
		}
		for( ; p < 10 ; ++p ) {
			dir[p][0] = -1 ;
			dir[p][1] = p-7 ;
		}
		for( ; p < 15 ; ++p ) {
			dir[p][0] = 0 ;
			dir[p][1] = p-12 ;
		}
		for( ; p < 20 ; ++p ) {
			dir[p][0] = 1 ;
			dir[p][1] = p-17 ;
		}
		for( ; p < 25 ; ++p ) {
			dir[p][0] = 2 ;
			dir[p][1] = p-22 ;
		}
		fin.open(INPUT_FILE);
		float f ;
		for( p = 0 ; fin >> f ; ++p ) 
			pic[p] = f/255 ;
		SC_METHOD(le);
		sensitive << clock.neg();
	}
};

