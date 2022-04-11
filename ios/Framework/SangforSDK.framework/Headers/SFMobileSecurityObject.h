//
//  SFMobileSecurityObject.h
//  comp_ios
//
//  Created by panghongqin on 2019/10/28.
//

#import <Foundation/Foundation.h>
#import "SFMobileSecurityTypes.h"

NS_ASSUME_NONNULL_BEGIN

#pragma mark - BaseMessage
/*! @brief 错误信息的基础类
 *
 */
@interface BaseMessage : NSObject
/*! 错误码 */
@property (nonatomic, assign) SF_ERROR_CODE errCode;
/*! 错误提示字符串 */
@property (nonatomic, retain) NSString *errStr;
/*! 服务端透传字符串 */
@property (nonatomic, retain) NSString *serverInfo;

@end

#pragma mark - SmsMessage
/*! @brief 下一次认证类型为VPNAuthTypeSms时，返回的信息
 *
 *  短信认证需要的结构体
 */
@interface SmsMessage : BaseMessage
/*! 短信认证的手机号码 */
@property (nonatomic, retain) NSString *phoneNum;
/*! 重新发送短信倒计时时间 */
@property (nonatomic, assign) int countDown;
/*! 上次发送的短信验证码是否在有效期 */
@property (nonatomic, assign) BOOL stillValid;
/*! 网关类型,用于判断短信验证码是否发往moa */
@property (nonatomic, retain) NSString *smsApps;
@end

/*! @brief 下一次认证类型为AuthTypeRadius时，返回的信息
 *
 *  挑战认证(Radius认证)需要的结构体
 */
@interface RadiusMessage : BaseMessage
/*! 挑战认证的提示信息 */
@property (nonatomic, retain) NSString *radiusMsg;

@end

/*! @brief 下一次认证类型为AuthTypeToken时，返回的信息
 *
 *  挑战认证(Token认证)需要的结构体
 */
@interface TokenMessage : BaseMessage
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
@interface ResetPswMessage : BaseMessage
/*! 请求修改密码认证的密码规则信息 */
@property (nonatomic, retain) NSString *resetPswMsg;

@end

/*! @brief onAuthSuccessPre时 准入检测规则的Message,根据准入检测规则
 *
 *  准入检测规则所需要的Message
 */
@interface AccessMessage : BaseMessage
/*! root检测规则 */
@property (nonatomic, assign) int rootRule;

@end

NS_ASSUME_NONNULL_END
