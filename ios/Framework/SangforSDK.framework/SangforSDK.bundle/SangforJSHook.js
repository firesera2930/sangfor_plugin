/*
 * 此文件目的是为了解决以下两个问题：
 一、iOS12 XMLHTTPRequest 同步请求body过大导致页面崩溃问题
 二、WKWebView使用XMLHttpRequest和fetch进行post请求body丢失问题
   目前XMLHttpRequest和Fetch请求支持如下类型的body.
   支持字符串：完全支持 @zhangrong
   Uint8Array：开发自测已支持，但现实未遇到此场景@caobin
   FormData类型中通过FormData.append函数添加的值:已支持@caobin
   FormData类型中通过构造函数解析HTML DOM的场景：已支持大部分（泛微用到的），某些元素可能还不支持（比如<input type=file>类型的）@zhangrong
 **/
// 处理FormData
var __awaiter = (this && this.__awaiter) || function (thisArg, _arguments, P, generator) {
    return new (P || (P = Promise))(function (resolve, reject) {
        function fulfilled(value) { try { step(generator.next(value)); } catch (e) { reject(e); } }
        function rejected(value) { try { step(generator["throw"](value)); } catch (e) { reject(e); } }
        function step(result) { result.done ? resolve(result.value) : new P(function (resolve) { resolve(result.value); }).then(fulfilled, rejected); }
        step((generator = generator.apply(thisArg, _arguments || [])).next());
    });
};
var __generator = (this && this.__generator) || function (thisArg, body) {
    var _ = { label: 0, sent: function() { if (t[0] & 1) throw t[1]; return t[1]; }, trys: [], ops: [] }, f, y, t, g;
    return g = { next: verb(0), "throw": verb(1), "return": verb(2) }, typeof Symbol === "function" && (g[Symbol.iterator] = function() { return this; }), g;
    function verb(n) { return function (v) { return step([n, v]); }; }
    function step(op) {
        if (f) throw new TypeError("Generator is already executing.");
        while (_) try {
            if (f = 1, y && (t = op[0] & 2 ? y["return"] : op[0] ? y["throw"] || ((t = y["return"]) && t.call(y), 0) : y.next) && !(t = t.call(y, op[1])).done) return t;
            if (y = 0, t) op = [op[0] & 2, t.value];
            switch (op[0]) {
                case 0: case 1: t = op; break;
                case 4: _.label++; return { value: op[1], done: false };
                case 5: _.label++; y = op[1]; op = [0]; continue;
                case 7: op = _.ops.pop(); _.trys.pop(); continue;
                default:
                    if (!(t = _.trys, t = t.length > 0 && t[t.length - 1]) && (op[0] === 6 || op[0] === 2)) { _ = 0; continue; }
                    if (op[0] === 3 && (!t || (op[1] > t[0] && op[1] < t[3]))) { _.label = op[1]; break; }
                    if (op[0] === 6 && _.label < t[1]) { _.label = t[1]; t = op; break; }
                    if (t && _.label < t[2]) { _.label = t[2]; _.ops.push(op); break; }
                    if (t[2]) _.ops.pop();
                    _.trys.pop(); continue;
            }
            op = body.call(thisArg, _);
        } catch (e) { op = [6, e]; y = 0; } finally { f = t = 0; }
        if (op[0] & 5) throw op[1]; return { value: op[0] ? op[1] : void 0, done: true };
    }
};
var __values = (this && this.__values) || function (o) {
    var m = typeof Symbol === "function" && o[Symbol.iterator], i = 0;
    if (m) return m.call(o);
    return {
        next: function () {
            if (o && i >= o.length) o = void 0;
            return { value: o && o[i++], done: !o };
        }
    };
};

