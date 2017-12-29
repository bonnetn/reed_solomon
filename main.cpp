#include <iostream>
#include <ctime>
#include <cstdlib>
#include "ReedSolomon_coder.h"


#include "GaloisFieldElement8bits.h"
#include "GaloisFieldPolynom.h"

#define N 255
#define K 223

#include <fstream>
#include <string>

using namespace std;

void writeDataInFile( char const * const path, uint8_t data[], int cnt )
{
    ofstream f;
    f.open(path);

    f << std::hex;
    for(int i(0); i < cnt; i++)
    {
        if(data[i] < 16)
            f << "0";
        f << (int) data[i] << " ";
        if(i%8==7)
            f << endl;
    }

    f.close();
}

void writeDataTextInFile( char const * const path, uint8_t data[], int cnt )
{
    ofstream f;
    f.open(path);

    for(int i(0); i < cnt; i++)
    {
        f << (char) data[i];
//        if(i%8==7)
//            f << endl;

    }

    f.close();
}

uint8_t * readDataFromBMP( char const * const path, unsigned long * blockCount )
{
	ifstream f(path, ifstream::binary);
	uint16_t tmp;
	uint32_t fSize;
	uint32_t offset;
	uint32_t w,h;

	f.seekg(f.tellg()+2);

	f.read((char *)&fSize, 4);
	
	f.seekg(f.tellg()+2);
	
	f.seekg(f.tellg()+2);
	
	f.read((char *) &offset,4 );
	
	f.seekg(f.tellg()+4);

	f.read((char *) &w,4);
	f.read((char *) &h,4);

	uint16_t bpp;
	f.seekg(f.tellg()+2);
	f.read((char *) &bpp,2);

	cout << "Reading " << path << endl;
	cout << "- Size: "<< fSize << "bytes" << endl;
	cout << "- Bytes of data:" << w*h*3 << endl;
	cout << "- BPP: "<< bpp << endl;

	unsigned long size = w*h*3;
	unsigned long blockSize = (size/K+1)*K;


	uint8_t * data = new uint8_t[blockSize];
	*blockCount = blockSize/K;
	f.seekg(offset);
	f.read( (char *) data, size);

	for(unsigned long i=size; i<blockSize; i++)
		data[i] = 0;
	
	cout << "-> "<< blockSize / K << " blocs imported" << endl;

	f.close();
	return data;
}


void writeDataToBMP( char const * const path, uint8_t * data )
{
	fstream f(path, fstream::in | fstream::out);
	f.seekg(2+4+2+2);
	uint32_t offset;
	f.read((char *) &offset,4);
	
	f.seekg(f.tellg()+4);
	uint32_t w,h;

	f.read((char *) &w,4);
	f.read((char *) &h,4);


	f.seekg(offset);
	f.write((char*)data, w*h*3); 

}
int main( int argc, char ** argv )
{

    if(argc <= 1)
	    return 1;
    std::string str(argv[1]);
    ifstream src(argv[1], ios::binary);
    ofstream dst((str+string("_dataDecoded.bmp")).c_str(), ios::binary);
    ofstream dst2((str+string("_dataCorrupted.bmp")).c_str(), ios::binary);

    dst << src.rdbuf();
    src.seekg(0);
    dst2 << src.rdbuf();

    unsigned long blockCount;
    uint8_t * data = readDataFromBMP(str.c_str(), &blockCount);
    uint8_t corruptedData[blockCount * K];
    for(unsigned long i(0); i < blockCount*K; i++)
	    corruptedData[i] = data[i];


    uint8_t encodedData[ blockCount * N ];

    ReedSolomon_coder coder;

    for( unsigned long i=0; i< blockCount; i++)
    {
	cout<< " Encoding: " << i*100/blockCount << "%\r" << flush ;
    	coder.encode(data+K*i, encodedData+N*i, N, K);
    }
    cout << " Encoding: Done!" << endl;

    delete[] data;
    
    
    //writeDataInFile("encodeddata.txt", encodedData, N*blockCount);

    
    srand(time(NULL));
    for(unsigned long i(0); i<blockCount; i++)
    {
	   
							
		unsigned long radd = (rand()%N);
		unsigned long index = i*N+radd;
		uint8_t d = rand()%255;
		if(index < ((unsigned long)blockCount)*K)
		{	
			corruptedData[index] = d;
		}
		if( index < blockCount*N)
			encodedData[index] = d;
		
			

		radd = (rand()%N);
		index = i*N+radd;
		d = rand()%255;
		if(index < ((unsigned long)blockCount)*K)
		{	
			corruptedData[index] = d;
		}
		if( index < blockCount*N)
			encodedData[index] = d;
			
//*/
 	/*	
	unsigned long index = i*N;
	if(index+2 < blockCount*K)
	{
		corruptedData[index]=255;
		corruptedData[index+1]=255;
    		corruptedData[index+2]=255;
	}
	
	encodedData[index+2]=255;
    	encodedData[index+1]=255;
	encodedData[index]=255;
	//*/
    }
    
    writeDataToBMP((str+string("_dataCorrupted.bmp")).c_str(), corruptedData);  


    uint8_t data2[ blockCount * K ];

    for( unsigned int i(0); i<blockCount; i++)
    {
        cout << " Decoding: " << i*100/blockCount << "%\r" << flush;
        coder.decode(encodedData+i*N, data2+i*K, N, K);
    }
    cout << " Decoding: Done!" << endl;

    writeDataToBMP((str+string("_dataDecoded.bmp")).c_str(), data2);  
    
    return 0;
}
