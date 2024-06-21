/*********************************************************************
 * Copyright (C), 2021-2022, Sangfor Technologies Inc.
 * File name: SFUemSDK.h
 * Version: v1.0.0
 * Date: 2022-3-24
 * Description:  SFUemSDK SDK的入口头文件，定义了初始化、认证、注销、状态查询等接口类
********************************************************************/

#import <Foundation/Foundation.h>
#import "SFMobileSecurityTypes.h"
#import "SFMobileSecurityObject.h"
#import "SFSecurityProtocol.h"
#import "SFLog.h"
#import "SFAuth.h"
#import "SFConfig.h"
#import "SFLaunch.h"
#import "SFTunnel.h"

NS_ASSUME_NONNULL_BEGIN

@interface SFUemSDK : NSObject
/// SDK版本号
@property (nonatomic, copy, readonly) NSString *sdkVersion;
/// 日志相关接口
@property (nonatomic, strong, readonly) SFLog *log;
/// 认证相关接口
@property (nonatomic, strong, readonly) SFAuth *auth;
/// 配置相关接口
@property (nonatomic, strong, readonly) SFConfig *config;
/// 主从相关接口
@property (nonatomic, strong, readonly) SFLaunch *launch;
/// 隧道相关接口
@property (nonatomic, strong, readonly) SFTunnel *tunnel;

#pragma mark - init
/**
 * @brief 创建SDK服务单例对象
 * @return 单例对象
 */
+ (instancetype)sharedInstance;

/**
 * @brief 初始化SDK服务
 * @param sdkMode SDK模式
 * @param sdkFlags sdk配置选项，SFSDKFlags枚举组合
 * @param extra 额外配置,补充属性
 * @discussion
 * SFSDKFlags在不指定SFSDKFlagsVpnModeTcp和SFSDKFlagsVpnModeL3VPN时，默认为SFSDKFlagsVpnModeTcp
 * extra参数对于iOS当前不生效
 * 多应用场景，即一个主应用，多个子应用，所有应用需要开启AppGroup功能且保证groupid一致，同时设置groupid包括sfshare后缀，例如group.com.***.sfshare
 */
- (BOOL)initSDK:(SFSDKMode)sdkMode
          flags:(SFSDKFlags)sdkFlags
          extra:(NSDictionary<NSNumber *, NSString *> * __nullable)extra;

#pragma mark - auth

/**
 * @brief 设置认证回调
 * @discussion
 * 调用认证相关接口之前一定要先设置此回调
 * 主应用调用，子应用调用会导致断言
 * @param delegate 回调对象,为nil则反注册
 */
- (void)setAuthResultDelegate:(id<SFAuthResultDelegate>)delegate;

/**
 * @brief  用户名密码主认证方式
 * @discussion
 * 此接口调用之前必须已经调用过setAuthResultDelegate并设置了非空的认证回调
 * 主应用调用，子应用调用会导致断言
 * @param url 认证的url信息
 * @param username 用户名信息
 * @param password 用户密码
 */
- (void)startPasswordAuth:(NSURL * __nonnull)url userName:(NSString * __nonnull)username password:(NSString * __nonnull)password;

/**
 * @brief 设置通用https请求的回调
 * @discussion
 * 调用认证相关接口之前一定要先设置此回调
 * 主应用调用，子应用调用会导致断言
 * @param delegate 回调对象,为nil则反注册
 */
- (void)setCommonHttpsRequestResultDelegate:(id<SFCommonHttpsRequestResultDelegate>)delegate;

/**
 * @brief  通用https认证方式 该认证只是做透传
 * @discussion
 * 此接口调用之前必须已经调用过setstartCommonHttpsResultDelegate并设置了非空的认证回调
 * @param url 认证的url信息
 * @param type 认证类型
 * @param value 认证参数
 */
- (void)commonHttpsRequest:(NSURL * __nonnull)url type:(NSString * __nonnull)type value:(NSString * __nonnull)value;

