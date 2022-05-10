package com.firesera.sangfor_plugin;

import android.content.Context;
import android.text.TextUtils;

import androidx.annotation.NonNull;

import com.sangfor.sdk.SFMobileSecuritySDK;
import com.sangfor.sdk.base.SFAuthResultListener;
import com.sangfor.sdk.base.SFAuthType;
import com.sangfor.sdk.base.SFBaseMessage;

import java.util.Map;

import io.flutter.embedding.engine.plugins.FlutterPlugin;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;

/** SangforPlugin */
public class SangforPlugin implements FlutterPlugin, MethodCallHandler,SFAuthResultListener {
  /// The MethodChannel that will the communication between Flutter and native Android
  ///
  /// This local reference serves to register the plugin with the Flutter Engine and unregister it
  /// when the Flutter Engine is detached from the Activity
  private MethodChannel channel;

  private Context mContext = null;

  private String mVpnAddress = null;          //VPN服务器url地址信息
  private String mUserName = null;            //用户名信息
  private String mUserPassword = null;        //密码信息

  private SFBaseMessage sfBaseMessage = null;
  private String message = null;

  @Override
  public void onAttachedToEngine(@NonNull FlutterPluginBinding flutterPluginBinding) {
    if (null == mContext) {
      mContext = flutterPluginBinding.getApplicationContext();

      channel = new MethodChannel(flutterPluginBinding.getBinaryMessenger(), "sangfor_plugin");
      channel.setMethodCallHandler(this);
    }
  }

  @Override
  public void onMethodCall(@NonNull MethodCall call, @NonNull Result result) {
    String callMethod = call.method;
    switch (callMethod) {
      case "getPlatformVersion":
        result.success("Android " + android.os.Build.VERSION.RELEASE);
        break;
      case "startPasswordAuth":
        startPasswordAuth((Map) call.arguments, result);
        break;
      case "cancelAuth":
        cancelAuth((Map) call.arguments);
        break;
      case "logout":
        logout();
        break;
      case "flutter":
        flutter((Map) call.arguments);
        break;
      case "authResult":
        result.success(this.message);
        break;
      default:
        result.notImplemented();
        break;
    }
  }

  @Override
  public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {
    channel.setMethodCallHandler(null);
  }

  /**
   * 设置startPasswordAuth
   *
   * @param apiKeyMap
   */
  private void startPasswordAuth(Map apiKeyMap,Result result) {
    if (null != apiKeyMap) {
      if (apiKeyMap.containsKey("address") && !TextUtils.isEmpty((String) apiKeyMap.get("address"))) {
        mVpnAddress = (String) apiKeyMap.get("address");
      }
      if (apiKeyMap.containsKey("userName") && !TextUtils.isEmpty((String) apiKeyMap.get("userName"))) {
        mUserName = (String) apiKeyMap.get("userName");
      }
      if (apiKeyMap.containsKey("userPassword") && !TextUtils.isEmpty((String) apiKeyMap.get("userPassword"))) {
        mUserPassword = (String) apiKeyMap.get("userPassword");
      }

      SFMobileSecuritySDK.getInstance().startPasswordAuth(mVpnAddress, mUserName, mUserPassword);

      SFMobileSecuritySDK.getInstance().setAuthResultListener(this);

      System.out.println("登录...");

      result.success("登录中");
    }
  }

  /**
   * 清除授权cancelAuth
   *
   * @param apiKeyMap
   */
  private void cancelAuth(Map apiKeyMap) {
    if (null != apiKeyMap) {
      SFMobileSecuritySDK.getInstance().cancelAuth();
    }
  }

  /**
   * 登出logout
   *
   */
  private void logout() {
    SFMobileSecuritySDK.getInstance().logout();
    System.out.println("注销VPN!");
  }

  /**
   * 测试
   *
   * @param apiKeyMap
   */
  private void flutter(Map apiKeyMap) {
    if (null != apiKeyMap) {
      if (apiKeyMap.containsKey("flutter") && !TextUtils.isEmpty((String) apiKeyMap.get("flutter"))) {
        String result = (String) apiKeyMap.get("flutter");
      }
    }
  }

  @Override
  public void onAuthSuccess(SFBaseMessage sfBaseMessage) {
    this.sfBaseMessage = sfBaseMessage;
    this.message = "成功";
    System.out.println("登录成功");
  }

  @Override
  public void onAuthFailed(SFAuthType sfAuthType, SFBaseMessage sfBaseMessage) {
    this.sfBaseMessage = sfBaseMessage;
    this.message = "失败";
    System.out.println("登录失败!");
  }

  @Override
  public void onAuthProgress(SFAuthType sfAuthType, SFBaseMessage sfBaseMessage) {
    this.sfBaseMessage = sfBaseMessage;

  }
}
