#ifndef CANLIB_H
#define CANLIB_H

#include <QtCore/QObject>
#include "webServer.h"
#include "server.h"
#include "FileSystem.h"

class CANlib : public QObject
{
    Q_OBJECT
public:

    CANlib();

    ~CANlib();

    /**
     * @brief init
     * @return true if connections are established
     *
     * Connect all CANlib external interfaces with internal TCP server, WS server
     *  and FileSystem class that saves data to log file
     *
     */
    bool init();

public slots:

    /**
     * @brief createConectionTCP
     * @param IP
     * @param port
     *
     * Invoke method to start listening TCP port on specified IP address
     * After new connection with TCP client established TCP server generates
     * thread that manage all data transfer with TCP client
     *
     * emit's signal internal_createConectionTCP for thread
     */
    void createConectionTCP(const QString &IP, int port);

    /**
     * @brief writeToTCP
     * @param message
     *
     * Method to send messager to TCP client
     *
     * emit's signal internal_writeToTCP for thread
     */
    void writeToTCP(const QString &message);

    /**
     * @brief createConectionWS
     * @param IP
     * @param port
     *
     * Invoke method to start listening WS port on specified IP address
     * and use connection with last newly connected WS client
     *
     * emit's signal internal_createConectionWS for private instance of WebServer
     */
    void createConectionWS(const QString &IP, int port);

    /**
     * @brief writeToWS
     * @param message
     *
     * Method to send messager to WS client
     *
     * emit's signal internal_writeToWS for private instance of WebServer
     */
    void writeToWS(const QString &message);

    /**
     * @brief writeToFS
     * @param path
     * @param message
     *
     * To save data on File System use writeToFS that crates file if not
     * exist or rewrite existed file with new message
     *
     * emit's signal internal_writeToWS for private instance of FileSystem
     */
    void writeToFS(const QString &path, const QString &message);

    /**
     * @brief readFromTCP
     * @param qMessage
     *
     * SLOT emit's external interface incomingMesageTCP
     */
    void readFromTCP(const QString &qMessage);

    /**
     * @brief readFromWS
     * @param message
     *
     * SLOT emit's external interface incomingMesageWS
     */
    void readFromWS(const QString &message);

    /**
     * @brief log
     * @param qMessage
     * @param color
     *
     * SLOT emit's external interface loger
     */
    void log(const QString &qMessage, int color);


signals:

    /**
     * @brief incomingMesageWS
     * @param qMessage
     *
     * External interface with data came from WS client
     */
    void incomingMesageWS(const QString &qMessage);

    /**
     * @brief incomingMesageTCP
     * @param qMessage
     *
     * External interface with data came from TCP client
     */
    void incomingMesageTCP(const QString &qMessage);

    /**
     * @brief loger
     * @param qMessage
     * @param color
     *
     * External interface with data to be used for displaying log messages
     */
    void loger(const QString &qMessage, int color);

    //--------------------------INTERNAL-SIGNALS------------------------------

    /**
     * @brief internal_createConectionTCP
     * @param IP
     * @param port
     *
     * Internal signal interface for TCP server
     */
    void internal_createConectionTCP(const QString &IP, int port);

    /**
     * @brief internal_writeToTCP
     * @param message
     *
     * Internal signal interface for TCP server
     */
    void internal_writeToTCP(const QString &message);

    /**
     * @brief internal_createConectionWS
     * @param IP
     * @param port
     *
     * Internal signal interface for WS server
     */
    void internal_createConectionWS(const QString &IP, int port);

    /**
     * @brief internal_writeToWS
     * @param message
     *
     * Internal signal interface for WS server
     */
    void internal_writeToWS(const QString &message);

    /**
     * @brief internal_writeToFS
     * @param path
     * @param message
     *
     * Internal signal interface for private instance of FileSystem
     */
    void internal_writeToFS(const QString &path, const QString &message);


private:
    WebServer webServer;
    Server tcpServer;
    FileSystem fileSystem;
};

#endif // CANLIB_H
