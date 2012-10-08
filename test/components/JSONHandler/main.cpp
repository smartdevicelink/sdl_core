



#include <json/value.h>
#include <json/reader.h>
#include <json/writer.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include "JSONHandler/MobileRPCMessage.h"
#include "JSONHandler/JSONHandler.h"

char * readJsonContent ( const char * file_name );
int parseJson ( const std::string & json_content );

int main( int argc, char* argv[] ) {
    char * fileName = "/home/dev/Projects/Ford/source/workspace/jsonSample/connection_log_json_only" ;    
    if ( argc > 1 )
    {
        fileName = argv[1];
    }

    char * json_content = readJsonContent( fileName );

    int result = 0;
    size_t next_message_pos = 0;
    char *const json_content_beginning = json_content;

    while ( next_message_pos != std::string::npos ) {

        std::string json_string( json_content );
        if ( !json_string.empty() ) {
            MobileRPCMessage * message = JSONHandler::createObjectFromJSON( json_string );
            std::cout << "type: " << message->getMessageType() << std::endl;
            std::cout << "protocol version: " << message->getProtocolVersion() << std::endl;
            std::cout << "correation id: " << message -> getCorrelationID() << std::endl;
            std::cout << "function name: " << message -> getFunctionName() << std::endl;
            std::cout << "original string: " << message -> getOriginalString() << std::endl;
            delete message;
        }

        next_message_pos = json_string.find( "\n" );
        json_content += next_message_pos + 1;

        //sleep( 1 );
    }

    delete [] json_content_beginning;

    return result;
}

char *
readJsonContent ( const char * file_name ) {
    using namespace std;

    ifstream file_str;
    file_str.open ( file_name );

    file_str.seekg( 0, ios::end );
    int length = file_str.tellg();
    file_str.seekg( 0, ios::beg );

    char * raw_data = new char[length];
    file_str.read( raw_data, length );
    file_str.close();

    return raw_data;
}

int
parseJson ( const std::string & json_content ) {
    Json::Value root;   // will contains the root value after parsing.
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( json_content, root );
    if ( !parsingSuccessful ) {
        std::cout  << "Failed to parse configuration\n"
                   << reader.getFormatedErrorMessages();
        return -1;
    }

    // Get the value of the member of root named 'encoding', return 'UTF-8' if there is no
    // such member.
    //Json::Value val = root.get("encoding", Json::Value::null);
    Json::Value val = root["request"];
    if ( val.isNull() ) {
        std::cout<<"null"<<std::endl;
    } else if ( val.isArray() ) {
        //for ( int i = 0; i < val.size(); ++i ) {
            Json::Value x = val["correlationID"];
            if ( x.isNull() ) {
                std::cout << "no correlationID" << std::endl;
            }
        //}
    } else {
        Json::Value x = val["correlationID"];
        if ( x.isNull() ) {
            std::cout << "no correlationID" << std::endl;
        } else {
            std::cout << x.asInt() << std::endl;
        }
        std::cout << "smth" << std::endl;
    }
    //.asString();
    //std::cout << encoding << std::endl;

    Json::StyledWriter writer;
    std::string root_to_print = writer.write( root );
    std::cout << root_to_print;
    return 0;
}
