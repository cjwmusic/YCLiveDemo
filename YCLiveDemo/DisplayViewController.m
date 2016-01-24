//
//  DisplayViewController.m
//  YCloudVoipDemo
//
//  Created by bluex on 14/12/23.
//  Copyright (c) 2014年 duowan. All rights reserved.
//

#import "DisplayViewController.h"
#import "LoginViewController.h"
#import "YCVideoView.h"
#import "YCMediaDefine.h"
#import <time.h>

#import <AVFoundation/AVFoundation.h>
#import "GTMBase64.h"

#import "AppInfo.h"
#import "ProtoSDKMgr.h"

#define kDefaultNameTextFont  [UIFont systemFontOfSize:8.0]
#define kDefaultChatTextFont  [UIFont systemFontOfSize:14.0]

#define APP_SCREEN_WIDTH [UIScreen mainScreen].bounds.size.width
#define APP_SCREEN_HEIGHT [UIScreen mainScreen].bounds.size.height



#define VIDEO_REMOTE_RECT CGRectMake(0, 0, APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT - 44 - 44 - 44)

#define VIDEO_PREVIEW_RECT CGRectMake(APP_SCREEN_WIDTH - 70, APP_SCREEN_HEIGHT - 44 - 44 -90, 60, 80)

#define TOKEN_EXPIRE_TIME 30*60*24  //seconds

@interface DisplayViewController ()<ChannelSessionDelegate>
{
	BOOL _isVideoLinkConnected;
	BOOL _isAudioLinkConnected;
	BOOL _isMicOpened;
	BOOL _isAudioMute;
	
	UIView   *_mainView;
	UIButton *_btnVideoSwitch;
	UIButton *_btnCameraSwitch;
	UIButton *_btnMicSwitch;
	UIButton *_btnAudioMute;
	UITextField *_textField;
	UITableView* _tableView;

	ChannelSession * _channelSession;
	YCVideoView *_cameraPreview;			//摄像头预览view
	YCVideoView *_remoteVideoView;		//远端视频view
	
	NSMutableArray *_chatArrary;
	NSTimer *_updateTokenTimer;
	NSData* binToken;
    
    BOOL _isGotVideoData;
}


@end

@implementation DisplayViewController

