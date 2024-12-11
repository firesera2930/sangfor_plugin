/******************************************************
 * Copyright (C), 2021-2022, Sangfor Technologies Inc.
 * File name: SFSecurityTypes.h
 * Author:  hj
 * Version: v1.0.0
 * Date: 2022-2-25
 * Description: SDK定义的常用类型
*******************************************************/

#import <Foundation/Foundation.h>

#if defined(__cplusplus)
    #define SFSDK_EXTERN extern "C"
#else
    #define SFSDK_EXTERN extern
#endif
#define SFSDK_EXPORT SFSDK_EXTERN

SFSDK_EXPORT NSString * const kExtraFileIsolation;
SFSDK_EXPORT NSString * const kExtraHostAppPackageName;
SFSDK_EXPORT NSString * const kExtraAppId;

/// SDK模式
typedef NS_ENUM(NSInteger, SFSDKMode)
{
    SFSDKModeSupportVpn        DEPRECATED_MSG_ATTRIBUTE("Use SFSDKModeSupportMutable")
    = 1<<0,               // SDK启用VPN接入功能
    SFSDKModeSupportSandbox    DEPRECATED_MSG_ATTRIBUTE("Use SFSDKModeSupportMutable")
    = 1<<1,               // SDK启用安全沙箱功能
    SFSDKModeSupportVpnSandbox DEPRECATED_MSG_ATTRIBUTE("Use SFSDKModeSupportMutable")
    = 1<<0 | 1<<1,        // SDK启动VPN和安全沙箱功能
    SFSDKModeSupportMutable
    = 1<<0 | 1<<1 | 1<<2, // SDK启动模式会根据配置变动
};

/// SDK配置选项
typedef NS_ENUM(NSInteger, SFSDKFlags) {
    SFSDKFlagsNone                       = 1<<0,         //初始化值
    SFSDKFlagsVpnModeTcp                 = 1<<1,         //TCP模式
    SFSDKFlagsVpnModeL3VPN               = 1<<2,         //L3VPN模式
    SFSDKFlagsHostApplication            = 1<<3,         //主应用
    SFSDKFlagsSubApplication             = 1<<4,         //子应用模式
    SFSDKFlagsEnableFileIsolation        = 1<<5,         //启用文件隔离
    SFSDKFlagsSupportManagePolicy        = 1<<6,         //SDK支持外部(第三方)更新策略
    SFSDKFlagsVpnServer                  = 1<<16,        //对接vpn服务器
    SFSDKFlagsSdpServer                  = 1<<17,        //对接sdp服务器
    SFSDKFlagsDisableBreakPad            = 1<<18,        //不初始化breakPad
};

/// 认证状态
typedef NS_ENUM(NSInteger, SFAuthStatus) {
    SFAuthStatusNone         = 0,      //未认证
    SFAuthStatusLogining     = 1,      //正在认证
    SFAuthStatusPrimaryAuthOK = 2,      //主认证成功
    SFAuthStatusAuthOk       = 3,      //认证成功
    SFAuthStatusLogouting    = 4,      //正在注销
    SFAuthStatusLogouted     = 5,      //已经注销
};

/// 日志等级
typedef NS_ENUM(NSInteger, SFLogLevel) {
    SFLogLevelDebug   = 1,
    SFLogLevelInfo    = 2,
    SFLogLevelWarn    = 3,
    SFLogLevelError   = 4,
    SFLogLevelFatal   = 5
};

/// 日志模式
typedef NS_ENUM(NSInteger, SFLogMode) {
    SFLogModeNone           = 0,      //不输出日志
    SFLogModeFile           = 1,      //File 默认值
    SFLogModeAll            = 2       //Console and File
};

/// 注销原因类型
typedef NS_ENUM(NSInteger, SFLogoutType) {
    SFLogoutTypeUser                = 0,    //用户注销
    SFLogoutTypeTicketAuthError     = 1,    //免密失败
    SFLogoutTypeServerShutdown      = 2,    //服务端shutdown
    SFLogoutTypeAuthorError         = 3,    //授权失败
    SFLogoutTypeOther               = 100,  //其他注销
};

