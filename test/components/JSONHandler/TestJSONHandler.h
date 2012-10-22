#ifndef TEST_JSON_HANDLER_CLASS
#define TEST_JSON_HANDLER_CLASS 

#include "JSONHandler/JSONHandler.h"

class TestJSONHandler
{
public:
    TestJSONHandler() {}
    ~TestJSONHandler() {}
    void secondRelease( const std::string & jsonString );
    void firstRelease( const std::string & jsonString );
    void RPC2( const std::string & jsonString );

    /* data */
    JSONHandler * jsonHandler;
};


#endif