library sangfor_plugin;

import 'dart:async';

import 'package:flutter/services.dart';

part 'model/sangfor_model.dart';

class SangforPlugin {
  static const MethodChannel _methodChannel = MethodChannel('sangfor_plugin');
  static const EventChannel _eventChannel = EventChannel("sangfor_plugin_event");

  // 订阅
  static StreamSubscription? _streamSubscription;

  ///登录状态
  static SFAuthStatus sfAuthStatus = SFAuthStatus.authStatusNone;

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
    await _methodChannel.invokeMethod('startPasswordAuth', sangForModel.toJson());
  }

  /// 注销VPN
  static Future<void> logout() async {
    await _methodChannel.invokeMethod('logout') ?? '';
  }

  /// 登录状态
  static Future<SFAuthStatus> authResult() async {
    final statusCode = await _methodChannel.invokeMethod('authResult');
    return SFAuthStatus.valueOf(statusCode);
  }

  ///开始监听
  static void startListen(Function(dynamic value) onEvent) {
    _onListenStreamData(onEvent);
  }

  ///监听 native event 数据流
  static void _onListenStreamData(
    Function(dynamic value) onEvent,
  ) {
    _streamSubscription = _eventChannel.receiveBroadcastStream().listen((dynamic value) {
      sfAuthStatus = SFAuthStatus.valueOf(value);
      onEvent(sfAuthStatus);
    });
  }

  // 取消监听
  static void cancelListen() {
    _streamSubscription?.cancel();
    _streamSubscription = null;
  }

  static void dispose() {
    cancelListen();
  }
}
