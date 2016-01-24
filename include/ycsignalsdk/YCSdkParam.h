//
//  YCSdkParam.h
//  ycsdk
//
//  Created by WangQiming on 15-3-27.
//  Copyright (c) 2015年 yy. All rights reserved.
//

#ifndef ycsdk_YCSdkParam_h
#define ycsdk_YCSdkParam_h


typedef enum
{
    kProtoSdkLogLevelDebug = 1,
    kProtoSdkLogLevelInfo = 2,
    kProtoSdkLogLevelError = 3,
} ProtoSdkLogLevel;

typedef NS_ENUM(NSInteger, YCSignalSDKReq) {
    //login
    YCSignalSDKReqLogin                = 1,   //登录
    YCSignalSDKReqLogout               = 2,   //登出
    YCSignalSDKReqChangePic            = 3,   //更换图片验证码（未启用）
    YCSignalSDKReqUidLogin             = 4,   //使用uid/token方式登录
    YCSignalSDKReqOpenCh               = 5,   //通道重启（通道关闭后调用）
    YCSignalSDKReqCreateTopCh          = 6,   //创建顶级频道
    YCSignalSDKReqGetMyCh              = 7,   //获取我的频道列表
    YCSignalSDKReqSearchCh             = 8,   //搜索频道
    
    //sess
    YCSignalSDKReqSessJoin              = 1000,     //进入频道
    YCSignalSDKReqSessLeave             = 1001,     //退出频道
    YCSignalSDKReqSessUpdateTempChInfo  = 1002,     //更新频道信息（缓存型频道信息）
    YCSignalSDKReqSessUpdateUserInfo    = 1003,     //更新用户信息
    YCSignalSDKReqSessPullUserList      = 1004,     //拉取在线用户uid列表
    YCSignalSDKReqSessQueryUserInfo     = 1005,     //请求用户信息
    YCSignalSDKReqSessGetTempChInfo     = 1006,     //获取频道信息（缓存型频道信息）
    YCSignalSDKReqSessTextChat          = 1007,     //发送公屏消息
    YCSignalSDKReqSessTextChatBatch     = 1008,     //发送公屏消息（聚合发送，一秒一发）
    YCSignalSDKReqSessPullOnlineUser    = 1009,     //按位置拉取在线用户信息
    YCSignalSDKReqSessGetChUserCount    = 1010,     //获取频道在线人数
    YCSignalSDKReqSessUpdateDBChInfo    = 1011,     //更新频道信息（数据库型频道信息）
    YCSignalSDKReqSessCreateSubCh       = 1012,     //创建子频道
    YCSignalSDKReqSessDelSubCh          = 1013,     //删除子频道
    YCSignalSDKReqSessChangeSubCh       = 1014,     //变更子频道
    YCSignalSDKReqSessSetRoler          = 1015,     //设置用户角色
    YCSignalSDKReqSessKickoff           = 1016,     //踢出频道
    YCSignalSDKReqSessGetMemberList     = 1017,     //获取马甲用户列表
    YCSignalSDKReqSessGetDBChInfo       = 1018,     //获取频道信息（数据库型频道信息）
    YCSignalSDKReqSessBatchGetDBSubChInfo = 1019,   //批量获取子频道频道信息（数据库型频道信息）
    YCSignalSDKReqSessLoadTextChatHist  = 1020,     //获取公屏历史
    
    //sess queue
    YCSignalSDKReqSessJoinQ             = 1100,
    YCSignalSDKReqSessLeaveQ            = 1101,
    YCSignalSDKReqSessQueryQ            = 1102,
    
    //one2one
    YCSignalSDKReqU2UData               = 2000,
    
    //client to Srv
    YCSignalSDKReqTransmitSvrnameData   = 2001,
    YCSignalSDKReqSubSvcType            = 2002,
    YCSignalSDKReqUnSubSvcType          = 2003,
    YCSignalSDKReqJoinGroup             = 2004,
    YCSignalSDKReqLeaveGroup            = 2005,
};

