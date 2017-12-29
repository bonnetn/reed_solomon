#ifndef GALOISFIELDELEMENT8BITS_H
#define GALOISFIELDELEMENT8BITS_H

#include <iostream>
#include <cstdint>

class GaloisFieldElement8bits
{
    public:
        GaloisFieldElement8bits(int value);
        GaloisFieldElement8bits();

        virtual ~GaloisFieldElement8bits();

        uint8_t getValue() const;
        void setValue( uint8_t value );
        std::string getString() const;
        int getMsbPos() const;
        GaloisFieldElement8bits getInv() const;

        static bool lookupTablesGenerated();
        static GaloisFieldElement8bits getPower( int n );
        static uint8_t getLog( GaloisFieldElement8bits value );

        static GaloisFieldElement8bits multiply( GaloisFieldElement8bits const& a, GaloisFieldElement8bits const& b);

    protected:
    private:
        uint8_t m_value;

        static bool lookup_tables_generated;
        static GaloisFieldElement8bits lookup_table[256];
        static uint8_t lookup_table_log[256];
        static void generateLookupTables();



};

GaloisFieldElement8bits operator+( GaloisFieldElement8bits const& a, GaloisFieldElement8bits const& b);
GaloisFieldElement8bits operator-( GaloisFieldElement8bits const& a, GaloisFieldElement8bits const& b);
GaloisFieldElement8bits operator*( GaloisFieldElement8bits const& a, GaloisFieldElement8bits const& b);
GaloisFieldElement8bits operator/( GaloisFieldElement8bits const& a, GaloisFieldElement8bits const& b);
GaloisFieldElement8bits operator^( GaloisFieldElement8bits const& a, unsigned int pow);

#endif // GALOISFIELDELEMENT8BITS_H
