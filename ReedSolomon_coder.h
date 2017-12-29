#ifndef REEDSOLOMON_CODER_H
#define REEDSOLOMON_CODER_H

#include "GaloisFieldElement8bits.h"
#include "GaloisFieldPolynom.h"

class ReedSolomon_coder
{
    public:
        ReedSolomon_coder();
        virtual ~ReedSolomon_coder();
        void encode( uint8_t const * const msgDecoded, uint8_t * const msgCoded,   int n, int k) const;
        void decode( uint8_t const * const msgCoded,   uint8_t * const msgDecoded, int n, int k) const;


    protected:
    private:
        GaloisFieldPolynom interpolatePolynom(GaloisFieldElement8bits const * const x, GaloisFieldElement8bits const * const syndroms, int t) const;
        GaloisFieldPolynom berlekampMassy_algorithm( GaloisFieldPolynom const& s, int N ) const ;
        GaloisFieldPolynom peterson( GaloisFieldPolynom const& s, GaloisFieldElement8bits const * const syndroms, int nbSyndroms, unsigned int n, unsigned int k ) const;

};

#endif // REEDSOLOMON_CODER_H
