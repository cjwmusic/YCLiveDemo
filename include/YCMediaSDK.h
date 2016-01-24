#import <Foundation/Foundation.h>
#import "YCChannelSession.h"

@interface YCMediaSDK : NSObject

/**
 *  sdk单例
 *
 *  @return sdk 实例
 */
+ (YCMediaSDK *)sharedInstance;

/**
 * 释放sdk, 只有在销毁app时才需要调用
 */
+(void)  releaseInstance;

/**
 *  获取sdk版本号
 *
 *  @return 版本号字符串
 */
+ (NSString *)getSdkVersion;

/**
 *  创建频道会话, 一个app同时只能创建一个会话实例
 *
 *  @return 频道回话实例
 */
- (ChannelSession *)createChannelSession;

/**
 *  释放 ChannelSession
 */
- (void)releaseChannelSession;

@end
