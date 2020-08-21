#include "RAM.h"


void RAM::r_data(){
	
	if(wr.read())
		data_out = RAM_[ram_addr.read()];

}

void RAM::w_data(){
	
	if(!(wr.read())) {
		RAM_[ram_addr.read()] = data_in;
		// cout << "ram addr : " << ram_addr.read() << " value : " << data_in << endl ;
	}

}



