#import "SangforPlugin.h"
#import <SangforSDK/SFUemSDK.h>

@interface SangforPlugin ()<SFAuthResultDelegate,FlutterStreamHandler,SFAuthResultDelegate,SFLogoutDelegate>



@end

static NSString *flutterResult = @"";
static int statusCode = 0;

@implementation SangforPlugin
- (instancetype)init {
    self = [super init];
    if (self) {
        [[SFUemSDK sharedInstance] initSDK:SFSDKModeSupportMutable flags:SFSDKFlagsHostApplication|SFSDKFlagsVpnModeTcp extra:nil];
        [[SFUemSDK sharedInstance] setAuthResultDelegate:self];
    }
    return self;
}
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar; {
    FlutterMethodChannel* channel = [FlutterMethodChannel methodChannelWithName:@"sangfor_plugin" binaryMessenger:[registrar messenger]];
        SangforPlugin* instance = [[SangforPlugin alloc] init];
    [registrar addMethodCallDelegate:instance channel:channel];
    FlutterEventChannel*
    eventChannel = [FlutterEventChannel eventChannelWithName:@"sangfor_plugin_event" binaryMessenger:[registrar messenger]];
    [eventChannel setStreamHandler: instance];
}

- (void)handleMethodCall:(FlutterMethodCall*)call result:(FlutterResult)result {
    
    if ([@"startPasswordAuth" isEqualToString:call.method]){
        [self startPasswordAuth:call];
    } else if ([@"authResult" isEqualToString:call.method]){
    
          [self getAuthStatus];
        result(@(statusCode));
    } else if ([@"logout" isEqualToString:call.method]){
          [self logout];
        result(@(statusCode));
    }
}

- (FlutterError*)onListenWithArguments:(id _Nullable)arguments
                             eventSink:(FlutterEventSink)eventSink {
    self.eventSink = eventSink;
    return nil;
}
- (FlutterError* _Nullable)onCancelWithArguments:(id _Nullable)arguments {
    self.eventSink = nil;
    return nil;
}


/**
 账号密码认证
 */
- (void)startPasswordAuth: (FlutterMethodCall*)call{
    NSString *vpnUrlStr = call.arguments[@"address"];
    NSURL *vpnUrl = [[NSURL alloc] initWithString:vpnUrlStr];
    NSString *username = call.arguments[@"userName"];
    NSString *password = call.arguments[@"userPassword"];
    [[SFUemSDK sharedInstance] startPasswordAuth:vpnUrl userName:username password:password];
}

- (void) getAuthStatus{
    SFAuthStatus sfAuthStatus= [[SFUemSDK sharedInstance] getAuthStatus];
    /** SFAuthStatusNone         = 0,      //未认证
    SFAuthStatusLogining     = 1,      //正在认证
    SFAuthStatusPrimaryAuthOK= 2,      //主认证成功
    SFAuthStatusAuthOk       = 3,      //认证成功
    SFAuthStatusLogouting    = 4,      //正在注销
    SFAuthStatusLogouted     = 5,      //已经注销
     */
    switch (sfAuthStatus) {
        case SFAuthStatusNone:
             statusCode =0;
            break;
        case SFAuthStatusLogining:
             statusCode =1;
            break;
        case SFAuthStatusPrimaryAuthOK:
             statusCode =2;
            break;
        case SFAuthStatusAuthOk:
             statusCode =3;
            break;
        case SFAuthStatusLogouting:
             statusCode =4;
            break;
        case SFAuthStatusLogouted:
             statusCode =5;
            break;
        default:
            statusCode =0;
            break;
    }
  
}

- (void)logout{
    [[SFUemSDK sharedInstance] registerLogoutDelegate:self];
    [[SFUemSDK sharedInstance] logout];
    [self sendEvent];
}

- (void)sendEvent {
  if (self.eventSink) {
   [self getAuthStatus];
    self.eventSink(@(statusCode));
  }
}

#pragma mark - SFAuthResultDelegate
/**
 认证失败
 
 
 @param msg 错误信息
 */
- (void)onAuthFailed:(BaseMessage *)msg
{
    flutterResult = @"失败";
    [self sendEvent];
    NSLog(@"onAuthFailed：%@", flutterResult);
}

/**
 认证过程回调
 
 @param nextAuthType 下个认证类型
 */
- (void)onAuthProcess:(SFAuthType)nextAuthType message:(BaseMessage *)msg
{
    NSLog(@"onAuthProcess：%@", flutterResult);
}

/**
 认证成功
 */
- (void)onAuthSuccess:(BaseMessage *)msg
{
    flutterResult = @"成功";
    [self sendEvent];
    NSLog(@"onAuthSuccess：%@", flutterResult);
}
///**
// 认证前回调
// 会阻塞当前线程
// 
// @param msg 认证成功前服务端返回的信息
// @return YES: 表示可以认证成功，否则认为是失败
// */
- (BOOL)onAuthSuccessPre:(BaseMessage *)msg;{
    return YES;
}

- (void)onLogout:(SFLogoutType)type message:(BaseMessage *)msg{
    [self sendEvent];
    NSLog(@"type: %ld", (long)type);
}

@end
