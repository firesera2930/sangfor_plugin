(function() {
    //防止重复注入
    if (window.SangforSDKBridge) {
        return;
    }

    if (!window.onerror) {
        window.onerror = function(msg, url, line) {
            console.log("SangforSDKBridge: ERROR: " + msg + "@" + url + ":" + line);
        }
    }
    console.log("load SangforSDKBridge js...");
    window.SangforSDKBridge = {
        init: init,
        send: send,
        registerHandler: registerHandler,
        callHandler: callHandler,
        _handleMessageFromNative: _handleMessageFromNative,
        _fetchQueueMessages: _fetchQueueMessages,
    };

    var isIOS = (/(iPhone|iPad|iPod|iOS)/i.test(navigator.userAgent));

    var receiveMessageQueue = [];
    var messageHandlers = {};

    var responseCallbacks = {};
    var uniqueId = 1;
    //iOS发送消息队列
    var sendMessageQueue = [];
    var CUSTOM_PROTOCOL_SCHEME = 'https';
    var QUEUE_HAS_MESSAGE = '__sfbridge_fetch_queue_messages__';

    //设置默认messageHandler
    function init(messageHandler) {
        if (SangforSDKBridge._messageHandler) {
            //不抛出异常
            console.log('SangforSDKBridge: WARNING: init called twice.');
            return;
        }
        SangforSDKBridge._messageHandler = messageHandler;
        //将native发送在队列的消息分发出去
        var receivedMessages = receiveMessageQueue;
        receiveMessageQueue = null;
        for (var i = 0; i < receivedMessages.length; i++) {
            _dispatchMessageFromNative(receivedMessages[i]);
        }
    }

    //使用默认messageHandler发送数据,native需要实现对应send的handler
    function send(data, responseCallback) {
        if (isIOS) {
            _doSend('', data, responseCallback, false);
        } else {
            _doSend('send', data, responseCallback, false);
        }
    }
    //注册handler给native调用
    function registerHandler(handlerName, handler) {
        messageHandlers[handlerName] = handler;
    }
    //调用native注册的handler
    function callHandler(handlerName, data, responseCallback) {
        if (arguments.length == 2 && typeof data == 'function') {
            responseCallback = data;
            data = null;
        }
        _doSend(handlerName, data, responseCallback, false);
    }

    //sendMessage add message, 触发native处理 sendMessage
    function _doSend(handlerName, data, responseCallback, isResponse) {
        var callbackId;
        if (typeof responseCallback === 'string') {
            callbackId = responseCallback;
        } else if (responseCallback) {
            callbackId = 'cb_' + (uniqueId++) + '_' + new Date().getTime();
            responseCallbacks[callbackId] = responseCallback;
        } else {
            callbackId = '';
        }

        if (isIOS) {
            var message;
            if (isResponse) { //响应消息
                message = {
                    handlerName: handlerName,
                    responseData: data,
                    responseId: callbackId
                };
            } else {
                message = {
                    handlerName: handlerName,
                    data: data,
                    callbackId: callbackId
                };
            }
            sendMessageQueue.push(message);
            //触发native事件通知消息队列有新消息
            sendMessageIframe.src = CUSTOM_PROTOCOL_SCHEME + '://' + QUEUE_HAS_MESSAGE;
        } else {
            console.log("SangforSDKBridge will call android function: " + handlerName);
            try {
                var fn = eval('window.sfandroid.' + handlerName);
            } catch(exception) {
                console.log("SangforSDKBridge: ERROR: get handler %o.", exception);
            }
            if (typeof fn === 'function') {
                //存在对应hander时，直接调用native接口并将结果返回给js的callback
                var responseData = fn.call(window.sfandroid, JSON.stringify(data), callbackId);
                if (responseData != null) {
                    console.log('response message: ' + responseData);
                    responseCallback = responseCallbacks[callbackId];
                    if (!responseCallback) {
                        return;
                    }
                    responseCallback(responseData);
                    delete responseCallbacks[callbackId];
                }
            } else { //调用默认的handler
                window.sfandroid.send(handlerName, JSON.stringify(data), callbackId)
            }
        }
    }

    //提供给native使用，分发native消息
    function _dispatchMessageFromNative(messageJSON) {
        setTimeout(function() {
            var message = JSON.parse(messageJSON);
            var responseCallback;

            //响应消息，回调js对应callback
            if (message.responseId) {
                responseCallback = responseCallbacks[message.responseId];
                if (!responseCallback) {
                    return;
                }
                responseCallback(message.responseData);
                delete responseCallbacks[message.responseId];
            } else {
                //携带callback的调用消息，将callback
                if (message.callbackId) {
                    var callbackResponseId = message.callbackId;
                    responseCallback = function(responseData) {
                        if (isIOS) {
                            _doSend(message.handlerName, responseData, callbackResponseId, true);
                        } else {
                            _doSend('response', responseData, callbackResponseId, true);
                        }
                    };
                }
                //默认消息handler
                var handler = SangforSDKBridge._messageHandler;
                if (message.handlerName) {
                    //查找指定handler
                    handler = messageHandlers[message.handlerName];
                }
                try {
                    handler(message.data, responseCallback);
                } catch(exception) {
                    console.log("SangforSDKBridge: ERROR: javascript handler %o.", exception);
                }
            }
        });
    }

    //提供给native调用receiveMessageQueue会在页面加载完后清空
    function _handleMessageFromNative(messageJSON) {

        console.log('handle message: ' + messageJSON);
        if (receiveMessageQueue) {
            receiveMessageQueue.push(messageJSON);
        }
        _dispatchMessageFromNative(messageJSON);

    }
    //iOS提供给native调用,获取js发送的消息列表
    function _fetchQueueMessages() {
        var messageQueueString = JSON.stringify(sendMessageQueue);
        sendMessageQueue = [];
        return messageQueueString;
    }
    //创建iOS通信的iframe
    if (isIOS) {
        sendMessageIframe = document.createElement('iframe');
        sendMessageIframe.style.display = 'none';
        sendMessageIframe.src = CUSTOM_PROTOCOL_SCHEME + '://' + QUEUE_HAS_MESSAGE;
        document.documentElement.appendChild(sendMessageIframe);
    }

    //发送bridge完成事件
    var bridge = window.SangforSDKBridge;
    var doc = document;
    var readyEvent = doc.createEvent('Events');
    readyEvent.initEvent('SangforSDKBridgeReady');
    readyEvent.bridge = bridge;
    doc.dispatchEvent(readyEvent);
})();
