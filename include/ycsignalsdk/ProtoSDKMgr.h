//
//  ProtoSDKMgr.h
//  yysdk
//
//  Created by kele on 2/24/14.
//  Copyright (c) 2014 kele. All rights reserved.
//

#import <Foundation/Foundation.h>
//#import "IProtoSDKMgr.h"
#import "YCSdkDef.h"
//#import "YCMediaSDK.h"

@interface ProtoSDKMgr : NSObject// <IProtoSDKMgr>
+(ProtoSDKMgr*)     Instance;
+(void)             Release;

-(SDKInitParam*)            getSDKInitParam;
-(void) initSDK: (SDKInitParam*) param loginInfo:(LoginInfo*)info;
-(int)  sendRequest: (NSData*) request;
-(int)  sendBinRequest: (AdapterRequestBase*) request;
-(NSData*) getYCTokenHexDataFormat: (NSData*) jsonData;
-(NSData*) getYCTokenBinFormat: (NSData*) jsonData;
-(void) log: (NSString*)msg mod:(NSString*) mod;

//@property         ChannelSession            *mMediaChannelSession;
/* App前后台状态切换情况
 *
 */
-(void) appDidEnterBackground;
-(void) appWillEnterForeground;
@end
