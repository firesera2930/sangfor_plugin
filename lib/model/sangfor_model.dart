part of sangfor_plugin;

/// sangfor数据结构
class SangForModel{

  /// VPN服务器url地址信息
  String? address;

  /// 用户名信息
  String? userName;

  /// 密码信息
  String? userPassword;

  SangForModel({this.address, this.userName, this.userPassword});

  Map<String, dynamic> toJson() {
    Map<String, dynamic> data = {};
    data['address'] = address;
    data['userName'] = userName;
    data['userPassword'] = userPassword;
    return data;
  }
}