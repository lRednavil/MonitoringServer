#pragma once

#include <iostream>
#include <Windows.h>

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

#pragma comment (lib, "NetworkLibrary")
#pragma comment (lib, "Winmm")

