#import <Foundation/Foundation.h>
#import <UIKit/UIApplication.h>
#import "YCMediaTypes.h"

@protocol ChannelSessionDelegate <NSObject>

@required

/**
 *  音频链路状态 [必须实现]
 *
 *  @param state 链路状态, 状态定义:
 *				 enum MediaLinkStatus
 *				 {
 *					MEDIA_LINK_STATUS_CONNECTING = 0,	//连接中
 *					MEDIA_LINK_STATUS_CONNECTED,		//已连接
 *					MEDIA_LINK_STATUS_DISCONNECT		//已断开
 *				 };
 *  @param ip    已连接的音频服务器地址
 *  @param port  已连接的音频服务器端口
 */
- (void)onAudioLinkStatus:(uint32_t)state ip:(uint32_t)ip port:(uint16_t)port;


/**
 *  有人开始说话, 即上麦
 *
 *  @param uid 说话者ID
 */
- (void)onAudioStreamStarted:(uint32_t)uid;


/**
 *  有人停止说话, 即下麦
 *
 *  @param uid 说话者ID
 */
- (void)onAudioStreamStoped:(uint32_t)uid;


/**
 *  视频链路状态 [必须]
 *
 *  @param state 链路状态, 状态定义:
 *				 enum MediaLinkStatus
 *				 {
 *					MEDIA_LINK_STATUS_CONNECTING = 0,	//连接中
 *					MEDIA_LINK_STATUS_CONNECTED,		//已连接
 *					MEDIA_LINK_STATUS_DISCONNECT		//已断开
 *				 };
 *  @param ip    已连接的视频服务器地址
 *  @param port  已连接的视频服务器端口
 */
- (void)onVideoLinkStatus:(uint32_t)state ip:(uint32_t)ip port:(uint16_t)port;

/**
 *  有视频流可以播放, 可以通过startSubscibeVideoWithUserGroupId进行订阅
 *
 *  @param userGroupId 用户组ID
 *  @param streamId    视频流ID,  (streamId >> 32) 表示发布视频流的uid
 *  @param publishId   保留字段
 *  @param streamFlag  保留字段
 */
- (void)onVideoStreamArrived:(uint64_t)userGroupId streamId:(uint64_t )streamId publishId:(uint32_t) publishId streamFlag:(uint32_t) streamFlag;

/**
 *  当订阅成功后, 表示视频流已开始播放
 *
 *  @param userGroupId 用户组ID
 *  @param streamId    视频流ID,  (streamId >> 32) 表示发布视频流的uid
 */
- (void)onVideoStreamStarted:(uint64_t)userGroupId streamId:(uint64_t)streamId;


/**
 *  视频流已经停止播放
 *
 *  @param userGroupId 用户组ID
 *  @param streamId    视频流ID,  (streamId >> 32) 表示发布视频流的uid
 */
- (void)onVideoStreamClosed:(uint64_t)userGroupId streamId:(uint64_t)streamId;


/**
 *  收到公屏信息
 *
 *  @param chatTextInfo 消息内容
 */
- (void)onChatText:(ChatTextInfo*) chatTextInfo;

/**
 *  媒体前端命令执行通知, 媒体前端发送命令由本地执行, 应用层收到此消息要释放相关资源或者用户提示
 *
 *  @param command	 执行的命令, 1 停止视频直播,相当调用了stopPublishVideo; 2 停止音频直播, 相当调用了closeMic
 */
- (void)onMediaInnerCommand:(uint32_t)command;


@optional

/**
 *  说话者音量大小
 *
 *  @param uid		说话者uid
 *  @param volume	音量级别
 */
- (void)onAudioVolumeArrived:(uint32_t)uid volume:(int) volume;

