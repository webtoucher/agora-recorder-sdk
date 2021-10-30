export declare type AgoraDecryptionMode =
    | 'aes-128-xts'
    | 'aes-256-xts'
    | 'aes-128-ecb'
    | 'sm4-128-ecb'
    | 'aes-128-gcm'
    | 'aes-256-gcm'
    | 'aes-128-gcm2'
    | 'aes-256-gcm2'
    | 'none'

export declare const enum AgoraRecorderEvent {
    REC_EVENT_JOIN_CHANNEL = 'joinchannel',
    REC_EVENT_LEAVE_CHANNEL = 'leavechannel',
    REC_EVENT_ERROR = 'error',
    REC_EVENT_USER_JOIN = 'userjoin',
    REC_EVENT_USER_LEAVE = 'userleave',
    REC_EVENT_ACTIVE_SPEAKER = 'activespeaker',
    REC_EVENT_CONN_LOST = 'connectionlost',
    REC_EVENT_CONN_INTER = 'connectioninterrupt',
    REC_EVENT_STREAM_CHANGED = 'receivingstreamstatuschanged',
    REC_EVENT_FIRST_VIDEO_FRAME = 'firstremotevideodecoded',
    REC_EVENT_FIRST_AUDIO_FRAME = 'firstremoteaudioframe',
    RTC_EVENT_AUDIO_VOLUME_INDICATION = 'audiovolumeindication',
    REC_EVENT_REMOTE_VIDEO_STREAM_STATE_CHANGED = 'remoteVideoStreamStateChanged',
    REC_EVENT_REMOTE_AUDIO_STREAM_STATE_CHANGED = 'remoteAudioStreamStateChanged',
    REC_EVENT_REJOIN_SUCCESS = 'rejoinChannelSuccess',
    REC_EVENT_CONN_STATE_CHANGED = 'connectionStateChanged',
    REC_EVENT_REMOTE_VIDEO_STATS = 'remoteVideoStats',
    REC_EVENT_REMOTE_AUDIO_STATS = 'remoteAudioStats',
    REC_EVENT_RECORDING_STATS = 'recordingStats',
    REC_EVENT_LOCAL_USER_REGISTER = 'localUserRegistered',
    REC_EVENT_USER_INFO_UPDATED = 'userInfoUpdated',
}

export declare const enum AgoraMixedAvCodecType {
    /**
     * 0: (Default) Mixes the audio and video respectively in the composite recording mode.
     */
    MIXED_AV_DEFAULT = 0,

    /**
     * 1: Mixes the audio and video in real time into an MP4 file in the composite recording mode. Supports limited players.
     */
    MIXED_AV_CODEC_V1 = 1,

    /**
     * 2: Mixes the audio and video in real time into an MP4 file in the composite recording mode. Supports more players.
     */
    MIXED_AV_CODEC_V2 = 2,

    /**
     * 4: Outputs an TS file only in the individual recording mode.
     */
    AV_CODEC_INDIVIDUAL_TS_ONLY = 4,

    /**
     * 5: Outputs an TS file and an MP4 file respectively in the individual recording mode.
     *
     * @note The Agora SDK deletes the TS file after successful recording.
     */
    AV_CODEC_INDIVIDUAL_TS_AND_MP4 = 5,

    /**
     * 6: Outputs an TS file only in the composite recording mode.
     */
    AV_CODEC_MIXED_TS_ONLY = 6,

    /**
     * 7: Outputs an TS file and an MP4 file respectively in the composite recording mode.
     *
     * @note The Agora SDK deletes the TS file after successful recording.
     */

    AV_CODEC_MIXED_TS_AND_MP4 = 7,
}

