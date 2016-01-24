//
//  LoginViewController.m
//  YCloudVoipDemo
//
//  Created by bluex on 14/12/23.
//  Copyright (c) 2014年 duowan. All rights reserved.
//

#import "LoginViewController.h"
#import "DisplayViewController.h"

@interface LoginViewController ()<UITextFieldDelegate>

@end

@implementation LoginViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
	_uid.delegate = self;
	_sid.delegate = self;
	
	srand((unsigned)time(0));
	int i = rand() % 1000000;
	self.uid.text =[NSString stringWithFormat:@"%d",i];
	
}

- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}


#pragma mark - Navigation

- (BOOL)shouldPerformSegueWithIdentifier:(NSString *)identifier sender:(id)sender NS_AVAILABLE_IOS(6_0)
{
	if([self.uid.text length] == 0 || [self.sid.text length] == 0) {
		UIAlertView *alter = [[UIAlertView alloc] initWithTitle:@"登录" message:@"请输入完整信息再登录" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil];
		
		[alter show];
		return  NO;
	}
	return YES;
}


//点击了登录按钮之后---->
- (void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender {

    
    //传递uid与sid（频道号）到DisplayViewController
	NSString* uid = self.uid.text;
	NSString* sid = self.sid.text;
	
	DisplayViewController* view = segue.destinationViewController;

	if ([view respondsToSelector:@selector(setSid:)]) {
		[view setValue:sid forKey:@"sid"];
	}
	
	if ([view respondsToSelector:@selector(setUid:)]) {
		[view setValue:uid forKey:@"uid"];
	}

}

- (BOOL)textFieldShouldReturn:(UITextField *)theTextField {
	if (theTextField == self.uid || theTextField == self.sid) {
		[theTextField resignFirstResponder];
	}
	return YES;
}

@end
