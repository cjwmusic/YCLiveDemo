//
//  VideoView.h
//  medialibrary
//
//  Created by Hao Richard on 13-6-6.
//  Copyright (c) 2013年 daiyue. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIView.h>
#import "YCMediaTypes.h"

@protocol YCVideoViewDelegate<NSObject>

@optional

- (void)onPlayPause;
- (void)onPlayResume;
@end



@interface YCVideoView : UIView

@property (nonatomic, unsafe_unretained) id<YCVideoViewDelegate> delegate;
@property (nonatomic, readwrite) int acceptableMaxFrameInterval; // in milliseconds

@property (nonatomic, readonly) VideoInfo *videoInfo;

- (id)initWithFrame:(CGRect)viewFrame;

- (void)setRotateAngle:(VideoViewRotationAngle)rotationAngle;
- (void)setScaleMode:(VideoScaleMode)scaleMode;
- (void)setAutoRotation:(BOOL)autoRotation;

- (void)connectToVideoInfo:(VideoInfo *)videoInfo;			//把view与视频流绑定
- (void)disconnectFromVideoStream;							//把view与视频流解除绑定

// Methods below are used for Render, UI doesn't care of them.
- (int)drawView;
- (void)clearAllresource;

- (BOOL)render:(id)param;

- (void *)getRenderView;
- (void *)getDataProvider;

- (uint64_t) getUserGroupId;
- (uint64_t) getStreamId;

- (UIImage *)getVideoScreenshot;

-(void) setGPUFID:(uint32_t) fID;

@end
