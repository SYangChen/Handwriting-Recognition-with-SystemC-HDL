#include "Lenet.h"

void lenet::le(){
	if ( reset ) {
		steps = 0 ;
		rom_rd = 0 ;
		ram_wr = 1 ;
		rom_addr = 0 ;
		ram_addr = 0 ;
		ram_data_out = 0 ;
		result = 0 ;
		times = 0 ;
		readKernel = true ;
		i_ker = 0 ;
		i_rom = 0 ;
		ramCnt = 0 ;
		/*
		cout << "Pic : " << times << endl ;
		for ( int x = 0 ; x < 28*28 ; ++x ) {
			cout << pic[x] << "\t" ;
			if ( (x+1)%28 == 0 )
				cout << endl ;
		}
		*/
	}
	else if ( steps == 0 ) {
		if ( readKernel ) {
			// read kernel in advanced
			if ( i_ker > 0 )
				kernel5x5[i_ker-1] = rom_data_in.read() ;
			++i_ker ;
			if ( i_ker == 26 ) {		// i 25 rom i 26
				readKernel = false ;
				i_ker = 0 ;
				rom_rd.write( 1 ) ;
				rom_addr.write( i_rom++ ) ;
			}
			else {
				// ram_wr.write( 1 ) ;
				rom_rd.write( 1 ) ;
				rom_addr.write( i_rom++ ) ;
			}
			getBias = false ;
		}
		else {
			if ( !getBias ) {
				/*
				cout << "Kerenel : " << times << endl ;
				for ( int x = 0 ; x < 25 ; ++x ) {
					cout << kernel5x5[x] << "\t" ;
					if ( (x+1)%5 == 0 )
						cout << endl ;
				}
				cout << endl ;
				*/
				getBias = true ;
				rom_rd.write( 0 ) ;
				bias = rom_data_in.read() ;
				// cout << "bias:" << bias << endl ;
				temp = 0 ;
				cnt = 0 ;
				curPixel = 58 ;
			}
			ram_wr.write( 1 ) ;
			i = curPixel/28+dir[cnt][0] ;
			j = curPixel%28+dir[cnt][1] ;
			if ( !( i >= 0 && i < 28 && j >= 0 && j < 28 ) )
				cout << "fuck " << i << " " << j << endl ;
			temp += pic[i*28+j]*kernel5x5[cnt] ;
			if ( cnt == 24 ) {
				ram_wr.write( 0 ) ;
				ram_addr.write( ramCnt++ ) ;
				if ( temp+bias < 0 )
					temp = 0 ;
				else
					temp += bias ;
				ram_data_out.write( temp ) ;
				// cout << "ram_addr:" << ramCnt-1 << "value:" << temp << endl ;
				temp = 0 ;
				cnt = -1 ;
				++curPixel ;
				if( curPixel == 25*28+25+1 ) {
					++times ;
					readKernel = true ;
				}
				if( curPixel-82 >= 0 && ( curPixel-82 )%28 == 0 )
					curPixel += 4 ;
			}
			++cnt ;
		}
		if ( times == 6 ) {
			++steps ;
			cnt = 0 ;
			times = 0 ;
			curPixel = 0 ;
			ramCnt = 0 ;
		}
	}
	else if ( steps == 1 ) {
		if ( cnt == 0 ) {
			ram_wr.write( 1 ) ;
			ram_addr.write( curPixel+dir2x2[cnt++] ) ;
			tempMaxPool = -999 ;
		}
		else if ( cnt == 4 ) {
			if ( tempMaxPool < ram_data_in.read() )
				tempMaxPool = ram_data_in.read() ;
			ram_wr.write( 0 ) ;
			ram_addr.write( ramCnt++ ) ;
			ram_data_out.write( tempMaxPool ) ;
			// cout << "maxpool ram addr:" << ramCnt-1 << " data:" << tempMaxPool << endl ;
			cnt = 0 ;
			if ( ( curPixel-550 )%576 == 0 )
				++times ;
			if ( ( curPixel-22 )%48 == 0 )
				curPixel += 26 ;
			else
				curPixel += 2 ;
		}
		else {
			if ( tempMaxPool < ram_data_in.read() )
				tempMaxPool = ram_data_in.read() ;
			ram_wr.write( 1 ) ;
			ram_addr.write( curPixel+dir2x2[cnt++] ) ;
		}
		if ( times == 6 ) {
			++steps ;
			readKernel = true ;
			i_ker = 0 ;
			times = 0 ;
			ramCnt = 1000 ;
			i_rom = 156 ;
		}
	}
	else if ( steps == 2 ) {
		if ( readKernel ) {
			if ( i_ker > 0 )
				kernel5x5x6[i_ker-1] = rom_data_in.read() ;
			++i_ker ;
			if ( i_ker == 5*5*6+1 ) {		// i 25 rom i 26
				readKernel = false ;
				i_ker = 0 ;
				rom_rd.write( 1 ) ;
				rom_addr.write( i_rom++ ) ;
				getBias = false ;
			}
			else {
				ram_wr.write( 1 ) ;
				rom_rd.write( 1 ) ;
				rom_addr.write( i_rom++ ) ;
			}
		}
		else {
			if ( !getBias ) {
				/*
				cout << "Kerenel : " << times << endl ;
				for ( int x = 0 ; x < 5*5*6 ; ++x ) {
					cout << kernel5x5x6[x] << "\t" ;
					if ( (x+1)%5 == 0 )
						cout << endl ;
					if ( (x+1)%25 == 0 )
						cout << endl ;
				}
				*/
				getBias = true ;
				rom_rd.write( 0 ) ;
				bias = rom_data_in.read() ;
				// cout << "bias:" << bias << endl ;
				temp = 0 ;
				cnt = 0 ;
				curPixel = 26 ;
				depth = 0 ;
				ram_addr.write( 0 ) ;
				ram_wr.write( 1 ) ;
			}
			else {
				// ram_wr.write( 1 ) ;
				if ( cnt == -1 ) {
					++cnt ;
					i = curPixel/12+dir[cnt][0] ;
					j = curPixel%12+dir[cnt][1] ;
					ram_addr.write( i*12+j+depth*144 ) ;
					ram_wr.write( 1 ) ;
				}
				else {
					i = curPixel/12+dir[cnt][0] ;
					j = curPixel%12+dir[cnt][1] ;
					if ( !( i >= 0 && i < 12 && j >= 0 && j < 12 ) ) {
						cout << endl << endl << "fuck " << i << " " << j << endl << endl ;
					}
					temp += ram_data_in.read()*kernel5x5x6[cnt+depth*25] ;
					// cout << "ram correct? addr:" << i*12+j+depth*144 << " value:" << ram_data_in.read() << endl ;
					
					if ( depth == 5 && cnt == 24 ) {
						ram_wr.write( 0 ) ;
						ram_addr.write( ramCnt++ ) ;
						if ( temp+bias < 0 )
							temp = 0 ;
						else
							temp += bias ;
						ram_data_out.write( temp ) ;
						// cout << endl << "!!!!!ram_addr:" << ramCnt-1 << "value:" << temp << endl << endl ;
						temp = 0 ;
						cnt = -1 ;
						depth = 0 ;
						++curPixel ;
						if( curPixel == 9*12+9+1 ) {
							++times ;
							readKernel = true ;
						}
						if( curPixel-34 >= 0 && ( curPixel-34 )%12 == 0 )
							curPixel += 4 ;
					}
					else if ( cnt == 24 ) {
						++depth ;
						cnt = 0 ;
						i = curPixel/12+dir[cnt][0] ;
						j = curPixel%12+dir[cnt][1] ;
						ram_addr.write( i*12+j+depth*144 ) ;
						ram_wr.write( 1 ) ;
					}
					else {
						++cnt ;
						i = curPixel/12+dir[cnt][0] ;
						j = curPixel%12+dir[cnt][1] ;
						ram_addr.write( i*12+j+depth*144 ) ;
						ram_wr.write( 1 ) ;
					}
				}
				/*
				if ( depth == 5 ) {
					++cnt ;
					depth = 0 ;
				}
				else
					++depth ;
				*/
			}
		}
		if ( times == 16 ) {
			++steps ;
			cnt = 0 ;
			times = 0 ;
			curPixel = 1000 ;
			ramCnt = 0 ;
			dir2x2[0] = 0 ;
			dir2x2[1] = 1 ;
			dir2x2[2] = 8 ;
			dir2x2[3] = 9 ;
		}
	}
	else if ( steps == 3 ) {
		if ( cnt == 0 ) {
			ram_wr.write( 1 ) ;
			ram_addr.write( curPixel+dir2x2[cnt++] ) ;
			tempMaxPool = -999 ;
		}
		else if ( cnt == 4 ) {
			if ( tempMaxPool < ram_data_in.read() )
				tempMaxPool = ram_data_in.read() ;
			ram_wr.write( 0 ) ;
			ram_addr.write( ramCnt++ ) ;
			ram_data_out.write( tempMaxPool ) ;
			// cout << "maxpool2 ram addr:" << ramCnt-1 << " data:" << tempMaxPool << endl ;
			cnt = 0 ;
			if ( ( curPixel-1000-54 )%64 == 0 )
				++times ;
			if ( ( curPixel-1000-6 )%16 == 0 )
				curPixel += 10 ;
			else
				curPixel += 2 ;
		}
		else {
			if ( tempMaxPool < ram_data_in.read() )
				tempMaxPool = ram_data_in.read() ;
			ram_wr.write( 1 ) ;
			ram_addr.write( curPixel+dir2x2[cnt++] ) ;
		}
		if ( times == 16 ) {
			++steps ;
			neuralSum = 0 ;
			times = 0 ;
			ramCnt = 1000 ;
			cnt = 0 ;
			i_rom = 2572 ;
		}
	}
	else if ( steps == 4 ) {
		if ( cnt == 0 ) {
			rom_rd.write( 1 ) ;
			rom_addr.write( i_rom++ ) ;
			ram_wr.write( 1 ) ;
			ram_addr.write( cnt++ ) ;
			neuralSum = 0 ;
		}
		else if ( cnt == 257  ) {
			bias = rom_data_in.read() ;
			// cout << endl << "bias:" << bias << endl ;
			ram_addr.write( ramCnt++ ) ;
			if ( neuralSum+bias < 0 )
				neuralSum = 0 ;
			else
				neuralSum += bias ;
			ram_data_out.write( neuralSum ) ;
			ram_wr.write( 0 ) ;
			// cout << "layer1(256/128) ram addr:" << ramCnt-1 << " data:" << neuralSum << endl ;
			++times ;
			cnt = 0 ;
		}
		else {
			// cout << "times:" << times << "\t" << ram_data_in.read() << "\t" << rom_data_in.read() << endl ;
			neuralSum += ram_data_in.read()*rom_data_in.read() ;
			rom_rd.write( 1 ) ;
			rom_addr.write( i_rom++ ) ;
			ram_wr.write( 1 ) ;
			ram_addr.write( cnt++ ) ;
		}
		if ( times == 128 ) {
			++steps ;
			neuralSum = 0 ;
			times = 0 ;
			ramCnt = 2000 ;
			cnt = 0 ;
			i_rom = 35468 ;
		}
	}
	else if ( steps == 5 ) {
		if ( cnt == 0 ) {
			rom_rd.write( 1 ) ;
			rom_addr.write( i_rom++ ) ;
			ram_wr.write( 1 ) ;
			ram_addr.write( 1000+cnt++ ) ;
			neuralSum = 0 ;
		}
		else if ( cnt == 129  ) {
			bias = rom_data_in.read() ;
			// cout << endl << "bias:" << bias << endl << endl ;
			ram_addr.write( ramCnt++ ) ;
			if ( neuralSum+bias < 0 )
				neuralSum = 0 ;
			else
				neuralSum += bias ;
			ram_data_out.write( neuralSum ) ;
			ram_wr.write( 0 ) ;
			// cout << "layer2(128/84) ram addr:" << ramCnt-1 << " data:" << neuralSum << endl ;
			++times ;
			cnt = 0 ;
		}
		else {
			// cout << "times:" << times << "\t" << ram_data_in.read() << "\t" << rom_data_in.read() << endl ;
			neuralSum += ram_data_in.read()*rom_data_in.read() ;
			rom_rd.write( 1 ) ;
			rom_addr.write( i_rom++ ) ;
			ram_wr.write( 1 ) ;
			ram_addr.write( 1000+cnt++ ) ;
		}
		if ( times == 84 ) {
			++steps ;
			neuralSum = 0 ;
			times = 0 ;
			ramCnt = 3000 ;
			cnt = 0 ;
			i_rom = 46304 ;
		}
	}
	else if ( steps == 6 ) {
		if ( times == 10 ) {
			++steps ;
			cnt = 3000 ;
			ram_wr.write( 1 ) ;
			ram_addr.write( cnt++ ) ;
		}
		else if ( cnt == 0 ) {
			rom_rd.write( 1 ) ;
			rom_addr.write( i_rom++ ) ;
			ram_wr.write( 1 ) ;
			ram_addr.write( 2000+cnt++ ) ;
			neuralSum = 0 ;
		}
		else if ( cnt == 85  ) {
			bias = rom_data_in.read() ;
			ram_addr.write( ramCnt++ ) ;
			ram_data_out.write( neuralSum+bias ) ;
			ram_wr.write( 0 ) ;
			// cout << "layer3(84/10) ram addr:" << ramCnt-1 << " data:" << neuralSum+bias << endl ;
			++times ;
			cnt = 0 ;
		}
		else {
			// cout << "times:" << times << "\t" << ram_data_in.read() << "\t" << rom_data_in.read() << endl ;
			neuralSum += ram_data_in.read()*rom_data_in.read() ;
			rom_rd.write( 1 ) ;
			rom_addr.write( i_rom++ ) ;
			ram_wr.write( 1 ) ;
			ram_addr.write( 2000+cnt++ ) ;
		}
	}
	else {
		result.write(  ram_data_in.read() ) ;
		valid.write( true ) ;
		ram_wr.write( 1 ) ;
		ram_addr.write( cnt++ ) ;
	}
}

