#import "SangforPlugin.h"
#import <SangforSDK/SFMobileSecuritySDK.h>

@interface SangforPlugin ()<SFAuthResultDelegate>



@end

static NSString *flutterResult = @"";

@implementation SangforPlugin
+ (void)registerWithRegistrar:(NSObject<FlutterPluginRegistrar>*)registrar; {
    [[SFMobileSecuritySDK sharedInstance] initSDK:SFSDKModeSupportVpn flags:SFSDKFlagsHostApplication|SFSDKFlagsVpnModeTcp extra:nil];
    FlutterMethodChannel* channel = [FlutterMethodChannel methodChannelWithName:@"sangfor_plugin" binaryMessenger:[registrar messenger]];
    
    SangforPlugin* instance = [[SangforPlugin alloc] init];
    [registrar addMethodCallDelegate:instance channel:channel];
}

- (void)handleMethodCall:(FlutterMethodCall*)call result:(FlutterResult)result {
    [[SFMobileSecuritySDK sharedInstance] setAuthResultDelegate:self];
   
    if ([@"startPasswordAuth" isEqualToString:call.method]){
        [self startPasswordAuth:call result:result];
    } else if ([@"authResult" isEqualToString:call.method]){
        NSLog(@"result  %@", flutterResult);
        result(flutterResult);
    }
}




/**
 账号密码认证
 */
- (void)startPasswordAuth: (FlutterMethodCall*)call result:(FlutterResult)result{
    
    NSString *vpnUrlStr = call.arguments[@"address"];
    NSURL *vpnUrl = [[NSURL alloc] initWithString:vpnUrlStr];
    NSString *username = call.arguments[@"userName"];
    NSString *password = call.arguments[@"userPassword"];
    
    [[SFMobileSecuritySDK sharedInstance] startPasswordAuth:vpnUrl userName:username password:password];
}



#pragma mark - SFAuthResultDelegate
/**
 认证失败
 
 @param msg 错误信息
 */
- (void)onAuthFailed:(BaseMessage *)msg
{
    flutterResult = @"失败";
    NSLog(@"%@", flutterResult);
}

/**
 认证过程回调
 
 @param nextAuthType 下个认证类型
 */
- (void)onAuthProcess:(SFAuthType)nextAuthType message:(BaseMessage *)msg
{
   
}

/**
 认证成功
 */
- (void)onAuthSuccess:(BaseMessage *)msg
{
    flutterResult = @"成功";
    NSLog(@"%@", flutterResult);
}

@end
