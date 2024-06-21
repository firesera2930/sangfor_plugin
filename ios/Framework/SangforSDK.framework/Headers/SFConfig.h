/*********************************************************************
 * Copyright (C), 2021-2022, Sangfor Technologies Inc.
 * File name: SFConfig.h
 * Version: v1.0.0
 * Date: 2022-3-24
 * Description:  SFConfig SDK配置相关接口类
********************************************************************/

#import <Foundation/Foundation.h>
#import "SFMobileSecurityTypes.h"

NS_ASSUME_NONNULL_BEGIN

@interface SFConfig : NSObject

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
- (BOOL)setOption:(SFSDKOption)key value:(NSString * __nonnull)value;

/*! @brief 根据key获取高级配置选项
 @param key 高级配置选项的key
 @return 高级配置选项的value,没有则返回空
 */
- (NSString * __nullable)getOption:(SFSDKOption)optionKey;


@end

NS_ASSUME_NONNULL_END
