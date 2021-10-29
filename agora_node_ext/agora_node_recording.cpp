#include <node.h>
#include <v8.h>
#include "agora_node_recording.h"
#include "IAgoraLinuxSdkCommon.h"
#include "node_log.h"
#include <string>
#include <cstring>
#include <iostream>
using namespace std;
using std::string;
//using namespace v8;

namespace agora {
    namespace recording {
        DEFINE_CLASS(AgoraRecorder);

        void AgoraRecorder::Init(Local<Object> &module)
        {
            Isolate *isolate = module->GetIsolate();
            #if (!defined(_WIN32) && !defined(_WIN64))
            signal(SIGPIPE, SIG_IGN);
            #endif
            BEGIN_PROPERTY_DEFINE(AgoraRecorder, createInstance, 2) //AgoraRecorder count of member var
            PROPERTY_METHOD_DEFINE(joinChannel)
            PROPERTY_METHOD_DEFINE(leaveChannel)
            PROPERTY_METHOD_DEFINE(release)
            PROPERTY_METHOD_DEFINE(setMixLayout)
            PROPERTY_METHOD_DEFINE(on)
            END_PROPERTY_DEFINE()
            [&]{
                return module->Set(isolate->GetCurrentContext(), String::NewFromUtf8(isolate, "AgoraRecorder").FromMaybe(Local<String>()), tpl->GetFunction(isolate->GetCurrentContext()).FromMaybe(Local<Function>()));
            }();
        }

        //The function is used as class constructor in JS layer