@synthesize uid = _uid;
@synthesize sid = _sid;

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
	[UIApplication sharedApplication].idleTimerDisabled = YES;
	
	_isVideoLinkConnected = FALSE;
	_isAudioLinkConnected = FALSE;
	_isMicOpened = FALSE;
	_isAudioMute = FALSE;
	_chatArrary = [[NSMutableArray alloc] init];
	
	//创建一个导航栏
	UINavigationBar *navBar = [[UINavigationBar alloc] initWithFrame:CGRectMake(0, 0, APP_SCREEN_WIDTH, 44)];
	UINavigationItem *navItem = [[UINavigationItem alloc] initWithTitle:@"YCloudLive"];
	[navBar pushNavigationItem:navItem animated:YES];
	
	//创建一个返回按钮
	UIBarButtonItem *btnBack = [[UIBarButtonItem alloc] initWithBarButtonSystemItem: UIBarButtonSystemItemReply target:self action:@selector(backLoginView)];
	[navItem setLeftBarButtonItem:btnBack];
	[self.view addSubview:navBar];
	
	_mainView = [[UIView alloc] initWithFrame:CGRectMake(0, 44, APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT-44)];
	[self.view addSubview:_mainView];
	
    //创建渲染视频流所需的view
    _remoteVideoView = [self createVideoView:VIDEO_REMOTE_RECT];
    [_mainView addSubview:_remoteVideoView];
    
	CGRect mainRect = [_mainView bounds];

	//创建工具条
	_btnVideoSwitch = [UIButton buttonWithType:UIButtonTypeCustom];
	[_btnVideoSwitch setBackgroundImage:[UIImage imageNamed:@"btn_video_off.png"] forState:UIControlStateNormal];
	[_btnVideoSwitch addTarget:self action:@selector(publishVideo) forControlEvents:UIControlEventTouchUpInside];
	_btnVideoSwitch.frame = CGRectMake(0, 0, 40, 40);
	
	_btnCameraSwitch = [UIButton buttonWithType:UIButtonTypeCustom];
	[_btnCameraSwitch setBackgroundImage:[UIImage imageNamed:@"btn_camera_switch.png"] forState:UIControlStateNormal];
	[_btnCameraSwitch addTarget:self action:@selector(switchCamera) forControlEvents:UIControlEventTouchUpInside];
	_btnCameraSwitch.frame = CGRectMake(0, 0, 40, 40);
	
	_btnMicSwitch = [UIButton buttonWithType:UIButtonTypeCustom];
	[_btnMicSwitch setBackgroundImage:[UIImage imageNamed:@"mic_disabled.png"] forState:UIControlStateNormal];
	[_btnMicSwitch addTarget:self action:@selector(switchMic) forControlEvents:UIControlEventTouchUpInside];
	_btnMicSwitch.frame = CGRectMake(0, 0, 40, 40);
	
	_btnAudioMute = [UIButton buttonWithType:UIButtonTypeCustom];
	[_btnAudioMute setBackgroundImage:[UIImage imageNamed:@"btn_audio_on.png"] forState:UIControlStateNormal];
	[_btnAudioMute addTarget:self action:@selector(muteAudio) forControlEvents:UIControlEventTouchUpInside];
	_btnAudioMute.frame = CGRectMake(0, 0, 40, 40);

	UIBarButtonItem *barBtnVideo = [[UIBarButtonItem alloc] initWithCustomView:_btnVideoSwitch];
	UIBarButtonItem *barBtnCamera = [[UIBarButtonItem alloc] initWithCustomView:_btnCameraSwitch];
	UIBarButtonItem *barBtnMic = [[UIBarButtonItem alloc] initWithCustomView:_btnMicSwitch];
	UIBarButtonItem *barBtnAudioMute = [[UIBarButtonItem alloc] initWithCustomView:_btnAudioMute];
	UIBarButtonItem *spacer = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemFlexibleSpace target:nil action:nil];
	NSArray *buttons = [NSArray arrayWithObjects:barBtnVideo, spacer, barBtnCamera, spacer, barBtnMic, spacer, barBtnAudioMute, nil];
	
	UIToolbar *toolbar = [[UIToolbar alloc]initWithFrame:CGRectMake(0, mainRect.size.height - 44 -44, APP_SCREEN_WIDTH, 44)];
	[toolbar setItems:buttons];
	[toolbar setTintColor:[UIColor grayColor]];
    [toolbar setAlpha:0.6];
	[_mainView addSubview:toolbar];
    
	//创建显示消息的tableview
	CGRect tableViewRect = CGRectMake(0, mainRect.size.height - 44 - 44 -  200, APP_SCREEN_WIDTH, 200);
	_tableView = [[UITableView alloc] initWithFrame:tableViewRect style:UITableViewStylePlain];
	_tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
    _tableView.backgroundColor = [UIColor clearColor];
	_tableView.delegate = self;
	_tableView.dataSource = self;
	[_mainView addSubview:_tableView];
	
	//创建消息发送工具条
	UIToolbar *chatToolbar = [[UIToolbar alloc]initWithFrame:CGRectMake(0, mainRect.size.height-44, APP_SCREEN_WIDTH, 44)];
	_textField = [[UITextField alloc] initWithFrame:CGRectMake(0, mainRect.size.height-44+7, mainRect.size.width-32, 30)];
	_textField.returnKeyType = UIReturnKeySend;
	_textField.borderStyle = UITextBorderStyleRoundedRect;
	_textField.placeholder = @"说点什么吧";
	_textField.delegate = self;
	
	UIBarButtonItem *barBtnInput = [[UIBarButtonItem alloc] initWithCustomView:_textField];
	
	NSArray *barItems = [NSArray arrayWithObjects:barBtnInput, nil];
	[chatToolbar setItems:barItems];
	[chatToolbar setTintColor:[UIColor grayColor]];
	[_mainView addSubview:chatToolbar];
	[_mainView setBackgroundColor:[UIColor blackColor]];
	
	//键盘事件
	NSNotificationCenter *notification = [NSNotificationCenter defaultCenter];
	[notification addObserver:self
					 selector:@selector(keyboardWillShow)
						 name:UIKeyboardWillShowNotification
					   object:nil];
	
	[notification addObserver:self
					 selector:@selector(keyboardWillHide)
						 name:UIKeyboardWillHideNotification
					   object:nil];
	
	NSNotificationCenter *center = [NSNotificationCenter defaultCenter];
	[center addObserver:self selector:@selector(onSDKEvent:) name:kSdkEventNotification object:nil];

	//初始化sdk
	[self initMediaSDK];

}