export declare const enum AgoraVideoFormatType {
    /**
     * 0: Default video format. Depending on your codec, the recording service generates either MP4 or WebM files.
     */
    VIDEO_FORMAT_DEFAULT_TYPE = 0,

    /**
     * @deprecated
     * 1: Video frame in H.264 format.
     */
    VIDEO_FORMAT_H264_FRAME_TYPE = 1,

    /**
     * 1: Video frame in H.264 or H.265 format.
     * You receive video frames from the AgoraRecordingEngineEventHandler.videoFrameReceived callback.
     */
    VIDEO_FORMAT_ENCODED_FRAME_TYPE = 1,

    /**
     * 2: Video frame in YUV format.
     * You receive video frames from the AgoraRecordingEngineEventHandler.videoFrameReceived callback.
     */
    VIDEO_FORMAT_YUV_FRAME_TYPE = 2,

    /**
     * 3: Video frame in JPG format.
     * You receive video frames from the AgoraRecordingEngineEventHandler.videoFrameReceived callback.
     */
    VIDEO_FORMAT_JPG_FRAME_TYPE = 3,

    /**
     * 4: JPG file format.
     */
    VIDEO_FORMAT_JPG_FILE_TYPE = 4,

    /**
     * 5: JPG file format and MP4 or WebM video.
     * - Individual mode (AgoraRecordingConfig.isMixingEnabled is set as `false`): MP4 or WebM video and JPG files.
     * - Composite mode (AgoraRecordingConfig.isMixingEnabled is set as `true`): MP4 or WebM video file for mixed
     *   streams and JPG files for individual streams.
     */
    VIDEO_FORMAT_JPG_VIDEO_FILE_TYPE = 5,
}

export declare const enum AgoraAudioFormatType {
    /**
     * 0: Default audio file format (ACC).
     */
    AUDIO_FORMAT_DEFAULT_TYPE = 0,

    /**
     * 1: Audio frame in AAC format.
     * You receive audio frames from the AgoraRecordingEngineEventHandler.audioFrameReceived callback.
     */
    AUDIO_FORMAT_AAC_FRAME_TYPE = 1,

    /**
     * 2: Audio frame in PCM format.
     * You receive audio frames from the AgoraRecordingEngineEventHandler.audioFrameReceived callback.
     */
    AUDIO_FORMAT_PCM_FRAME_TYPE = 2,

    /**
     * 3: Audio-mixing frame in PCM format.
     * You receive audio frames from the AgoraRecordingEngineEventHandler.audioFrameReceived callback.
     */
    AUDIO_FORMAT_MIXED_PCM_FRAME_TYPE = 3,
}

export declare const enum AgoraChannelProfileType {
    /**
     * 0: (Default) Communication mode.
     * This is used in one-on-one or group calls, where all users in the channel can talk freely.
     */
    CHANNEL_PROFILE_COMMUNICATION = 0,

    /**
     * 1: Live broadcast mode.
     * The host sends and receives voice/video, while the audience only receives voice/video.
     * Host and audience roles can be set by calling setClientRole.
     */
    CHANNEL_PROFILE_LIVE_BROADCASTING = 1,
}

export declare const enum AgoraRemoteVideoStreamType {
    /**
     * 0: (Default) High stream.
     */
    REMOTE_VIDEO_STREAM_HIGH = 0,

    /**
     * 1: Low stream.
     */
    REMOTE_VIDEO_STREAM_LOW = 1,
}

export declare const enum AgoraTriggerModeType {
    /**
     * 0: (Default) Automatically.
     */
    AUTOMATICALLY_MODE = 0,

    /**
     * 1: Manually.
     * To start and stop recording, call AgoraRecordingEngine.startService and AgoraRecordingEngine.stopService respectively.
     */
    MANUALLY_MODE = 1,
}

export declare const enum AgoraLanguageType {
    /**
     * 0: (Default) C++.
     */
    CPP_LANG = 0,

    /**
     * 1: Java.
     */
    JAVA_LANG = 1,
}

export declare const enum AgoraAudioProfileType {
    /**
     * 0: (Default) Sampling rate of 48 kHz, communication encoding, mono, and a bitrate of up to 48 Kbps.
     */
    AUDIO_PROFILE_DEFAULT = 0,

    /**
     * 1: Sampling rate of 48 kHz, music encoding, mono, and a bitrate of up to 128 Kbps.
     */
    AUDIO_PROFILE_HIGH_QUALITY = 1,

    /**
     * 2: Sampling rate of 48 kHz, music encoding, stereo, and a bitrate of up to 192 Kbps.
     */
    AUDIO_PROFILE_HIGH_QUALITY_STEREO = 2,
}

