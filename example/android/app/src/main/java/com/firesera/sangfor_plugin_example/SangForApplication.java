package com.firesera.sangfor_plugin_example;

import android.content.Context;

import com.sangfor.sdk.SFMobileSecuritySDK;
import com.sangfor.sdk.base.SFSDKFlags;
import com.sangfor.sdk.base.SFSDKMode;

import io.flutter.app.FlutterApplication;

public class SangForApplication extends FlutterApplication {
    
    private static SFSDKMode mSDKMode = SFSDKMode.MODE_VPN;

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
        SFSDKMode sdkMode = SFSDKMode.MODE_VPN;                 //表明启用VPN安全接入功能,详情参考集成指导文档
        int sdkFlags = SFSDKFlags.FLAGS_HOST_APPLICATION;       //表明是单应用(或者是主应用)
        sdkFlags |=SFSDKFlags.FLAGS_VPN_MODE_TCP;               //表明使用VPN功能中的TCP模式
        SFMobileSecuritySDK.getInstance().initSDK(base, sdkMode, sdkFlags, null);
        mSDKMode = sdkMode;
    }

}
