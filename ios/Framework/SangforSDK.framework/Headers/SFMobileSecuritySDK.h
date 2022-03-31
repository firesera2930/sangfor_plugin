//
//  SFMobileSecuritySDK.h
//  SDK的入口头文件，定义了初始化、认证、注销、状态查询、沙箱等接口
//
//  Created by SANGFOR on 2019/10/22.
//

#import <Foundation/Foundation.h>
#import "SFMobileSecurityTypes.h"
#import "SFMobileSecurityObject.h"
#import "SFMobileSecurityProtocol.h"

NS_ASSUME_NONNULL_BEGIN

#pragma mark - 回调定义

/*! @brief 获取VPN隧道状态的block
 *  @param status VPN隧道状态
 */
typedef void (^ SFTunnelStatusBlock)(SFTunnelStatus status);

/*! @brief 重置密码的block
 * 如果为nil则修改成功;否则修改密码错误,读取NSError的domain
 * @param error 重置密码错误
 */
typedef void (^ SFResetPasswordBlock)(NSError *__nullable error);

/*! @brief 重新获取短信校验码的block
 * 如果为nil则获取短信验证码成功；否则获取短信验证码失败
 * @param message 短信验证码信息
 * @param error 重新获取短信校验码错误
 */
typedef void (^ SFRegetSmsCodeBlock)(SmsMessage *__nullable message, NSError *__nullable error);

/*! @brief 重新获取图形校验码的block
 *
 * @param randcode 短信验证码信息
 * @param error 重新获取图形校验码错误
 */
typedef void (^ SFRegetRandCodeBlock)(NSData *__nullable randcode, NSError *__nullable error);

/*! @brief 选路block
 *
 * @param error 选路结果错误
 */
typedef void (^ SFSelectLineBlock)(NSError *__nullable error);

@interface SFMobileSecuritySDK : NSObject

@property(nonatomic, copy, readonly)    NSString *sdkVersion;   //SFSDK版本号
@property (nonatomic, assign, readonly) SFSDKMode sdkMode;      //SDK模式
@property (nonatomic, assign, readonly) int sdkFlags;           //SDK配置选项

#pragma mark - 初始化

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
          flags:(int)sdkFlags
          extra:(NSDictionary<NSNumber *, NSString *> * __nullable)extra;

/**
 * @brief 设置高级配置选项
 * 配置必须在发起认证前设置才能生效
 * @param key 高级配置选项的key,枚举类型
 * @param value 对应的value
 * @discussion
 * 设置认证连接超时时间 : SFSDKOptionAuthTimeOut,枚举类型
 * {
 *  "timeOut" : 20
 * }
 * 若没有设置则使用默认超时时间30秒
 * 设置语言 : SFSDKOptionLanguage, 枚举类型
 * {
 *  "language" : "zh_CN"
 * }
 * 若没有设置则使用默认zh_CN
 * 设置日志 : SFSDKOptionsLogReport, 枚举类型
 * {
 *  @"crashCollect" : @true, @"crashReport" : @false
 * }
 * @discussion
 * 主应用调用，子应用调用会导致断言
 */
- (BOOL)setSDKOption:(SFSDKOption)key value:(NSString * __nonnull)value;

/*! @brief 根据key获取高级配置选项
 @param key 高级配置选项的key
 @return 高级配置选项的value,没有则返回空
 */
- (NSString * __nullable)getSDKOption:(SFSDKOption)optionKey;

/**
 * @brief 获取内部配置
 * @param key 关键字
 * @return value
 */
- (NSString* __nullable)getInternalConf:(NSString*)key;

/**
 * 清除所有数据
 */
- (void)clearAllData;

#pragma mark - 选路
/**
 * @brief  进行选路（可选接口，直接调用认证也可以）
 * @param url 认证的url信息
 * @param comp 选路回调
 */
- (void)startSelectLine:(NSURL * __nonnull)url comp:(SFSelectLineBlock)comp;

#pragma mark - 认证
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
- (BOOL)startPasswordAuth:(NSURL * __nonnull)url userName:(NSString * __nonnull)username password:(NSString * __nonnull)password;

/**
 * @brief  证书主认证方式
 * @discussion
 * 此接口调用之前必须已经调用过setAuthResultDelegate并设置了非空的认证回调
 * 主应用调用，子应用调用会导致断言
 * @param url 认证的url信息
 * @param path 证书路径
 * @param password 证书密码
 */
- (BOOL)startCertAuth:(NSURL * __nonnull)url certPath:(NSString * __nonnull)path  password:(NSString * __nonnull)password;