export declare const enum AgoraRenderMode {
    /**
     * 0: Cropped mode. Uniformly scale the video until it fills the visible boundaries (cropped).
     * One dimension of the video may have clipped contents.
     */
    RENDER_MODE_HIDDEN = 0,

    /**
     * 1: Fit mode. Uniformly scale the video until one of its dimension fits the boundary (zoomed to fit).
     * Areas that are not filled due to the disparity in the aspect ratio will be filled with black.
     */
    RENDER_MODE_FIT = 1,
}

export declare const enum AgoraWatermarkType {
    /**
     * 0: The text watermark.
     */
    WATERMARK_TYPE_LITERA,

    /**
     * 1: The timestamp watermark.
     */
    WATERMARK_TYPE_TIMESTAMP,

    /**
     * 2: The image watermark.
     */
    WATERMARK_TYPE_IMAGE,
}

export declare interface AgoraRegion {
    /**
     * The UID of the user (communication mode)/host (live broadcast mode) displaying the video in the region.
     *
     * @default 0
     */
    uid?: number

    /**
     * The relative horizontal position of the top-left corner of the region. The value is between 0.0 and 1.0.
     *
     * @default 0.0
     */
    x?: number

    /**
     * The relative vertical position of the top-left corner of the region. The value is between 0.0 and 1.0.
     *
     * @default 0.0
     */
    y?: number

    /**
     * The relative width of the region. The value is between 0.0 and 1.0.
     *
     * @default 0.0
     */
    width?: number

    /**
     * The relative height of the region. The value is between 0.0 and 1.0.
     *
     * @default 0.0
     */
    height?: number

    /**
     * The transparency of the image. The value is between 0.0 (transparent) and 1.0 (opaque).
     *
     * @default 1.0
     */
    alpha?: number

    /**
     * Render mode.
     *
     * @default AgoraRenderMode.RENDER_MODE_HIDDEN
     */
    renderMode?: AgoraRenderMode
}

export declare interface AgoraRecordingConfig {
    /**
     * Sets whether or not to record audio only.
     * - true: Enables audio recording and disables video recording.
     * - false: (Default) Enables both audio and video recording.
     *
     * Used together with `isVideoOnly` parameter:
     * - If `isAudioOnly` is true and `isVideoOnly` is false, only records audio;
     * - If `isAudioOnly` is false and `isVideoOnly` is true, only records video;
     * - If `isAudioOnly` is false and `isVideoOnly` is false, records both audio and video;
     * - `isAudioOnly` and `isVideoOnly` can not be set as true at the same time.
     *
     * @default false
     */
    isAudioOnly: boolean

    /**
     * Sets whether or not to record video only.
     * - true: Enables video recording and disables audio recording.
     * - false: (Default) Enables both audio and video recording.
     *
     * Used together with `isAudioOnly` parameter:
     * - If `isAudioOnly` is true and `isVideoOnly` is false, only records audio;
     * - If `isAudioOnly` is false and `isVideoOnly` is true, only records video;
     * - If `isAudioOnly` is false and `isVideoOnly` is false, records both audio and video;
     * - `isAudioOnly` and `isVideoOnly` cannot be set as true at the same time.
     *
     * @default false
     */
    isVideoOnly: boolean

    /**
     * Sets whether or not to enable composite recording mode.
     *
     * - true: Enables composite recording mode, which means the audio of all uids is mixed in an audio file
     *   and the video of all uids is mixed in a video file. You can set the audio profile of the recording file
     *   by the `audioProfile` parameter and set the video profile by the `mixResolution` parameter.
     * - false: (Default) Enables individual recording mode, which means one audio or video file for each uid.
     *   The sampling rate of the recording file is 48 kHz, and the bitrate and audio channel number of the recording
     *   file are the same as those of the original audio stream. The video profile of the recording file is the same
     *   as that of the original video profile.
     *
     * @default false
     */
    isMixingEnabled: boolean

