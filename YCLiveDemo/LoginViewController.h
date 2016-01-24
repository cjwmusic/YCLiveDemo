//
//  LoginViewController.h
//  YCloudVoipDemo
//
//  Created by bluex on 14/12/23.
//  Copyright (c) 2014年 duowan. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface LoginViewController : UIViewController<UITextFieldDelegate>

@property (weak, nonatomic) IBOutlet UITextField *uid;
@property (weak, nonatomic) IBOutlet UITextField *sid;

@end
