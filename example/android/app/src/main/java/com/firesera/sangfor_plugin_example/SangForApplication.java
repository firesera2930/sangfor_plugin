package com.firesera.sangfor_plugin_example;

import android.content.Context;

import com.sangfor.sdk.SFUemSDK;
import com.sangfor.sdk.base.SFSDKExtras;
import com.sangfor.sdk.base.SFSDKFlags;
import com.sangfor.sdk.base.SFSDKMode;

import java.util.HashMap;
import java.util.Map;

import io.flutter.app.FlutterApplication;

public class SangForApplication extends FlutterApplication {

    private static SFSDKMode mSDKMode = SFSDKMode.MODE_VPN;

    @Override
    protected void attachBaseContext(Context base) {
        initSDK(base);
        super.attachBaseContext(base);

    }

    /**
     * 初始化SangforSDK
     *
     * @param context
     */
    private void initSDK(Context context) {
        Map<SFSDKExtras, String> extra = new HashMap<>();

        int sdkFlags = SFSDKFlags.FLAGS_HOST_APPLICATION;
        sdkFlags |= SFSDKFlags.FLAGS_VPN_MODE_TCP;

        SFUemSDK.getInstance().initSDK(context, SFSDKMode.MODE_SUPPORT_MUTABLE, sdkFlags, null);
    }
}