(function (window) {
    //此方法只是为了给OC层打日志
    sendLogToOC = function(txt) {
        try {
            window.prompt("SendLogToOC", txt);  //此方式暂未发现冲突
//            window.webkit.messageHandlers.fix_sync_ajax_sendlog.postMessage(txt); //这种OC交互方式和中石油的瑞信app中的“考勤打卡”冲突了，他们判断了window.webkit
        } catch (e) {
 
        }
    }
    let sf_interceptors = [];

    function interceptor(fetch, ...args) {
        const reversedInterceptors = sf_interceptors.reduce((array, interceptor) => [interceptor].concat(array), []);
        let promise = Promise.resolve(args);
        
        // Register request sf_interceptors
        reversedInterceptors.forEach(({ request, requestError }) => {
                                    if (request || requestError) {
                                    promise = promise.then(args => request(...args), requestError);
                                    }
                                    });
        
        // Register fetch call
        promise = promise.then(args => fetch(...args));
        
        // Register response sf_interceptors
        reversedInterceptors.forEach(({ response, responseError }) => {
                                    if (response || responseError) {
                                    promise = promise.then(response, responseError);
                                    }
                                    });
        
        return promise;
    }

    function sf_Hook_fetch(env) {
        // Make sure fetch is avaibale in the given environment
        if (!env.fetch) {
            try {
                require('whatwg-fetch');
            } catch (err) {

                throw Error('No fetch avaibale. Unable to register fetch-intercept');
            }
        }
        env.fetch = (function (fetch) {
                    return function (...args) {
                    return interceptor(fetch, ...args);
                    };
                    })(env.fetch);
        
        return {
        register: function (interceptor) {
            sf_interceptors.push(interceptor);
            return () => {
                const index = sf_interceptors.indexOf(interceptor);
                if (index >= 0) {
                    sf_interceptors.splice(index, 1);
                }
            };
        },
        clear: function () {
            sf_interceptors = [];
        }
        };
    };

    /**
    * Hook FormData，由于低版本的 FormData 没有支持 entries() 等遍历 api，所以只是在 ajax send 里遍历，是无法获取到具体的值的，
    * 所以针对低版本的 iOS 系统做 Hook FormData 处理,hook append函数（可以支持完全js创建的formData场景），hook FormData的构造函数可以支持HTML形式的FormData。
    */
    function sf_hookFormData() {
        /**
         * 解析HTML的form DOM，获取其中的子对象的key value
         * @param {*} box 输入的HTML DOM
         * @param {*} outDict 输出的字典，FormData的key,value
         */
        function sf_scanDOM(box, outDict) {
            var eles = Array.from(box.children)
            for (var ele of eles) {
              if (ele.children.length && ele.nodeName !== 'SELECT' && ele.nodeName !== 'FIELDSET') { // select元素没有必要递归，表单包也暂时不用遍历
                sf_scanDOM(ele)
              } else {
                var nodeName = ele.nodeName.toLowerCase();
                var formElementp = function(v) {return (v === 'input' || v === 'select' || v === 'textarea' || v === 'fieldset')};
                if (formElementp(nodeName)) { // 确认元素是表单元素
                  if (ele.disabled === true) continue; // 如果元素为禁用则跳过
                  var k, v;
                  k = ele.name;
                  if (!k) continue; // 没有name的表单元素跳过
                  if (ele.type === 'radio') { // 单选按钮时，需要判断是否为选中状态
                    if (!ele.checked) continue;
                    v = ele.value;
                  } else if (ele.type === 'checkbox') {
                    if (!ele.checked) continue;
                    v = ele.value;
                  } else {
                    if (nodeName === 'select' && ele.multiple) {
                      var vs = []; // 收集所有多选状态下的 selected的value，
                      var options = toArray(ele.children);
                      for (var o of options) {
                        vs.push(o.value)
                      }
                      v = vs;
                    } else if (nodeName === 'fieldset') {
                        sf_scanDOM(ele)
                    } else {
                      v = ele.value;
                    }
                  }
                  if(v === undefined) continue;
                  var alive = k in outDict;
                  if (alive) { // 已经存在相应的name
                    var ov = outDict[k]
                    if (Object.prototype.toString.call(outDict[k]) === '[object Array]') {
                      outDict[k] = ov.concat(v)
                    } else {
                      outDict[k] = [ov, v]
                    }
                  } else {
                    outDict[k] = v;
                  }
                }
          
              }
            }
        }
  
        var sf_originAppend = window.FormData.prototype['append'];
        var sf_originEntries = window.FormData.prototype['entries'];
        
        //只有针对低版本不存在entries函数的情况下才需要进行hook
        if (!sf_originEntries) {
            if (!window.sf_oriFormData) {
                //hook formData的append函数
                window.FormData.prototype['append'] = function () {
                    if (!this._entries) {
                        this._entries = [];
                    }
                    this._entries.push(arguments);
                    return sf_originAppend.apply(this, arguments);
                };

                window.sf_oriFormData = window.FormData;
                //hook FormData的构造函数
                window.FormData = function(box) {
                    var ret = new window.sf_oriFormData(box);
                    try {
                        if (box) {
                            var outDict = {};
                            sf_scanDOM(box, outDict);
                            if (!ret._entries) {
                                ret._entries = [];
                            }
                            sendLogToOC("parse FormData constructor params:" + outDict);
                            for (var key in outDict) { //转成数组形式
                                ret._entries.push([key, outDict[key]]);
                            }
                        }
                    } catch (e) {
                        sendLogToOC("formData to Json error : " + e.message);
                    }
    
                    return ret;
                }
            }
            sendLogToOC("hook FormData OK");
        } else {
            sendLogToOC("no need hook FormData");
        }
    }
    
    //Ajax的hook工具方法
    function sf_initHookAjaxMethod(ob) {
    
        //Save original XMLHttpRequest as RealXMLHttpRequest
        var realXhr = "SFRealXMLHttpRequest"
    
        //Call this function will override the `XMLHttpRequest` object
        ob.sf_HookAjax = function (proxy) {
    
            // Avoid double hook
            window[realXhr] = window[realXhr] || XMLHttpRequest
    
            XMLHttpRequest = function () {
                var sf_xhr = new window[realXhr];
                // We shouldn't hook XMLHttpRequest.prototype because we can't
                // guarantee that all attributes are on the prototype。
                // Instead, hooking XMLHttpRequest instance can avoid this problem.
                for (var attr in sf_xhr) {
                    var type = "";
                    try {
                        type = typeof sf_xhr[attr] // May cause exception on some browser
                    } catch (e) {
                    }
                    if (type === "function") {
                        // hook methods of xhr, such as `open`、`send` ...
                        this[attr] = hookFunction(attr);
                    } else {
                        Object.defineProperty(this, attr, {
                            get: getterFactory(attr),
                            set: setterFactory(attr),
                            enumerable: true,
							configurable: true
                        })
                    }
                }
                this.sf_xhr = sf_xhr;
    
            }
    
            // Generate getter for attributes of xhr
            function getterFactory(attr) {
                return function () {
                    var v = this.hasOwnProperty(attr + "_") ? this[attr + "_"] : this.sf_xhr[attr];
                    var attrGetterHook = (proxy[attr] || {})["getter"]
                    return attrGetterHook && attrGetterHook(v, this) || v
                }
            }
    
            // Generate setter for attributes of xhr; by this we have an opportunity
            // to hook event callbacks （eg: `onload`） of xhr;
            function setterFactory(attr) {
                return function (v) {
                    var sf_xhr = this.sf_xhr;
                    var that = this;
                    var hook = proxy[attr];
                    if (typeof hook === "function") {
                        // hook  event callbacks such as `onload`、`onreadystatechange`...
                        sf_xhr[attr] = function () {
                            proxy[attr](that) || v.apply(sf_xhr, arguments);
                        }
                    } else {
                        //If the attribute isn't writable, generate proxy attribute
                        var attrSetterHook = (hook || {})["setter"];
                        v = attrSetterHook && attrSetterHook(v, that) || v
                        try {
                            sf_xhr[attr] = v;
                        } catch (e) {
                            this[attr + "_"] = v;
                        }
                    }
                }
            }
    
            // Hook methods of xhr.
            function hookFunction(fun) {
                return function () {
                    var args = [].slice.call(arguments)
    
    //                if (proxy[fun] && proxy[fun].call(this, args, this.sf_xhr)) {
    //                    return;
                    //这个位置有修改开源hook框架，主要是针对getAllResponseHeaders和getResponseHeader方法，我们有时需要修改返回值
                    if (proxy[fun]) {
                        var proxyRet = proxy[fun].call(this, args, this.sf_xhr)
                        if (proxyRet) {
                            if (typeof(proxyRet) === "boolean") {
                                return;
                            } else {
                                return proxyRet;
                            }
                        }
                    }
                    
                    return this.sf_xhr[fun].apply(this.sf_xhr, args);
                }
            }
    
            // Return the real XMLHttpRequest
            return window[realXhr];
        }
    
        // Cancel hook
        ob.sf_unHookAjax = function () {
            if (window[realXhr]) XMLHttpRequest = window[realXhr];
            window[realXhr] = undefined;
        }
    
        //for typescript
        ob["sfdefault"] = ob;
    }
    
    /**
         * SFJSBridge 工具
         */
    var SFJSBridgeUtil = /** @class */ (function () {
        function SFJSBridgeUtil() {
        }
        SFJSBridgeUtil.convertFormDataToJson = function (formData, callback) {
            var _this = this;
            var promise = new Promise(function (resolve, reject) { return __awaiter(_this, void 0, void 0, function () {
                var e_1, _a, formDataJson, formDataFileKeys, formDatas, i, pair, key, value, fileName, singleKeyValue, formDataFile, _b, _c, pair, key, value, singleKeyValue, formDataFile, e_1_1;
                return __generator(this, function (_d) {
                    switch (_d.label) {
                        case 0:
                            formDataJson = {};
                            formDataFileKeys = [];
                            formDatas = [];
                            if (!formData._entries) return [3 /*break*/, 7];
                            i = 0;
                            _d.label = 1;
                        case 1:
                            if (!(i < formData._entries.length)) return [3 /*break*/, 6];
                            pair = formData._entries[i];
                            key = pair[0];
                            value = pair[1];
                            fileName = pair.length > 2 ? pair[2] : null;
                            singleKeyValue = [];
                            singleKeyValue.push(key);
                            if (!(value instanceof File || value instanceof Blob)) return [3 /*break*/, 3];
                            return [4 /*yield*/, SFJSBridgeUtil.convertFileToJson(value)];
                        case 2:
                            formDataFile = _d.sent();
                            if (fileName) { // 文件名需要处理下
                                formDataFile.name = fileName;
                            }
                            singleKeyValue.push(formDataFile);
                            formDataFileKeys.push(key);
                            return [3 /*break*/, 4];
                        case 3:
                           /*ios11 webkit formdata append obj need to string */
                           if (value) {
                               value = value.toString();
                           }
                            singleKeyValue.push(value);
                            _d.label = 4;
                        case 4:
                            formDatas.push(singleKeyValue);
                            _d.label = 5;
                        case 5:
                            i++;
                            return [3 /*break*/, 1];
                        case 6: return [3 /*break*/, 16];
                        case 7:
                            _d.trys.push([7, 14, 15, 16]);
                            _b = __values(formData.entries()), _c = _b.next();
                            _d.label = 8;
                        case 8:
                            if (!!_c.done) return [3 /*break*/, 13];
                            pair = _c.value;
                            key = pair[0];
                            value = pair[1];
                            singleKeyValue = [];
                            singleKeyValue.push(key);
                            if (!(value instanceof File || value instanceof Blob)) return [3 /*break*/, 10];
                            return [4 /*yield*/, SFJSBridgeUtil.convertFileToJson(value)];
                        case 9:
                            formDataFile = _d.sent();
                            singleKeyValue.push(formDataFile);
                            formDataFileKeys.push(key);
                            return [3 /*break*/, 11];
                        case 10:
                            singleKeyValue.push(value);
                            _d.label = 11;
                        case 11:
                            formDatas.push(singleKeyValue);
                            _d.label = 12;
                        case 12:
                            _c = _b.next();
                            return [3 /*break*/, 8];
                        case 13: return [3 /*break*/, 16];
                        case 14:
                            e_1_1 = _d.sent();
                            e_1 = { error: e_1_1 };
                            return [3 /*break*/, 16];
                        case 15:
                            try {
                                if (_c && !_c.done && (_a = _b.return)) _a.call(_b);
                            }
                            finally { if (e_1) throw e_1.error; }
                            return [7 /*endfinally*/];
                        case 16:
                            formDataJson['fileKeys'] = formDataFileKeys;
                            formDataJson['formData'] = formDatas;
                            resolve(formDataJson);
                            return [2 /*return*/];
                    }
                });
            }); });
            
            promise.then(function (json) {
                //延迟500MS 是为了OC层能先收到网络请求，再收到formdata的解析参数，否则drop body功能会失效
                setTimeout(function () {
                    callback(json);
                }, 500);
       
            }).catch(function (error) {
                console.log(error);
                sendLogToOC("formData to Json error : " + error);
            });
        };
        /**
         * 读取单个文件数据，并转成 base64，最后返回 json 对象
         * @param file
         */
        SFJSBridgeUtil.convertFileToJson = function (file) {
            return new Promise(function (resolve, reject) {
                var reader = new FileReader();
                reader.readAsDataURL(file);
                reader.onload = function (ev) {
                    var base64 = ev.target.result;
                    var formDataFile = {
                        name: file instanceof File ? file.name : '',
                        lastModified: file instanceof File ? file.lastModified : 0,
                        size: file.size,
                        type: file.type,
                        data: base64
                    };
                    resolve(formDataFile);
                    return null;
                };
                reader.onerror = function (ev) {
                    reject(Error("formdata 表单读取文件数据失败"));
                    return null;
                };
            });
        };

        SFJSBridgeUtil.getIOSVersion = function (userAgent) {
            var ua = userAgent.toLowerCase();
            if(ua.indexOf("like mac os x") > 0){
               var reg = /os [\d._]*/gi ;
               var verinfo = ua.match(reg) ;
               var version = (verinfo+"").replace(/[^0-9|_.]/ig,"").replace(/_/ig,".");
               var arr = version.split(".");
               if (arr[0]) {
                    return parseInt(arr[0]);
                }
            }
            return 0;
        }

        return SFJSBridgeUtil;
    }());
    
    
    //ajax hook的代理，所有ajax请求会先走到这里面
    sf_AjaxHookProxy = {

        // 拦截属性
        readyState: {
            getter: function (v, xhr) {
                xhr = xhr.sf_xhr; //函数参数xhr是代理的xhr
                if (xhr.isSync && xhr.responseFromOC) {
                    return xhr.responseFromOC.readyState;
                }
            }
        },
        status: {
            getter: function (v, xhr) {
                xhr = xhr.sf_xhr; //函数参数xhr是代理的xhr
                if (xhr.isSync && xhr.responseFromOC) {
                    return xhr.responseFromOC.status;
                }
            }
        },
        statusText: {
            getter: function (v, xhr) {
                xhr = xhr.sf_xhr; //函数参数xhr是代理的xhr
                if (xhr.isSync && xhr.responseFromOC) {
                    return xhr.responseFromOC.statusText;
                }
            }
        },
        responseText: {
            getter: function (v, xhr) {
                xhr = xhr.sf_xhr; //函数参数xhr是代理的xhr
                if (xhr.isSync && xhr.responseFromOC) {
                    return xhr.responseFromOC.responseText;
                }
            }
        },
        response: {
            getter: function (v, xhr) {
                xhr = xhr.sf_xhr; //函数参数xhr是代理的xhr
                if (xhr.isSync && xhr.responseFromOC) {
                    return xhr.responseFromOC.responseText;
                }
            }
        },
        //拦截方法
        open: function (arg, xhr) {
            try {
                //函数参数xhr是真实的xhr
                sendLogToOC("open called: method:" + arg[0] + ", url:" + arg[1] + ", async:" + arg[2])
                //如果是异步请求非GET和HEAD都可能存在body丢失，则需要将body内容传给OC，因为iOS11和10中需要
                if (!((arg[0] == "GET") || (arg[0] == "get") || (arg[0] == "HEAD") || (arg[0] == "head"))) {
                    xhr.needFixDropBody = true;
                    xhr.method = arg[0];
                    xhr.uniqueID = Math.round(Math.random() * 10000000) + ""
                    xhr.requestParamsToOC =  {
                        "sangfor_sftaskid": xhr.uniqueID,
                        "method": arg[0],
                        "url": arg[1],
                    }
                 }

                var iosVersion = SFJSBridgeUtil.getIOSVersion(navigator.userAgent);
                xhr.iosVersion = iosVersion;
                if(arg[2] != undefined && !arg[2] && iosVersion <= 12) {
                    //如果是同步请求才走下面流程，保存参数，准备传给OC
                    xhr.isSync = true
                    xhr.uniqueID = Math.round(Math.random() * 100000)
                    xhr.requestParamsToOC =  {
                        "taskID": xhr.uniqueID,
                        "method": arg[0],
                        "url": arg[1],
                        "scheme": window.location.protocol,
                        "host": window.location.hostname,
                        "port": window.location.port,
                        "href": window.location.href,
                        "referer": document.referrer != "" ? document.referrer : window.location.href,
                        "useragent": navigator.userAgent,
                        "async": arg[2],
                        "timeout": xhr.timeout
                    }
                    return true;
                }
    
            } catch (e) {
                sendLogToOC("Except in hook open:" + e.message);
            }
            
        },
        send: function (arg, xhr) {
            try {
                if (xhr.isSync) {
                    sendLogToOC("send called: params:" + arg[0])
                    sendLogToOC("uniqueID:" + xhr.uniqueID);
					var data = arg[0]
					//将uint8Array单独提取出来处理,formdata格式暂时还未做处理
					if (data instanceof Uint8Array) {
						data = Array.from(data)
					} else {
						data = data.toString()
					}
					xhr.requestParamsToOC.sendBody = data
                    //利用prompt方法与OC进行同步交互，把请求参数传入，OC端会传出请求结果，对应OC中的方法AjaxHookHelper_WKUIDelegate_webView:runJavaScriptTextInputPanelWithPrompt:
                    responseJsonStrFromOC = window.prompt("FixAjaxSync", JSON.stringify(xhr.requestParamsToOC))
                    sendLogToOC("responseJsonStrFromOC:" + responseJsonStrFromOC)
                    xhr.responseFromOC = JSON.parse(responseJsonStrFromOC)
        
                    //模拟onreadystatechange事件，正常的ajax同步方法也会回调此函数
                    if (xhr.onreadystatechange) {
                        xhr.onreadystatechange();
                    }
        
                    //模拟load事件，正常的ajax同步方法也会回调此函数
                    if (xhr.responseFromOC.readyState === xhr.DONE) {
                        if (xhr.onload) {
                            xhr.onload();
                        }
                        //分发事件load事件
                        var load = document.createEvent("Events");
                        load.initEvent("load");
                        xhr.dispatchEvent(load);
                     }else {
                        if (xhr.onerror) {
                            xhr.onerror();
                        }
                        //分发事件error事件
                        var error = document.createEvent("Events");
                        error.initEvent("error");
                        xhr.dispatchEvent(error);
                     }
        
                    //给OC层网络请求，这里return true就不会调用原始send方法进行js层的请求了，所以也不会自动调用onload等事件，需要手动调用
                    return true;
                } else if (xhr.needFixDropBody) {
                    sendLogToOC("xhr send called: params:" + arg[0])
                    sendLogToOC("uniqueID:" + xhr.uniqueID);
                    var data = arg[0];
                    if(data) {
                        
                        if (data instanceof Uint8Array) {
                            if(xhr.iosVersion && (xhr.iosVersion < 12)) {
                                xhr.setRequestHeader("sangfor_sftaskid", xhr.requestParamsToOC.sangfor_sftaskid)
                                xhr.setRequestHeader("sangfor_sftaskid_cors" + xhr.requestParamsToOC.sangfor_sftaskid, "1")
                                sendLogToOC("body data is Uint8Array...");
                                // 特殊处理字节数据
                                data = Array.from(data);
                                xhr.requestParamsToOC.data = data;
                                window.prompt("FixDropBodyByte", JSON.stringify(xhr.requestParamsToOC))
                                sendLogToOC("FixDropBodyByte.")
                            }
                        } else if((data instanceof FormData) || (window.sf_oriFormData && data instanceof window.sf_oriFormData)) {
                            sendLogToOC("body data is FormData...");
                            xhr.setRequestHeader("sangfor_sftaskid", xhr.requestParamsToOC.sangfor_sftaskid)
                            xhr.setRequestHeader("sangfor_sftaskid_cors" + xhr.requestParamsToOC.sangfor_sftaskid, "1")
                            // formData 表单
                            xhr.setRequestHeader("sangfor_isformdata", "1");
                            SFJSBridgeUtil.convertFormDataToJson(data, function (json) {
                                
                                sendLogToOC("parse formData json:" + JSON.stringify(json));
                                xhr.requestParamsToOC.data = json;
                                window.prompt("FixDropBodyFormData", JSON.stringify(xhr.requestParamsToOC))
                             });
						} else if (data instanceof File) {
							sendLogToOC("body data is File...");
							var form = new FormData();
							form.append('file', data);
							xhr.setRequestHeader("sangfor_sftaskid", xhr.requestParamsToOC.sangfor_sftaskid)
							xhr.setRequestHeader("sangfor_sftaskid_cors" + xhr.requestParamsToOC.sangfor_sftaskid, "1")
							// formData 表单
							xhr.setRequestHeader("sangfor_isformdata", "1");
							SFJSBridgeUtil.convertFormDataToJson(form, function (json) {

								sendLogToOC("parse formData json:" + JSON.stringify(json));
								xhr.requestParamsToOC.data = json;
								window.prompt("FixDropBodyFormData", JSON.stringify(xhr.requestParamsToOC))
							});

						} else {
                            if(xhr.iosVersion && (xhr.iosVersion < 12)) {
                                sendLogToOC("body data is Others such as String...");
                                xhr.setRequestHeader("sangfor_sftaskid", xhr.requestParamsToOC.sangfor_sftaskid)
                                xhr.setRequestHeader("sangfor_sftaskid_cors" + xhr.requestParamsToOC.sangfor_sftaskid, "1")
								xhr.requestParamsToOC.sendBody = data.toString()
                                window.prompt("FixDropBody4XHR", JSON.stringify(xhr.requestParamsToOC))
                            }
                        }
                     }
                }
                
            } catch (e) {
                sendLogToOC("Except in hook send:" + e.message);
            }
        },

        setRequestHeader: function (arg, xhr) {
            try {
                if (xhr.isSync && xhr.requestParamsToOC && arg[0] && arg[1]) {
                    sendLogToOC("setRequestHeader called:" + arg)
                    if (!xhr.requestParamsToOC.requestHeader) {
                        xhr.requestParamsToOC.requestHeader = new Object()
                    }
                    xhr.requestParamsToOC.requestHeader[arg[0]] = arg[1]
                    return true;
                }
            } catch (e) {
                sendLogToOC("Except in hook setRequestHeader:" + e.message);
            }
        },
        overrideMimeType: function (arg, xhr) {
            try {
                if (xhr.isSync && xhr.requestParamsToOC && arg[0]) {
                    sendLogToOC("overrideMimeType called:" + arg[0])
                    xhr.requestParamsToOC.overrideMimeType = arg[0]
                    return true;
                }
            } catch (e) {
                sendLogToOC("Except in hook overrideMimeType:" + e.message);
            }
        },
        getAllResponseHeaders: function (arg, xhr) {
            try {
                if (xhr.isSync && xhr.responseFromOC && xhr.responseFromOC.headers) {
                    sendLogToOC("hooked getAllResponseHeaders called");
                    var strHeaders = '';
                    for (var name_1 in xhr.responseFromOC.headers) {
                        strHeaders += (name_1 + ": " + xhr.responseFromOC.headers[name_1] + "\r\n");
                    }
                    sendLogToOC("hooked getAllResponseHeaders:" + strHeaders);
                    return strHeaders;
                }
            } catch (e) {
                sendLogToOC("Except in hook getAllResponseHeaders:" + e.message);
            }
        },
        getResponseHeader: function (arg, xhr) {
            try {
                if (xhr.isSync && xhr.responseFromOC && xhr.responseFromOC.headers && arg[0]) {
                    sendLogToOC("hooked getResponseHeader called:" + arg[0]);
                    var headerName = arg[0];
                    var strHeaders = '';
                    var upperCaseHeaderName = headerName.toUpperCase();
                    for (var name_2 in xhr.responseFromOC.headers) {
                        if (upperCaseHeaderName == name_2.toUpperCase())
                            strHeaders = xhr.responseFromOC.headers[name_2];
                    }
                    sendLogToOC("hooked getResponseHeader:" + strHeaders);
                    return strHeaders;
                }
            } catch (e) {
                sendLogToOC("Except in hook getResponseHeader:" + e.message);
            }
        }
    }

    sf_FetchHookProxy = {
        request: function (ori, config) {
            // Modify the url or config here
            //真正的hook业务代码
            try {

                var url = "";
                var method = "GET";
                var headers = {};
                var body = "";
                //fetch 默认第一个参数是url或者request对象，第二个可选
                if (ori instanceof Request) {
                    url = ori.url;
                    if (ori.method && config.method != "undefined") {
                        method = ori.method
                    }
                    if (ori.headers) {
                        headers = ori.headers
                    }

                    if (ori.body) {
                        body = ori.body
                    }
                } else {
                    url = ori;
                }

                if (config && config != "undefined") {
                    if (config.method && config.method != "undefined") {
                        method = config.method;
                    }
                    /*除了GET和HEAD都能携带body*/
                    if ((method == "GET") || (method == "get") || (method == "HEAD") || (method == "head")) {
                        return [url, config];
                    }
                    
                    if (config.headers) {
                        headers = config.headers;
                    }
                    
                    if (config.body) {
                        body = config.body;
                    }
                }

                
                var uniqueID = Math.round(Math.random() * 10000000) + "";

                if (headers instanceof Headers) {
                    headers.append("sangfor_sftaskid", uniqueID);
                    headers.append("sangfor_sftaskid_cors" + uniqueID, "1")
                } else {
                    headers["sangfor_sftaskid"] = uniqueID;
                    headers["sangfor_sftaskid_cors" + uniqueID] = "1";
                }

                sendLogToOC("fetch url:" + url + ",method:" + method);

                var requestParamsToOC =  {
                    "sangfor_sftaskid": uniqueID,
                    "method": method,
                    "url": url,
                }

                if (body) {
                    var iosVersion = SFJSBridgeUtil.getIOSVersion(navigator.userAgent);
                    if (body instanceof Uint8Array) {
                        if (iosVersion && (iosVersion < 12)) {
                            sendLogToOC("body data is Uint8Array...");
                            // 特殊处理字节数据
                            body = Array.from(body);
                            requestParamsToOC.data = body;
                            window.prompt("FixDropBodyByte", JSON.stringify(requestParamsToOC))
                            sendLogToOC("FixDropBodyByte.")
                        }
                    } else if((body instanceof FormData) || (window.sf_oriFormData && body instanceof window.sf_oriFormData)) {
                        sendLogToOC("body data is FormData...");
                        // formData 表单
                        if (headers instanceof Headers) {
                            headers.append("sangfor_isformdata", "1");
                        } else {
                            headers["sangfor_isformdata"] = "1";
                        }
                        SFJSBridgeUtil.convertFormDataToJson(body, function (json) {
                            
                            sendLogToOC("parse formData json:" + JSON.stringify(json));
                            requestParamsToOC.data = json;
                            window.prompt("FixDropBodyFormData", JSON.stringify(requestParamsToOC))
                         });
                    } else {
                        if(iosVersion && (iosVersion < 12)) {
                            sendLogToOC("body data is Others such as String...");

                            requestParamsToOC.sendBody = body
                            window.prompt("FixDropBody4Fetch", JSON.stringify(requestParamsToOC))
                        }
                    }
                }

            } catch (e) {
                sendLogToOC("Except in hook fetch register:" + e.message);
            }
            
            return [ori, config];
        }
    }
    
    //####################################################开始hook XMLHttpRequest################
    if (window.sanforallhook) {
        sendLogToOC("no repeat hook")
        return
    } else {
        sendLogToOC("init sangfor hook")
        window.sanforallhook = 1
    }

    sf_initHookAjaxMethod(window)  //开始创建hookajax的工具函数
    sendLogToOC("sf_xhr_Hook OK,href is: " + window.location.href)
    sf_HookAjax(sf_AjaxHookProxy); //真正的使用AjaxHook

    //####################################################开始hook fetch################
    sf_Hook_fetch(window).register(sf_FetchHookProxy);
    sendLogToOC("sf_fetch_Hook OK,href is: " + window.location.href);

    sf_hookFormData(); //开始hook FormData
})(window);