/// 定义支持的认证类型
typedef NS_ENUM(NSInteger, SFAuthType)
{
    SFAuthTypeCertificate       = 0,         //证书认证
    SFAuthTypePassword          = 1,         //用户名密码认证
    SFAuthTypeSMS               = 2,         //短信认证
    SFAuthTypeSendSMS           = 3,         //发送短信认证
    
    SFAuthTypeRadius            = 6,         //挑战认证或者Radius认证
    SFAuthTypeToken             = 7,         //令牌认证
    
    SFAuthTypeCode              = 11,        //钉钉code认证 无
    
    SFAuthTypeNone              = 17,        //无认证
    SFAuthTypeRenewPassword     = 18,        //强制修改密码认证
    SFAuthTypeRenewPassword2    = 20,        //强制修改密码认证,处理之前没有输入密码的情况。 不支持
    SFAuthTypeRand              = 22,        //图形校验码认证
    SFAuthTypeQyWeChat          = 24,        //sdp企业微信认证
    
    SFAuthTypeTokenTotp         = 25,        //Totp谷歌令牌认证
    SFAuthTypeTokenRadius       = 26,        //Radius动态令牌认证
    SFAuthTypeTokenHttps        = 27,        //HTTP(S)令牌认证
    
    SFAuthTypeAuthCheck                  = 28,        // 认证策略检查-内部认证
    SFAuthTypePureBindAuthDevice         = 29,        // 上线后纯净bindAuthDevice,仅接口调用,无需参数
    SFAuthTypeApplyBindAuthDevice        = 30,        // 上线前申请绑定授信终端认证, 提供申请理由
    SFAuthTypeUnbindAuthDevice           = 31,        // 上线前授信终端解绑认证, 需要先解绑其它终端才能继续认证
    SFAuthTypePureTrustDevice            = 32,        // 上线后纯净trustDevice接口, 仅接口调用,无需参数
    SFAuthTypeApplyTrustDevice           = 33,        // 上线后申请绑定授信终端认证, 提供申请理由
    SFAuthTypeUnbindTrustDevice          = 34,        // 上线后授信终端解绑认证, 需要先解绑其它终端才能继续认证
    
    SFAuthTypeForgetPwd                  = 37,        // 找回密码
    SFAuthTypeResetPwd                   = 38,        // 重置密码
    SFAuthTypeCASPre                     = 39,        // cas认证前置处理
    SFAuthTypeCAS                        = 40,        // cas认证
    SFAuthTypePrimarySMSPre              = 41,        // 短信主认证前置处理
    SFAuthTypePrimarySMS                 = 42,        // 短信主认证

    
    SFAuthTypeExdingTalk               = (1<<10),   //专有钉钉登录

    SFAuthTypeUnknown           = -1,        //未知认证类型
};

typedef NS_ENUM(NSInteger, SFLaunchReason) {
    SFLaunchReasonHostappAuthAuthorization = 0,    //子应用启动主应用登录授权
    SFLaunchReasonHostappApplockAuthorization,     //子应用启动主应用解锁应用锁授权
    SFLaunchReasonSubappAuthBack,                  //主应用登录成功返回子应用
    SFLaunchReasonSubappApplockBack,               //主应用解锁应用锁返回子应用
    SFLaunchReasonSubappActive,                    //主应用主动启动子应用
    SFLaunchReasonSubappAuthRefuseBack,            //主应用拒绝授权sapp返回
    SFLaunchReasonSubappSSORecord,                 //主应用拉起子应用进行单点录制
    SFLaunchReasonSubappSSORecordBack,             //单点登录录制完成拉起aWork
    SFLaunchReasonSubappPushData           = 8,    //主应用向子应用推送数据(仅安卓用)
    SFLaunchReasonSubappCollectLog         = 9,    //主应用拉起子应用收集日志
    SFLaunchReasonHostappUpdateMe          = 10,   //子应用拉起主应用来更新自己
    SFLaunchReasonAppUploadLog             = 11    //atrust拉起SDK应用通过接口上传日志(封装主或封装子)
};

typedef NS_ENUM(NSInteger, SFNotificationType) {
    SFNotificaitonTypeTicketAuthFailed = 0,     // 内部免密认证失败
    SFNotificaitonTypeTicketAuthSuccess,          // 内部免密成功
    SFNotificaitonTypeDisconnectFromProxy,            // 探测到与服务端断开连接
    SFNotificaitonTypeConnectedToProxy,               // 探测到和服务端恢复连接
};

typedef NS_ENUM(NSInteger, SFOnlineType) {
    SFOnlineTypeNone = 0,              //忽略
    SFOnlineTypeInner,                 //内部通过完整数据上线,接收到此事件可认为登录成功
    SFOnlineTypeSession,               //需要通过session上线,接收到此事件需要调用免密认证接口
};

typedef NS_ENUM(NSInteger, SFAclAction) {
    SFAclActionForbidAccess,          // 禁止访问
    SFAclActionLogout,                // 注销账户
    SFAclActionDisableAccount,        // 禁用账户
    SFAclActionBaselineForbid,        // 权限基线禁止访问
};

typedef NS_ENUM(NSInteger, SFAclActionType) {
    SFAclActionTypeEffect,                  // 直接处置
    SFAclActionTypePrevEffect,              // 灰度处置
};

typedef NS_ENUM(NSInteger, SFTunnelStatus) {
    SFTunnelStatus_INIT,
    SFTunnelStatus_ONLINE,
    SFTunnelStatus_OFFLINE,
    SFTunnelStatus_UNKNOWN
};

typedef NS_ENUM(NSInteger, SFDeviceBindStatus) {
    SFDeviceBindStatusSelfServiceBind         = 0,          // 自助绑定流程
    SFDeviceBindStatusNeedRemoveTerminal      = 1,          // 授信终端超限需要删除
    SFDeviceBindStatusApplyInApproval         = 2,          // 授信终端审批中
    SFDeviceBindStatusNeedApply               = 3           // 授信终端需要申请
};


typedef NS_ENUM(NSInteger, SFDeviceApplyStatus) {
    SFDeviceApplyStatusApprovaling     = 1,    // 审批中
    SFDeviceApplyStatusBindSuccess     = 2,    // 绑定成功
    SFDeviceApplyStatusApprovalReject  = 3     // 审批拒绝
};

/*! @brief 拉起回调
 *
 * @param success YES代表拉起成功，反之失败
 */
typedef void (^ SFLaunchCompleteHandler)(BOOL success);
