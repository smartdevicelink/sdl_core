1. Install log4cxx library using the following command:

	sudo apt-get install liblog4cxx10 liblog4cxx10-dev

2. Replace 'log4cplus' statement with 'log4j' in your log4cplus.properties file.

3. Define the SocketHubAppender the way it is shown below:

	log4j.appender.SOCKET=org.apache.log4j.net.SocketHubAppender 
	log4j.appender.SOCKET.port=4555
	log4j.appender.SOCKET.locationInfo=true

4. Configure your logger using the following instruction in your C++ code:

	log4cxx::PropertyConfigurator::configure("log4cxx.properties");

5. See the example of instantiating new logger below:

	log4cxx::LoggerPtr ProtocolHandlerImpl::logger_ =
		log4cxx::LoggerPtr(log4cxx::Logger::getLogger( "ProtocolHandler"));	

6. The main macros for C++ code are:

	LOG4CXX_DEBUG, LOG4CXX_INFO, LOG4CXX_WARN, LOG4CXX_ERROR, LOG4CXX_FATAL

and have the same signatures as corresponding macros in log4cplus logging library. For more specific macros see documentation on http://logging.apache.org/log4cxx/ .

7. Run olv-2013-01-24/olv.bat on Windows or olv-2013-01-24/olv.sh on Linux. For viewing logs in OtrosLogViewer go to File -> Connect to Log4j socket hub, enter ip-address and port number.

 


