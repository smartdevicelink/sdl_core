#ifndef LANGUAGE_STRUCT
#define LANGUAGE_STRUCT value

#include <string>

struct Language
{
    enum LanguageEnum { EN_US, ES_MX, FR_CA };

    void setInternalString( std::string internalName ) { mInternalName = internalName; }

    static LanguageEnum valueForString ( const std::string & input ) 
    {
        if ( input.compare( "EN-US" ) == 0 )
        {
            return EN_US;
        }

        if ( input.compare( "ES-MX" ) == 0 )
        {
            return ES_MX;
        }

        if ( input.compare( "FR-CA" ) == 0 )
        {
            return FR_CA;
        }
    }

    std::string toString() const { return mInternalName; }

private:
    std::string mInternalName;
};

#endif