    /**
     * Sets whether to record the streams of all users or specified users.
     * - true: (Default) Record the streams of all users.
     * - false: Record the streams of specified users.
     *
     * @note If you set `autoSubscribe` parameter as false, you should set `subscribeVideoUids` or `subscribeAudioUids`
     * to specify users whose video or audio you want to record.
     *
     * @default true
     */
    autoSubscribe: boolean

    /**
     * Sets whether or not to enable the cloud proxy:
     * - true: Enables the cloud proxy.
     * - false: (Default) Disables the cloud proxy.
     *
     * For more information, see [Use Cloud Proxy](https://docs.agora.io/en/Recording/cloudproxy_recording?platform=Linux)
     *
     * @default false
     */
    enableCloudProxy: boolean

    /**
     * If you set `isMixingEnabled` parameter as true and enable composite recording mode, `mixedVideoAudio` allows you
     * to mix the audio and video in an MP4 file in real time.
     *
     * @default AgoraMixedAvCodecType.MIXED_AV_DEFAULT
     */
    mixedVideoAudio: AgoraMixedAvCodecType

    /**
     * If you set `isMixingEnabled` parameter as true and enable composite recording mode, `mixResolution` allows you
     * to set the video profile, including the width, height, frame rate, and bitrate. The default setting is 360 x 640,
     * 15 fps, 500 Kbps.
     *
     * @note Agora supports the following frame rates: 1 fps, 7 fps, 10 fps, 15 fps, 24 fps, 30 fps, and 60 fps.
     * The default value is 15 fps. If you set other frame rates, the SDK uses the default value.
     *
     * See the [Video Profile Table](https://docs.agora.io/en/cloud-recording/recording_video_profile?platform=RESTful).
     *
     * @default null
     */
    mixResolution?: string

    /**
     * When the whole channel is encrypted, the recording SDK uses `decryptionMode` parameter to enable the built-in
     * decryption function.
     *
     * @note The decryption method of the recording server must be the same as that of the Native/Web SDK.
     *
     * @default null
     */
    decryptionMode?: AgoraDecryptionMode

    /**
     * Sets the decryption password when the `decryptionMode` parameter is enabled.
     *
     * @default null
     */
    secret?: string

    /**
     * Sets the path of AgoraCoreService. Default path is Agora_Recording_SDK_for_Linux_FULL/bin/.
     *
     * @default null
     */
    appliteDir?: string

    /**
     * Sets the path of the recorded files.
     * After setting `recordFileRootDir`, the subdirectory will be automatically generated according to the date of the recording.
     *
     * @default null
     */
    recordFileRootDir?: string

    /**
     * Sets the path of the configuration file. For example, `--cfgFilePath /home/guest/recording_dir/cfg.json`.
     *
     * The content in the configuration file must be in JSON format. You can configure the following parameters:
     * - `Recording_Dir`: The absolute directory of the output in the string format. The recording service
     *    does not automatically create a subdirectory.
     *    For example, `{"Recording_Dir": "/home/guest/recording_dir/"}`, which means the recording service stores
     *    the recording files in the `/home/guest/recording_dir/` directory.
     * - `Chunk_Time_Span`: Time interval (s) between two successive recorded files in the string format.
     *    Only in the `LiveBroadcasting` channel profile and in an individual recording session can you set
     *    `Chunk_Time_Span`, which must be >= 10.
     *    For example: `{"Chunk_Time_Span": "15"}`, which means the recording service creates a file every 15 seconds.
     *
     * @note
     * - You can set the `Chunk_Time_Span` parameter only when you use the recording service in individual recording
     *   mode in a live interactive streaming channel.
     * - To use the `Chunk_Time_Span` parameter, you must set `enableIntraRequest` as `true` to enable the keyframe request.
     *   Whether the sender sends the keyframe depends on the Agora RTC SDK version used by the sender.
     * - Slicing occurs only when an I frame appears, therefore the actual slicing time interval may be slightly
     *   different from the set time interval.
     *
     * @default null
     */
    cfgFilePath?: string