-(void)viewWillAppear:(BOOL)animated{
	[super viewWillAppear: animated];

	NSNumber* nsUid = [[[NSNumberFormatter alloc]init]numberFromString:_uid];
	[self signalLoginAp:nsUid];
}

- (void)viewDidUnload
{
	[UIApplication sharedApplication].idleTimerDisabled = NO;
	[super viewDidUnload];
}

- (void)initMediaSDK
{
	NSLog(@"initMediaSDK sdk version: %@", [YCMediaSDK getSdkVersion]);

    //创建session
	_channelSession = [[YCMediaSDK sharedInstance] createChannelSession];
    //设置session的delegate
	_channelSession.delegate = self;
	
	//参数配置
	NSDictionary* configs = [[NSDictionary alloc] initWithObjectsAndKeys:
							 [NSNumber numberWithInt:LOGIN_MODULE_CHANNEL], [NSNumber numberWithInt:CONFIG_KEY_LOGIN_MODLE],
							 [NSNumber numberWithInt:VIDEO_QUALITY_HIGHT], [NSNumber numberWithInt:CONFIG_KEY_VIDEO_RECORD_QUALITY],
							 [NSNumber numberWithInt:100], [NSNumber numberWithInt:CONFIG_KEY_VIDEO_MIN_BUFFER],
							 [NSNumber numberWithInt:AUDIO_QUALITY_HIGHT], [NSNumber numberWithInt:CONFIG_KEY_AUDIO_RECORD_QUALITY],
							 [NSNumber numberWithInt:100], [NSNumber numberWithInt:CONFIG_KEY_AUDIO_MIN_BUFFER],
							 [NSNumber numberWithInt:350], [NSNumber numberWithInt:CONFIG_KEY_MIN_CODERATE],
							 [NSNumber numberWithInt:800], [NSNumber numberWithInt:CONFIG_KEY_MAX_CODERATE],
							 [NSNumber numberWithInt:700], [NSNumber numberWithInt:CONFIG_KEY_CUR_CODERATE],
							 [NSNumber numberWithInt:1], [NSNumber numberWithInt:CONFIG_KEY_USE_CRCONTROL],
							 nil];
    
	[_channelSession setConfigs:kYCAppId configs:configs];
}


- (YCVideoView*)createVideoView:(CGRect)frame
{
	
	YCVideoView*  videoView = [[YCVideoView alloc] initWithFrame:frame];
	videoView.bounds = frame;
	
	[videoView setScaleMode:VideoScaleModeAspectFit];
	videoView.transform = CGAffineTransformMakeRotation(0);
	return videoView;
}


#pragma mark -
#pragma mark KeyBoard Events

- (void)keyboardWillShow
{
	//当键盘出现时，将整个mainview往上推
	CGRect frame = [_mainView bounds];
	//252  keyboard's height
	frame.origin.y = -252 + 44;
	[UIView beginAnimations:@"showKeyboard" context:nil];
	[UIView setAnimationDuration:0.30];
	[UIView setAnimationDelegate:self];
	
	[_mainView setFrame:frame];
	
	[UIView commitAnimations];
}

