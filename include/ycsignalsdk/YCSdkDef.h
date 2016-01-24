//
//  YCSdkParam.h
//  ycsdk
//
//  Created by WangQiming on 15-3-27.
//  Copyright (c) 2015年 yy. All rights reserved.
//

#ifndef ycsdk_YCSdkDef_h
#define ycsdk_YCSdkDef_h
#import "YCSdkParam.h"
//AppDelegate inherit
@protocol IProtoSdkLogDelegate <NSObject>
-(void) log: (ProtoSdkLogLevel)logLevel msg:(NSString*)msg mod:(NSString*) mod;
@end

@interface LoginInfo : NSObject
@property           NSData*  account;
@property           NSData*  password;
@property           uint32_t loginType;
@property           NSMutableDictionary* prop;
@end

@interface SDKInitParam : NSObject
@property           NSNumber*   appKey;     //[required]32位整型标识app
@property(copy)     NSData*     appVer;     //[required]app的版本号。
@property           NSNumber*   terminalType;   //[required] 终端类型
@property(copy)     NSString*   iosVer;         // ios系统版本.
@property(copy)     NSString*   devModel;       // 设备model.
@property(copy)     NSString*   idForVendor;    //[required]即UIDevice中得identifierForVendor
@property           bool        sdkEventToMainThread;  //[required]sdk的回调事件是否抛给主线程
@property(strong)   id<IProtoSdkLogDelegate>    logDelegate; //sdk层的log回调接口, 可为nil， 这时sdk将无log打入app的log文件.
@end

//Binary Request
@interface AdapterRequestBase : NSObject
@property NSNumber*     reqType;
@property NSData*       context;
@end

@interface TransmitDataReq : AdapterRequestBase
@property NSString*     dstServerName;
@property NSNumber*     innerUri;
@property NSNumber*     bWrapperRouter;
@property NSData*       payload;
@end

//Binary Event
@interface AdapterEventBase : NSObject
@property NSData*           context;
@end

@interface TransmitData : AdapterEventBase
@property NSString*         svrName;
@property NSNumber*         innerUri;
@property NSData*           payload;
@end

@interface BroadcastData : AdapterEventBase
@property NSNumber*         svcType;
@property NSData*           data;
@end

@interface UnicastData : AdapterEventBase
@property NSNumber*         svcType;
@property NSNumber*         uri;
@property NSData*           data;
@end

@interface SendUserBroadcastMsg : AdapterRequestBase
@property NSNumber*         topSid;
@property NSNumber*         subSid;
@property NSNumber*         svcType;
@property NSData*           data;
@end

@interface UserBroadcastMsg : AdapterEventBase
@property NSNumber*         from;
@property NSNumber*         topSid;
@property NSNumber*         subSid;
@property NSNumber*         svcType;
@property NSData*           data;
@end
#endif
