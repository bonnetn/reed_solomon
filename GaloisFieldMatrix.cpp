#include "GaloisFieldMatrix.h"

GaloisFieldMatrix::GaloisFieldMatrix( unsigned int l, unsigned int c):
    m_l(l), m_c(c)
{
    m_lines = new GaloisFieldElement8bits* [l];
    for(unsigned int i(0); i < l; i++)
    {
        m_lines[i] = new GaloisFieldElement8bits[c];
    }
}

GaloisFieldMatrix::GaloisFieldMatrix( GaloisFieldMatrix const& m )
{
    m_l = m.getL();
    m_c = m.getC();

    m_lines = new GaloisFieldElement8bits* [m_l];
    for(unsigned int i(0); i < m_l; i++)
    {
        m_lines[i] = new GaloisFieldElement8bits[m_c];
        for(unsigned int j(0); j < m_c; j++)
        {
            m_lines[i][j] = m.getCoef(i,j);
        }
    }

}
unsigned int GaloisFieldMatrix::getL() const
{
    return m_l;
}

unsigned int GaloisFieldMatrix::getC() const
{
    return m_c;
}

GaloisFieldMatrix::~GaloisFieldMatrix()
{
    for(unsigned int i(0); i < m_l; i++)
    {
        delete[] m_lines[i];
    }
    delete[] m_lines;
}

void GaloisFieldMatrix::print() const
{
    for(unsigned int i(0); i < m_l; i++)
    {
        for(unsigned int j(0); j < m_c; j++)
        {
            std::cout << (int) m_lines[i][j].getValue() << " ";
        }
        std::cout << std::endl;
    }
}

void GaloisFieldMatrix::setCoef( unsigned int l, unsigned int c, GaloisFieldElement8bits v)
{
    m_lines[l][c] = v;
}

GaloisFieldElement8bits GaloisFieldMatrix::getCoef( unsigned int l, unsigned int c) const
{
    return m_lines[l][c];
}
GaloisFieldElement8bits GaloisFieldMatrix::det() const
{

    GaloisFieldMatrix m(*this);

    GaloisFieldElement8bits det;
    unsigned int i, j, k;

    for (i = 0; i < m_l-1; i++)
    {
        for(j = i + 1; j < m_l; j++)
            for(k = i + 1; k < m_l; k++)
            {
                m.setCoef(j,k, m.getCoef(j,k)*m.getCoef(i,i)-m.getCoef(j,i)*m.getCoef(i,k) );
                if(i)
                    m.setCoef(j, k, m.getCoef(j, k)/m.getCoef(i-1,i-1));
            }
    }

    return m.getCoef(m_l-1,m_l-1);
}




void GaloisFieldMatrix::scaleDown( unsigned int l, unsigned int c)
{
    m_l = l;
    m_c = c;
}

GaloisFieldMatrix GaloisFieldMatrix::gauss( GaloisFieldMatrix const& b ) const
{

    GaloisFieldMatrix a = GaloisFieldMatrix(*this);
    GaloisFieldElement8bits coef;
    GaloisFieldMatrix result(b);
    GaloisFieldMatrix lambda(b.getL(),1);

    // On met le systeme sous forme triangulaire
    for( int i(0); i < m_c; i++ )
    {

	int pivot = i;
	int max = a.getCoef(i,i).getValue();
	for(int j(i+1); j<m_l; j++)
	{
		if(a.getCoef(j,i).getValue() > max)
		{
			pivot = j;
			max = a.getCoef(j,i).getValue();
		}
	}
	

	for(int j(i); j<m_c; j++)
	{
		GaloisFieldElement8bits v = a.getCoef(i,j);
		a.setCoef(i,j,a.getCoef(pivot,j));
		a.setCoef(pivot, j, v);
	}
	GaloisFieldElement8bits v = result.getCoef(pivot,0);
	result.setCoef(pivot, 0, result.getCoef(i,0));
	result.setCoef(i, 0, v);

        if(a.getCoef(i,i).getValue() == 0)
            std::cout << "ERREUR PIVOT." << std::endl;

        for( int j(i+1); j < m_l; j++)
        {
            coef = a.getCoef(j,i)/a.getCoef(i,i);
            for( int k(i); k < m_c; k++ )
            {
                a.setCoef( j,k, a.getCoef(j,k)-coef*a.getCoef(i,k) );

            }
            result.setCoef( j,0, result.getCoef(j,0)-coef*result.getCoef(i,0) );

        }
    }


    // On remonte
    for( int i(m_l-1); i>=0;i--)
    {

        GaloisFieldElement8bits value(0);
        for( int j(i+1); j < m_c; j++)
        {
            value = value + a.getCoef(i,j)*lambda.getCoef(j,0);
        }
        lambda.setCoef(i,0, (result.getCoef(i,0)-value) / a.getCoef(i,i) );
    }
    //lambda.setCoef(0,GaloisFieldElement8bits(1));



    return lambda;


}
/*
GaloisFieldMatrix GaloisFieldMatrix::gauss( GaloisFieldMatrix const& b ) const
{

    GaloisFieldMatrix a = GaloisFieldMatrix(*this);
    GaloisFieldElement8bits coef;
    GaloisFieldMatrix result(b);
    GaloisFieldMatrix lambda(b.getL(),1);

    // On met le systeme sous forme triangulaire
    for( int i(0); i < m_c; i++ )
    {
        if(a.getCoef(i,i).getValue() == 0)
            std::cout << "ERREUR PIVOT." << std::endl;

        for( int j(i+1); j < m_l; j++)
        {
            coef = a.getCoef(j,i)/a.getCoef(i,i);
            for( int k(i); k < m_c; k++ )
            {
                a.setCoef( j,k, a.getCoef(j,k)-coef*a.getCoef(i,k) );

            }
            result.setCoef( j,0, result.getCoef(j,0)-coef*result.getCoef(i,0) );

        }
    }


    // On remonte
    for( int i(m_l-1); i>=0;i--)
    {

        GaloisFieldElement8bits value(0);
        for( int j(i+1); j < m_c; j++)
        {
            value = value + a.getCoef(i,j)*lambda.getCoef(j,0);
        }
        lambda.setCoef(i,0, (result.getCoef(i,0)-value) / a.getCoef(i,i) );
    }
    //lambda.setCoef(0,GaloisFieldElement8bits(1));



    return lambda;


}*/