- (void)keyboardWillHide
{
	//当键盘隐藏时，恢复正常显示
	CGRect frame = [_mainView bounds];
	frame.origin.y = 44;
	[UIView beginAnimations:@"hideKeyboard" context:nil];
	[UIView setAnimationDuration:0.30];
	[UIView setAnimationDelegate:self];
	
	[_mainView setFrame:frame];
	[UIView commitAnimations];
}

#pragma mark - 
#pragma mark Button touch events
//返回login界面
-(void)backLoginView
{
	[_updateTokenTimer invalidate];
	
	if(_cameraPreview) {
		[_channelSession stopCamera];
		[_channelSession stopPublishVideo];
		[_remoteVideoView removeFromSuperview];
		_remoteVideoView = nil;
	}
	
	if(_remoteVideoView) {
	
		[_channelSession stopSubscibeVideoWithUserGroupId: [_remoteVideoView getUserGroupId] streamId:[_remoteVideoView getStreamId]];
		[_remoteVideoView disconnectFromVideoStream];
		[_channelSession removeVideoView:_remoteVideoView];
	
		[_remoteVideoView removeFromSuperview];
		_remoteVideoView = nil;
	}
	
	[_channelSession logout];
	[self signalLoginout];
	
	[self dismissViewControllerAnimated:YES completion:^{
		NSLog(@"dismissViewController");
	}];
}

//切换摄像头
-(void)switchCamera
{
#if (!TARGET_IPHONE_SIMULATOR)
	[_channelSession switchCamera];
#endif
}

//打开/关闭 麦克风
-(void)switchMic
{
	if(!_isAudioLinkConnected) {
		UIAlertView *alter = [[UIAlertView alloc] initWithTitle:@"" message:@"服务器连接失败,请稍后再试" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil];
		[alter show];
		alter = nil;
		return;
	}
	
#if (!TARGET_IPHONE_SIMULATOR)
	if(!_isMicOpened)
	{
		[_btnMicSwitch setBackgroundImage:[UIImage imageNamed:@"mic_enabled.png"] forState:UIControlStateNormal];
		[_channelSession openMic];
		_isMicOpened = TRUE;
		
	}
	else
	{
		[_btnMicSwitch setBackgroundImage:[UIImage imageNamed:@"mic_disabled.png"] forState:UIControlStateNormal];
		[_channelSession closeMic];
		_isMicOpened = FALSE;
	}
#endif
}

//静音切换
-(void)muteAudio
{
#if (!TARGET_IPHONE_SIMULATOR)
	if(_isAudioMute) {
		[_btnAudioMute setBackgroundImage:[UIImage imageNamed:@"btn_audio_on.png"] forState:UIControlStateNormal];
		[_channelSession muteAudio:FALSE];
		_isAudioMute = FALSE;
	}
	else {
		[_btnAudioMute setBackgroundImage:[UIImage imageNamed:@"btn_audio_off.png"] forState:UIControlStateNormal];
		[_channelSession muteAudio:TRUE];
		_isAudioMute = TRUE;
	}
#endif
	
}

