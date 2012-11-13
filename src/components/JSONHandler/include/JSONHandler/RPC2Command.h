#ifndef RPC2COMMAND_CLASS
#define RPC2COMMAND_CLASS

/**
  *\namespace RPC2Communication
  *\brief Namespace for communication with HMI.
*/
namespace RPC2Communication
{
    /**
     * \class RPC2Command
     * \brief Base class for objects generated from Json RPC2
    */
    class RPC2Command
    {
    public:
        /**
         * \enum CommandType
         * \brief RPC2 message types.
        */
        enum CommandType { REQUEST = 0x0, RESPONSE = 0x1, NOTIFICATION = 0x2, ERROR = 0x3, UNDEFINED };
    public:
        /**
         * \brief Constructor
        */
        RPC2Command( );

        /**
         * \brief Constructor
         * \param type RPC2 type
         * \sa CommandType
        */
        RPC2Command( CommandType type );

        /**
         * \brief Constructor
         * \param type RPC2 type
         * \sa CommandType
         * \param method ID of the method.
        */
        RPC2Command( CommandType type, int method );

        /**
         * \brief Destructor
        */
        virtual ~RPC2Command();

        /**
         * \brief Getter for message type.
         * \return Type of Json RPC2 message.
         * \sa CommandType.
         */
        virtual CommandType getCommandType( ) const;

        /**
         * \brief Setter for message type.
         * \param commandType Type of Json RPC2 message.
         */
        virtual void setCommandType( CommandType commandType );

        /**
         * \brief Getter for method id
         * \return Id of the method in message.
         */
        virtual int getMethod() const;

        /**
         * \brief Setter for method id.
         * \param method Id of method in message.
         */
        virtual void setMethod( int method );

    protected:
        /**
          *\brief Type of Json RPC2 message.
        */
        CommandType mCommandType;

        /**
          *\brief Id of method in message.
        */
        int mMethod;
    };

}

#endif