    /**
     * Sets the video decoding format.
     *
     * @note When AgoraVideoFormatType = 1, 2, 3 or 4, AgoraRecordingConfig.isMixingEnabled cannot be set as true.
     *
     * @default AgoraVideoFormatType.VIDEO_FORMAT_DEFAULT_TYPE
     */
    decodeVideo: AgoraVideoFormatType

    /**
     * Sets the audio decoding format.
     *
     * @note When AgoraAudioFormatType = 1 or 2, AgoraRecordingConfig.isMixingEnabled cannot be set as true.
     *
     * @default AgoraAudioFormatType.AUDIO_FORMAT_DEFAULT_TYPE
     */
    decodeAudio: AgoraAudioFormatType

    /**
     * Sets the lowest UDP port.
     * Ensure that the value of highUdpPort - lowUdpPort is ≥ 6.
     *
     * @default 0
     */
    lowUdpPort: number

    /**
     * Sets the highest UDP port.
     * Ensure that the value of highUdpPort - lowUdpPort is ≥ 6.
     *
     * @default 0
     */
    highUdpPort: number

    /**
     * Sets a time period. The value must be ≥ 3 seconds.
     * When the Agora Recording SDK is recording, if there is no user in the channel after a time period of
     * `idleLimitSec`, it automatically stops recording and leaves the channel.
     *
     * @note
     * - We charge you this time period.
     * - In a communication channel, the recording service does not recognize a channel as an idle channel, so long
     *   as the channel has users, regardless of whether they send stream or not.
     * - If a live streaming channel has an audience without a host for a set time (`idleLimitSec`), the recording
     *   service automatically stops and leaves the channel.
     *
     * @default 300
     */
    idleLimitSec: number

    /**
     * Sets the interval of the screen capture.
     * The interval must be longer than 1 second.
     *
     * @note `captureInterval` parameter is only valid when `decodeVideo` parameter is set as 3, 4, or 5.
     *
     * @default 5
     */
    captureInterval: number

    /**
     * Sets whether or not to detect the users who speak.
     *
     * - ≤ 0: (Default) Do not detect the users who speak.
     * - > 0: Sets the interval (ms) of detecting the users who speak. Agora recommends setting the interval
     *   to be longer than 200 ms. When the SDK detects the users who speak, the SDK returns the UID of the user who
     *   speaks loudest in the AgoraRecordingEngineEventHandler.onActiveSpeaker callback and returns the UIDs of all
     *   users who speak and their voice volumes in the AgoraRecordingEngineEventHandler.onAudioVolumeIndication callback.
     *
     * @default 0
     */
    audioIndicationInterval: number

    /**
     * Sets the channel mode. For more information, see agora::linuxsdk::CHANNEL_PROFILE_TYPE "CHANNEL_PROFILE_TYPE".
     *
     * @default AgoraChannelProfileType.CHANNEL_PROFILE_COMMUNICATION
     */
    channelProfile: AgoraChannelProfileType

    /**
     * Takes effect only when the Agora Native SDK enables the dual-stream mode (high stream by default).
     *
     * @default AgoraRemoteVideoStreamType.REMOTE_VIDEO_STREAM_HIGH
     */
    streamType: AgoraRemoteVideoStreamType

    /**
     * Sets whether to record automatically or manually.
     *
     * @default AgoraTriggerModeType.AUTOMATICALLY_MODE
     */
    triggerMode: AgoraTriggerModeType

    /**
     * Sets the programming language.
     *
     * @default AgoraLanguageType.CPP_LANG
     */
    lang: AgoraLanguageType

    /**
     * Sets the type of the proxy server:
     * - 0: Deploy the proxy server of the SOCKS5 type.
     * - 1: (Default) Use the cloud proxy service, and configure the domain (recommended).
     * - 2: Use the cloud proxy service, and configure the IP list (recommended when you can not resolve a domain to an IP address).
     *
     * After setting the `proxyType` parameter, you need to set the `proxyServer` parameter.
     * See [Use Cloud Proxy](https://docs.agora.io/en/Recording/cloudproxy_recording?platform=Linux) for details.
     *
     * @default 1
     */
    proxyType: 0 | 1 | 2