//打开/关闭 本地摄像头和发布
-(void)publishVideo
{
	if(!_isVideoLinkConnected) {
		UIAlertView *alter = [[UIAlertView alloc] initWithTitle:@"" message:@"服务器连接失败,请稍后再试" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil];
		[alter show];
		alter = nil;
		return;
	}
	
#if (!TARGET_IPHONE_SIMULATOR)
	if(_cameraPreview == nil)
	{
		[_btnVideoSwitch setBackgroundImage:[UIImage imageNamed:@"btn_video_on.png"] forState:UIControlStateNormal];
		
		//创建摄像头本地预览view
		_cameraPreview = [self createVideoView:VIDEO_PREVIEW_RECT];
		[self.view addSubview:_cameraPreview];
		[self.view bringSubviewToFront:_cameraPreview];

		//打开摄像头并本地预览
		[_channelSession startCameraWithPreview:_cameraPreview];
		//发布本地视频流
		[_channelSession startPublishVideo];
		//服务器录制
		uint32_t appId = kYCAppId;
		uint32_t sid = (uint32_t)[_sid longLongValue];
		uint32_t uid = (uint32_t)[_uid longLongValue];
		NSString *programId = [NSString stringWithFormat:@"%u_%u_%u_%ld", sid,  appId, uid, time(NULL)];
		NSLog(@"start server record %@", programId);
		[_channelSession startServerRecordWithProgramId:programId];
		
	}
	else
	{
		[_btnVideoSwitch setBackgroundImage:[UIImage imageNamed:@"btn_video_off.png"] forState:UIControlStateNormal];
		//停止服务器录制
		[_channelSession stopServerRecord];
		//停止本地流程发布
		[_channelSession stopPublishVideo];
		//关闭摄像头
		[_channelSession stopCamera];

		//释放摄像头预览view
		[_cameraPreview removeFromSuperview];
		_cameraPreview = nil;
		
	}
#endif
	
}

#pragma mark - 
#pragma mark TextField delegate
- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField
{
	//[self removeCombox];
	return YES;
}
- (BOOL)textFieldShouldEndEditing:(UITextField *)textField
{
	return YES;
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range
replacementString:(NSString *)string
{
	return YES;
}

- (BOOL)textFieldShouldClear:(UITextField *)textField{
	return YES;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
	if(textField.text.length > 0) {
		ChatTextInfo* chatTextInfo = [[ChatTextInfo alloc] init];
		chatTextInfo.uid = [_uid integerValue]; //发送端可以不填
		chatTextInfo.text = textField.text;
		chatTextInfo.height = 0; //使用默认值
		chatTextInfo.color = 0;
		
        //发送文本消息
		[_channelSession sendChatText: chatTextInfo];
        
        //刷新tableView
		[_chatArrary addObject:chatTextInfo];
		[_tableView reloadData];
		
		_textField.text = @"";
	}
	[_textField resignFirstResponder];
	return YES;
}

#pragma mark - Table view delegate

- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
	return 44.0;
}


#pragma mark - Table view data source
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
	// Return the number of sections.
	return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
	// Return the number of rows in the section.
	return [_chatArrary count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{

	UITableViewCell *cell = nil;
	UILabel *nicknameLable = nil;
	UILabel *textLabel = nil;
	
	cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"ChatCell"];

	nicknameLable = [[UILabel alloc] initWithFrame:CGRectMake(5, 0, 320, 16)];
	[nicknameLable setBackgroundColor:[UIColor clearColor]];
    nicknameLable.textColor = [UIColor whiteColor];
	
	textLabel = [[UILabel alloc] initWithFrame:CGRectMake(15, 16, 320, 28)];
	textLabel.font = [UIFont boldSystemFontOfSize:16.0];
	textLabel.backgroundColor = [UIColor redColor];
    textLabel.textColor = [UIColor whiteColor];
    
	[textLabel setBackgroundColor:[UIColor clearColor]];

	ChatTextInfo* chatTextInfo = [_chatArrary objectAtIndex:indexPath.row];
	if(chatTextInfo.uid == (NSUInteger)[_uid integerValue]) {
		nicknameLable.text = [[NSString alloc] initWithFormat:@"%lu(自己):", (unsigned long)chatTextInfo.uid];
	}
	else{
		nicknameLable.text = [[NSString alloc] initWithFormat:@"%lu:", (unsigned long)chatTextInfo.uid];
	}
	textLabel.text = chatTextInfo.text;
    
    cell.backgroundColor = [UIColor clearColor];
	[cell addSubview:nicknameLable];
	[cell addSubview:textLabel];
	
	return cell;
}


#pragma mark - 
#pragma mark ChannelSessionDelegate

