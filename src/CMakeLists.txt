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
						
SET(AIRJOY_SRC          AirJoy/AirJoyConfig.h
                        AirJoy/AirJoyCommon.h
                        AirJoy/AirJoyCommon.cpp
                        AirJoy/AirJoy.h
                        AirJoy/AirJoy.cpp
                        AirJoy/AirJoyProcessor.h
                        AirJoy/AirJoyProcessor.cpp
                        AirJoy/AirJoyProcessorDelegate.h
                        AirJoy/AirJoyMessage.h
                        AirJoy/AirJoyMessage.cpp
                        AirJoy/AirJoyRequest.h
                        AirJoy/AirJoyRequest.cpp
			AirJoy/AirJoyRequestDelegate.h
                        AirJoy/AirJoyRequestThread.h
                        AirJoy/AirJoyRequestThread.cpp
                        AirJoy/AirCriticalSection.h
                        AirJoy/AirCriticalSection.cpp
                        AirJoy/AirThread.h
                        AirJoy/AirThread.cpp)

SET(MINIIPC_SRC 	MiniIpc/include/MiniNotify.h
                	MiniIpc/include/MiniNotifyInfo.h
                	MiniIpc/include/MiniNotifyTypedef.h
                	MiniIpc/src/MiniNotifyInfo.cpp
                	MiniIpc/src/MiniNotify.cpp)

ADD_LIBRARY(AirJoy_static STATIC ${TCPSERVER_SRC} ${AIRJOY_SRC} ${MINIIPC_SRC})
