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

uint8_t * readDataFromFile( char const * const path, unsigned int * blockCount )
{
	ifstream f(path);
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

	cout <<"Offset:"<<offset << endl << "Size: "<< fSize << endl << "W:" << w << endl << "H:" << h << endl;;

	unsigned int size = w*h*3;
	unsigned int blockSize = (size/K+1)*K;


	uint8_t * data = new uint8_t[blockSize];
	*blockCount = blockSize/K;
	f.seekg(offset);
	f.read( (char *) data, size);

	for(unsigned int i=size; i<blockSize; i++)
		data[i] = 0;
	
	cout << blockSize / K << " blocs imported" << endl;

	f.close();
	return data;
}

int main()
{
    unsigned int blockCount;
    uint8_t * data = readDataFromFile("data.bmp", &blockCount);


    ReedSolomon_coder coder;
    //srand(time(NULL));

    //uint8_t data[K]="Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Aenean commodo ligula eget dolor. Aenean massa. Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus. Donec quam felis, ultricies ne";
    uint8_t data2[K];

    uint8_t encodedData[N];


    writeDataTextInFile("data.txt", data, K);

    coder.encode(data, encodedData, N, K);


    writeDataInFile("encodeddata.txt", encodedData, N);

    for(int i(0); i<16; i++)
    {
        cout << "Inserting an error at position " << i << ": " <<  (int) encodedData[i];
        encodedData[i] = 0*(rand())%255;
        cout << " => " << (int) encodedData[i] << endl;
    }


    writeDataInFile("encodeddata_corrupted.txt", encodedData, N);





    GaloisFieldPolynom p = coder.decode(encodedData, data2, N, K);

    for(int i(0); i < K; i++)
    {
        if(i < p.getDegree()+1)
            data2[i] = p.getCoef(i).getValue();
        else
            data2[i] = 0;
    }


    writeDataTextInFile("decodeddata.txt", data2, K);

    delete[] data;

    return 0;
}