- (void)onAudioLinkStatus:(uint32_t)state ip:(uint32_t)ip port:(uint16_t)port
{
	NSLog(@"[callBack] onAudioLinkStatus, state %u, ip %u, port %u", state, ip, port);
	if (state == 1)
	{
		_isAudioLinkConnected = TRUE;
	}
	else
	{
		_isAudioLinkConnected = FALSE;
	}
}

- (void)onAudioStreamStarted:(uint32_t)uid
{
	NSLog(@"[callBack] onAudioStreamStarted %u", uid);
}

- (void)onAudioStreamStoped:(uint32_t)uid
{
	NSLog(@"[callBack] onAudioStreamStoped %u", uid);
}

- (void)onAudioVolumeArrived:(uint32_t)uid volume:(int) volume
{
	
}

- (void)onVideoLinkStatus:(uint32_t)state ip:(uint32_t)ip port:(uint16_t)port
{
	NSLog(@"[callBack] onVideoLinkStatus, state %u, ip %u, port %u", state, ip, port);
	if (state == 1)
	{
		_isVideoLinkConnected = TRUE;
	}
	else
	{
		_isVideoLinkConnected = FALSE;
	}
}

- (void)onVideoStreamArrived:(uint64_t)userGroupId streamId:(uint64_t)streamId publishId:(uint32_t) publishId streamFlag:(uint32_t) streamFlag
{
	NSLog(@"[callBack] onVideoStreamArrived %llu %llu", userGroupId, streamId);
	//当前demo只订阅1路视频
//	if(_remoteVideoView)
//		return;
    if (_isGotVideoData) {
        return;
    }
    
    _isGotVideoData = YES;
    
	
	VideoInfo* videoInfo = [VideoInfo new];
	videoInfo.userGroupId = userGroupId;
	videoInfo.streamId = streamId;
	
//	//创建渲染视频流所需的view
//	_remoteVideoView = [self createVideoView:VIDEO_REMOTE_RECT];
//	[_mainView addSubview:_remoteVideoView];
	
	//绑定视频流和view
	[_remoteVideoView connectToVideoInfo:videoInfo];
	[_channelSession addVideoView:_remoteVideoView];
	
	//订阅视频流
	[_channelSession startSubscibeVideoWithUserGroupId:videoInfo.userGroupId streamId:videoInfo.streamId];
	
	//如果摄像头预览已打开，要将预览view放到最前面，不然会被遮挡了
	if(_cameraPreview) {
		[self.view bringSubviewToFront:_cameraPreview];
	}
}

- (void)onVideoStreamStarted:(uint64_t)userGroupId streamId:(uint64_t)streamId
{
	NSLog(@"[callBack] onVideoStreamStarted %llu %llu",  userGroupId, streamId);
}

- (void)onVideoStreamClosed:(uint64_t)userGroupId streamId:(uint64_t)streamId
{
	NSLog(@"[callBack] onVideoStreamClosed %llu %llu", userGroupId, streamId);
	VideoInfo* videoInfo = [VideoInfo new];
	videoInfo.userGroupId = userGroupId;
	videoInfo.streamId = streamId;
	
	if(!_remoteVideoView || ![videoInfo isEqual:_remoteVideoView.videoInfo])
		return;
	
	[_remoteVideoView disconnectFromVideoStream];
	[_channelSession removeVideoView:_remoteVideoView];
	[_channelSession stopSubscibeVideoWithUserGroupId:videoInfo.userGroupId streamId:videoInfo.streamId];
	[_remoteVideoView removeFromSuperview];
	_remoteVideoView = nil;
}

//响应公屏消息
- (void)onChatText:(ChatTextInfo*) chatTextInfo
{
	NSLog(@"[callBack] onChatText %@", chatTextInfo.text);
	[_chatArrary addObject:chatTextInfo];
	[_tableView reloadData];
}

