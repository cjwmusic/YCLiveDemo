//
//  DisplayViewController.h
//  YCloudVoipDemo
//
//  Created by bluex on 14/12/23.
//  Copyright (c) 2014年 duowan. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "YCMediaSDK.h"

@interface DisplayViewController : UIViewController<ChannelSessionDelegate, UITextFieldDelegate, UITableViewDelegate, UITableViewDataSource>

@property (strong,nonatomic) NSString* uid;
@property (strong,nonatomic) NSString* sid;

@end