    /**
     * Sets the IP address (domain) and port of the proxy server for a recording within the intranet according
     * to the type of the proxy server that you choose with the `proxyType` parameter.
     * - If proxyType is 0, set it as `"<ip>:<port>"`.
     * - If proxyType is 1, set it as `"<domain>:<port>"`.
     * - If proxyType is 2, set it as `"<ip1>,<ip2>,...,<ipx>:<port>"`.
     *
     * See [Use Cloud Proxy](https://docs.agora.io/en/Recording/cloudproxy_recording?platform=Linux) for details.
     *
     * @default null
     */
    proxyServer?: string

    /**
     * If you set `isMixingEnabled` parameter as true and enable composite recording mode, `audioProfile` parameter
     * allows you to set the audio profile of the recording file.
     *
     * @default AgoraAudioProfileType.AUDIO_PROFILE_DEFAULT
     */
    audioProfile: AgoraAudioProfileType

    /**
     * Sets the directory of the default background image of the canvas in composite recording mode.
     * If `defaultVideoBg` parameter is not set, the canvas displays the background color.
     *
     * @note Only supports local images in JPEG format.
     *
     * @default null
     */
    defaultVideoBg?: string

    /**
     * Sets the directory of the default background image of users in composite recording mode.
     * The background image is displayed when a user is online and does not send any video stream.
     * If `defaultUserBg` parameter is not set, the user region displays the background color.
     *
     * @note
     * - Only supports local images in JPEG format.
     * - The background image is not displayed for users using the Agora Web SDK.
     *
     * @default null
     */
    defaultUserBg?: string

    /**
     * An array of UIDs whose video streams you want to record.
     * If you set `autoSubscribe` parameter as false, `subscribeVideoUids` parameter enables you to record the video
     * streams of specified users.
     *
     * @default null
     */
    subscribeVideoUids?: [string]

    /**
     * An array of UIDs whose audio streams you want to record.
     * If you set `autoSubscribe` parameter as false, `subscribeAudioUids` parameter enables you to record the audio
     * streams of specified users.
     *
     * @default null
     */
    subscribeAudioUids?: [string]

    /**
     * Sets whether to enable the keyframe request. The default value is `true`, which can improve the audio and video
     * quality under poor network conditions. To play the video file recorded in individual recording mode from
     * a specified position, you must set `enableIntraRequest` as false.
     * - true: (Default) Leave it to the sender to decide whether to enable the keyframe request. After the keyframe
     *   request is enabled, you cannot play a video file, which is recorded in individual recording mode, from
     *   a specified position.
     * - false: Disable the keyframe request. All senders in the channel send the keyframe at an interval of 2 seconds.
     *   After the keyframe request is disabled, you can play a video file, which is recorded in individual recording
     *   mode, from a specified position.
     *
     * @note If the sender uses Agora RTC SDK v2.9.2 or earlier, this parameter is valid only in the live-broadcast scenario.
     *
     * @default true
     */
    enableIntraRequest: boolean

    /**
     * Sets whether to enable recording video stream in H.265 format:
     * - true: Enable recording video stream in H.265 format.
     * - false: (Default) Disable recording stream in H.265 format. Other remote users in the channel can no longer
     *   send video stream in H.265 format.
     *
     * @default false
     */
    enableH265Support: boolean
}

export declare interface AgoraRecordingEngineProperties {
    /**
     * The relative path of the recorded files and recording log.
     *
     * @default null
     */
    storageDir?: string
}

/**
 * The configuration of the text watermark.
 */
export declare interface AgoraLiteraWatermarkConfig {
    /**
     * The watermark text in the string format.
     *
     * @note
     * - The supported characters depend on the font. The default font is NotoSansMonoCJKsc-Regular.
     *   See the [font introduction](https://www.google.com/get/noto/help/cjk/). To change the font, you can also
     *   set the `font_file_path` parameter to specify the path of the font file.
     * - Supports word wrap. The text will wrap to next line when it exceeds the watermark rectangle.
     * - Supports line breaks.
     * - There is no limit on the string length. The display of the text on the watermark rectangle is influenced by
     *   the font size and the size of the watermark rectangle. The part that exceeds the rectangle will not be displayed.
     */
    wm_litera: string

