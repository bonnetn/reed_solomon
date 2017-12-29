#include "GaloisFieldElement8bits.h"
#include <bitset>

#define magic_modulo_number 0x11D

bool GaloisFieldElement8bits::lookup_tables_generated = false;
GaloisFieldElement8bits GaloisFieldElement8bits::lookup_table[256];
uint8_t GaloisFieldElement8bits::lookup_table_log[256];

void GaloisFieldElement8bits::generateLookupTables()
{

    if(!lookupTablesGenerated())
    {
        lookup_tables_generated = true;
        GaloisFieldElement8bits m(2);
        GaloisFieldElement8bits alpha(1);

        for(int i(0);i<256;i++)
        {
            lookup_table[i] = alpha;
            lookup_table_log[alpha.getValue()] = (uint8_t) i;
            alpha = multiply(alpha,m);

        }
    }

}
bool GaloisFieldElement8bits::lookupTablesGenerated()
{
    bool v = lookup_tables_generated;
    lookup_tables_generated = true;
    return v;
}

GaloisFieldElement8bits GaloisFieldElement8bits::getPower( int n )
{
    generateLookupTables();
    return lookup_table[n];
}

uint8_t GaloisFieldElement8bits::getLog( GaloisFieldElement8bits value )
{
    generateLookupTables();
    return lookup_table_log[value.getValue()];
}

GaloisFieldElement8bits::GaloisFieldElement8bits( int value )
{
    if(value >= 0)
        m_value = (uint8_t) value;
    else
    {        generateLookupTables();


        uint8_t nb(value+magic_modulo_number);
        for(int i=16-1; i >=8; i--)
        if(nb & (1<<i))
            nb ^= magic_modulo_number<<(i-8);

        std::cout << value << " becomes " << (int) nb << std::endl;
        m_value = lookup_table[ ((int)lookup_table_log[nb]) ].getValue();
    }


}

GaloisFieldElement8bits::GaloisFieldElement8bits() :
    m_value(0)
{
}

GaloisFieldElement8bits::~GaloisFieldElement8bits()
{
    //dtor
}

uint8_t GaloisFieldElement8bits::getValue() const
{
    return m_value;
}

void GaloisFieldElement8bits::setValue( uint8_t value )
{
    m_value = value;
}
std::string GaloisFieldElement8bits::getString() const
{
    std::string str;
    for(int i=7; i>=0; i--)
    {
        if(m_value & 1<<i)
            str.append("1");
        else
            str.append("0");
        //str.append(" ");
    }
    return str;
}

GaloisFieldElement8bits operator+( GaloisFieldElement8bits const& a, GaloisFieldElement8bits const& b)
{
    return GaloisFieldElement8bits(a.getValue()^b.getValue());
}

GaloisFieldElement8bits operator-( GaloisFieldElement8bits const& a, GaloisFieldElement8bits const& b)
{
    return a+b;
}

GaloisFieldElement8bits GaloisFieldElement8bits::multiply( GaloisFieldElement8bits const& a, GaloisFieldElement8bits const& b)
{
    uint16_t result = 0;
    uint8_t valA = a.getValue();
    uint8_t valB = b.getValue();

    /* On multiplie les bytes */
    for(int i=0; i < 8; i++)
        if(valB & (1<<i))
            result ^= valA << i;

    /* On cherche le reste de la division du resultat par 0x11D */
    /* pour avoir un nombre qui appartient à GL(2^8) */
    /* 011D est un polynôme irréductible de degré 8 (cf Rijndael finite field)*/
    for(int i=16-1; i >=8; i--)
        if(result & (1<<i))
            result ^= magic_modulo_number<<(i-8);


    return GaloisFieldElement8bits(result);
}

GaloisFieldElement8bits operator*( GaloisFieldElement8bits const& a, GaloisFieldElement8bits const& b)
{
    if(a.getValue() == 0 || b.getValue() == 0)
        return GaloisFieldElement8bits(0);
    return GaloisFieldElement8bits::getPower( (GaloisFieldElement8bits::getLog(a)+GaloisFieldElement8bits::getLog(b))%255 );
}


int GaloisFieldElement8bits::getMsbPos() const
{
    int pos(0);
    while(m_value >> pos)
        pos++;
    return pos;
}

GaloisFieldElement8bits operator/( GaloisFieldElement8bits const& a, GaloisFieldElement8bits const& b)
{
    if(a.getValue() == 0)
        return GaloisFieldElement8bits(0);
    return GaloisFieldElement8bits::getPower( ((int)GaloisFieldElement8bits::getLog(a)+255-(int)GaloisFieldElement8bits::getLog(b))%255 );


}


GaloisFieldElement8bits operator^( GaloisFieldElement8bits const& a, unsigned int pow)
{
    GaloisFieldElement8bits result(1);
    for(int i(0); i < pow; i++)
    {
        result = result*a;
    }
    return result;
}

GaloisFieldElement8bits GaloisFieldElement8bits::getInv() const
{
    return GaloisFieldElement8bits( getPower( (255-getLog(m_value))%255 ) );
}
