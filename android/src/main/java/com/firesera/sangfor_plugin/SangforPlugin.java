package com.firesera.sangfor_plugin;

import android.content.Context;
import android.os.Handler;
import android.text.TextUtils;

import androidx.annotation.NonNull;

import com.sangfor.sdk.SFMobileSecuritySDK;
import com.sangfor.sdk.base.SFAuthResultListener;
import com.sangfor.sdk.base.SFAuthStatus;
import com.sangfor.sdk.base.SFAuthType;
import com.sangfor.sdk.base.SFBaseMessage;
import com.sangfor.sdk.base.SFLogoutListener;
import com.sangfor.sdk.base.SFLogoutType;
import com.sangfor.sdk.base.SFTunnelStatusListener;

import java.util.Map;

import io.flutter.embedding.engine.plugins.FlutterPlugin;
import io.flutter.plugin.common.EventChannel;
import io.flutter.plugin.common.MethodCall;
import io.flutter.plugin.common.MethodChannel;
import io.flutter.plugin.common.MethodChannel.MethodCallHandler;
import io.flutter.plugin.common.MethodChannel.Result;

/**
 * SangforPlugin
 */
public class SangforPlugin implements FlutterPlugin, MethodCallHandler, EventChannel.StreamHandler, SFAuthResultListener, SFLogoutListener {
    /// The MethodChannel that will the communication between Flutter and native Android
    ///
    /// This local reference serves to register the plugin with the Flutter Engine and unregister it
    /// when the Flutter Engine is detached from the Activity
    private MethodChannel channel;
    private EventChannel eventChannel;
    private EventChannel.EventSink eventSink;
    // 事件 Handler
    private Handler eventHandler;

    private Context mContext = null;

    private String mVpnAddress = null;          //VPN服务器url地址信息
    private String mUserName = null;            //用户名信息
    private String mUserPassword = null;        //密码信息

    private SFBaseMessage sfBaseMessage = null;
    ///vpn状态
    private int sfAuthStatusCode = 0;

    @Override
    public void onAttachedToEngine(@NonNull FlutterPluginBinding flutterPluginBinding) {
        if (null == mContext) {
            mContext = flutterPluginBinding.getApplicationContext();
            channel = new MethodChannel(flutterPluginBinding.getBinaryMessenger(), "sangfor_plugin");
            channel.setMethodCallHandler(this);
            eventChannel = new EventChannel(flutterPluginBinding.getBinaryMessenger(), "sangfor_plugin_event");
            eventChannel.setStreamHandler(this);
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
            case "authResult":
                result.success(getAuthStatus());
                break;
            default:
                result.notImplemented();
                break;
        }
    }

    @Override
    public void onDetachedFromEngine(@NonNull FlutterPluginBinding binding) {
        channel.setMethodCallHandler(null);
        eventChannel.setStreamHandler(null);
    }

    /**
     * 设置startPasswordAuth
     *
     * @param apiKeyMap
     */
    private void startPasswordAuth(Map apiKeyMap, Result result) {
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
            SFMobileSecuritySDK.getInstance().setAuthResultListener(this);

            SFMobileSecuritySDK.getInstance().startPasswordAuth(mVpnAddress, mUserName, mUserPassword);
            SFMobileSecuritySDK.getInstance().registerLogoutListener(this);
        }
    }

    //推送消息给Event数据流，flutter层负责监听数据流
    private void sendEventToStream() {
        eventHandler = new Handler();
        eventHandler.post(runnable);
    }

    private final Runnable runnable = new Runnable() {
        @Override
        public void run() {
            sfAuthStatusCode = getAuthStatus();
            eventSink.success(sfAuthStatusCode);
            eventHandler.postDelayed(this, 2000);
        }
    };

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
     */
    private void logout() {
        SFMobileSecuritySDK.getInstance().logout();
        System.out.println("注销VPN!");
    }

    private int getAuthStatus() {
        SFAuthStatus sfAuthStatus = SFMobileSecuritySDK.getInstance().getAuthStatus();
        return sfAuthStatus.intValue();
    }


    @Override
    public void onAuthSuccess(SFBaseMessage sfBaseMessage) {
        this.sfBaseMessage = sfBaseMessage;
        eventSink.success("成功");
    }

    @Override
    public void onAuthFailed(SFAuthType sfAuthType, SFBaseMessage sfBaseMessage) {
        this.sfBaseMessage = sfBaseMessage;
        System.out.println("登录失败!");
    }

    @Override
    public void onAuthProgress(SFAuthType sfAuthType, SFBaseMessage sfBaseMessage) {
        this.sfBaseMessage = sfBaseMessage;
        System.out.println("onAuthProgress" + "登录中。。。");
    }

    @Override
    public void onListen(Object arguments, EventChannel.EventSink events) {
        // eventChannel 建立连接
        eventSink = events;
        sendEventToStream();
    }

    @Override
    public void onCancel(Object arguments) {
        eventSink = null;
        eventHandler.removeCallbacks(runnable);
        eventHandler = null;
    }


    @Override
    public void onLogout(SFLogoutType sfLogoutType, SFBaseMessage sfBaseMessage) {
        System.out.println("onLogout-sfLogoutType" + sfLogoutType);
        System.out.println("onLogout-sfBaseMessage" + sfBaseMessage.mErrCode);
    }

}
