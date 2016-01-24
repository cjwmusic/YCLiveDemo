//
//  AppDelegate.m
//  YCloudVoipDemo
//
//  Created by bluex on 14/12/23.
//  Copyright (c) 2014年 duowan. All rights reserved.
//

#import "AppDelegate.h"
#import "ProtoSDKMgr.h"
#import <Bugly/CrashReporter.h>

@interface AppDelegate ()

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
	// Override point for customization after application launch.
	[[CrashReporter sharedInstance] installWithAppId:@"900012319"];
	[self _initProtoSDK];
	return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application {
	// Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
	// Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application {
	// Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later.
	// If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
	[[ProtoSDKMgr Instance]appDidEnterBackground];
}

- (void)applicationWillEnterForeground:(UIApplication *)application {
	// Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
	[[ProtoSDKMgr Instance]appWillEnterForeground];
}

- (void)applicationDidBecomeActive:(UIApplication *)application {
	// Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application {
	// Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

-(void)_initProtoSDK{
	//sdk init
	SDKInitParam* param = [[SDKInitParam alloc] init];
	NSNumber* appKey = [NSNumber numberWithUnsignedInt:2134267130];
	param.appKey = appKey;
	param.appVer =[@"1.0.0.0" dataUsingEncoding: NSUTF8StringEncoding];
	NSNumber* terminalType = [NSNumber numberWithUnsignedInt:0x10001];
	param.terminalType = terminalType;
	param.idForVendor = [[[UIDevice currentDevice] identifierForVendor] UUIDString];
	param.sdkEventToMainThread = true;
	param.logDelegate = nil;
	
	[[ProtoSDKMgr Instance]initSDK:param loginInfo:nil];
}
@end
