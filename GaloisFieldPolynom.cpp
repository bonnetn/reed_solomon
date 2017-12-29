#include "GaloisFieldPolynom.h"
#include <sstream>

GaloisFieldPolynom::GaloisFieldPolynom()
{
    //ctor
}

GaloisFieldPolynom::GaloisFieldPolynom(GaloisFieldElement8bits const& a, GaloisFieldElement8bits const& b)
{
    m_coefs.push_back(a);
    m_coefs.push_back(b);
}
GaloisFieldPolynom::GaloisFieldPolynom(GaloisFieldElement8bits const& a)
{
    m_coefs.push_back(a);
}

GaloisFieldPolynom::GaloisFieldPolynom(GaloisFieldPolynom const& p)
{
    for(int i(0); i < p.getDegree()+1; i++)
    {
        setCoef(i, p.getCoef(i));
    }
}

GaloisFieldPolynom::GaloisFieldPolynom(uint8_t const * const data, int size)
{
    for(int i(0); i<size; i++)
    {
        setCoef(i,GaloisFieldElement8bits(data[i]));
    }
}

GaloisFieldPolynom::~GaloisFieldPolynom()
{
    //dtor
}

GaloisFieldElement8bits GaloisFieldPolynom::getCoef(unsigned int num) const
{
    if( m_coefs.size() <= num )
    {
        return 0;
    }
    else
    {
        return m_coefs[num];
    }
}

void GaloisFieldPolynom::setCoef( int num, GaloisFieldElement8bits const& coef )
{

    while( (int)m_coefs.size() <= num-1)
    {
        m_coefs.push_back(GaloisFieldElement8bits(0));
    }

    if(m_coefs.size() > num)
    {
        m_coefs[num] = coef;
    }
    else
    {
        m_coefs.push_back(coef);
    }

    while(m_coefs.size()>0 && m_coefs[m_coefs.size()-1].getValue() == 0)
    {
        m_coefs.pop_back();
    }

}

std::string GaloisFieldPolynom::getString() const
{
    std::stringstream str;
    for(int i(m_coefs.size()-1); i >= 0; i--)
    {
        str << "[" << getCoef(i).getString() << "]*X" << i << " + ";
    }
    return str.str();
}


std::string GaloisFieldPolynom::getStringNum() const
{
    std::stringstream str;
    for(int i(m_coefs.size()-1); i >= 0; i--)
    {
        str << "" << (int) getCoef(i).getValue() << "*X" << i << " + ";
    }
    return str.str();
}



int GaloisFieldPolynom::getDegree() const
{
    return m_coefs.size()-1;
}

GaloisFieldPolynom operator+( GaloisFieldPolynom const& a, GaloisFieldPolynom const& b )
{
    GaloisFieldPolynom result;

    for(int i(0); i < std::max(a.getDegree()+1,b.getDegree()+1); i++)
    {
        result.setCoef(i, a.getCoef(i) + b.getCoef(i));
    }
    return result;
}
GaloisFieldPolynom operator-( GaloisFieldPolynom const& a, GaloisFieldPolynom const& b)
{
    GaloisFieldPolynom result;

    for(int i(0); i < std::max(a.getDegree()+1,b.getDegree()+1); i++)
    {
        result.setCoef(i, a.getCoef(i) - b.getCoef(i));
    }
    return result;
}

GaloisFieldPolynom operator<<( GaloisFieldPolynom const& p, unsigned int x )
{
    GaloisFieldPolynom result(p);
    result <<= x;
    return result;
}

void GaloisFieldPolynom::operator<<=( unsigned int x )
{
    for(int i(0); i<x; i++)
    {
        m_coefs.push_front(GaloisFieldElement8bits(0));
    }
}

GaloisFieldPolynom operator*( GaloisFieldElement8bits k, GaloisFieldPolynom const& a)
{
    GaloisFieldPolynom result(a);
    for(int i(0); i < result.getDegree()+1; i++)
    {
        result.setCoef(i,result.getCoef(i)*k);
    }
    return result;
}

GaloisFieldPolynom operator/( GaloisFieldPolynom const& a, GaloisFieldElement8bits k)
{
    GaloisFieldPolynom result(a);
    for(int i(0); i < result.getDegree()+1; i++)
    {
        result.setCoef(i,result.getCoef(i)/k);
    }
    return result;
}



GaloisFieldPolynom operator*( GaloisFieldPolynom const& a, GaloisFieldPolynom const& b)
{
    GaloisFieldPolynom p;
    for(int i(0); i <= b.getDegree()+1; i++)
    {
        p = p + ((b.getCoef(i) * a) << i);

    }
    return p;
}

GaloisFieldElement8bits GaloisFieldPolynom::evaluate( GaloisFieldElement8bits const& x ) const
{
    GaloisFieldElement8bits result;
    for(int i(0); i<getDegree()+1; i++)
    {
        result = result + (getCoef(i)*(x^i));
    }
    return result;
}
