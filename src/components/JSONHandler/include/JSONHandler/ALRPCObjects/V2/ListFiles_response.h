#ifndef NSAPPLINKRPCV2_LISTFILES_RESPONSE_INCLUDE
#define NSAPPLINKRPCV2_LISTFILES_RESPONSE_INCLUDE

#include <vector>
#include <string>

#include "JSONHandler/ALRPCResponse.h"


/*
  interface Ford Sync RAPI
  version   2.0O
  date      2012-11-02
  generated at  Thu Jan 24 06:36:23 2013
  source stamp  Thu Jan 24 06:35:41 2013
  author    robok0der
*/

namespace NsAppLinkRPCV2
{
    /**
         Returns the current list of resident filenames for the registered app along with the current space available
         Not supported on First generation SYNC vehicles.
    */
    class ListFiles_response : public NsAppLinkRPC::ALRPCResponse
    {
    public:
        ListFiles_response(const ListFiles_response& c);
        ListFiles_response(void);

        virtual ~ListFiles_response(void);

        ListFiles_response& operator =(const ListFiles_response&);

        bool checkIntegrity(void);

        const std::vector<std::string>* get_filenames(void) const;
        unsigned int get_spaceAvailable(void) const;

        void reset_filenames(void);
        bool set_filenames(const std::vector<std::string>& filenames_);
        bool set_spaceAvailable(unsigned int spaceAvailable_);

    private:
        friend class ListFiles_responseMarshaller;

        /**
             An array of all filenames resident on SYNC for the given registered app.
             If omitted, then no files currently reside on the system.
        */
        std::vector<std::string>* filenames;  //!<   [%s..%s] (500)

        ///  Provides the total local space available on SYNC for the registered app.
        unsigned int spaceAvailable;  //!<  (0,2000000000)
    };
}

#endif
