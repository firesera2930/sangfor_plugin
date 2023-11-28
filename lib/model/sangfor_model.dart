part of sangfor_plugin;

enum SFAuthStatus {
  authStatusNone(0),
  authStatusLogining(1),
  authStatusPrimaryAuthOK(2),
  authStatusAuthOk(3),
  authStatusLogouting(4),
  authStatusLogouted(5);

  final int intValue;

  const SFAuthStatus(this.intValue);

  static SFAuthStatus valueOf(int value) => values.firstWhere((element) => element.intValue == value);
}

/// sangfor数据结构
class SangForModel {
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
