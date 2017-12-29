#include "ReedSolomon_coder.h"
#include "GaloisFieldMatrix.h"

#include <vector>

ReedSolomon_coder::ReedSolomon_coder()
{
    //ctor
}

ReedSolomon_coder::~ReedSolomon_coder()
{
    //dtor
}
void ReedSolomon_coder::encode( uint8_t const * const msgDecoded, uint8_t * const msgCoded, int n, int k) const
{

    GaloisFieldPolynom dataPolynom((uint8_t *)msgDecoded, k);
    GaloisFieldPolynom generatorPolynom(GaloisFieldElement8bits(1));
    for(int i(0);i<n-k;i++)
    {
        generatorPolynom = generatorPolynom * GaloisFieldPolynom(GaloisFieldElement8bits(2)^i,GaloisFieldElement8bits(1));
    }

    GaloisFieldPolynom dataCoded = dataPolynom*generatorPolynom;
    for(int i(0); i<n;i++)
    {
        msgCoded[i] = dataCoded.getCoef(i).getValue();
    }

}
using namespace std;
void ReedSolomon_coder::decode( uint8_t const * const msgCoded,   uint8_t * const msgDecoded, int n, int k) const
{
    GaloisFieldPolynom codedPolynom((uint8_t *)msgCoded, n);
    GaloisFieldElement8bits x[n-k];
    GaloisFieldElement8bits syndroms[n-k];

    bool noError(true);

    for(int i(0);i<n-k;i++)
    {
        x[i] = GaloisFieldElement8bits::getPower(i);
        syndroms[i] = codedPolynom.evaluate(x[i]);
        if(noError && syndroms[i].getValue() != 0)
        {
            noError = false;
        }
    }


    GaloisFieldPolynom polynomWithoutError;
    if(noError)
    {
        //std::cout << "There are not any error." << std::endl;
        polynomWithoutError = codedPolynom;

    }
    else
    {
       // std::cout << "There are errors." << std::endl;
        //std::cout << "The syndroms are calculated." << std::endl;
       // std::cout << "Test ? "<< berlekampMassy_algorithm(codedPolynom,n-k).getString() << std::endl;


        //return interpolatePolynom(x, syndroms,n-k);

        polynomWithoutError = peterson( codedPolynom, (GaloisFieldElement8bits const * const) syndroms,  (int) (n-k), n, k );
	if(polynomWithoutError.getDegree() == -1)
		return;
    }


    GaloisFieldPolynom generatorPolynom(GaloisFieldElement8bits(1));
    for(int i(0);i<n-k;i++)
    {
        generatorPolynom = generatorPolynom * GaloisFieldPolynom(GaloisFieldElement8bits(2)^i,GaloisFieldElement8bits(1));
    }

    GaloisFieldPolynom decodedPolynom;


    while(polynomWithoutError.getDegree() >= generatorPolynom.getDegree() )
    {


        GaloisFieldElement8bits f = polynomWithoutError.getCoef(polynomWithoutError.getDegree()) / generatorPolynom.getCoef(generatorPolynom.getDegree());
        int offset = polynomWithoutError.getDegree() - generatorPolynom.getDegree();

        decodedPolynom.setCoef(offset, f);

        for(int i(0); i < generatorPolynom.getDegree()+1; i++)
        {
            polynomWithoutError.setCoef( i+offset, polynomWithoutError.getCoef(i+offset) - f*generatorPolynom.getCoef(i));
        }


    }

    for(int i(0); i < k; i++)
    {
        if(i < decodedPolynom.getDegree()+1)
            msgDecoded[i] = decodedPolynom.getCoef(i).getValue();
        else
            msgDecoded[i] = 0;
    }


}

using namespace std;

GaloisFieldPolynom ReedSolomon_coder::peterson( GaloisFieldPolynom const& s, GaloisFieldElement8bits const * const syndroms, int nbSyndroms, unsigned int n, unsigned int k ) const
{

    unsigned int v = (n-k)/2;

    GaloisFieldMatrix m(v,v);

    for(int l(0); l < v; l++)
        for(int c(0); c < v; c++)
            m.setCoef(l,c,syndroms[l+c]);

    int detM = (int) m.det().getValue();

    while(detM == 0)
    {
        m.scaleDown(m.getL()-1, m.getC()-1);
        detM = (int) m.det().getValue();
    }

    unsigned int numberOfErrors = m.getL();

    /*if(numberOfErrors > 1)
        std::cout << numberOfErrors << " errors are to be corrected." << std::endl;
    else
        std::cout << numberOfErrors << " error is to be corrected." << std::endl;
*/
    GaloisFieldMatrix row(numberOfErrors, 1);
    for( int i(0); i<numberOfErrors; i++)
        row.setCoef(i,0,0-syndroms[numberOfErrors+i]);



    GaloisFieldMatrix errorLocatorMat = m.gauss(row);
    GaloisFieldPolynom errorLocatorPoly;

    for(int i(0); i<errorLocatorMat.getL(); i++)
    {
        errorLocatorPoly.setCoef(errorLocatorMat.getL()-i, errorLocatorMat.getCoef(i,0));
    }
    errorLocatorPoly.setCoef(0,GaloisFieldElement8bits(1));
    //std::cout << "Error locator polynom found : " << errorLocatorPoly.getStringNum() << std::endl;

    std::vector<int> errorLocators;
 
    GaloisFieldElement8bits a,b;
    
    for(int i(0); i < 255; i++)
    {
        a = GaloisFieldElement8bits::getPower(i);
        b = errorLocatorPoly.evaluate(a);

        if(b.getValue()==0)
        {

            errorLocators.push_back((255-i)%255);
        }

    }
     //std::cout << errorLocators.size() << " error locators found." << std::endl;

     if(numberOfErrors != errorLocators.size())
     {
	 cout << "errorsNumber:" << numberOfErrors << endl;
	 cout << "ErrorLocSize:"<< errorLocators.size() << endl;
	 //errorLocatorPoly.print();
         std::cout << "Error: data too corrupted!" << std::endl;
         return GaloisFieldPolynom();
     }
	
     GaloisFieldMatrix system(numberOfErrors,numberOfErrors);
     GaloisFieldMatrix result(numberOfErrors,1);


     for(int i(0); i<numberOfErrors; i++)
     {
         for( int j(0); j<numberOfErrors; j++)
         {
             int x = errorLocators[j];
	     system.setCoef(i,j, GaloisFieldElement8bits::getPower(i*x));
	 }
         result.setCoef(i,0,syndroms[i]);
     }


     GaloisFieldMatrix coefError = system.gauss(result);
     GaloisFieldPolynom errors;
     
     for( int i(0); i < coefError.getL(); i++)
     {
         errors.setCoef(errorLocators[i], coefError.getCoef(i,0));
     }


     //std::cout << "The error polynom has been found: " << errors.getStringNum() << std::endl;


    return s-errors;
}
