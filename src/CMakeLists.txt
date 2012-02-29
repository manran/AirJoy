#-------------------------------------------------------------------------------------
#
# module:AirJoy
#
#-------------------------------------------------------------------------------------

SET(TCPSERVER_SRC       AirJoy/TcpMaster.h
			AirJoy/TcpWorker.h
			AirJoy/TcpRequestHandler.h
			AirJoy/TcpMaster.cpp
			AirJoy/TcpWorker.cpp)
						
SET(AIRJOY_SRC          AirJoy/AirJoy.h
                        AirJoy/AirJoyConfig.h
                        AirJoy/AirJoyProcessor.h
                        AirJoy/AirJoy.cpp
                        AirJoy/AirJoyProcessor.cpp
                        AirJoy/AirJoyMessage.h
                        AirJoy/AirJoyMessage.cpp)

ADD_LIBRARY(AirJoy_static STATIC ${TCPSERVER_SRC} ${AIRJOY_SRC})
