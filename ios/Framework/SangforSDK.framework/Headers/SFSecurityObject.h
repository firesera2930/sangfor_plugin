/******************************************************
 * Copyright (C), 2021-2022, Sangfor Technologies Inc.
 * File name: SFSecurityObject.h
 * Author:  hj
 * Version: v1.0.0
 * Date: 2022-2-25
 * Description: SDK定义的对象
*******************************************************/

#import <Foundation/Foundation.h>
#import "SFSecurityTypes.h"

NS_ASSUME_NONNULL_BEGIN

@interface SFServiceInfo : NSObject
@property (nonatomic, copy) NSString *authId;
@property (nonatomic, assign) SFAuthType authType;
@property (nonatomic, copy) NSString *iconType;
@property (nonatomic, copy) NSString *iconPath;
@property (nonatomic, copy) NSString *authName;
@property (nonatomic, copy) NSString *desc;
@property (nonatomic, copy) NSString *subType;

@end

/*! @brief 错误信息的基础类
 *
 */
// cppcheck-suppress syntaxError
@interface SFBaseMessage : NSObject
/*! 错误码 */
@property (nonatomic, assign) NSInteger errCode;
/*! 错误提示字符串 */
@property (nonatomic, copy) NSString *errStr;
/*! 服务端透传字符串 */
@property (nonatomic, copy) NSString *serverInfo;
/*! 增强认证触发条件 */
@property (nonatomic, copy) NSArray *enhanceAuthTips;
/*! 展示的用户名 */
@property (nonatomic, copy) NSString *displayName;
/*! 辅助认证多选一列表 */
@property (nonatomic, copy) NSArray<SFServiceInfo *> *nextServiceList;

@end

/*! @brief 下一次认证类型为VPNAuthTypeSms时，返回的信息
 *
 *  短信认证需要的结构体
 */
@interface SFSmsMessage : SFBaseMessage
/*! 短信认证的手机号码 */
@property (nonatomic, copy) NSString *phoneNum;
/*! 重新发送短信倒计时时间 */
@property (nonatomic, assign) int countDown;
/*! 上次发送的短信验证码是否在有效期 */
@property (nonatomic, assign) BOOL stillValid;
/*! 网关类型,用于判断短信验证码是否发往moa */
@property (nonatomic, copy) NSString *smsApps;
/*! 下发的提示，sdp短信认证存在 */
@property (nonatomic, copy) NSString *tips;
@end

/*! @brief 下一次认证类型为SFAuthTypeRand时，返回的信息
 *
 *  图形验证码要的结构体
 */
@interface SFRandCodeMessage : SFBaseMessage
@property (nonatomic, strong) NSData * randcode;
@end

/*! @brief 下一次认证类型为SFAuthTypeApplyBindAuthDevice SFAuthTypeUnbindAuthDevice时，返回的信息
 *
 *  授信终端要的结构体
 */
// cppcheck-suppress syntaxError
@interface SFDeviceLastApplyInfo : NSObject
/*! 申请id */
@property (nonatomic, assign) BOOL applied;
/*! 上次申请状态 */
@property (nonatomic, assign) SFDeviceApplyStatus status;
/*! 时间 */
@property (nonatomic, copy) NSString *time;
/*! 用户名 */
@property (nonatomic, copy) NSString *userName;
/*! 上次申请理由 */
@property (nonatomic, copy) NSString *reason;
/*! 申请设备名称 */
@property (nonatomic, copy) NSString *deviceName;
/*! 申请设备mac地址 */
@property (nonatomic, copy) NSString *macAddress;
@end

@interface SFTrustDeviceInfo : NSObject
@property (nonatomic, copy) NSString *deviceId;
@property (nonatomic, copy) NSString *deviceName;
@property (nonatomic, copy) NSString *deviceType;
@property (nonatomic, copy) NSString *mac;
@property (nonatomic, copy) NSString *os;
@property (nonatomic, copy) NSString *userDirectoryId;
@property (nonatomic, copy) NSString *userDirectoryName;
@property (nonatomic, copy) NSString *domain;
@property (nonatomic, copy) NSString *bindType;
@property (nonatomic, copy) NSString *bindTime;
@property (nonatomic, copy) NSString *bindUser;
@property (nonatomic, copy) NSString *userId;
@property (nonatomic, copy) NSString *createdAt;
@property (nonatomic, copy) NSString *updatedAt;
@end


