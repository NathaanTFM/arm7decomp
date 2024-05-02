#include "Mongoose.h"

void WMSP_AutoDisconnect(void *msg)
{ // req_AutoDisconnect.c:49
    WMSP_DisconnectCore((u32 *)msg, 1, 0);
}