/**
 *  音频链路统计信息, 当rtt>200 或者 上下行数据丢包率>25%时会收到此消息 [可选]
 *
 *  @param rtt         rtt
 *  @param upSendNum   本地发送数据包数
 *  @param upRecvNum   服务端数据接收数
 *  @param downSendNum 下行服务器发送数据数
 *  @param downRecvNum 下行本地接收数据数
 *  @param state       网络状态, 这几种状态可能同时发生, 所以此字段是这些状态 | 的结果(AUDIO_LINK_RTT_BAD|AUDIO_LINK_UP_LOSS|AUDIO_LINK_DOWN_LOSS)
 *					 enum AudioLinkQuality
 *					 {
 *						AUDIO_LINK_NORMAL = 0x0,
 *						AUDIO_LINK_RTT_BAD = 0x01,	//rtt > 200
 *						AUDIO_LINK_UP_LOSS = 0x02,	//上行丢包率 > 25%
 *						AUDIO_LINK_DOWN_LOSS = 0x04	//下行丢包率 > 25%
 *					 };
 */
- (void)onAudioLinkStatics:(uint32_t)rtt upSendNum:(uint32_t)upSendNum upRecvNum:(uint32_t)upRecvNum downSendNum:(uint32_t)downSendNum downRecvNum:(uint32_t)downRecvNum state:(uint32_t)state;

/**
 *  音频播放质量通知
 *
 *  @param speakerUid        说话者ID
 *  @param playFrameCount    统计间隔内播放帧数
 *  @param lossFrameCount    统计间隔内丢帧数
 *  @param discardFrameCount 统计间隔内解码丢帧数
 *  @param duration          统计间隔
 */
- (void)onPlayAudioStateNotify:(uint32_t)speakerUid playFrameCount:(uint32_t)playFrameCount lossFrameCount:(uint32_t)lossFrameCount discardFrameCount:(uint32_t)discardFrameCount duration:(uint32_t)duration;

/**
 *  频道音频状态通知
 *
 *  @param state  1 频道没有人在说话; 2 频道听到声音; 3 频道有人说话, 但听不到声音
 *
 */
- (void)onChannelAudioStateNotify:(uint32_t)state;

/**
 *  本地视频发布状态通知
 *
 *  @param status 视频发布状态, 0 表示失败, 1 表示成功, 当前只有成功才会通知
 */
- (void)onVideoPublishStatus:(uint32_t)status;


/**
 *  当前绘图窗口的渲染状态
 *
 *  @param state 0 表示渲染开始; 1表示渲染停止
 */
- (void)onVideoRenderStatus:(uint32_t)state;

/**
 *  视频下行链路的丢包率
 *
 *  @param uid   视频所属用户ID
 *  @param plr   plr为小数，如0.15则表示15%的丢包率
 */
- (void)onVideoDownlinkPLR:(uint32_t)uid plr:(float)plr;


/**
 *  登录后通知是否有视频流, 通过此消息可以在视频链路连接成功前获知是否有视频流
 *
 *  @param hasvideo YES 表示有视频流; NO 表示没有视频流
 */
- (void)onVideoLiveBroadcast:(BOOL)hasVideo;


/**
 *  当前下行视频流有多个码率可选, 如果收到此通知, 则后续可以通过changeVideoCodeRate函数改变下行码流, 如果有多个下行视频流, 则对所有下行视频流生效 [必须]
 *
 *  @param codeRateLevels 可选的码率列表
 */
- (void)onVideoCodeRateLevelNotify:(NSMutableArray *)codeRateLevels;


/**
 *  调用 changeVideoCodeRate 更改下行视频流码率结果
 *
 *  @param codeRate 如果成功, 则表示已生效的当前码率
 *  @param result   0 表示失败, 可能不支持所设置的码率, 1 表示成功
 */
- (void)onVideoCodeRateChangeNotify:(uint32_t)codeRate result:(uint32_t)result;


/**
 *  下行视频流MetaData [可选]
 *
 *  @param streamId  视频流ID
 *  @param bitRate   码率
 *  @param frameRate       帧率
 */

- (void)onVideoMetaDataNotify:(uint64_t)streamId bitRate:(uint32_t)bitRate frameRate:(uint32_t)frameRate;