@interface SFBindAuthDeviceMessage : SFBaseMessage
/*!首次绑定 */
@property (nonatomic, assign) BOOL firstApply;
/*!绑定状态,详见枚举 */
@property (nonatomic, assign) SFDeviceBindStatus bindStatus;
/*!当前已绑定数 */
@property (nonatomic, assign) NSInteger curBindNum;
/*!设备绑定数限制 */
@property (nonatomic, assign) NSInteger bindNumLimit;
/*!用于显示的提示 */
@property (nonatomic, copy) NSString *tips;
/*!时间 */
@property (nonatomic, copy) NSString *time;
/*!最后一次申请绑定相关信息 */
@property (nonatomic, strong) SFDeviceLastApplyInfo *lastApplyInfo;
/*!绑定的授信终端列表 */
@property (nonatomic, strong) NSArray<SFTrustDeviceInfo *> *trustedDeviceList;
@end

/*! @brief 下一次认证类型为AuthTypeRadius时，返回的信息
 *
 *  挑战认证(Radius认证)需要的结构体
 */
@interface SFRadiusMessage : SFBaseMessage
/*! 挑战认证的提示信息 */
@property (nonatomic, copy) NSString *radiusMsg;
@end

/*! @brief 下一次认证类型为AuthTypeToken时，返回的信息
 *
 *  挑战认证(Token认证)需要的结构体
 */
@interface SFTokenMessage : SFBaseMessage
/*! Token认证的类型，普通token还是totp token */
@property (nonatomic, assign) BOOL totpType;
/*! Totp Token认证是否需要绑定 */
@property (nonatomic, assign) BOOL needBind;
/*! Totp Token认证是否可以重绑 */
@property (nonatomic, assign) BOOL isAllowRebind;
/*! Totp Token认证的绑定信息的user */
@property (nonatomic, copy) NSString *user;
/*! Totp Token认证的绑定信息的period */
@property (nonatomic, copy) NSString *period;
/*! Totp Token认证的绑定信息的digits */
@property (nonatomic, copy) NSString *digits;
/*! Totp Token认证的绑定信息的algorithm */
@property (nonatomic, copy) NSString *algorithm;
/*! Totp Token认证的绑定信息的secret */
@property (nonatomic, copy) NSString *secret;
/*! Totp Token认证的绑定信息的issuer */
@property (nonatomic, copy) NSString *issuer;
@end

/*! @brief 下一次认证类型为VPNAuthTypeForceUpdatePwd时，返回的信息
 *
 *  强制修改密码认证需要的结构体
 */
@interface SFResetPswMessage : SFBaseMessage
/*! 请求修改密码认证的密码规则对外展示信息，一个整体可显示的字符串 */
@property (nonatomic, copy) NSString *resetPswMsg;
/*! 请求修改密码认证的原因 */
@property (nonatomic, copy) NSString *resetPswReason;
/*! 请求修改密码认证的详细密码规则,json格式，key-value形式，key表示规则，value表示开关和配置值 */
@property (nonatomic, copy) NSString *pswJsonStrategy;
/*! 请求修改密码认证的详细密码规则,json格式，key-value形式，key表示规则，value表示规则对应的标题 */
@property (nonatomic, copy) NSString *pswJsonStrategyTitle;

@end

/*! @brief 主从接口回调信息
 *
 *  被拉起应用需要的信息
 */
@interface SFLaunchInfo : NSObject
@property (nonatomic, assign) SFLaunchReason launchReason;
@property (nonatomic, copy) NSString *srcAppName;
@property (nonatomic, copy) NSString *srcAppBundleId;
@property (nonatomic, copy) NSString *srcAppURLScheme;
@property (nonatomic, copy) NSString *extraData;
@property (nonatomic, assign) BOOL debugLogEnable;

@end

/*! @brief 通用的消息订阅信息
 *
 */
@interface SFGenericNotification : NSObject
@property (nonatomic, assign) SFNotificationType notificationType;
@property (nonatomic, copy) NSString *msg; // 提示信息
@property (nonatomic, assign) int code; // 事件详细错误码
@end

NS_ASSUME_NONNULL_END
