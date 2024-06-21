/*********************************************************************
 * Copyright (C), 2021-2022, Sangfor Technologies Inc.
 * File name: SFTunnel.h
 * Version: v1.0.0
 * Date: 2023-1-3
 * Description:  SFTunnel SDK隧道相关接口类
********************************************************************/

#import <Foundation/Foundation.h>
#import "SFSecurityProtocol.h"

NS_ASSUME_NONNULL_BEGIN
// cppcheck-suppress syntaxError
@interface SFTunnel : NSObject

/// 启动隧道
- (void)startTunnel;

/// 关闭隧道
- (void)stopTunnel;

/// 获取当前隧道状态
- (SFTunnelStatus)getTunnelStatus;

/// 设置隧道状态代理
/// @param delegate 代理对象
- (void)setTunnelStatusDelegate:(nullable id<SFTunnelStatusDelegate>)delegate;

@end

NS_ASSUME_NONNULL_END