/**
 *  没有视频通知
 *
 *  @param streamId 视频流ID
 *  @param reason   
					 enum NoVideoReason
					 {
						HAS_VIDEO_PLAY = 0,						    // 正常
						NO_VIDEO_SUBSCRIBE = 1,					    // 没有订阅
						NO_VIDEO_PACKET = 2,					    // 没有收到视频包
						NO_VIDEO_FRAME = 3,						    // 没有组成视频帧
						NO_VIDEO_PUSH_TO_DECODE = 4,			    // 没有视频帧解码
						NO_VIDEO_ON_PLAY = 5,					    // 没有视频帧播放
						VIDEO_DECODE_ERROR = 6,					    // 解码出错
						NOT_SUBSCRIBE_BY_PLUGIN = 7,			    // 没有订阅by plugin
						NO_VIDEO_I_FRAME = 8,					    // 没有收到I帧
						NO_VIDEO_VIEW = 9,						    // 没有view可渲染
						NO_FETCH_VIDEO_PROXY = 10,					// 没获取到视频前端
						NO_VIDEO_LIVE = 11,							// 该频道没视频开播
						TCP_LOGIN_FAILED = 12,						// tcp登录视频前端失败
						NO_RECV_STREAM_ID = 13,						// 没收到streamId
						
						UNKNOW_REASON = 99,							// 其它未知原因
					 };
 */
- (void)onNoVideoNotify:(uint64_t)streamId reason:(uint32_t)reason;

/**
 *  视频解码慢通知
 *
 *  @param streamId   streamId
 *  @param bitRate    码率
 *  @param frameRate  帧率
 *  @param decodeRate 当前解码帧率
 *  @param width      宽
 *  @param height     高
 */
- (void)onVideoDecodeSlowNotify:(uint64_t)streamId bitRate:(uint32_t)bitRate frameRate:(uint32_t)frameRate
					decodeRate:(uint32_t)decodeRate width:(uint32_t)width height:(uint32_t)height;

/**
 *  视频流丢包率通知消息, 一般每2s统计通知
 *
 *  @param streamId   streamId
 *  @param duration   统计间隔(一般2s)
 *  @param frameRate  视频流码率
 *  @param playCnt    统计间隔内播放的帧数
 *  @param netLossCnt 统计间隔内网络丢帧数
 *  @param discardCnt 统计间隔解码丢帧数
 */
- (void)onVideoFrameLossNotify:(uint64_t)streamId duration:(uint32_t)duration frameRate:(uint32_t)frameRate
					playCnt:(uint32_t)playCnt netLossCnt:(uint32_t)netLossCnt discardCnt:(uint32_t)discardCnt;

/**
 *  自适应多码流档位通知, 如果当前码率过高导致视频不流畅等, 则会收到此消息, 然后可以通过调用changeVideoCodeRate调整适当的码率
 *
 *  @param recvNum   当前实际接收的视频包数
 *  @param recvRange 需要接收的视频包数
 *  @param rtt       rtt
 *  @param result    0 表示需要降低码率档位
 */
- (void)onVideoCodeRateLevelSuggest:(uint32_t)recvNum recvRange:(uint32_t)recvRange rtt:(uint32_t)rtt result:(uint32_t)result;


/**
 *  视频上行链路的丢包率和rtt, 可以通过此判断上行链路状态
 *
 *  @param lossRate   如果lossRate >= 15 (15%), 则丢包率大, 表示网络状况不是很好
 *  @param rtt		  如果rtt >= 600, 表示网络状况不是很好
 */
- (void)onVideoUplinkLossRate:(uint32_t)lossRate rtt:(uint32_t)rtt;

@end

#pragma mark - ChannelSession
@class VideoOutput;
@class YCVideoView;

@interface ChannelSession : NSObject

@property (nonatomic, unsafe_unretained) id<ChannelSessionDelegate> delegate;

- (id)init;

/**
 *  配置参数, 此函数必须调用其他功能性函数前调用, 参数才会生效
 *
 *  @param appId   请保持与loginWithAppid一致的参数
 *  @param configs 详细的参数设置值参考@YCMediaDefine的ConfigKey
 */
- (void)setConfigs:(uint32_t)appId configs:(NSDictionary *)configs;

/**
 *  更新token信息
 *
 *  @param token 需要更新的token
 */
- (void)updateToken:(NSData *) token;


/**
 *  网络状态变化通知
 *
 *  @param state 网络状态
 */

