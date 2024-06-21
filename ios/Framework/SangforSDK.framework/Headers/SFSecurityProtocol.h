/******************************************************
 * Copyright (C), 2021-2022, Sangfor Technologies Inc.
 * File name: SFSecurityProtocol.h
 * Author:  sangfor
 * Version: v1.0.0
 * Date: 2022-2-25
 * Description: SDK定义的协议
*******************************************************/

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "SFSecurityTypes.h"
#import "SFSecurityObject.h"

NS_ASSUME_NONNULL_BEGIN
// cppcheck-suppress syntaxError
@protocol SFAuthResultDelegate<NSObject>

@required

/**
 * 认证成功回调
 * @param message 认证成功服务端返回的信息
 */
- (void)onAuthSuccess:(SFBaseMessage *)message;

/**
 * 认证过程回调，组合认证时回调下一次认证相关信息
 * nextAuthType为AuthTypeSMS、AuthTypeRadius、AuthTypeRenewPassword
 * 这三个类型的中的一个时，msg才不为空，具体的类参考上面对信息类的定义
 * @param nextAuthType 下个认证类型
 * @param message 认证需要的信息类
 */
- (void)onAuthProcess:(SFAuthType)nextAuthType message:(SFBaseMessage *)message;

/**
 * 认证失败回调
 * @param message 错误信息
 */
- (void)onAuthFailed:(SFBaseMessage *)message;

@end

@protocol SFLogoutDelegate <NSObject>

@required

/**
 * 注销回调
 * @param type 注销类型
 * @param message 错误信息
 */
- (void)onLogout:(SFLogoutType)type message:(SFBaseMessage *)msg;

@end

@protocol SFTunnelStatusDelegate<NSObject>

@required
/**
 * 隧道启动完成回调
 * @param message 错误信息
 */
- (void)onTunnelStarted:(SFBaseMessage *)msg;
/**
 * 隧道关闭完成回调
 */
- (void)onTunnelStoped;
/**
 * 隧道状态改变回调
 * @param status 隧道状态
 */
- (void)onTunnelStatusChanged:(SFTunnelStatus)status;

@end

@protocol SFAppLaunchDelegate<NSObject>

@optional

/// 收到未知拉起原因
- (void)onReceiveUnknowLaunchReason;

@required

/**
 * 应用被拉起回调
 * @param launchInfo 拉起信息
 */
- (void)onAppLaunched:(const SFLaunchInfo *)launchInfo;

@end

@protocol SFUploadLogDelegate <NSObject>

@optional
/**
 * 接收到上传日志事件的回调
 * @param randCode 当前上传任务id
 * @param title 弹框显示标题
 * @param content 弹框显示内容
 * @param packageName 服务端采集日志的目标应用包名(bundleid)
 */
- (void)onUploadLog:(NSString *)randCode
                      title:(NSString *)title
                    content:(NSString *)content
                packageName:(NSString *)packageName;

/**
 * 日志上传开始回调
 * @param randCode 任务id
 */
- (void)onUploadLogStart:(NSString *)randCode;

/**
 * 上传进度回调
 * @param randCode 任务id
 * @param uploadSize 已上传大小
 * @param totalSize 日志文件大小
 */
- (void)onUploadLogProgress:(NSString *)randCode
                 uploadSize:(CGFloat)uploadSize
                  totalSize:(CGFloat)totalSize;

/**
 * 日志上传成功
 * @param randCode 任务id
 */
- (void)onUploadLogSuccess:(NSString *)randCode;

/**
 * 日志上传失败
 * @param randCode 任务id
 * @param code 错误码
 * @param message 错误原因
 */
- (void)onUploadLogFail:(NSString *)randCode
                           code:(NSInteger)code
                        message:(NSString *)message;
@end

@protocol SFCommonHttpsRequestResultDelegate <NSObject>

@required

/**
 * 通用https请求回调
 * @param msg 服务端返回的信息
 */
- (void) onCommonHttpsRequestResult:(SFBaseMessage *) msg;

@end

@protocol SFGenericNotificationDelegate <NSObject>
/**
 * 通用订阅事件回调
 * 
 * @param notifContent 事件通知内容
 */
@required
- (void) onNotify:(SFGenericNotification *) notification;

@end

NS_ASSUME_NONNULL_END