/**
 * @brief  辅助认证方式,异步接口
 * @discussion
 * 此接口调用之前必须已经调用过setAuthResultDelegate并设置了非空的认证回调
 * 主应用调用，子应用调用会导致断言
 * @param type 认证类型
 * @param data 辅助认证数据对象
 * @discussion
 * 图形校验码认证：type用SFAuthTypeRand, data用kAuthKeyRandCode为key
 * 短信验证码认证：type用SFAuthTypeSMS, data用kAuthKeySMS为key
 * 短信验证码主认证：type用SFAuthTypePrimarySMS, data用kAuthKeyPrimarySmsAuthCode为key
 * 动态令牌认证：type用SFAuthTypeToken, data用kAuthKeyToken为key
 * Radius认证：type用SFAuthTypeRadius, data用kAuthKeyRadiusCode为key
 * 更新密码认证：type用SFAuthTypeRenewPassword, data用kAuthKeyRenewNewPassword为key，如果有旧密码带上kAuthKeyRenewOldPassword
 * 用户透传数据在data中用kAuthKeyUserContentData为key
 * 辅助认证多选一在data中用kAuthKeySecondAuthId为key，value为对应辅助认证的authId
 */
- (void)doSecondaryAuth:(SFAuthType)type data:(NSDictionary *__nullable)data;

/**
 * @brief 异步接口，取消vpn登录
 * @discussion
 * 主应用调用，子应用调用会导致断言
 */
- (void)cancelAuth;

/**
 * @brief 同步接口，获取认证状态信息
 * 具体值含义参考SFAuthStatus枚举
 */
- (SFAuthStatus)getAuthStatus;

/**
 * @brief 免密上线
 * @discussion
 * 已登录过且支持免密时，接口返回YES，内部自动免密上线，免密失败会调用注销回调，成功无回调
 * 未登录过或不支持免密时，接口返回NO，需要使用其他主认证方式上线
 * @return YES 免密调用成功 NO 免密调用失败
 */
- (BOOL)startAutoTicket;

#pragma mark - SPA

/**
 * @brief 同步接口，设置SPA配置
 * @discussion
 * @spaConfig SPA配置信息,格式如下
 * 一人一码格式如下  {"loginAddress":"https://10.12.4.236", "spaSecret":"AHE6-kF8U-tm6y"}
 * 共享模式扫描二维码后得到的个数如下{"model":1,"data":"94c1xxx"}
 * @comp 设置spa接口返回值回调
 * 阻塞接口,设置SPA配置,并返回解析结果
 */
- (void)setSPAConfig:(NSString *)spaConfig complete:(SFSetSpaConfigBlock)comp;

/**
 * @brief 判断某个URL的SPA种子是否存在
 * @url url地址. 比如https://10.12.4.236
 * @return YES表示该URL已经存在种子
 */
- (BOOL)isSpaSeedExist:(NSString *)url;

#pragma mark - logout
/**
 * @brief 异步接口，注销VPN
 * @discussion
 * 主应用调用，子应用调用会导致断言
 */
- (void)logout;

/**
 * @brief 注册注销回调
 * @param delegate 回调对象
 */
- (void)registerLogoutDelegate:(id<SFLogoutDelegate>)delegate;

/**
 反注册注销回调
 
 @param delegate 回调对象
 */
- (void)unRegisterLogoutDelegate:(id<SFLogoutDelegate>)delegate;

/**
 * @brief 清空VPN注销回调
 */
- (void)clearLogoutDelegate;

#pragma mark - Utils

/**
 * @brief 获取设备是否已经越狱
 * @return YES表示已经越狱
 */
+ (BOOL)isDeviceRooted;

/**
 * @brief 清除本应用数据，默认会清除应用所有私有数据
 * @discussion
 * 阻塞接口
 * 如果在调用此方法之前调用了initSDK，需要重启应用
 */
+ (void)clearApplicationData;

/**
 * @brief 获取设备是否开启密码
 * @return YES表示已经开启
 */
+ (BOOL)isDevicePasswordOn;

/**
 * 判断当前服务器是否是sdp服务器
 * @return YES 是  NO 不是
 */
+ (BOOL)isSDPServce;

#pragma mark - Notification
- (void)setGenericNotificationDelegate:(id<SFGenericNotificationDelegate>)delegate;

@end

NS_ASSUME_NONNULL_END
