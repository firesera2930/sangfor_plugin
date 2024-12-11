 (function() {

   if (window.SFWebSocketProxy) {
     console.error("already load SFWebSocketProxy");
     return;
   }

   console.log("load SFWebSocketProxy");
   if (!window.WebSocket) {
     throw Error('no WebSocket avaibale');
   }

   /**
    * SFWebSocket
    * 实现WebSocket API
    */

   // 参考：https://developer.mozilla.org/en-US/docs/Web/API/WebSocket
   class SFWebSocket
   /*extends EventTarget*/
   {
     // constants
     static get CONNECTING() {
       return 0;
     }
     static get OPEN() {
       return 1;
     }
     static get CLOSING() {
       return 2;
     }
     static get CLOSED() {
       return 3;
     }

     constructor(url, protocols) {
       // WebSocket
       this.url = url;
       this.binaryType = 'blob';
       this.bufferedAmount = 0;
       this.protocol = '';
       this.extensions = '';
       this.readyState = SFWebSocket.CONNECTING;

       // custom
       this._uniqueId = '';
       this.listeners = {};
       this._open(url, protocols);
     }

     get CONNECTING() {
       return SFWebSocket.CONNECTING;
     }
     get OPEN() {
       return SFWebSocket.OPEN;
     }
     get CLOSING() {
       return SFWebSocket.CLOSING;
     }
     get CLOSED() {
       return SFWebSocket.CLOSED;
     }

     _open(url, protocols) {
       //TODO 校验合法性
       if (typeof protocols == 'string') {
         protocols = [protocols];
       }
       this.readyState == SFWebSocket.CONNECTING;
       this._uniqueId = window.SFWebSocketProxy.onCallOpen(this, url, protocols, window.origin);
     }

     send(data) {
        //TODO 校验合法性
        //if (this.readyState == SFWebSocket.CONNECTING) throw;
        var that = this;
        var sendBlock = function (type, data, length) {
          that.bufferedAmount += length;
          if (that.readyState != SFWebSocket.OPEN) {
            return;
          }
          window.SFWebSocketProxy.onCallSend(that._uniqueId, type, data, length);
        };

        if (typeof data == "string") {
          sendBlock("string", data, data.length);
        } else if (typeof data == "object") {
          // Unit8Array需要先获其buffer才可以转换成Blob
          if (data instanceof Uint8Array) data = data.buffer;
          if (data instanceof ArrayBuffer) data = new Blob([data]);
          if (data instanceof Blob) {
            const reader = new FileReader();
            reader.onload = function () {
              //base64字符串
              sendBlock("binary", reader.result.split(",")[1], data.size);
            };
            reader.readAsDataURL(data);
          } else {
            console.log("[Websocket] send error, because format not supported");
          }
        }
      }
      
     close(code, reason) {
       //TODO 校验合法性
       if (this.readyState == SFWebSocket.CLOSING || this.readyState == SFWebSocket.CLOSED) return;
       this.readyState == SFWebSocket.CLOSING;
       window.SFWebSocketProxy.onCallClose(this._uniqueId, code, reason);
     }
   }

   //EventTarget
   //参考：https://developer.mozilla.org/zh-CN/docs/Web/API/EventTarget
   SFWebSocket.prototype.listeners = null;
   SFWebSocket.prototype.onopen = null;
   SFWebSocket.prototype.onmessage = null;
   SFWebSocket.prototype.onclose = null;
   SFWebSocket.prototype.onerror = null;

   SFWebSocket.prototype.addEventListener = function(type, callback) {
     if (! (type in this.listeners)) {
       this.listeners[type] = [];
     }
     this.listeners[type].push(callback);
   };

   SFWebSocket.prototype.removeEventListener = function(type, callback) {
     if (! (type in this.listeners)) {
       return;
     }
     var stack = this.listeners[type];
     for (var i = 0,
     l = stack.length; i < l; i++) {
       if (stack[i] === callback) {
         stack.splice(i, 1);
         return this.removeEventListener(type, callback);
       }
     }
   };

   SFWebSocket.prototype.dispatchEvent = function(event) {
     if (! (event.type in this.listeners)) {
       return;
     }
     var stack = this.listeners[event.type];
     // target是只读属性
     Object.defineProperty(event, 'target', {
       value: this,
       writable: false
     });
     for (var i = 0,
     l = stack.length; i < l; i++) {
       stack[i].call(this, event);
     }
   };

   /**
    * SFWebSocketProxy
    * 负责管理SFWebSocket 对接Native Proxy
    */
   window.SFWebSocketProxy = {
     dispatchNativeEvents: dispatchNativeEvents,
     onCallOpen: onCallOpen,
     onCallSend: onCallSend,
     onCallClose: onCallClose
   };

   // uniqueId -> SFWebSocket
   var webSockets = {};
   const Promptkey = "WebSocketProxy";

   function onCallOpen(websocket, url, protocols, origin) {
     const uniqueId = Math.round(Math.random() * 10000000) + ""
     var event = {
       title: "open",
       content: {
         url: url,
         protocols: protocols,
         origin: origin,
         uniqueId
       }
     };
     _sendEvent(event);
     if (uniqueId) {
       _addWebSocket(uniqueId, websocket);
     }
     return uniqueId;
   }

   function onCallSend(uniqueId, type, data, length) {
     var event = {
       title: "send",
       content: {
         uniqueId: uniqueId,
         type: type,
         data: data,
         //length : length
       }
     };
     _sendEvent(event);
   }

   function onCallClose(uniqueId, code, reason) {
     var event = {
       title: "close",
       content: {
         uniqueId: uniqueId,
         code: code ? code: 1005,
         //未传递关闭码时默认使用1005
         reason: reason ? reason: ""
       }
     };
     _sendEvent(event);
   }

   window.addEventListener("message",
   function(event) {
     var data = event.data;
     if (data && typeof data == 'object' && data.flag == Promptkey) {
       dispatchNativeEvents(data);
     }
   });

   function dispatchNativeEvents(event) {

     console.log('dispatch native event', event);

     var content = event.content;
     var uniqueId = content.uniqueId;

     //native只能调用到mainframe的dispatchNativeEvents，subframe的事件通过mainframe转发
     //当前window不能处理，转发给frames
     if (!_existWebSocket(uniqueId)) {
       //添加标记
       event.flag = Promptkey;
       var frames = window.frames;
       for (i = 0; i < frames.length; i++) {
         frames[i].postMessage(event, "*");
       }
       return;
     } else {
       _handleNativeEvents(event);
     }
   }

   function _handleNativeEvents(event) {

     console.log('handle native event', event);

     var title = event.title;
     var content = event.content;
     var uniqueId = content.uniqueId;

     if (!_existWebSocket(uniqueId)) return;

     switch (title) {
     case "onopen":
       _handleOnOpenEvent(uniqueId, content.protocol, content.extensions);
       break;
     case "onclose":
       _handleOnCloseEvent(uniqueId, content.code, content.reason);
       break;
     case "onmessage":
       _handleOnMessageEvent(uniqueId, content.type, content.data);
       break;
     case "onerror":
       _handleOnErrorEvent(uniqueId, content.code, content.message);
       break;
     case "bufferedAmount":
       _handleBufferedAmount(uniqueId, content.value);
       break;
     default:
       console.error("native event title invalid!", title);
     }
   }

   function _sendEvent(event) {
     // TODO 后续可以实现异步发送
     var json = JSON.stringify(event);
     console.log('send javaScript event:', json);
     return window.webkit.messageHandlers.websocketHandler.postMessage(json);
   }

   function _webSocketForUniqueId(uniqueId) {
     return webSockets[uniqueId];
   }

   function _addWebSocket(uniqueId, websocket) {
     webSockets[uniqueId] = websocket;
   }

   function _removeWebSocket(uniqueId) {
     delete webSockets[uniqueId];
   }

   function _existWebSocket(uniqueId) {
     if (webSockets[uniqueId]) {
       return true;
     }
     return false;
   }

   function _handleOnOpenEvent(uniqueId, protocol, extensions) {
     var websocket = _webSocketForUniqueId(uniqueId);
     // set protocol
     websocket.protocol = protocol;
     // set extensions
     websocket.extensions = extensions;
     // set readyState
     websocket.readyState = SFWebSocket.OPEN;

     var openEvent = new Event("open");
     websocket.dispatchEvent(openEvent);
     if (websocket.onopen) {
       websocket.onopen(openEvent);
     }
   }

   function _handleOnCloseEvent(uniqueId, code, reason) {
     var websocket = _webSocketForUniqueId(uniqueId);
     // set readyState
     websocket.readyState = SFWebSocket.CLOSED;
     var closeEvent = new CloseEvent("close", {
       code: code,
       reason: reason,
       wasClean: true //表示连接是否完全关闭，简单处理
     });
     websocket.dispatchEvent(closeEvent);
     if (websocket.onclose) {
       websocket.onclose(closeEvent);
     }
     // clean websocket
     _removeWebSocket(uniqueId);
   }

   function _handleOnMessageEvent(uniqueId, type, data) {
     var websocket = _webSocketForUniqueId(uniqueId);

     var dispatchBlock = function(ret) {
       var messageEvent = new MessageEvent("message", {
         data: ret
       });
       websocket.dispatchEvent(messageEvent);
       if (websocket.onmessage) {
         websocket.onmessage(messageEvent);
       }
     }

     if (type == 'string') {
       dispatchBlock(data);
     } else {
       data = _base64ToBlob(data);
       if (websocket.binaryType == 'arraybuffer') {
         const reader = new FileReader();
         reader.onload = function() {
           dispatchBlock(reader.result);
         }
         reader.readAsArrayBuffer(data);

       } else if (websocket.binaryType == 'blob') {
         dispatchBlock(data);
       }
     }
   }

   function _handleOnErrorEvent(uniqueId, code, message) {
     var websocket = _webSocketForUniqueId(uniqueId);
     var errorEvent = new Event("error");
     websocket.dispatchEvent(errorEvent);
     if (websocket.onerror) {
       websocket.onerror(errorEvent);
     }
   }

   function _handleBufferedAmount(uniqueId, value) {
     var websocket = _webSocketForUniqueId(uniqueId);
     // set bufferedAmount
     websocket.bufferedAmount = 0;
   }

   function _base64ToBlob(base64Str) {
     var rawData = atob(base64Str);
     var array = new Uint8Array(rawData.length);
     for (var i = 0; i < rawData.length; i++) {
       array[i] = rawData.charCodeAt(i);
     }
     return new Blob([array]);
   }

   window.SFWebSocketProxy.RealWebSocket = WebSocket;
   window.SFWebSocket = SFWebSocket;

   // WebSocket hook
   WebSocket = function (h, proto) {
     if (h.startsWith("ws://")) {
      // ws使用wss方案，SDK根据特定的APP来设置此标识
      // [网上问题][Q2024071601448]中信金融文档预览异常
      // 目前推测有依赖ws返回的cookie，当前ws方案会导致websocket请求的cookie丢失
      // 使用wss方案来解决ws的cookie丢失问题，但只支持iOS13及以上的系统版本
       if (window.sf_ws_use_wss_case) {
         return new window.SFWebSocket(h, proto);
       } else {
         return new window.SFWebSocketProxy.RealWebSocket("ws://127.0.0.1:%d/param?host=" + h, proto);
       }
     }
     return new window.SFWebSocket(h, proto);
   }
   WebSocket.CONNECTING = 0;
   WebSocket.OPEN = 1;
   WebSocket.CLOSING = 2;
   WebSocket.CLOSED = 3;

   // prototype也需要设置，业务侧可能会通过prototype去拿值
   WebSocket.prototype.CONNECTING = 0;
   WebSocket.prototype.OPEN = 1;
   WebSocket.prototype.CLOSING = 2;
   WebSocket.prototype.CLOSED = 3;
 

   console.log("complete load SFWebSocketProxy");
 })();