typedef NS_ENUM(NSInteger, YCSignalSDKEvent) {
    //login
    YCSignalSDKEventLoginRes                        = 1,  //登录返回
    YCSignalSDKEventLogout                          = 2,
    YCSignalSDKEventForceOut                        = 3,  //登录被踢
    YCSignalSDKEventCreateTopChannel                = 4,  //创建顶级频道
    YCSignalSDKEventGetMyChannelRes                 = 5,  //获取频道列表（我的）
    YCSignalSDKEventSearchChannelRes                = 6,  //搜索频道
    
    //report
    YCSignalSDKEventLoginStat                       = 100,
    YCSignalSDKEventReqTimeout                      = 101,  //请求超时
    YCSignalSDKEventChannelClosed                   = 103,  //通道关闭
    
    // session
    YCSignalSDKEventSessOperRes                     = 501,  //操作超时
    YCSignalSDKEventSessJoinRes                     = 502,  //进频道返回
    YCSignalSDKEventSessLeaveChannel                = 503,  //退出频道(一般不需要响应)
    YCSignalSDKEventSessMultiKick                   = 504,  //进频道多踢通知
    YCSignalSDKEventSessMultiKickNtf                = 505,  //被进频道多踢通知
    YCSignalSDKEventSessUpdateTempChInfo            = 506,  //更新频道信息(缓存型频道信息)
    YCSignalSDKEventSessUpdateUserInfo              = 507,  //更新用户信息
    YCSignalSDKEventSessPullUserListRes             = 508,  //拉取在线用户uid列表
    YCSignalSDKEventSessQueryUserInfoRes            = 509,  //查询用户信息
    YCSignalSDKEventSessPushOnlineUser              = 510,  //在线用户信息更新推送
    YCSignalSDKEventSessGetTempChInfoRes            = 511,  //获取频道信息(缓存型频道信息)
    YCSignalSDKEventSessTextChatBCRes               = 512,  //接收公屏消息
    YCSignalSDKEventSessTextChatBCBatchRes          = 513,  //接收公屏消息(服务器聚合，一秒一发）
    YCSignalSDKEventSessPullOnlineUserRes           = 514,  //按位置拉取在线用户信息返回
    YCSignalSDKEventSessUserCountRes                = 515,  //获取在线人数返回
    YCSignalSDKEventSessUpdateUserCountRes          = 516,  //在线人数变更推送
    YCSignalSDKEventSessUpdateDBChBC                = 517,  //更新频道信息(数据库型频道信息)
    YCSignalSDKEventSessCreateSubChBC               = 518,  //子频道创建广播
    YCSignalSDKEventSessDelSubChBC                  = 519,  //子频道删除广播
    YCSignalSDKEventSessChangeSubChRes              = 520,  //跳转子频道结果返回
    YCSignalSDKEventSessSetRolerBC                  = 521,  //设置角色广播
    YCSignalSDKEventSessKickoffBC                   = 522,  //频道人员被踢广播
    YCSignalSDKEventSessGetMemberListRes            = 523,
    YCSignalSDKEventSessGetDBChInfoRes              = 524,
    YCSignalSDKEventSessChangeSubChReport           = 525,
    YCSignalSDKEventSessBatchGetDBSubChInfoRes      = 526,
    YCSignalSDKEventSessGetTextChatHistRes          = 527,
    
    //session queue
    YCSignalSDKEventSessJoinQRes                  = 600,
    YCSignalSDKEventSessLeaveQRes                 = 601,
    YCSignalSDKEventSessQueryQRes                 = 602,
    
    //one2one
    YCSignalSDKEventU2UAck                        = 2000,
    YCSignalSDKEventU2UPushData                   = 2001,
    YCSignalSDKEventSvrnameData                   = 2002,
    YCSignalSDKEventBCMsg                         = 2003,
    YCSignalSDKEventUnicastMsg                    = 2004,
    YCSignalSDKEventJoinGroupAck                  = 2005,
    YCSignalSDKEventLeaveGroupAck                 = 2006,
};

extern NSString* kSdkEventNotification;
extern NSString* kSdkEventNotificationUserInfoKey;

extern NSString* kTransmitDataNotification;
extern NSString* kTransmitDataNotificationUserInfoKey;

extern NSString* kBroadcastDataNotification;
extern NSString* kBroadcastDataNotificationUserInfoKey;

extern NSString* kUnicastDataNotification;
extern NSString* kUnicastDataNotificationUserInfoKey;

extern NSString* kUserBroadcastMsgNotification;
extern NSString* kUserBroadcastMsgNotificationUserInfoKey;

extern NSString* OP_JOIN_GROUP;
extern NSString* OP_UPDATE_UINFO;
extern NSString* OP_UPDATE_SINFO;
#endif
