//
//  SFMobileSecuritySDK.h
//  SDK的入口头文件，定义了初始化、认证、注销、状态查询、沙箱等接口
//
//  Created by SANGFOR on 2019/10/22.
//

#import <Foundation/Foundation.h>
#import "SFMobileSecurityTypes.h"
#import "SFMobileSecurityObject.h"
#import "SFSecurityProtocol.h"

NS_ASSUME_NONNULL_BEGIN

@interface SFMobileSecuritySDK : NSObject

@property (nonatomic, copy, readonly) NSString *sdkVersion;     //SFSDK版本号
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
 * @brief 设置通用https认证回调
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
 * @param password 认证参数
 */
- (BOOL)startCommonHttpsAuth:(NSURL * __nonnull)url type:(NSString * __nonnull)type value:(NSString * __nonnull)value;

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
 * @brief 钉钉/企业微信认证方式
 * @discussion
 * 此接口调用之前必须已经调用过setAuthResultDelegate并设置了非空的认证回调
 * 主应用调用，子应用调用会导致断言
 * @param url 认证的url信息
 * @param info 钉钉/企业微信认证信息，必须包含如下key
 * kAuthKeyAuthInfo 透传的认证信息
 * kAuthKeyAuthType 认证类型为kAuthValueDingTalk / kAuthValueQyWechat / kAuthValueZwWechat
 * 参考 SFMobileSecurityTypes.h
 */
- (BOOL)startThirdAuth:(NSURL * __nonnull)url authInfo:(NSDictionary *__nullable)info;

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
 * 辅助认证多选一在data中用kAuthKeySecondAuthId为key，value为对应辅助认证的authId
 * @return YES:调用认证方法成功
 */
- (BOOL)doSecondaryAuth:(SFAuthType)type data:(NSDictionary *__nullable)data;

/**
 * @brief 免密上线
 * @discussion
 * 已登录过且支持免密时，接口返回YES，内部自动免密上线，免密失败会调用注销回调，成功无回调
 * 未登录过或不支持免密时，接口返回NO，需要使用其他主认证方式上线
 * @return YES 免密调用成功 NO 免密调用失败
 */
- (BOOL)startAutoTicket;

/**
 * @brief 异步接口，取消vpn登录
 * @discussion
 * 主应用调用，子应用调用会导致断言
 */
- (void)cancelAuth;

#pragma mark - SFCommonHttpsResultDelegate
/**
 * @brief 通用https接口，透传第三方认证结果回调
 * @discussion
 * 主应用调用，子应用调用会导致断言
 */
- (void)onRequestResult:(SFBaseMessage *) msg;

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

/**
 * @brief 设置日志级别
 */
- (void)setLogLevel:(SFLogLevel)level;

/**
 * @brief 设置日志控制台输出
 * @param enable YES代表输出控制台 NO代表不输出控制台
 */
- (void)setLogConsoleEnable:(BOOL)enable;

#pragma mark - 密码
/**
 * @brief  主动调用修改VPN账号密码
 * @discussion
 * 异步接口，通过ChangePasswordDelegate回调
 * @param oldpwd 旧密码
 * @param newpwd 新密码
 */
- (void)resetPassword:(NSString * __nonnull)oldpwd newPassword:(NSString * __nonnull)newpwd handler:(SFResetPasswordBlock)comp;

/**
 * 获取修改密码的规则
 * @param comp 结果回调
 */
- (void)getPswStrategy:(SFGetPswStrategyBlock)comp;

/**
 * 判断当前用户是否支持修改密码
 * @return true 支持  false 不支持
 */
- (BOOL)allowResetPassword;

#pragma mark - 重新获取验证码

/**
 * @brief 异步请求，重新获取短信验证码
 */
- (void)regetSmsCode:(SFRegetSmsCodeBlock)comp;

/**
 * @brief 异步请求，重新获取短信验证码
 * @param authId 辅助认证多选一由于有多个网关的场景，需要传对应的authId
 */
- (void)regetSmsWithAuthId:(NSString *)authId Code:(SFRegetSmsCodeBlock)comp;

/**
 * @brief 同步请求，重新获取图形校验码
 * @discussion
 * 阻塞请求，返回图片信息，用于更新图形校验码
 */
- (void)regetRandCode:(SFRegetRandCodeBlock)comp;

#pragma mark - SPA

/**
 * @brief 同步接口，设置SPA配置
 * @discussion
 * @spaConfig SPA配置信息
 * @comp 设置spa接口返回值回调
 * 阻塞接口,设置SPA配置,并返回解析结果
 */
- (void)setSPAConfig:(NSString *)spaConfig complete:(SFSetSpaConfigBlock)comp;

/**
 * @brief 判断某个URL的SPA种子是否存在
 * @url url地址
 * @return YES表示已经该URL存在种子
 */
- (BOOL)isSpaSeedExist:(NSString *)url;

#pragma mark - 隧道
/// 启动插件
- (void)startTunnel;

/// 停止插件
- (void)stopTunnel;

/// 获取当前隧道状态
- (SFTunnelStatus)getTunnelStatus;

/// 设置隧道状态代理
/// @param delegate 代理对象
- (void)setTunnelStatusDelegate:(nullable id<SFTunnelStatusDelegate>)delegate;

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

/**
 * 判断当前服务器是否是sdp服务器
 * @return YES 是  NO 不是
 */
- (BOOL)isSDPServce;

#pragma mark --远程获取日志相关接口
/**
 * 自定义实现日志上传
 * @brief delegate 回调对象
 */
- (void)registerUploadLopDelegate:(id<SFUploadLogDelegate>)delegate;

/**
 * 当前任务是否已被处理
 * @param randCode 日志上传任务对应的id
 */
- (BOOL)needProcess:(NSString *)randCode;

/**
 * 上传日志接口
 * @param randCode 日志上传任务对应的id
 */
- (void)uploadLog:(NSString *)randCode;

/**
 * 拒绝上传
 * @param randCode 日志上传任务对应的id
 */
- (void)reUploadLog:(NSString *)randCode;

/**
 * 上传失败，重新上传
 * @param randCode 日志上传任务对应的id
 */
- (void)refuseUploadLog:(NSString *)randCode;

#pragma mark --通用事件传递接口
- (void)setGenericNotificationDelegate:(id<SFGenericNotificationDelegate>)delegate;

@end

NS_ASSUME_NONNULL_END
