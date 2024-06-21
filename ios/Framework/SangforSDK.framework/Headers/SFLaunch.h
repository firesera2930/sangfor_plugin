/*********************************************************************
 * Copyright (C), 2021-2022, Sangfor Technologies Inc.
 * File name: SFLaunch.h
 * Version: v1.0.0
 * Date: 2022-3-24
 * Description:  SFLaunch SDK主从相关接口类
********************************************************************/

#import <Foundation/Foundation.h>
#import "SFMobileSecurityTypes.h"
#import "SFSecurityProtocol.h"

NS_ASSUME_NONNULL_BEGIN

@interface SFLaunch : NSObject

/// 启动应用
/// @param bundleId 应用的bundleid
/// @param reason 启动应用原因
/// @param extraData 扩展数据，用于自定义传输数据.
/// @discussion 通过该方法拉起子应用不会调用子应用原生openUrl方法传递参数，
/// 如果将原始URL scheme，即“myscheme://data?username='zhansan'&password='123'“，
/// 传递给extraData，就能正常调用子应用的openUrl方法传递参数。
- (BOOL)launchApp:(NSString *)bundleId
           reason:(SFLaunchReason)reason
        extraData:(nullable NSString *)extraData
API_DEPRECATED_WITH_REPLACEMENT("launchApp:reason:extraData:completeHandler:",
                                ios(2.0, 10.0))
NS_EXTENSION_UNAVAILABLE_IOS("");

/// 异步启动应用
/// @param bundleId 应用的bundleid
/// @param reason 启动应用原因
/// @param extraData 扩展数据，用于自定义传输数据.
/// @discussion 通过该方法拉起子应用不会调用子应用原生openUrl方法传递参数，
/// 如果将原始URL scheme，即“myscheme://data?username='zhansan'&password='123'“，
/// 传递给extraData，就能正常调用子应用的openUrl方法传递参数。
- (void)launchApp:(NSString *)bundleId
           reason:(SFLaunchReason)reason
        extraData:(nullable NSString *)extraData
  completeHandler:(nullable SFLaunchCompleteHandler)completeHandler;

/// 设置App拉起代理对象
/// @param delegate 代理对象
- (void)setAppLaunchDelegate:(nullable id<SFAppLaunchDelegate>)delegate;

/// 根据APP的BundleId判断应用是否授权
/// @param bundleId bundleId
- (BOOL)checkAppAuthorized:(NSString *)bundleId;

/// 设置绑定主应用bundleId
/// 子应用调用
/// @param bundleId
- (void)setMainAppBundleId:(NSString *)bundleId;

@end

NS_ASSUME_NONNULL_END