/**
 * @brief  钉钉/微信认证方式
 * @discussion
 * 此接口调用之前必须已经调用过setAuthResultDelegate并设置了非空的认证回调
 * 主应用调用，子应用调用会导致断言
 * @param url 认证的url信息
 * @param code 微信/钉钉code
 */
- (BOOL)startDingTalkAuth:(NSURL * __nonnull)url code:(NSString * __nonnull)code;

/**
 * @brief  通用定制认证方式,异步接口
 * @discussion
 * 此接口调用之前必须已经调用过setAuthResultDelegate并设置了非空的认证回调
 * 主应用调用，子应用调用会导致断言
 * @param url 认证的url信息
 * @param path 认证路径
 * @param data 透传数据,JSON格式
 * @return YES:调用认证方法成功
 */
- (BOOL)startPrimaryAuth:(NSURL * __nonnull)url path:(NSString * __nonnull)path data:(NSString * __nullable)data;

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
 * 动态令牌认证：type用SFAuthTypeToken, data用kAuthKeyToken为key
 * Radius认证：type用SFAuthTypeRadius, data用kAuthKeyRadiusCode为key
 * 更新密码认证：type用SFAuthTypeRenewPassword, data用kAuthKeyRenewNewPassword为key，如果有旧密码带上kAuthKeyRenewOldPassword
 * 用户透传数据在data中用kAuthKeyUserContentData为key
 * @return YES:调用认证方法成功
 */
- (BOOL)doSecondaryAuth:(SFAuthType)type data:(NSDictionary *__nullable)data;

/**
 * @brief  是否支持免密认证
 * @discussion
 * 主应用调用，子应用调用会导致断言
 */
- (BOOL)supportTicketAuth;

/**
 * @brief 免密主认证方式
 * @discussion
 * 主应用调用，子应用调用会导致断言
 * 此接口调用之前必须已经调用过setAuthResultDelegate并设置了非空的认证回调
 */
- (void)startTicketAuth;

/**
 * @brief 异步接口，取消vpn登录
 * @discussion
 * 主应用调用，子应用调用会导致断言
 */
- (void)cancelAuth;

#pragma mark - 注销

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

#pragma mark - 状态
/**
 * @brief 同步接口，获取认证状态信息
 * 具体值含义参考SFAuthStatus枚举
 */
- (SFAuthStatus)getAuthStatus;

#pragma mark - 沙箱
/**
 * @brief  设置白名单
 * @param bundleIds 白名单列表数组，设置为空则表示所有应用可访问
 * @discussion
 * 可选择调用且只能主应用调用，否则会导致断言，不设置白名单所有应用可访问VPN
 */
- (void)setWhiteAppList:(NSArray <NSString *> * __nonnull)bundleIds;

/**
 * @brief 获取白名单列表
 * @return 返回白名单列表，没有则返回空
 */
- (NSArray <NSString *> * __nullable)getWhiteApplist;

/**
 * @brief
 * 外部更新全局安全策略, 注意 SDK初始化flags为SDKFlagsSupportManagePolicy模式，外部更新策略才生效
 * @param jsonPolicy 全局策略信息，设置多应用列表信息
 *  {
 *        "appId": "com.sangfor.awork",
 *        "screenShot": {
 *            "enable": 1,
 *            "mode": 0,
 *            "settings": {}
 *        },
 *        "screenShotAudit": {
 *            "enable": 1,
 *            "mode": 0,
 *            "settings": {}
 *        },
 *        "waterMark": {
 *            "enable": 1,
 *            "mode": 0,
 *            "settings": {
 *                "waterMarkGradient": "30",
 *                "waterMarkIntervalY": "20",
 *                "waterMarkFontSize": "10",
 *                "waterMarkBorderOpacity": "70",
 *                "waterMarkIntervalX": "20",
 *                "waterMarkContentOpacity": "70",
 *                "watermarkStatus": 0,
 *                "waterMarkContentColor": "AAAAAA",
 *                "waterMarkBorderColor": "AAAAAA",
 *                "waterMarkBorderSize": "0",
 *                "waterMarkExpireMsg": "试用证书不可商用"
 *            }
 *        },
 *        "networkIsolation": {
 *            "enable": 0,
 *            "mode": 0,
 *            "settings": {
 *                "whiteList": ["com.sangfor.vpn.awork", "com.sangfor.easyconnect"],
 *                "addr": {}
 *            }
 *        },
 *        "backgroudBlur": {
 *            "enable": 1,
 *            "mode": 0,
 *            "settings": {}
 *        },
 *        "shareIsolation": {
 *            "enable": 1,
 *            "mode": 14,
 *            "settings": {
 *                "androidWhiteList" :["",""]
 *                "iosWhiteList" : {"urlSchemes":[],"bundleIds":[]}
 *                "whiteList": {
 *                    "android": [""],
 *                    "ios": {
 *                        "urlScheme": [""],
 *                        "bundleId": [""]
 *                    }
 *                }
 *            }
 *        },
 *        "clipboardIsolation": {
 *            "enable": 1,
 *            "mode": 14,
 *            "setting": {
 *                "allowCopySize": 0
 *            }
 *        }
 *    }
 * @return SFUpdatePolicyCode
 * SFUpdatePolicySuccess            //更新成功
 * SFUpdatePolicyFormatError        //策略格式错误
 * SFUpdatePolicyNotAuth            //未通过认证
 * SFUpdatePolicyInnerError         //内部错误
 */
