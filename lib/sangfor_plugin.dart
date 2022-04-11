library sangfor_plugin;

import 'dart:async';

import 'package:flutter/services.dart';

part 'model/sangfor_model.dart';

class SangforPlugin {
  static const MethodChannel _methodChannel = MethodChannel('sangfor_plugin');

  
  /// 输入账号密码
  /// 
  /// [SangForModel]
  /// 
  /// VPN服务器url地址信息
  /// [String]? address;
  /// 
  /// 用户名信息
  /// [String]? userName;
  /// 
  /// 密码信息
  /// [String]? userPassword;
  static Future<void> startPasswordAuth(SangForModel sangForModel) async {
    await _methodChannel.invokeMethod('startPasswordAuth',sangForModel.toJson());
  }

  /// 注销VPN
  static Future<String?> logout() async {
    final String? resultStr = await _methodChannel.invokeMethod('logout');
    return resultStr;
  }

  /// 登录状态
  static Future<String?> authResult() async {
    final String? resultStr = await _methodChannel.invokeMethod('authResult') ?? '失败';
    return resultStr;
  }
}

