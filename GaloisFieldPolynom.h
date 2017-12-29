#ifndef GALOISFIELDPOLYNOM_H
#define GALOISFIELDPOLYNOM_H

#include "GaloisFieldElement8bits.h"
#include <deque>
#include <string>

class GaloisFieldPolynom
{
    public:
        GaloisFieldPolynom();
        GaloisFieldPolynom(GaloisFieldElement8bits const& a);
        GaloisFieldPolynom(GaloisFieldElement8bits const& a, GaloisFieldElement8bits const& b);
        GaloisFieldPolynom(GaloisFieldPolynom const& p);
        GaloisFieldPolynom(uint8_t const * const data, int size);


        virtual ~GaloisFieldPolynom();

        GaloisFieldElement8bits getCoef(unsigned int num) const;
        void setCoef( int num, GaloisFieldElement8bits const& coef );
        int getDegree() const;

        std::string getString() const;
        std::string getStringNum() const;

        GaloisFieldElement8bits evaluate( GaloisFieldElement8bits const& x ) const;

        void operator<<=( unsigned int x );



    protected:
    private:
        std::deque<GaloisFieldElement8bits> m_coefs;
};

GaloisFieldPolynom operator+( GaloisFieldPolynom const& a, GaloisFieldPolynom const& b);
GaloisFieldPolynom operator-( GaloisFieldPolynom const& a, GaloisFieldPolynom const& b);
GaloisFieldPolynom operator<<( GaloisFieldPolynom const& p, unsigned int x );
GaloisFieldPolynom operator*( GaloisFieldPolynom const& a, GaloisFieldPolynom const& b);
GaloisFieldPolynom operator*( GaloisFieldElement8bits k, GaloisFieldPolynom const& a);
GaloisFieldPolynom operator/( GaloisFieldPolynom const& a, GaloisFieldElement8bits k);

#endif // GALOISFIELDPOLYNOM_H