    /**
     * The path of the font file. If not specified, use the default font `NotoSansMonoCJKsc-Regular`.
     *
     * @note Supports font files in the formats such as ttf and otf.
     *
     * @default null
     */
    font_file_path?: string

    /**
     * The font size. The default value is 10, which equals to 10 x 15 points at 144 dpi.
     *
     * @default 10
     */
    font_size: number

    /**
     * The horizontal shift (pixel) of the rectangle containing the watermark from the left border of the canvas.
     *
     * @default 0
     */
    offset_x: number

    /**
     * The vertical shift (pixel) of the rectangle containing the watermark from the top border of the canvas.
     *
     * @default 0
     */
    offset_y: number

    /**
     * The width (pixel) of the rectangle containing the watermark.
     *
     * @default 0
     */
    wm_width: number

    /**
     * The height (pixel) of the rectangle containing the watermark.
     *
     * @default 0
     */
    wm_height: number
}

/**
 * The configuration of the timestamp watermark.
 * The dynamic timestamp shows the current time of the recording server, such as "2019:06:18 14:30:35".
 */
export declare interface AgoraTimestampWatermarkConfig {
    /**
     * The font size. The default value is 10, which equals to 10 x 15 points at 144 dpi.
     *
     * @default 10
     */
    font_size: number

    /**
     * The horizontal shift (pixel) of the rectangle containing the watermark from the left border of the canvas.
     *
     * @default 0
     */
    offset_x: number

    /**
     * The vertical shift (pixel) of the rectangle containing the watermark from the top border of the canvas.
     *
     * @default 0
     */
    offset_y: number

    /**
     * The width (pixel) of the rectangle containing the watermark.
     *
     * @default 0
     */
    wm_width: number

    /**
     * The height (pixel) of the rectangle containing the watermark.
     *
     * @default 0
     */
    wm_height: number
}

/**
 * The configuration of the image watermark.
 */
export declare interface AgoraImageWatermarkConfig {
    /**
     * The path of the image file.
     *
     * @note
     * - Only supports local PNG images.
     * - The resolution of the image should not exceed 480p.
     * - If the image is smaller than the watermark rectangle, the SDK centers the image and does not stretch the image;
     *   if the image is larger than the watermark rectangle, the SDK scales down the image and then centers the image
     *   in the watermark rectangle.
     */
    image_path: string

    /**
     * The horizontal shift (pixel) of the rectangle containing the watermark from the left border of the canvas.
     *
     * @default 0
     */
    offset_x: number

    /**
     * The vertical shift (pixel) of the rectangle containing the watermark from the top border of the canvas.
     *
     * @default 0
     */
    offset_y: number

    /**
     * The width (pixel) of the rectangle containing the watermark.
     *
     * @default 0
     */
    wm_width: number

    /**
     * The height (pixel) of the rectangle containing the watermark.
     *
     * @default 0
     */
    wm_height: number
}

export declare interface AgoraWatermarkConfigUnion {
    /**
     * The configuration of the text watermark.
     */
    litera: AgoraLiteraWatermarkConfig

    /**
     * The configuration of the timestamp watermark.
     */
    timestamp: AgoraTimestampWatermarkConfig

    /**
     * The configuration of the image watermark.
     */
    image: AgoraImageWatermarkConfig
}

export declare interface AgoraWatermarkConfig {
    /**
     * Choose one of the three watermark types.
     */
    type: AgoraWatermarkType

    /**
     * Define the parameters of the watermark type that you choose in the `type` parameter.
     */
    config: AgoraWatermarkConfigUnion
}

export declare interface AgoraVideoMixingLayout {
    /**
     * The width of the canvas (the display window or screen).
     *
     * @default 0
     */
    canvasWidth?: number