- (void)onMediaInnerCommand:(uint32_t)command
{
	if(command == 1) { //服务器强制停止本地视频上行, 相当调用了[_channelSession stopPublishVideo]
		if(_cameraPreview) {
			[_btnVideoSwitch setBackgroundImage:[UIImage imageNamed:@"btn_video_off.png"] forState:UIControlStateNormal];
			[_channelSession stopCamera];
			[_cameraPreview removeFromSuperview];
			_cameraPreview = nil;
			UIAlertView *alter = [[UIAlertView alloc] initWithTitle:@"通知" message:@"视频上行已被强制停止" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil];
			
			[alter show];
		}
		
	}
	else if(command == 2) { //服务器强制停止本地音频上行, 相当调用了[_channelSession closeMic]
		if(_isMicOpened) {
			[_btnMicSwitch setBackgroundImage:[UIImage imageNamed:@"mic_disabled.png"] forState:UIControlStateNormal];
			_isMicOpened = FALSE;
			
			UIAlertView *alter = [[UIAlertView alloc] initWithTitle:@"通知" message:@"视频上行已被强制停止" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil];
			[alter show];
		}
	}
}

#pragma mark -
#pragma mark signal operation

-(void)signalLoginAp:(NSNumber*)uid
{
    
    //首先获得Token
	binToken = [self getYCMediaTokenFromHttp:[NSString stringWithFormat:@"%d",kYCAppId] userId:_uid sessionId:_sid];
	if(binToken == nil)
	{
		NSLog(@"token is nil.");
		return;
	}
	
	NSInteger len = [binToken length];
	Byte* bytes = (Byte* )[binToken bytes];
	NSString *hexToken = @"";
	for(NSInteger i = 0; i < len; ++i)
	{
		hexToken = [NSString stringWithFormat:@"%@%02x ",hexToken, bytes[i]];
	}
	
	NSError* error = [[NSError alloc]init];
	NSMutableDictionary* theData = [[NSMutableDictionary alloc] init];
	[theData setObject: [NSNumber numberWithUnsignedInteger:4] forKey:@"reqType"];
	[theData setObject: [NSNumber numberWithUnsignedInteger:3] forKey:@"type"]; //UID LOGIN
	[theData setObject: uid forKey:@"uid"];
	[theData setObject: hexToken forKey:@"ycToken"];
	NSData *jsonData = [NSJSONSerialization dataWithJSONObject:theData
													   options:NSJSONWritingPrettyPrinted
														 error:&error];
	
	[[ProtoSDKMgr Instance]sendRequest:jsonData];
}

-(void)signalLoginout
{
	//信令登出ap
	NSError* error = [[NSError alloc]init];
	NSMutableDictionary* theData = [[NSMutableDictionary alloc] init];
	[theData setObject: [NSNumber numberWithUnsignedInteger:2] forKey:@"reqType"];
	
	
	NSData *jsonData = [NSJSONSerialization dataWithJSONObject:theData
													   options:NSJSONWritingPrettyPrinted
														 error:&error];
	[[ProtoSDKMgr Instance]sendRequest:jsonData];
}

#pragma mark  -----------------------obtain token----------------------------------------
- (NSData*) getYCMediaTokenFromHttp:(NSString*) appId userId:(NSString*)userId sessionId:(NSString*)sessionId{
	
	NSString *urlString = [NSString stringWithFormat:@"http://live.huanjuyun.com/token/%@", appId];
	NSURL *url = [NSURL URLWithString:urlString];
	NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];
	
	[request setHTTPMethod:@"POST"];
	[request setValue:@"application/json" forHTTPHeaderField:@"Content-Type"];
	
	uint32_t expiredTime = (unsigned)time(NULL) + TOKEN_EXPIRE_TIME;
	NSString *expired_time = [NSString stringWithFormat:@"%d",expiredTime];
	NSDictionary *json = @{
						   @"appid" : appId,
						   @"uid" : userId,
						   @"auth" : @"3",
						   @"context": @"",
						   @"tokenType": @"LIVE",
						   @"tokenExpire" : expired_time,
						   @"sid" : sessionId,
						   @"Audio_recv_expire" : expired_time,
						   @"Audio_send_expire" : expired_time,
						   @"Video_recv_expire" : expired_time,
						   @"Video_send_expire" : expired_time,
						   @"Text_recv_expire" : expired_time,
						   @"Text_send_expire" : expired_time,
						   };
	
	NSData *data = [NSJSONSerialization dataWithJSONObject:json options:NSJSONWritingPrettyPrinted error:nil];
	request.HTTPBody = data;
	
	NSString *base64Token = nil;
	NSHTTPURLResponse *response = nil;
	NSError *error = nil;
	NSData *result = [NSURLConnection sendSynchronousRequest:request returningResponse:&response error:&error];
	
	if (response.statusCode == 200) {
		
		NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:result options:NSJSONReadingAllowFragments error:nil];
		base64Token = [dict objectForKey:@"token"];
	}
	else {
		NSLog(@"getToken failed, code: %ld", (long)response.statusCode);
		return nil;
	}
	
	//解码base64的token为字节流
	NSData *decodeToken = [GTMBase64 webSafeDecodeString:base64Token];
	if(decodeToken == nil)
	{
		NSLog(@"decode base64 token failed");
		return nil;
	}
	
	return decodeToken;
}

