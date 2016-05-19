var LibraryHttpClient = {

    http_client_async_wget2_data : function(url, request, param, arg, free, onload, onerror, onprogress) {
        var _url = Pointer_stringify(url);
        var _request = Pointer_stringify(request);
        var _param = Pointer_stringify(param);
        
        var http = new XMLHttpRequest();
        http.open(_request, _url, true);
        http.responseType = 'arraybuffer';
        
        var handle = Browser.getNextWgetRequestHandle();
        
        // LOAD
        http.onload = function http_onload(e) {
            if (http.status == 200 || _url.substr(0,4).toLowerCase() != "http") {
                var byteArray = new Uint8Array(http.response);
                var buffer = _malloc(byteArray.length);
                HEAPU8.set(byteArray, buffer);
                if (onload) Runtime.dynCall('viiii', onload, [handle, arg, buffer, byteArray.length]);
                if (free) _free(buffer);
            } else {
                if (onerror) Runtime.dynCall('viiii', onerror, [handle, arg, http.status, http.statusText]);
            }
            delete Browser.wgetRequests[handle];
        };
        
        // ERROR
        http.onerror = function http_onerror(e) {
            if (onerror) {
                Runtime.dynCall('viiii', onerror, [handle, arg, http.status, http.statusText]);
            }
            delete Browser.wgetRequests[handle];
        };
        
        // PROGRESS
        http.onprogress = function http_onprogress(e) {
            if (onprogress) Runtime.dynCall('viiii', onprogress, [handle, arg, e.loaded, e.lengthComputable || e.lengthComputable === undefined ? e.total : 0]);
        };
        
        // ABORT
        http.onabort = function http_onabort(e) {
            delete Browser.wgetRequests[handle];
        };
        
        // Useful because the browser can limit the number of redirection
        try {
            if (http.channel instanceof Ci.nsIHttpChannel)
                http.channel.redirectionLimit = 0;
        } catch (ex) { /* whatever */ }
        
        try
        {
            if (_request == "POST") {
                //Send the proper header information along with the request
                http.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
                http.send(_param);
            } else {
                http.send(null);
            }
        }
        catch(err) { }
        
        Browser.wgetRequests[handle] = http;
        
        return handle;
    }
};

mergeInto(LibraryManager.library, LibraryHttpClient);