    /**
     * The height of the canvas (the display window or screen).
     *
     * @default 0
     */
    canvasHeight?: number

    /**
     * The background color of the canvas (the display window or screen) in RGB hex value. e.g. "#C0C0C0" in RGB
     *
     * @note If you set the `AgoraRecordingConfig.defaultVideoBg` parameter when calling
     * the `AgoraRecordingEngine.joinChannel` method, the `backgroundColor` parameter does not take effect.
     *
     * @default null
     */
    backgroundColor?: string

    /**
     * The number of the users (communication mode)/hosts (live broadcast mode) in the channel.
     *
     * @default 0
     */
    regionCount?: number

    /**
     * The user (communication mode)/host (live broadcast mode) list of `AgoraVideoMixingLayout`.
     * Each user (communication mode)/host (live broadcast mode) in the channel has a region to display the video
     * on the screen with the following parameters to be set.
     *
     * @default null
     */
    regions?: AgoraRegion[]

    /**
     * User-defined data.
     *
     * @default null
     */
    appData?: string

    /**
     * The length of the user-defined data.
     *
     * @default 0
     */
    appDataLength?: number

    /**
     * Sets whether or not to show the last frame of a user in the region after the user leaves the channel:
     * - true: The user's last frame shows in the region.
     * - false: (Default) The user's last frame does not show in the region.
     *
     * @default false
     */
    keepLastFrame?: boolean

    /**
     * Sets the number of watermarks that you want to add to the video.
     * You can add up to 15 watermarks, including one timestamp watermark, four image watermarks, and ten text watermarks.
     *
     * @default 0
     */
    wm_num?: number

    /**
     * The configuration of the watermarks. Pointer to an array of WatermarkConfig.
     *
     * @default null
     */
    wm_configs?: AgoraWatermarkConfig[]
}

export declare class AgoraRecorderSdk {
    joinChannel: (
        appid: string,
        token: string,
        channel: string,
        uid: string,
        binPath: string,
        configPath: string
    ) => boolean
    leaveChannel: () => boolean
    release: () => void
    setMixLayout: (layout: AgoraVideoMixingLayout) => void
    on(eventName: AgoraRecorderEvent.REC_EVENT_JOIN_CHANNEL, callback: (channel: string, uid: string | number) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_LEAVE_CHANNEL, callback: () => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_ERROR, callback: (err: number, statCode: number) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_USER_JOIN, callback: (uid: string | number) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_USER_LEAVE, callback: (uid: string | number) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_ACTIVE_SPEAKER, callback: (uid: string | number) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_CONN_LOST, callback: () => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_CONN_INTER, callback: () => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_STREAM_CHANGED, callback: (receivingAudio: boolean, receivingVideo: boolean) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_FIRST_VIDEO_FRAME, callback: (uid: string | number, width: number, height: number, elapsed: number) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_FIRST_AUDIO_FRAME, callback: (uid: string | number, elapsed: number) => void): void
    on(eventName: AgoraRecorderEvent.RTC_EVENT_AUDIO_VOLUME_INDICATION, callback: (speakers: string, speakerNum: number) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_REMOTE_VIDEO_STREAM_STATE_CHANGED, callback: (uid: string | number, state: number, reason: number) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_REMOTE_AUDIO_STREAM_STATE_CHANGED, callback: (uid: string | number, state: number, reason: number) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_REJOIN_SUCCESS, callback: (channel: string, uid: string | number) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_CONN_STATE_CHANGED, callback: (state: number, reason: number) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_REMOTE_VIDEO_STATS, callback: (uid: string | number, stats: any) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_REMOTE_AUDIO_STATS, callback: (uid: string | number, stats: any) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_RECORDING_STATS, callback: (stats: any) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_LOCAL_USER_REGISTER, callback: (uid: string | number, account: string) => void): void
    on(eventName: AgoraRecorderEvent.REC_EVENT_USER_INFO_UPDATED, callback: (uid: string | number, info: any) => void): void
    on(eventName: string, callback: (...args: any[]) => void): void
}

export default AgoraRecorderSdk