        void AgoraRecorder::createInstance(const FunctionCallbackInfo<Value> &args)
        {
            LOG_ENTER;
            Isolate *isolate = args.GetIsolate();
            //Called from new
            if (args.IsConstructCall())
            {
                AgoraRecorder *recording = new AgoraRecorder(isolate);
                recording->Wrap(args.This());
                args.GetReturnValue().Set(args.This());
            }
            else
            {
                Local<Function> cons = Local<Function>::New(isolate, constructor);
                Local<Context> context = isolate->GetCurrentContext();
                Local<Object> instance = cons->NewInstance(context).ToLocalChecked();
                args.GetReturnValue().Set(instance);
            }
            LOG_LEAVE;
        }
        AgoraRecorder::AgoraRecorder(Isolate *isolate)
            : m_isolate(isolate)
        {
            LOG_ENTER;
            m_agorasdk = new AgoraSdk();
            m_resolutionMap = {
                {"3840x2160", "12000"},
                {"2560x1440", "6400"},
                {"1920x1080", "4000"},
                {"1280x720", "2400"},
                {"960x720", "1920"},
                {"848x480", "1200"},
                {"640x480", "1000"},
                {"480x480", "800"},
                {"640x360", "800"},
                {"360x360", "520"},
                {"640x240", "520"},
                {"424x240", "440"},
                {"320x240", "360"},
                {"240x240", "280"},
                {"320x180", "280"},
                {"240x180", "240"},
                {"180x180", "200"},
                {"160x120", "120"},
                {"120x120", "100"}
            };
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(AgoraRecorder, joinChannel)
        {
            LOG_ENTER;
            cout << "joinChannel..." << endl;

            do{
                agora::recording::RecordingConfig config;

                NodeString appid, token, channel, uid, appliteDir, cfgPath;
                AgoraRecorder *pRecording = NULL;
                napi_get_native_this(args, pRecording);
                CHECK_NATIVE_THIS(pRecording);

                napi_status status;

                status = napi_get_value_nodestring_(args[0], appid);
                CHECK_NAPI_STATUS(status);
                string str_appid = (string)appid;

                status = napi_get_value_nodestring_(args[1], token);
                CHECK_NAPI_STATUS(status);
                string str_token = (string)token;

                status = napi_get_value_nodestring_(args[2], channel);
                CHECK_NAPI_STATUS(status);
                string str_channel = (string)channel;

                status = napi_get_value_nodestring_(args[3], uid);
                CHECK_NAPI_STATUS(status);
                string str_uid = (string)uid;

                status = napi_get_value_nodestring_(args[4], appliteDir);
                cout << "appliteDir " << appliteDir << endl;
                CHECK_NAPI_STATUS(status);
                string str_appliteDir = (string)appliteDir;

                status = napi_get_value_nodestring_(args[5], cfgPath);
                CHECK_NAPI_STATUS(status);
                string str_cfgPath = (string)cfgPath;

                config.appliteDir = const_cast<char*>(str_appliteDir.c_str());
                config.cfgFilePath = const_cast<char*>(str_cfgPath.c_str());
                config.isMixingEnabled = true;
                config.mixedVideoAudio = agora::linuxsdk::MIXED_AV_CODEC_V2;
                config.idleLimitSec = 10;
                // config.decodeVideo = agora::linuxsdk::VIDEO_FORMAT_MIX_JPG_FILE_TYPE;
                config.channelProfile = agora::linuxsdk::CHANNEL_PROFILE_LIVE_BROADCASTING;
                // config.captureInterval = 1;
                config.triggerMode = agora::linuxsdk::AUTOMATICALLY_MODE;
                config.mixResolution = "640,480,15,500";

                agora::linuxsdk::VideoMixingLayout layout = pRecording->m_agorasdk->getMixLayout();
                std::stringstream out;
                out << layout.canvasWidth << "x" << layout.canvasHeight;
                string resolutionKey = out.str();
                string resolutionValue;
                map<string, string>::iterator it = pRecording->m_resolutionMap.find(resolutionKey);
                if (it == pRecording->m_resolutionMap.end()) {
                    // no values found, throw error
                    cout << "unsupported resolution" << endl;
                    int err = 2000, stat_code = 4;
                    pRecording->m_agorasdk->emitError(err, stat_code);
                    break;
                } else {
                    resolutionValue = pRecording->m_resolutionMap[it->first];
                }
                out.str("");
                out << layout.canvasWidth << "," << layout.canvasHeight << ",15," << resolutionValue;
                string mixResolution = out.str();
                config.mixResolution = &mixResolution[0u];
                config.audioIndicationInterval = 0;
                //todo
                // pRecording->m_agorasdk->updateMixModeSetting(0, 0, true);
                int result = pRecording->m_agorasdk->createChannel(str_appid, str_token, str_channel, str_uid, config);
                cout << "pRecording->m_agorasdk->createChannel return result:" << result << endl;
                napi_set_int_result(args, result);
            } while(false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(AgoraRecorder, leaveChannel)
        {
            LOG_ENTER;
            cout << "leaveChannel..." << endl;
            do
            {
                AgoraRecorder *pRecording = NULL;
                napi_get_native_this(args, pRecording);
                CHECK_NATIVE_THIS(pRecording);
                /*std::shared_ptr<agora::rtc::NodeVideoStreamChannel> chan;
                for (auto& channel : pEngine->m_channels) {
                    if (channel->getUid() == 0) {
                        chan = channel;
                        break;
                    }
                }
                pEngine->m_channels.clear();
                pEngine->m_channels.push_back(chan);*/
                int result = pRecording->m_agorasdk->leaveChannel();
                napi_set_int_result(args, result);
            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(AgoraRecorder, setMixLayout)
        {
            LOG_ENTER;
            cout << "setting mix layout..." << endl;
            do
            {
                AgoraRecorder *pRecording = NULL;
                napi_get_native_this(args, pRecording);
                CHECK_NATIVE_THIS(pRecording);

                int canvasWidth, canvasHeight;
                NodeString backgroundColor;
                Local<Array> regions;
                Local<Object> layoutData = args[0]->ToObject(args.GetIsolate()->GetCurrentContext()).FromMaybe(Local<Object>());
                napi_status status = napi_get_value_int32_object_(args.GetIsolate(), layoutData, "canvasWidth", canvasWidth);
                CHECK_NAPI_STATUS(status);

                status = napi_get_value_int32_object_(args.GetIsolate(), layoutData, "canvasHeight", canvasHeight);
                CHECK_NAPI_STATUS(status);

                status = napi_get_value_string_object_(args.GetIsolate(), layoutData, "backgroundColor", backgroundColor);
                CHECK_NAPI_STATUS(status);

                status = napi_get_value_array_object_(args.GetIsolate(), layoutData, "regions", regions);
                CHECK_NAPI_STATUS(status);

                agora::linuxsdk::VideoMixingLayout::Region * regionList = new agora::linuxsdk::VideoMixingLayout::Region[regions->Length()];
                for (size_t i = 0; i < regions->Length(); i++) {
                    int zOrder;
                    double alpha, width, height, x, y;
                    uid_t uid;
                    Local<Value> regionValue = regions->Get(args.GetIsolate()->GetCurrentContext(), i).FromMaybe(Local<Value>());
                    if(!regionValue->IsObject()) {
                        cout << "invalid region found: " << i << endl;
                        break;
                    }
                    Local<Object> region = Local<Object>::Cast(regionValue);

                    napi_status regionStatus = napi_get_value_double_object_(args.GetIsolate(), region, "x", x);
                    CHECK_NAPI_STATUS(regionStatus);

                    regionStatus = napi_get_value_double_object_(args.GetIsolate(), region, "y", y);
                    CHECK_NAPI_STATUS(regionStatus);

                    regionStatus = napi_get_value_int32_object_(args.GetIsolate(), region, "zOrder", zOrder);
                    CHECK_NAPI_STATUS(regionStatus);

                    regionStatus = napi_get_value_double_object_(args.GetIsolate(), region, "alpha", alpha);
                    CHECK_NAPI_STATUS(regionStatus);

                    regionStatus = napi_get_value_double_object_(args.GetIsolate(), region, "width", width);
                    CHECK_NAPI_STATUS(regionStatus);

                    regionStatus = napi_get_value_double_object_(args.GetIsolate(), region, "height", height);
                    CHECK_NAPI_STATUS(regionStatus);

                    regionStatus = napi_get_value_uint32_object_(args.GetIsolate(), region, "uid", uid);
                    CHECK_NAPI_STATUS(regionStatus);

                    regionList[i].uid = uid;
                    regionList[i].x = x / canvasWidth;
                    regionList[i].y = y / canvasHeight;
                    regionList[i].width = width / canvasWidth;
                    regionList[i].height = height / canvasHeight;
                    regionList[i].alpha = alpha;
                    // regionList[i].zOrder = zOrder;
                    regionList[i].renderMode = 0;
                }

                agora::linuxsdk::VideoMixingLayout layout;
                string backgroundColor_str = (string)backgroundColor;
                size_t str_len = backgroundColor_str.length();
                char* backgroundColor_copy = new char[str_len];
                strcpy(backgroundColor_copy, backgroundColor_str.c_str());
                layout.canvasWidth = canvasWidth;
                layout.canvasHeight = canvasHeight;
                layout.backgroundColor = backgroundColor_copy;
                layout.regionCount = regions->Length();
                layout.regions = regionList;

                pRecording->m_agorasdk->updateMixLayout(layout);

            } while (false);
            LOG_LEAVE;
        }

        NAPI_API_DEFINE(AgoraRecorder, on)

        {

            //LOG_ENTER;

            do
            {
                AgoraRecorder *pEngine = nullptr;

                napi_status status = napi_ok;

                napi_get_native_this(args, pEngine);

                CHECK_NATIVE_THIS(pEngine);

                NodeString eventName;

                status = napi_get_value_nodestring_(args[0], eventName);

                CHECK_NAPI_STATUS(status);

                if (!args[1]->IsFunction())
                {

                    LOG_ERROR("Function expected");

                    break;
                }

                Local<Function> callback = args[1].As<Function>();

                if (callback.IsEmpty())
                {

                    LOG_ERROR("Function expected.");

                    break;
                }

                Persistent<Function> persist;

                persist.Reset(args.GetIsolate(), callback);

                Local<Object> obj = args.This();

                Persistent<Object> persistObj;

                persistObj.Reset(args.GetIsolate(), obj);

                pEngine->m_agorasdk->addEventHandler((char *)eventName, persistObj, persist);

            } while (false);

            //LOG_LEAVE;
        }

        NAPI_API_DEFINE(AgoraRecorder, release)
        {
            do
            {
                AgoraRecorder *pEngine = nullptr;
                napi_status status = napi_ok;
                CHECK_NAPI_STATUS(status);
                napi_get_native_this(args, pEngine);
                CHECK_NATIVE_THIS(pEngine);

                pEngine->m_agorasdk->release();
            } while (false);
        }

        AgoraRecorder::~AgoraRecorder() {
            if(m_agorasdk) {
                m_agorasdk->release();
                m_agorasdk = NULL;
            }
        }
    }
}
