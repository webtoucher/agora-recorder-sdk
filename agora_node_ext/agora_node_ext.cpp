#include "agora_node_ext.h"
#include "agora_node_recording.h"
using v8::Object;
using agora::recording::AgoraRecorder;

/**
 * Initialize NODEJS ADDON
 */
void InitExt(Local<Object> module)
{
    LOG_ENTER;
    AgoraRecorder::Init(module);
    LOG_LEAVE;
}

/**
 * NODEJS registration
 */
NAPI_MODULE(NODE_GYP_MODULE_NAME, InitExt)