- (void)updateTokenTimeout:(NSTimer *)theTimer
{
	NSLog(@"updateToken");
	//获取token
	NSData* decodeToken = [self getYCMediaTokenFromHttp:[NSString stringWithFormat:@"%d",kYCAppId] userId:_uid sessionId:@"12345"];
	if(decodeToken == nil)
	{
		NSLog(@"updateToken token invalid");
		return;
	}
	
	[_channelSession updateToken:decodeToken];
}

#pragma mark  -----------------------signal event----------------------------------------
-(void)onSDKEvent:(NSNotification*) notification
{
	NSDictionary* dict = [notification userInfo];
	NSData* evtJson = [[dict objectForKey:kSdkEventNotificationUserInfoKey] dataUsingEncoding: NSUTF8StringEncoding];
	
	NSError *error = [[NSError alloc]init];
	NSMutableDictionary* jsonObject = [[NSMutableDictionary alloc]init];
	jsonObject = [NSJSONSerialization JSONObjectWithData: evtJson
												 options:NSJSONReadingMutableContainers
												   error:&error];
	
	NSLog(@"ViewControll::onSDKEvent eventType=%@ context=%@ ", jsonObject[@"eventType"], jsonObject[@"context"]);
	switch ([jsonObject[@"eventType"] integerValue]) {
		case 3://FORCEOUT
		{
			NSString* result = [[NSString alloc]init];
			result = [result stringByAppendingFormat:@"FORCEOUT!! uReason %@, strReason %@ ",
					  jsonObject[@"uReason"], jsonObject[@"strReason"]];
			
			[self backLoginView];
		}
			break;
		case 1://LOGIN RES
		{
			NSString* result = [[NSString alloc]init];
			result = [result stringByAppendingFormat:@"uid %@ res: %@ udbRes: %@ udbDesc: %@ ",
					  jsonObject[@"uid"], jsonObject[@"res"], jsonObject[@"udbRes"], jsonObject[@"udbDescription"]];
			
			if(200 == [jsonObject[@"res"] intValue])
			{
				uint32_t sidUint = (uint32_t)[_sid longLongValue];
//				NSData* decodeToken = [self getYCMediaTokenFromHttp:[NSString stringWithFormat:@"%d",kYCAppId] userId:_uid sessionId:_sid];
//				if(decodeToken == nil) {
//					NSLog(@"token is nil.");
//					return;
//				}
				
				[_channelSession loginWithAppid:kYCAppId sid:sidUint uid:[_uid intValue] token:binToken];
				
				//需要在token过期前定时更新token
				_updateTokenTimer = [NSTimer scheduledTimerWithTimeInterval:(TOKEN_EXPIRE_TIME-5) target:self selector:@selector(updateTokenTimeout:) userInfo:nil repeats:YES];

			}
			
			
		}
			break;
		default:
			break;
			
	}
}

@end
