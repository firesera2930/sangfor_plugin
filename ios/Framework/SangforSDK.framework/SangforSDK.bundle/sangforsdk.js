(function () {
    if (window.SangforSDK) {
        return;
    }
    if (!window.onerror) {
        window.onerror = function (msg, url, line) {
            log("ERROR: " + msg + "@" + url + ":" + line);
        }
    }
    console.log("load SangforSDK js...");
    window.SangforSDK = {
//        startPassWordAuth: startPassWordAuth,
//        startTicketAuth: startTicketAuth,
//        logout: logout,
//        registerSFAuthResultListener: registerSFAuthResultListener,
//        registerLogoutListener: registerLogoutListener,
        notifyClientOnline: notifyClientOnline,
        getMobileCommonInfo: getMobileCommonInfo,
        setMobileCommonInfo: setMobileCommonInfo
    };

    /**
     * 开启账号密码认证
     * @param url 链接地址
     * @param userName 用户名
     * @param passWord 密码
     */
    function startPassWordAuth(url, userName, passWord) {
        var data = {url: url, userName: userName, passWord: passWord};
        window.SangforSDKBridge.callHandler("startPassWordAuth", data)
    }

    /**
     * 开启免密认证
     */
    function startTicketAuth() {
        window.SangforSDKBridge.callHandler("startPassWordAuth")
    }

    /**
     * 注销
     */
    function logout() {
        window.SangforSDKBridge.callHandler("logout")
    }

    /**
     * 注册认证监听
     * @param onAuthSuccessHandler
     * @param onAuthFailedHandler
     * @param onAuthProgressHandler
     */
    function registerSFAuthResultListener(onAuthSuccessHandler, onAuthFailedHandler, onAuthProgressHandler) {
        window.SangforSDKBridge.registerHandler("onAuthSuccess", onAuthSuccessHandler);
        window.SangforSDKBridge.registerHandler("onAuthFailed", onAuthFailedHandler);
        window.SangforSDKBridge.registerHandler("onAuthProgressed", onAuthProgressHandler);
    }

    /**
     * 注册注销监听
     * @param onLogoutHandler
     */
    function registerLogoutListener(onLogoutHandler) {
        window.SangforSDKBridge.registerHandler("onLogout", onLogoutHandler)
    }

    /**
     * 页面通知Native层上线
     * @param selectLine 选路地址
     * @param userName 用户名
     * @param guid 设备ID
     * @param sangforid SangforID
     */
    function notifyClientOnline(data) {
        window.SangforSDKBridge.callHandler("notifyClientOnline", data)
    }

    /**
     * 获取移动端应用相关信息
     * @param callBack 数据会通过callBack回调
     * eg: {"appName":"HostAppDemo","lang":"zh-CN","mobileId":"03538472b78840b15b13e83793e63a08","platform":"Android","platformVersion":"10","sdkVersion":"2.2.0.3.1"}
     */
    function getMobileCommonInfo(callBack) {
        return window.SangforSDKBridge.callHandler("getMobileCommonInfo", null, callBack);
    }
    
    /**
     * 设置移动端相关信息
     * @param data AuthServerInfo
     */
    function setMobileCommonInfo(data) {
        window.SangforSDKBridge.callHandler("setMobileCommonInfo", data)
    }

    //发送bridge完成事件
    var sdk = window.SangforSDK;
    var doc = document;
    var readyEvent = doc.createEvent('Events');
    readyEvent.initEvent('SangforSDKReady');
    readyEvent.sdk = sdk;
    doc.dispatchEvent(readyEvent);

})();
