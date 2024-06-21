/*********************************************************************
 * Copyright (C), 2021-2022, Sangfor Technologies Inc.
 * File name: SFLog.h
 * Version: v1.0.0
 * Date: 2022-3-24
 * Description:  SFLog SDK日志相关接口类
********************************************************************/

#import <Foundation/Foundation.h>
#import "SFMobileSecurityTypes.h"
#import "SFSecurityProtocol.h"

NS_ASSUME_NONNULL_BEGIN

@interface SFLog : NSObject

/**
 * @brief 设置日志控制台输出
 * @param enable YES代表输出控制台 NO代表不输出控制台
 */
- (void)setLogConsoleEnable:(BOOL)enable;

/**
 * @brief 设置debug日志开关
 */
- (void)setLogLevel:(SFLogLevel)level;

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
- (NSString *)packLog:(NSString*)zipPath;

/**
 供双域SDK获取SDK日志路径,因为双域SDK我们没有界面，所以可由宿主应用获取并提交日志
 */
- (NSString *)getSDKLogDir;

/**
 * 设置远程获取日志代理对象
 * 不设置此代理，SDK内部会处理远程获取日志相关事件
 * @brief delegate 回调对象
 */
- (void)setUploadLopDelegate:(id<SFUploadLogDelegate>)delegate;

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
- (void)refuseUploadLog:(NSString *)randCode;

@end

NS_ASSUME_NONNULL_END
