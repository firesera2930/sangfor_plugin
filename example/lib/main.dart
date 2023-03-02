import 'dart:io';

import 'package:dio/adapter.dart';
import 'package:dio/dio.dart';
import 'package:flutter/material.dart';

import 'package:sangfor_plugin/sangfor_plugin.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {

  String _code = '0';

  SangForModel sangForModel = SangForModel(
    address: 'https://183.222.57.218:4455',
    userName: 'admin',
    userPassword: 'Krd@20220107'
  );

  @override
  void initState() {
    super.initState();
  }

  @override
  void dispose(){
    logout();
    super.dispose();
  }

  void login() async {
    await SangforPlugin.startPasswordAuth(sangForModel);
  }


  void logout() async {
    await SangforPlugin.logout();
  }

  void authResult() async {
    String?  string = await SangforPlugin.authResult();
    debugPrint(string);
  }

 

  @override
  Widget build(BuildContext context) {
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
    return  Container(
      height: 54,
      width: double.infinity,
      child: TextButton(
        style: TextButton.styleFrom(
          shape: const RoundedRectangleBorder(
            borderRadius: BorderRadius.all(Radius.circular(8))),
          onSurface: Colors.white,
          backgroundColor: Colors.blue,
        ),
        child: const Text('登录',style: TextStyle(color: Colors.white, fontSize: 16)),
        onPressed: () {
          login();
        },
      ),
    );
  }

   /// 注销按键
  Widget logoutButton() {
    return  Container(
      height: 54,
      width: double.infinity,
      child: TextButton(
        style: TextButton.styleFrom(
          shape: const RoundedRectangleBorder(
            borderRadius: BorderRadius.all(Radius.circular(8))),
          onSurface: Colors.white,
          backgroundColor: Colors.blue,
        ),
        child: const Text('注销',style: TextStyle(color: Colors.white, fontSize: 16)),
        onPressed: () {
          logout();
        },
      ),
    );
  }

   /// 接口
  Widget apiButton() {
    return  Container(
      height: 54,
      width: double.infinity,
      child: TextButton(
        style: TextButton.styleFrom(
          shape: const RoundedRectangleBorder(
            borderRadius: BorderRadius.all(Radius.circular(8))),
          onSurface: Colors.white,
          backgroundColor: Colors.blue,
        ),
        child: const Text('接口',style: TextStyle(color: Colors.white, fontSize: 16)),
        onPressed: () {
          requestPublishInfo();
        },
      ),
    );
  }

   /// 状态
  Widget authButton() {
    return  Container(
      height: 54,
      width: double.infinity,
      child: TextButton(
        style: TextButton.styleFrom(
          shape: const RoundedRectangleBorder(
            borderRadius: BorderRadius.all(Radius.circular(8))),
          onSurface: Colors.white,
          backgroundColor: Colors.blue,
        ),
        child: const Text('登录状态',style: TextStyle(color: Colors.white, fontSize: 16)),
        onPressed: () {
          authResult();
        },
      ),
    );
  }

  /// 获取版本管理信息
  void requestPublishInfo() async {    
    getBaiduNews(
      onSucc: (publish) { 
        _code = publish;
        debugPrint('🛰️ 远程构建版本号' + publish);
        setState(() { });
      }, 
      onFail: (msg) { 
        // 若更新接口获取失败，容错方案为也可以直接进入APP
        debugPrint(' ❌ 版本信息文件获取失败,直接进入APP ');
      });
  }

  // 获取百热点信息
  static void getBaiduNews({required void Function(String str) onSucc, required void Function(String msg) onFail}) async { 

    /// 数据解析
    String ip             = '172.16.31.5';   
    
    String path =  'http://' + ip + ':11010';
    // 发起请求
    final dio = initDio();

    try {
      Response response = await dio.get(
        path,
        options: Options(
          headers: {'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/95.0.4638.69 Safari/537.36'},
          contentType: 'text/html;charset=utf-8'
        ),
      );
      if(response.statusCode == 200){
        var resp = response.data;
        return onSucc(resp.toString());
      }else{
        return onFail('获取新闻失败!');
      }
    } catch (e) {
      return onFail(e.toString());
    }
  }

   /// 创建 DIO 对象
  static Dio initDio([bool? shortTimeOut, int? time]) { 

    var timeOutValue = time ?? 15000;
    if(shortTimeOut == true) timeOutValue = 5000;
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