- (void)networkStateChange:(uint32_t)state;

/**
 *  登录
 *
 *  @param appid 应用ID
 *  @param sid   频道ID
 *  @param uid   用户ID
 *  @param token token
 *
 *  @return YES 成功, NO 失败
 */
- (BOOL)loginWithAppid:(uint32_t)appid sid:(uint32_t)sid uid:(uint32_t)uid token:(NSData *)token;

/**
 *  退出
 */
- (void)logout;

/**
 *  发送公屏消息信息
 *  只有当音频链路已成功连接服务器, 即收到onAudioLinkStatus消息, 而且stats == MEDIA_LINK_STATUS_CONNECTED, 才能调用此函数发送消息, 否则消息可能无法发送
 *
 *  @param textChatInfo 消息
 *
 *  @return return YES 成功, NO 失败
 */
- (BOOL)sendChatText:(ChatTextInfo *)textChatInfo;

/**
 *	获取时间戳
 */
- (long)getTickCount;

#pragma mark -  audio
/**
 *  打开麦克风, 即上麦
 *
 *  @return YES 成功, 并不表示发送成功; NO 失败
 */
- (BOOL)openMic;

/**
 *  关闭麦克风, 即下麦
 */
- (void)closeMic;


/**
 *  判断当前麦克风是否打开
 *
 *  @return YES 打开; NO 关闭
 */
- (BOOL)isMicOpened;


/**
 *  设置音频播发模式, sdk 默认为扬声器播放模式
 *
 *  @param on YES 表示扬声器模式; NO 为听筒模式
 */
- (void)setSpeakerphoneOn:(BOOL)on;

/**
 *  判断当前是否扬声器播放模式
 *
 *  @return YES 扬声器播放; NO  听筒/耳机/蓝牙播放
 */
- (BOOL)isSpeakerphoneOn;

/**
 *  声音输出状态切换, sdk 默认输出声音
 *
 *  @param enable YES 表示允许声音播放; NO 表示不允许声音播放, 即静音
 */
- (void)muteAudio:(BOOL)enable;

/**
 *  判断声音输出状态
 *
 *  @return YES 表示没有声音输出, 即静音; NO 表示有声音输出
 */
- (BOOL)isAudioMuted;

#pragma mark -  video
/**
 *  订阅观看视频流
 *
 *  @param userGroupId 从onVideoStreamArrived回调获取
 *  @param streamId    从onVideoStreamArrived回调获取, (streamId >> 32) 表示发布视频流的uid
 *
 *  @return YES 成功; NO 失败
 */
- (BOOL)startSubscibeVideoWithUserGroupId:(uint64_t)userGroupId streamId:(uint64_t)streamId;


/**
 *  停止观看视频流
 *
 *  @param userGroupId 从onVideoStreamArrived回调获取
 *  @param streamId    从onVideoStreamArrived回调获取, (streamId >> 32) 表示发布视频流的uid
 */
- (void)stopSubscibeVideoWithUserGroupId:(uint64_t)userGroupId streamId:(uint64_t)streamId;


/**
 *  添加用于渲染视频流的view,注意要通过VideoView的connectToVideoInfo接口把view与视频流绑定
 *
 *  @param videoView videoView description
 */
- (void)addVideoView:(YCVideoView *)videoView;


/**
 *  删除用于渲染视频流的view
 *
 *  @param videoView videoView description
 */
- (void)removeVideoView:(YCVideoView *)videoView;


/**
 *  设置发布的视频画面上的水印信息
 *
 *  @param watermarkInfo 水印信息
 *
 *  @return YES 成功; NO 失败
 */
- (BOOL)setVideoWatermark:(WatermarkInfo *)watermarkInfo;

/**
 *  更改下行视频流的码率
 *
 *  @param codeRateLevel 码率, 只能填写onVideoCodeRateLevelNotify返回的列表中得参数
 */
- (void)changeVideoCodeRate:(uint32_t)codeRateLevel;


/**
 *  打开摄像头采集
 *
 *  @param preview 用于显示摄像头预览画面的view, 如果不需要在本地显示, 则可以设置nil
 *
 *  @return YES 成功; NO 失败
 */
