(function() {
    if (window.SFWebSocket) {
        console.error("already load SFWebSocket");
        return;
    }
    console.log("load SFWebSocketProxy");
    if (!window.WebSocket) {
        throw Error('no WebSocket avaibale');
    }
    window.SFWebSocket = WebSocket;
    WebSocket = function(h, proto) {
        if (h.startsWith("ws://")) {
          return new window.SFWebSocket("ws://127.0.0.1:%d/param?host=" + h, proto);
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