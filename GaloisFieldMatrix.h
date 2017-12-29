#ifndef GALOISFIELDMATRIX_H
#define GALOISFIELDMATRIX_H

#include "GaloisFieldPolynom.h"
#include "GaloisFieldElement8bits.h"

class GaloisFieldMatrix
{
    public:
        GaloisFieldMatrix( unsigned int l, unsigned int c);
        GaloisFieldMatrix( GaloisFieldMatrix const& m );

        virtual ~GaloisFieldMatrix();

        void setCoef( unsigned int l, unsigned int c, GaloisFieldElement8bits v);
        GaloisFieldElement8bits getCoef( unsigned int l, unsigned int c) const;
        GaloisFieldElement8bits det() const ;
        GaloisFieldMatrix gauss( GaloisFieldMatrix const& b ) const;

        unsigned int getL() const;
        unsigned int getC() const;

        void scaleDown( unsigned int l, unsigned int c);


        void print() const;

    protected:
    private:
        GaloisFieldElement8bits ** m_lines;
        unsigned int m_l,m_c;
};

#endif // GALOISFIELDMATRIX_H