- (BOOL)startCameraWithPreview:(YCVideoView *) preview;

//
/**
 *  切换摄像头
 *
 *  @return YES 成功; NO 失败
 */
- (BOOL)switchCamera;

/*
 *  得到当前摄像头
 *
 *  @return 0 前置; 1 后置
 */
- (int)getCurrentCameraType;

/**
 *  关闭摄像头, 除关闭本地预览外,同时也会停止本地视频发布, 相当调用了stopPublishVideo
 */
- (void)stopCamera;


/**
 *  开始发布本地视频流, 调用此函数前请先调用startCameraWithPreview
 *
 *  @return YES 成功; NO 失败
 */
- (BOOL)startPublishVideo;


/**
 *  停止发布本地视频流
 */
- (void)stopPublishVideo;

/**
 *  开始录制音视频, 将本地发布的音视频数据录制保存到服务器, 在调用此函数前需要先发布了本地的视音频
 *
 *	@param programId 唯一识别字符串, 生成字符串的规则是: channelid_appid_spkuid_timestamp
 *
 *  @return YES 成功; NO 失败
 */
- (BOOL)startServerRecordWithProgramId:(NSString *)programId;

/**
 *  停止录制音视频
 */
- (void)stopServerRecord;


/**
 *  设置闪光灯
 *
 *  @param flashlightMode 闪光灯模式
 *
 *  @return YES 成功; NO 失败
 */
- (BOOL)setCameraFlashlightMode:(FlashlightMode)flashlightMode;


/**
 *  截取摄像头画面
 *
 *  @return 截取到的图片；如果截取失败，返回nil
 */
- (UIImage*)captureCameraSnapshot;

/**
 *  视频变焦缩放因子
 *
 *  @param factor 变焦因子
 *
 */
@property float videoZoomFactor;

/**
 *   暂停GPU摄像头采集
 */
- (void)pauseGPUCameraCaptrue;

/**
 *   恢复GPU摄像头采集
 */
- (void)resumeGPUCameraCaptrue;

-(void)setGPUOrientation:(GPUViewOrientation)orient;

typedef enum{
    gpuFilterTypeInvalid    = 0,/**< 无效 */
    gpuFilterTypeNone       = 1,/**< 去噪原画,只做裁剪缩放，字符串参数：@“”*/
    gpuFilterTypeBeauty     = 2,/**< 美白,字符串参数：@“磨皮度float(0-10,默认为7), 白皙度float（0-1，默认为0.85，建议设为1.0或默认）”*/
    gpuFilterTypeMask       = 3,/**< 蒙版,字符串参数：@“蒙版宽,蒙版高,蒙版位置x,蒙版位置y,蒙版Rect宽,蒙版Rect高,apha”，建议采用默认值@“,,,,,,apha”*/
    gpuFilterTypeWarm       = 4,/**< 温馨,字符串参数：@"温暖度(0-2,默认1.4)" */
    gpuFilterTypeNoble      = 5,/**< 复古,字符串参数：@""，建议无参*/
    gpuFilterTypeBlackWhite = 8,/**< 黑白,字符串参数：@""，建议无参 */
    gpuFilterTypeSweet      = 16,/*< 甜美,字符串参数：@""，建议无参*/
    gpuFilterTypeMint       = 32,/*< 薄荷,字符串参数：@""，建议无参 */
    gpuFilterTypeMagic      = 64,/*< 魔幻,字符串参数：@"魔幻度float(1-10,默认1)"，建议默认@"" ******该功能未稳定，不建议使用*/

}GPUFILTERTYPE;

-(void)setVideoFilterParamswithType:(GPUFILTERTYPE)TID  FilterParams:(NSString*)sVal dstImgWidth:(uint32_t)imgWidth dstImgHeight:(uint32_t)imgHeight withFilterData:(NSData*)imgData;


/**
 *
 *SDK内部传递消息用
 *
 */

- (void)signal2Media:(int)msgId data:(NSData *)data;
- (void)onProtoLinkConnected:(uint32_t)wanIp;
- (void)signalBroadcast:(NSData *)data;

@end