- (SFUpdatePolicyCode)updatePolicy:(NSString * __nonnull)jsonPolicy;

/**
 * 设置自定义网络隔离白名单，此功能是为兼容老版本vpn设置。
 * 老版本vpn不支持下发网络隔离白名单策略给SDK，因此提供此接口以便用户自定义网络隔离白名单、
 *
 * @param jsonRules 网络隔离白名单配置rule
 * @return 是否设置成功
 *
 * @discussion
 * 白名单配置格式如下：
 *  {
 *   "NetworkWhiteList":[
 *      {"host":"www.baidu.com", "is_resolved":"0"},
 *      {"host":"www.baidu.com:14.215.177.38,14.215.177.39", "is_resolved":"1"},
 *      {"host":"192.168.0.1~192.168.0.224", "is_resolved":"1"},
 *      {"host":"192.168.0.1", "is_resolved":"1"}
 *      ]
 *  }
 *  字段解释：
 *  host：允许访问的地址，
 *        针对域名，若存在指定的解析则在冒号后填写以逗号分隔的ip地址并将is_resolved置为1；
 *        针对ip地址，若为IP地址段，使用波浪号分隔，多个独立ip应设置为多个规则
 *  is_resolved：是否已解析，域名白名单根据实际情况填写，ip白名单默认设置为1
 */
- (BOOL)setNetworkWhitelist:(NSString * __nonnull)jsonRules;

#pragma mark - 日志
/**
 * @brief 设置debug日志开关
 */
+ (void)enableDebugLog:(BOOL)enable;

/**
 * @brief 打包日志到本地任务，会删除14天以前的日志
 * @param zipPath 打包后的文件全路径，传入目录名（非.zip结尾），或者指定文件名(dir + xx.zip)，目录不存在会帮着创建，原始文件存在会先删除
 * @discussion
 * 需要传入完整路径
 * 1. 输入：/var/mobile/Containers/Data/Application/1D488E35-1706-4C58-A357-4893E051A9C6/Library/Caches/log
 * 1. 输出：/var/mobile/Containers/Data/Application/1D488E35-1706-4C58-A357-4893E051A9C6/Library/Caches/log/****.zip
 * 2. 输入：/var/mobile/Containers/Data/Application/1D488E35-1706-4C58-A357-4893E051A9C6/Library/Caches/log.zip
 * 2. 输出：/var/mobile/Containers/Data/Application/1D488E35-1706-4C58-A357-4893E051A9C6/Library/Caches/log.zip
 * @return 打包后的路径，返回空表示打包失败
 */
- (NSString*)packLog:(NSString*)zipPath;

/**
 供双域SDK获取SDK日志路径,因为双域SDK我们没有界面，所以可由宿主应用获取并提交日志
 */
- (NSString*)getSDKLogDir;

#pragma mark - 密码
/**
 * @brief  主动调用修改VPN账号密码
 * @discussion
 * 异步接口，通过ChangePasswordDelegate回调
 * @param oldpwd 旧密码
 * @param newpwd 新密码
 */
- (void)resetPassword:(NSString * __nonnull)oldpwd newPassword:(NSString * __nonnull)newpwd handler:(SFResetPasswordBlock)comp;

#pragma mark - 重新获取验证码

/**
 * @brief 异步请求，重新获取短信验证码
 */
- (void)regetSmsCode:(SFRegetSmsCodeBlock)comp;

/**
 * @brief 同步请求，重新获取图形校验码
 * @discussion
 * 阻塞请求，返回图片信息，用于更新图形校验码
 */
- (void)regetRandCode:(SFRegetRandCodeBlock)comp;

#pragma mark - 工具方法

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
@end

NS_ASSUME_NONNULL_END
