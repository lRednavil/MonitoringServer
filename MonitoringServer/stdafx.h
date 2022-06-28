#pragma once

#include <iostream>
#include <WinSock2.h>
#include <Windows.h>

#include <MSWSock.h>

#include "CommonProtocol.h"

#include "include/Dump.h"
#include "include/ErrorReason.h"
#include "include/LockFreeMemoryPool.h"
#include "include/LockFreeQueue.h"
#include "include/LockFreeStack.h"
#include "include/Logging.h"
#include "include/SerializedBuffer.h"
#include "include/TLSMemoryPool.h"

#include "include/LanClient.h"
#include "include/LanServer.h"
#include "include/NetServer.h"

#include "TLSDBConnector.h"

#pragma comment (lib, "WS2_32")
#pragma comment (lib, "NetworkLibrary")
#pragma comment (lib, "Winmm")
#pragma comment (lib, "mysqlclient")

