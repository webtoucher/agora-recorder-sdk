#include <csignal>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "IAgoraLinuxSdkCommon.h"
#include "IAgoraRecordingEngine.h"
#include "AgoraSdk.h"
#include "node_async_queue.h"

#include "base/atomic.h"
#include "opt_parser.h"
namespace agora {

#define MAKE_JS_CALL_0(ev) \
        auto it = m_callbacks.find(ev); \
        if (it != m_callbacks.end()) {\
            Isolate *isolate = Isolate::GetCurrent();\
            HandleScope scope(isolate);\
            NodeEventCallback& cb = *it->second;\
            [&]{\
                return cb.callback.Get(isolate)->Call(isolate->GetCurrentContext(), cb.js_this.Get(isolate), 0, nullptr);\
            }();\
        }

#define MAKE_JS_CALL_1(ev, type, param) \
        auto it = m_callbacks.find(ev); \
        if (it != m_callbacks.end()) {\
            Isolate *isolate = Isolate::GetCurrent();\
            HandleScope scope(isolate);\
            Local<Value> argv[1]{ napi_create_##type##_(isolate, param)\
                                };\
            NodeEventCallback& cb = *it->second;\
            [&]{\
                return cb.callback.Get(isolate)->Call(isolate->GetCurrentContext(), cb.js_this.Get(isolate), 1, argv);\
            }();\
        }

#define MAKE_JS_CALL_2(ev, type1, param1, type2, param2) \
        auto it = m_callbacks.find(ev); \
        if (it != m_callbacks.end()) {\
            Isolate *isolate = Isolate::GetCurrent();\
            HandleScope scope(isolate);\
            Local<Value> argv[2]{ napi_create_##type1##_(isolate, param1),\
                                  napi_create_##type2##_(isolate, param2)\
                                };\
            NodeEventCallback& cb = *it->second;\
            [&]{\
                return cb.callback.Get(isolate)->Call(isolate->GetCurrentContext(), cb.js_this.Get(isolate), 2, argv);\
            }();\
        }

#define MAKE_JS_CALL_3(ev, type1, param1, type2, param2, type3, param3) \
        auto it = m_callbacks.find(ev); \
        if (it != m_callbacks.end()) {\
            Isolate *isolate = Isolate::GetCurrent();\
            HandleScope scope(isolate);\
            Local<Value> argv[3]{ napi_create_##type1##_(isolate, param1),\
                                  napi_create_##type2##_(isolate, param2),\
                                  napi_create_##type3##_(isolate, param3) \
                                };\
            NodeEventCallback& cb = *it->second;\
            [&]{\
                return cb.callback.Get(isolate)->Call(isolate->GetCurrentContext(), cb.js_this.Get(isolate), 3, argv);\
            }();\
        }

#define MAKE_JS_CALL_4(ev, type1, param1, type2, param2, type3, param3, type4, param4) \
        auto it = m_callbacks.find(ev); \
        if (it != m_callbacks.end()) {\
            Isolate *isolate = Isolate::GetCurrent();\
            HandleScope scope(isolate);\
            Local<Value> argv[4]{ napi_create_##type1##_(isolate, param1),\
                                  napi_create_##type2##_(isolate, param2),\
                                  napi_create_##type3##_(isolate, param3), \
                                  napi_create_##type4##_(isolate, param4), \
                                };\
            NodeEventCallback& cb = *it->second;\
            [&]{\
                return cb.callback.Get(isolate)->Call(isolate->GetCurrentContext(), cb.js_this.Get(isolate), 4, argv);\
            }();\
        }

#define MAKE_JS_CALL_5(ev, type1, param1, type2, param2, type3, param3, type4, param4, type5, param5) \
        auto it = m_callbacks.find(ev); \
        if (it != m_callbacks.end()) {\
            Isolate *isolate = Isolate::GetCurrent();\
            HandleScope scope(isolate);\
            Local<Value> argv[5]{ napi_create_##type1##_(isolate, param1),\
                                  napi_create_##type2##_(isolate, param2),\
                                  napi_create_##type3##_(isolate, param3), \
                                  napi_create_##type4##_(isolate, param4), \
                                  napi_create_##type5##_(isolate, param5), \
                                };\
            NodeEventCallback& cb = *it->second;\
            [&]{\
                return cb.callback.Get(isolate)->Call(isolate->GetCurrentContext(), cb.js_this.Get(isolate), 5, argv);\
            }();\
        }


#define NODE_SET_OBJ_PROP_UINT32(obj, name, val) \
    { \
        Local<Value> propName = String::NewFromUtf8(isolate, name, NewStringType::kInternalized).ToLocalChecked(); \
        Local<Value> propVal = napi_create_uint32_(isolate, val); \
        v8::Maybe<bool> ret = obj->Set(isolate->GetCurrentContext(), propName, propVal); \
        if(!ret.IsNothing()) { \
            if(!ret.ToChecked()) { \
                break; \
            } \
        } \
    }
#define NODE_SET_OBJ_PROP_UID(obj, name, val) \
    { \
        Local<Value> propName = String::NewFromUtf8(isolate, name, NewStringType::kInternalized).ToLocalChecked(); \
        Local<Value> propVal = NodeUid::getNodeValue(isolate, val); \
        v8::Maybe<bool> ret = obj->Set(isolate->GetCurrentContext(), propName, propVal); \
        if(!ret.IsNothing()) { \
            if(!ret.ToChecked()) { \
                break; \
            } \
        } \
    }
#define NODE_SET_OBJ_PROP_STRING(obj, name, val) \
    { \
        Local<Value> propName = String::NewFromUtf8(isolate, name, NewStringType::kInternalized).ToLocalChecked(); \
        Local<Value> propVal = String::NewFromUtf8(isolate, val, NewStringType::kInternalized).ToLocalChecked(); \
        v8::Maybe<bool> ret = obj->Set(isolate->GetCurrentContext(), propName, propVal); \
        if(!ret.IsNothing()) { \
            if(!ret.ToChecked()) { \
                break; \
            } \
        } \
    }

AgoraSdk::AgoraSdk(): IRecordingEngineEventHandler() {
  m_engine = NULL;
  m_log_level = agora::linuxsdk::agora_log_level::AGORA_LOG_LEVEL_INFO;
  m_stopped.store(false);
  m_storage_dir = "./";
}

AgoraSdk::~AgoraSdk() {
  if (m_engine) {
    m_engine->leaveChannel();
    m_engine->release();
  }
}

bool AgoraSdk::stopped() const {
  return m_stopped;
}

bool AgoraSdk::release() {
  if (m_engine) {
    m_engine->release();
    m_engine = NULL;
  }

  return true;
}

bool AgoraSdk::setLogLevel(agora::linuxsdk::agora_log_level level) {
    m_log_level = level;
    if (m_engine) {
        m_engine->setLogLevel(level);
    }

    return true;
}

bool AgoraSdk::createChannel(const string &appid, const string &token, const string &name, const string &uid,
    agora::recording::RecordingConfig &config)
{
    if ((m_engine = agora::recording::IRecordingEngine::createAgoraRecordingEngine(appid.c_str(), this)) == NULL)
        return false;

    if (m_log_level != agora::linuxsdk::agora_log_level::AGORA_LOG_LEVEL_INFO) {
        m_engine->setLogLevel(m_log_level);
    }
    if (linuxsdk::ERR_OK != m_engine->joinChannelWithUserAccount(token.c_str(), name.c_str(), uid.c_str(), config))
        return false;

    m_config = config;
    return true;
}

bool AgoraSdk::leaveChannel() {
  if (m_engine) {
    m_engine->leaveChannel();
    m_stopped = true;
  }

  return true;
}

int AgoraSdk::startService() {
  if (m_engine)
    return m_engine->startService();

  return 1;
}

int AgoraSdk::stopService() {
  if (m_engine)
    return m_engine->stopService();

  return 1;
}

void AgoraSdk::updateMixLayout(agora::linuxsdk::VideoMixingLayout &layout) {
    m_layout = layout;
    setVideoMixLayout();
}

agora::linuxsdk::VideoMixingLayout AgoraSdk::getMixLayout() {
    return m_layout;
}

//Customize the layout of video under video mixing model
int AgoraSdk::setVideoMixLayout()
{
    // recording::RecordingConfig *pConfig = getConfigInfo();
    // size_t max_peers = pConfig->channelProfile == linuxsdk::CHANNEL_PROFILE_COMMUNICATION ? 7:17;
    if(m_layout.regionCount == 0) return 0;

    return setVideoMixingLayout(m_layout);
}

int AgoraSdk::setVideoMixingLayout(const agora::linuxsdk::VideoMixingLayout &layout)
{
   int result = -agora::linuxsdk::ERR_INTERNAL_FAILED;
   if(m_engine)
      result = m_engine->setVideoMixingLayout(layout);
   return result;
}

const agora::recording::RecordingEngineProperties* AgoraSdk::getRecorderProperties(){
    return m_engine->getProperties();
}

void AgoraSdk::onErrorImpl(int error, agora::linuxsdk::STAT_CODE_TYPE stat_code) {
    cerr << "Error: " << error <<",with stat_code:"<< stat_code << endl;
    m_engine->stoppedOnError();
    agora::recording::node_async_call::async_call([this, error, stat_code]() {
        MAKE_JS_CALL_2(REC_EVENT_ERROR, int32, error, int32, stat_code);
    });
}

void AgoraSdk::onWarningImpl(int warn) {
    cerr << "warn: " << warn << endl;
    //  leaveChannel();
}

void AgoraSdk::onJoinChannelSuccessImpl(const char * channelId, agora::linuxsdk::uid_t uid) {
    string channelName = channelId;
    agora::recording::node_async_call::async_call([this, channelName, uid]() {
        MAKE_JS_CALL_2(REC_EVENT_JOIN_CHANNEL, string, channelName.c_str(), uid, uid);
    });
}

void AgoraSdk::onLeaveChannelImpl(agora::linuxsdk::LEAVE_PATH_CODE code) {
    agora::recording::node_async_call::async_call([this]() {
        MAKE_JS_CALL_0(REC_EVENT_LEAVE_CHANNEL);
    });
}

void AgoraSdk::onUserJoinedImpl(unsigned uid, agora::linuxsdk::UserJoinInfos &infos) {
    if(infos.storageDir)
    {
        m_storage_dir = std::string(infos.storageDir);
        m_logdir = m_storage_dir;
    }

    m_peers.push_back(uid);

    //When the user joined, we can re-layout the canvas
    setVideoMixLayout();

    agora::recording::node_async_call::async_call([this, uid]() {
        MAKE_JS_CALL_1(REC_EVENT_USER_JOIN, uid, uid);
    });
}


void AgoraSdk::onUserOfflineImpl(unsigned uid, agora::linuxsdk::USER_OFFLINE_REASON_TYPE reason) {
    m_peers.erase(std::remove(m_peers.begin(), m_peers.end(), uid), m_peers.end());

    //When the user is offline, we can re-layout the canvas
    setVideoMixLayout();

    agora::recording::node_async_call::async_call([this, uid]() {
        MAKE_JS_CALL_1(REC_EVENT_USER_LEAVE, uid, uid);
    });
}

void AgoraSdk::onActiveSpeaker(uid_t uid) {
    agora::recording::node_async_call::async_call([this, uid]() {
        MAKE_JS_CALL_1(REC_EVENT_ACTIVE_SPEAKER, uid, uid);
    });
}

void AgoraSdk::audioFrameReceivedImpl(unsigned int uid, const agora::linuxsdk::AudioFrame *pframe) const
{
  char uidbuf[65];
  snprintf(uidbuf, sizeof(uidbuf),"%u", uid);
  std::string info_name = m_storage_dir + std::string(uidbuf) /*+ timestamp_per_join_*/;

  const uint8_t* data = NULL;
  uint32_t size = 0;
  if (pframe->type == agora::linuxsdk::AUDIO_FRAME_RAW_PCM) {
    info_name += ".pcm";

    agora::linuxsdk::AudioPcmFrame *f = pframe->frame.pcm;
    data = f->pcmBuf_;
    size = f->pcmBufSize_;
  } else if (pframe->type == agora::linuxsdk::AUDIO_FRAME_AAC) {
    info_name += ".aac";

    agora::linuxsdk::AudioAacFrame *f = pframe->frame.aac;
    data = f->aacBuf_;
    size = f->aacBufSize_;
  }

  FILE *fp = fopen(info_name.c_str(), "a+b");
  if(fp == NULL) {
      return;
  }

  ::fwrite(data, 1, size, fp);
  ::fclose(fp);
}

void AgoraSdk::videoFrameReceivedImpl(unsigned int uid, const agora::linuxsdk::VideoFrame *pframe) const {
  char uidbuf[65];
  snprintf(uidbuf, sizeof(uidbuf),"%u", uid);
  const char * suffix=".vtmp";
  if (pframe->type == agora::linuxsdk::VIDEO_FRAME_RAW_YUV) {
    agora::linuxsdk::VideoYuvFrame *f = pframe->frame.yuv;
    suffix=".yuv";
  } else if(pframe->type == agora::linuxsdk::VIDEO_FRAME_JPG) {
    suffix=".jpg";
    agora::linuxsdk::VideoJpgFrame *f = pframe->frame.jpg;

    struct tm date;
    time_t t = time(NULL);

    #if (defined(_WIN32) || defined(_WIN64))
    localtime_s(&date, &t);
    #else
    localtime_r(&t, &date);
    #endif

    char timebuf[128];
    sprintf(timebuf, "%04d%02d%02d%02d%02d%02d", date.tm_year + 1900, date.tm_mon + 1, date.tm_mday, date.tm_hour, date.tm_min, date.tm_sec);
    std::string file_name = m_storage_dir + std::string(uidbuf) + "_" + std::string(timebuf) + suffix;
    FILE *fp = fopen(file_name.c_str(), "w");
    if(fp == NULL) {
        return;
    }

    ::fwrite(f->buf_, 1, f->bufSize_, fp);
    ::fclose(fp);
    return;
  } else {
    suffix=".h264";
    agora::linuxsdk::VideoH264Frame *f = pframe->frame.h264;
  }

  std::string info_name = m_storage_dir + std::string(uidbuf) /*+ timestamp_per_join_ */+ std::string(suffix);
  FILE *fp = fopen(info_name.c_str(), "a+b");
  if(fp == NULL) {
        return;
  }


  //store it as file
  if (pframe->type == agora::linuxsdk::VIDEO_FRAME_RAW_YUV) {
      agora::linuxsdk::VideoYuvFrame *f = pframe->frame.yuv;
      ::fwrite(f->buf_, 1, f->bufSize_, fp);
  }
  else {
      agora::linuxsdk::VideoH264Frame *f = pframe->frame.h264;
      ::fwrite(f->buf_, 1, f->bufSize_, fp);
  }
  ::fclose(fp);

}

void AgoraSdk::addEventHandler(const std::string& eventName, Persistent<Object>& obj, Persistent<Function>& callback)
{
    NodeEventCallback *cb = new NodeEventCallback();;
    cb->js_this.Reset(Isolate::GetCurrent(), obj);
    cb->callback.Reset(Isolate::GetCurrent(), callback);
    m_callbacks.emplace(eventName, cb);
}

void AgoraSdk::emitError(int err, int stat_code) {
    agora::recording::node_async_call::async_call([this, err, stat_code]() {
        MAKE_JS_CALL_2(REC_EVENT_ERROR, int32, err, int32, stat_code);
    });
}

//added 2.3.3
void AgoraSdk::onAudioVolumeIndication_node(const agora::linuxsdk::AudioVolumeInfo* speakers, unsigned int speakerNumber) {
    auto it = m_callbacks.find(RTC_EVENT_AUDIO_VOLUME_INDICATION);
    if (it != m_callbacks.end()) {
        Isolate *isolate = Isolate::GetCurrent();
        HandleScope scope(isolate);
        Local<v8::Array> arrSpeakers = v8::Array::New(isolate, speakerNumber);
        for (unsigned int i = 0; i < speakerNumber; i++) {
            Local<Object> obj = Object::New(isolate);
            [&]{
                return obj->Set(isolate->GetCurrentContext(), napi_create_string_(isolate, "uid"), napi_create_uid_(isolate, speakers[i].uid));
            }();
            [&]{
                return obj->Set(isolate->GetCurrentContext(), napi_create_string_(isolate, "volume"), napi_create_uint32_(isolate, speakers[i].volume));
            }();
            [&]{
                return arrSpeakers->Set(isolate->GetCurrentContext(), i, obj);
            }();
        }

        Local<Value> argv[2]{ arrSpeakers,
                            napi_create_uint32_(isolate, speakerNumber)
                            };
        NodeEventCallback& cb = *it->second;
        [&]{
            return cb.callback.Get(isolate)->Call(isolate->GetCurrentContext(), cb.js_this.Get(isolate), 2, argv);
        }();
    }
}

void AgoraSdk::onAudioVolumeIndication(const agora::linuxsdk::AudioVolumeInfo* speakers, unsigned int speakerNum){
    if (speakers) {
        agora::linuxsdk::AudioVolumeInfo* localSpeakers = new agora::linuxsdk::AudioVolumeInfo[speakerNum];
        for (unsigned int i = 0; i < speakerNum; i++) {
            agora::linuxsdk::AudioVolumeInfo tmp = speakers[i];
            localSpeakers[i].uid = tmp.uid;
            localSpeakers[i].volume = tmp.volume;
        }
        agora::recording::node_async_call::async_call([this, localSpeakers, speakerNum] {
            this->onAudioVolumeIndication_node(localSpeakers, speakerNum);
            delete []localSpeakers;
        });
    }
}

void AgoraSdk::onFirstRemoteVideoDecoded(uid_t uid, int width, int height, int elapsed) {
    agora::recording::node_async_call::async_call([this, uid, width, height, elapsed]() {
        MAKE_JS_CALL_4(REC_EVENT_FIRST_VIDEO_FRAME, uid, uid, int32, width, int32, height, int32, elapsed);
    });
}

void AgoraSdk::onFirstRemoteAudioFrame(uid_t uid, int elapsed) {
    agora::recording::node_async_call::async_call([this, uid, elapsed]() {
        MAKE_JS_CALL_2(REC_EVENT_FIRST_AUDIO_FRAME, uid, uid, int32, elapsed);
    });
}

void AgoraSdk::onReceivingStreamStatusChanged(bool receivingAudio, bool receivingVideo) {
    agora::recording::node_async_call::async_call([this, receivingAudio, receivingVideo]() {
        MAKE_JS_CALL_2(REC_EVENT_STREAM_CHANGED, bool, receivingAudio, bool, receivingVideo);
    });
}

void AgoraSdk::onConnectionLost() {
    agora::recording::node_async_call::async_call([this]() {
        MAKE_JS_CALL_0(REC_EVENT_CONN_LOST);
    });
}

void AgoraSdk::onConnectionInterrupted() {
    agora::recording::node_async_call::async_call([this]() {
        MAKE_JS_CALL_0(REC_EVENT_CONN_INTER);
    });
}

void AgoraSdk::onRemoteVideoStreamStateChanged(uid_t uid, linuxsdk::RemoteStreamState state, linuxsdk::RemoteStreamStateChangedReason reason) {
    agora::recording::node_async_call::async_call([this, uid, state, reason]() {
        MAKE_JS_CALL_3(REC_EVENT_REMOTE_VIDEO_STREAM_STATE_CHANGED, uid, uid, int32, state, int32, reason);
    });
}

void AgoraSdk::onRemoteAudioStreamStateChanged(uid_t uid, agora::linuxsdk::RemoteStreamState state, agora::linuxsdk::RemoteStreamStateChangedReason reason) {
    agora::recording::node_async_call::async_call([this, uid, state, reason]() {
        MAKE_JS_CALL_3(REC_EVENT_REMOTE_AUDIO_STREAM_STATE_CHANGED, uid, uid, int32, state, int32, reason);
    });
}

void AgoraSdk::onRejoinChannelSuccess(const char* channelId, uid_t uid) {
    std::string sChannelId(channelId);
    agora::recording::node_async_call::async_call([this, sChannelId, uid]() {
        MAKE_JS_CALL_2(REC_EVENT_REJOIN_SUCCESS, string, sChannelId.c_str(), uid, uid);
    });
}

void AgoraSdk::onConnectionStateChanged(agora::linuxsdk::ConnectionStateType state, agora::linuxsdk::ConnectionChangedReasonType reason) {
    agora::recording::node_async_call::async_call([this, state, reason]() {
        MAKE_JS_CALL_2(REC_EVENT_CONN_STATE_CHANGED, int32, state, int32, reason);
    });
}
void AgoraSdk::onRemoteVideoStats(agora::linuxsdk::uid_t uid, const agora::linuxsdk::RemoteVideoStats& stats) {
    agora::recording::node_async_call::async_call([this, uid, stats]() {
        do {
            Isolate *isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            Local<Context> context = isolate->GetCurrentContext();
            Local<Object> obj = Object::New(isolate);

            NODE_SET_OBJ_PROP_UINT32(obj, "delay", stats.delay);
            NODE_SET_OBJ_PROP_UINT32(obj, "width", stats.width);
            NODE_SET_OBJ_PROP_UINT32(obj, "height", stats.height);
            NODE_SET_OBJ_PROP_UINT32(obj, "receivedBitrate", stats.receivedBitrate);
            NODE_SET_OBJ_PROP_UINT32(obj, "decoderOutputFrameRate", stats.decoderOutputFrameRate);
            NODE_SET_OBJ_PROP_UINT32(obj, "rxStreamType", stats.rxStreamType);
            Local<Value> arg[2] = { napi_create_uid_(isolate, uid), obj };
            auto it = m_callbacks.find(REC_EVENT_REMOTE_VIDEO_STATS);
            if (it != m_callbacks.end()) {
                [&]{
                    return it->second->callback.Get(isolate)->Call(context, it->second->js_this.Get(isolate), 2, arg); \
                }();
            }
        } while(false);
    });
}

void AgoraSdk::onRemoteAudioStats(agora::linuxsdk::uid_t uid, const agora::linuxsdk::RemoteAudioStats& stats) {
    agora::recording::node_async_call::async_call([this, uid, stats]() {
        do {
            Isolate *isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            Local<Context> context = isolate->GetCurrentContext();
            Local<Object> obj = Object::New(isolate);

            NODE_SET_OBJ_PROP_UINT32(obj, "quality", stats.quality);
            NODE_SET_OBJ_PROP_UINT32(obj, "networkTransportDelay", stats.networkTransportDelay);
            NODE_SET_OBJ_PROP_UINT32(obj, "jitterBufferDelay", stats.jitterBufferDelay);
            NODE_SET_OBJ_PROP_UINT32(obj, "audioLossRate", stats.audioLossRate);
            Local<Value> arg[2] = { napi_create_uid_(isolate, uid), obj };
            auto it = m_callbacks.find(REC_EVENT_REMOTE_AUDIO_STATS);
            if (it != m_callbacks.end()) {
                [&]{
                    return it->second->callback.Get(isolate)->Call(context, it->second->js_this.Get(isolate), 2, arg); \
                }();
            }
        } while(false);
    });
}

void AgoraSdk::onRecordingStats(const agora::linuxsdk::RecordingStats& stats) {
    agora::recording::node_async_call::async_call([this, stats]() {
        do {
            Isolate *isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            Local<Context> context = isolate->GetCurrentContext();
            Local<Object> obj = Object::New(isolate);

            NODE_SET_OBJ_PROP_UINT32(obj, "duration", stats.duration);
            NODE_SET_OBJ_PROP_UINT32(obj, "rxBytes", stats.rxBytes);
            NODE_SET_OBJ_PROP_UINT32(obj, "rxKBitRate", stats.rxKBitRate);
            NODE_SET_OBJ_PROP_UINT32(obj, "rxAudioKBitRate", stats.rxAudioKBitRate);
            NODE_SET_OBJ_PROP_UINT32(obj, "rxVideoKBitRate", stats.rxVideoKBitRate);
            NODE_SET_OBJ_PROP_UINT32(obj, "lastmileDelay", stats.lastmileDelay);
            NODE_SET_OBJ_PROP_UINT32(obj, "userCount", stats.userCount);
            NODE_SET_OBJ_PROP_UINT32(obj, "cpuAppUsage", stats.cpuAppUsage);
            NODE_SET_OBJ_PROP_UINT32(obj, "cpuTotalUsage", stats.cpuTotalUsage);
            Local<Value> arg[1] = { obj };
            auto it = m_callbacks.find(REC_EVENT_RECORDING_STATS);
            if (it != m_callbacks.end()) {
                [&]{
                    return it->second->callback.Get(isolate)->Call(context, it->second->js_this.Get(isolate), 1, arg); \
                }();
            }
        } while(false);
    });
}

void AgoraSdk::onLocalUserRegistered(uid_t uid, const char* userAccount) {
    std::string sUserAccount(userAccount);
    agora::recording::node_async_call::async_call([this, uid, sUserAccount]() {
        MAKE_JS_CALL_2(REC_EVENT_LOCAL_USER_REGISTER, uid, uid, string, sUserAccount.c_str());
    });
}

void AgoraSdk::onUserInfoUpdated(uid_t uid, const agora::linuxsdk::UserInfo& info) {
    agora::recording::node_async_call::async_call([this, uid, info]() {
        do {
            Isolate *isolate = Isolate::GetCurrent();
            HandleScope scope(isolate);
            Local<Context> context = isolate->GetCurrentContext();
            Local<Object> obj = Object::New(isolate);

            NODE_SET_OBJ_PROP_UID(obj, "uid", info.uid);
            NODE_SET_OBJ_PROP_STRING(obj, "userAccount", info.userAccount);

            Local<Value> arg[2] = {
                napi_create_uid_(isolate, uid),
                obj
            };
            auto it = m_callbacks.find(REC_EVENT_USER_INFO_UPDATED);
            if (it != m_callbacks.end()) {
                [&]{
                    return it->second->callback.Get(isolate)->Call(context, it->second->js_this.Get(isolate), 2, arg); \
                }();
            }
        } while(false);
    });
}

}
