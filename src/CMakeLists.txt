#-------------------------------------------------------------------------------------
#
# module:AirJoy
#
#-------------------------------------------------------------------------------------

SET(TCPSERVER_SRC       TcpServer/TcpMaster.h
			TcpServer/TcpWorker.h
			TcpServer/TcpRequestHandler.h
			TcpServer/TcpMaster.cpp
			TcpServer/TcpWorker.cpp
			AirJoy/AirJoy.h
			AirJoy/AirJoy.cpp)

ADD_LIBRARY(TcpServer_static STATIC ${TCPSERVER_SRC})
