#import <Foundation/Foundation.h>
#import <UIKit/UIImage.h>

#ifndef _YCMEDIATYPES_
#define _YCMEDIATYPES_

typedef enum {
    AudioPlayerStopReasonNone = 0,
    AudioPlayerStopReasonUnknown,
    AudioPlayerStopReasonFileInvalid,
    AudioPlayerStopReasonNoData,
} AudioPlayerStopReason;

typedef enum {
    AudioRecorderStopReasonNone = 0,
    AudioRecorderStopReasonUnknown,
    AudioRecorderStopReasonAudioDevice, // kErrAudioDevice
    AudioRecorderStopReasonFile,        // kErrFile
} AudioRecorderStopReason;

typedef enum {
    VideoViewRotationAngle0 = 0,
    VideoViewRotationAngle90,
    VideoViewRotationAngle180,
    VideoViewRotationAngle270,
} VideoViewRotationAngle;

typedef enum {
    VideoScaleModeFill,
    VideoScaleModeAspectFit,
    VideoScaleModeClipToBounds,
} VideoScaleMode;

typedef enum {
    FlashlightModeOff = 0,
    FlashlightModeOn,
    FlashlightModeAuto,
} FlashlightMode;

typedef enum {
    
    OrientationUnknown= 0,
    OrientationPortrait,
    OrientationPortraitUpsideDown,
    OrientationLandscapeLeft,
    OrientationLandscapeRight,
    
}GPUViewOrientation;


typedef enum {
    CallNetworkTypeUnknown,
    CallNetworkTypeWifi,
    CallNetworkType2G,
    CallNetworkType3G,
    CallNetworkType4G,
    CallNetworkTypeLagecy,
} CallNetworkType;

typedef enum {
    SignalNone = 0,
    SignalWeak,
    SignalNormal,
    SignalStrong,
} SignalStrength;

typedef enum {
    TextChatVersion1,
    TextChatVersion2,
    TextChatVersion3,
} TextChatVersion;


#pragma mark - MediaFrameInfo
@interface MediaFrameInfo : NSObject

@property (assign, nonatomic) NSInteger transMode;
@property (assign, nonatomic) NSUInteger numOfHigh;
@property (assign, nonatomic) NSUInteger numOfLow;

@end


#pragma mark - MediaAVInfo
@interface MediaAVInfo : NSObject

@property (assign, nonatomic) uint32_t appid;
@property (assign, nonatomic) uint32_t retryType;
@property (assign, nonatomic) uint32_t retryInterval;

@end

#pragma mark - MediaProxyInfo
@interface MediaProxyInfo : NSObject

@property (assign, nonatomic) NSInteger ipv4;
@property (assign, nonatomic) NSInteger ispType;
@property (strong, nonatomic) NSArray *tcpPorts;
@property (strong, nonatomic) NSArray *udpPorts;

@end

#pragma mark - WatermarkInfo
typedef enum {
    MarginLeftTop,
    MarginRightTop,
    MarginLeftBottom,
    MarginRightBottom,
} MarginType;

@interface WatermarkInfo : NSObject

@property (strong, nonatomic) UIImage *watermark;
@property (assign, nonatomic) MarginType marginType;
@property (assign, nonatomic) CGPoint margin;

@end

typedef enum {
    VideoInfoStateOpening,
    VideoInfoStateOpened,
    VideoInfoStateClosed,
} VideoInfoState;

@interface VideoInfo : NSObject

@property (assign, nonatomic) uint64_t userGroupId;
@property (assign, nonatomic) uint64_t streamId;

- (BOOL)isEqual:(VideoInfo *)videoInfo;
- (uint32_t)getUid;
- (uint32_t)getAppId;
- (BOOL)isEmpty;
- (VideoInfo *)clone;

@end

@interface ChatTextInfo : NSObject
@property (assign, nonatomic) NSUInteger uid;
@property (assign, nonatomic) NSUInteger color;
@property (assign, nonatomic) NSUInteger height;
@property (strong, nonatomic) NSString *text;

@end

@interface VideoSPSInfo : NSObject

@property (assign, nonatomic) NSUInteger appId;
@property (assign, nonatomic) uint64_t streamId;
@property (assign, nonatomic) NSInteger fps;
@property (assign, nonatomic) NSInteger width;
@property (assign, nonatomic) NSInteger height;

@end

@interface VideoLiveNotifyInfo : NSObject
@property (assign, nonatomic) uint32_t appid;
@property (assign, nonatomic) uint32_t subSid;
@property (assign, nonatomic) uint32_t hasVideo;
@end

#endif
