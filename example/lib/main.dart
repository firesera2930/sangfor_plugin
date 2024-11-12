import 'dart:io';

import 'package:dio/adapter.dart';
import 'package:dio/dio.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:sangfor_plugin/sangfor_plugin.dart';

void main() {
  WidgetsFlutterBinding.ensureInitialized();
  SystemChannels.lifecycle.setMessageHandler((msg) async {
    debugPrint('SystemChannels> $msg');
    if (msg == 'AppLifecycleState.inactive') {
      SangforPlugin.cancelListen();
    } else if (msg == 'AppLifecycleState.resumed') {
      SangforPlugin.startListen((value) => null);
    }
    // msg是个字符串，是下面的值
    // AppLifecycleState.resumed
    // AppLifecycleState.inactive
    // AppLifecycleState.paused
    // AppLifecycleState.detached
    return msg;
  });
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _code = '0';

  // SangForModel sangForModel = SangForModel(address: 'https://your service IP', userName: 'your account', userPassword: 'your password');

  SangForModel sangForModel = SangForModel(address: 'https://111.75.176.154:4455', userName: 'consumer', userPassword: 'Nfjk@2022');
  String _receivedData = '';
  @override
  void initState() {
    _onReceiveEventData();
    super.initState();
  }

  @override
  void dispose() {
    logout();
    SangforPlugin.cancelListen();
    super.dispose();
  }

  void login() async {
    debugPrint('address:${sangForModel.address}');
    debugPrint('userName:${sangForModel.userName}');
    debugPrint('Password:${sangForModel.userPassword}');
    await SangforPlugin.startPasswordAuth(sangForModel);
  }

  void logout() async {
    // SangforPlugin.cancelCounting();
    SangforPlugin.logout();
  }

  void authResult() async {
    SFAuthStatus? sfAuthStatus = await SangforPlugin.authResult();
    debugPrint(sfAuthStatus.toString());
  }

  /// 监听 eventChannel 数据流
  void _onReceiveEventData() {
    SangforPlugin.startListen(
      (data) {
        print(data);
        setState(() {
          _receivedData = data.toString();
        });
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    print('-------------------------------------------------');
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('VPN测试'),
        ),
        body: ListView(
          children: [
            const SizedBox(
              height: 20,
            ),
            // ControlWidget(
            //   sangForModel: sangForModel,
            //   onChange: (sm){
            //     sangForModel = sm;
            //     setState(() {});
            //   },
            // ),
            Text('状态：$_receivedData'),
            const SizedBox(
              height: 20,
            ),
            loginButton(),
            const SizedBox(
              height: 20,
            ),
            logoutButton(),
            const SizedBox(
              height: 20,
            ),
            authButton(),

            const SizedBox(
              height: 20,
            ),
            apiButton(),
            const SizedBox(
              height: 20,
            ),
            Text(_code)
          ],
        ),
      ),
    );
  }

  /// 登录按键
  Widget loginButton() {
    return Container(
      height: 54,
      width: double.infinity,
      child: TextButton(
        style: TextButton.styleFrom(
          shape: const RoundedRectangleBorder(borderRadius: BorderRadius.all(Radius.circular(8))),
          disabledForegroundColor: Colors.white.withOpacity(0.38),
          backgroundColor: Colors.blue,
        ),
        child: const Text('登录', style: TextStyle(color: Colors.white, fontSize: 16)),
        onPressed: () {
          login();
        },
      ),
    );
  }

  /// 注销按键
  Widget logoutButton() {
    return Container(
      height: 54,
      width: double.infinity,
      child: TextButton(
        style: TextButton.styleFrom(
          shape: const RoundedRectangleBorder(borderRadius: BorderRadius.all(Radius.circular(8))),
          disabledForegroundColor: Colors.white.withOpacity(0.38),
          backgroundColor: Colors.blue,
        ),
        child: const Text('注销', style: TextStyle(color: Colors.white, fontSize: 16)),
        onPressed: () {
          logout();
        },
      ),
    );
  }

  /// 接口
  Widget apiButton() {
    return SizedBox(
      height: 54,
      width: double.infinity,
      child: TextButton(
        style: TextButton.styleFrom(
          shape: const RoundedRectangleBorder(borderRadius: BorderRadius.all(Radius.circular(8))),
          disabledForegroundColor: Colors.white.withOpacity(0.38),
          backgroundColor: Colors.blue,
        ),
        child: const Text('接口', style: TextStyle(color: Colors.white, fontSize: 16)),
        onPressed: () {
          requestPublishInfo();
        },
      ),
    );
  }

  /// 状态
  Widget authButton() {
    return SizedBox(
      height: 54,
      width: double.infinity,
      child: TextButton(
        style: TextButton.styleFrom(
          shape: const RoundedRectangleBorder(borderRadius: BorderRadius.all(Radius.circular(8))),
          disabledForegroundColor: Colors.white.withOpacity(0.38),
          backgroundColor: Colors.blue,
        ),
        child: const Text('登录状态', style: TextStyle(color: Colors.white, fontSize: 16)),
        onPressed: () {
          authResult();
        },
      ),
    );
  }

  /// 获取版本管理信息
  void requestPublishInfo() async {
    getBaiduNews(onSucc: (publish) {
      _code = publish;
      debugPrint('🛰️ 远程构建版本号' + publish);
      setState(() {});
    }, onFail: (msg) {
      // 若更新接口获取失败，容错方案为也可以直接进入APP
      debugPrint(' ❌ 版本信息文件获取失败,直接进入APP ');
    });
  }

  // 获取百热点信息
  static void getBaiduNews({required void Function(String str) onSucc, required void Function(String msg) onFail}) async {
    /// 数据解析
    String ip = '192.168.30.52';

    String path = 'http://' + ip + ':3337';
    // 发起请求
    final dio = initDio();

    try {
      Response response = await dio.put(path + '/ams/login/appPwdLogin', data: {"password": "e10adc3949ba59abbe56e057f20f883e", "phone": "15659776180"});
      if (response.statusCode == 200) {
        var resp = response.data;
        return onSucc(resp.toString());
      } else {
        return onFail('获取新闻失败!');
      }
    } catch (e) {
      return onFail(e.toString());
    }
  }

  /// 创建 DIO 对象
  static Dio initDio([bool? shortTimeOut, int? time]) {
    var timeOutValue = time ?? 15000;
    if (shortTimeOut == true) timeOutValue = 5000;
    var options = BaseOptions(
      connectTimeout: timeOutValue,
      receiveTimeout: timeOutValue,
      validateStatus: (status) {
        return true; // 不使用http状态码判断状态，使用 AdapterInterceptor 来处理（适用于标准REST风格）
      },
    );
    var dio = Dio(options);
    var adapter = dio.httpClientAdapter as DefaultHttpClientAdapter;
    adapter.onHttpClientCreate = (HttpClient client) {
      client.findProxy = (_) => 'DIRECT';
      client.badCertificateCallback = (X509Certificate cert, String host, int port) => true;
    };
    return dio;
  }
}
