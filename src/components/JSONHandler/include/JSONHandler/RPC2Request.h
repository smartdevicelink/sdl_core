#ifndef RPC2_REQUEST_CLASS
#define RPC2_REQUEST_CLASS

#include "RPC2Command.h"

/**
  *\namespace RPC2Communication
  *\brief Namespace for communication with HMI.
*/
namespace RPC2Communication
{  
    /**
     * \class RPC2Request
     * \brief Base class for requests generated from Json RPC2
    */
    class RPC2Request : public RPC2Command
    {
    public:
        /**
         * \brief Default Constructor
        */
        RPC2Request( );

        /**
         * \brief Constructor
         * \param method Method Id.
        */
        RPC2Request( int method );

        /**
         * \brief Constructor
         * \param method Method Id.
         * \param id Message id.
        */
        RPC2Request( int method ,unsigned int id);

        /**
         * \brief Destructor
        */
        virtual ~RPC2Request();

        /**
         * \brief Getter of message id.
         * \return Message id.
         */
        virtual unsigned int getId() const;

        /**
         * \brief Setter of message id.
         * \param id Message id.
         */
        virtual void setId(unsigned int id);        

    private:
        /**
          *\brief Message id
          * //! mId should be >0
        */
        unsigned int mId;
        
    };

}

#endif
