//
//  SangforProtocol.h
//  SangforSDK
//  定义了常用的protocol，包括SFAuthResultDelegate(认证)、SFTunnelStatusDelegate(VPN隧道状态)、SFLogoutDelegate(注销)
//
//  Created by SANGFOR on 2019/6/11.
//  Copyright © 2019 SANGFOR. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "SFMobileSecurityTypes.h"
#import "SFMobileSecurityObject.h"

NS_ASSUME_NONNULL_BEGIN

/*!
 @protocol AuthResultDelegate 认证回调接口，用于接收认证结果事件
 */
@protocol SFAuthResultDelegate <NSObject>

/*!
 * @brief 认证成功回调
 * @param msg 认证成功服务端返回的信息
 */
- (void)onAuthSuccess:(BaseMessage *)msg;

/**
 认证前回调
 会阻塞当前线程
 
 @param msg 认证成功前服务端返回的信息
 @return YES: 表示可以认证成功，否则认为是失败
 */
- (BOOL)onAuthSuccessPre:(BaseMessage *)msg;

/*!
 * @brief 认证过程回调
 * nextAuthType为AuthTypeSMS、AuthTypeRadius、AuthTypeRenewPassword
 * 这三个类型的中的一个时，msg才不为空，具体的类参考上面对信息类的定义
 * @param nextAuthType 下个认证类型
 * @param msg 认证需要的信息类
 */
- (void)onAuthProcess:(SFAuthType)nextAuthType message:(BaseMessage *)msg;

/*!
 * @brief 认证失败回调
 * @param msg 错误信息，主要读取类中的errCode和errStr
 */
- (void)onAuthFailed:(BaseMessage *)msg;

@end

/*!
 @protocol SFTunnelStatusDelegate VPN隧道状态回调接口，用于接收VPN隧道变化事件
 */
@protocol SFTunnelStatusDelegate <NSObject>

/*! @brief VPN隧道状态变化回调
 * @param current 当前的VPN隧道状态
 * @param prior 上一次的VPN隧道状态
 */
- (void)onTunnelStatusChanged:(SFTunnelStatus)current prior:(SFTunnelStatus)prior;

/*! @brief VPN隧道网络流量变化回调
 * @param upFlow 上行流量值
 * @param downFlow 下行流量值
 */
- (void)onTunnelFlowRate:(double)upFlow downFlow:(double)downFlow;

@end

/*!
 @protocol SFLogoutDelegate VPN注销
 */
@protocol SFLogoutDelegate <NSObject>

/**
 * 注销回调
 * @param type 注销原因类型
 */
- (void)onLogout:(SFLogoutType)type message:(BaseMessage *)msg;

@end

NS_ASSUME_NONNULL_END
