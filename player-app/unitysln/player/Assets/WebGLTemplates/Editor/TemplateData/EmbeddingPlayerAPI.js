var wsUri = "ws://localhost:33168";
var websocket = null;
var gameInstance = null;

function wsInitialize() {
    try {
        if (typeof MozWebSocket == 'function')
            WebSocket = MozWebSocket;
        if (websocket && websocket.readyState == 1)
            websocket.close();
        websocket = new WebSocket(wsUri);
        websocket.onopen = function (evt) {
            console.log("CONNECTED");
            wsSendMessage("__ready__|0|");
        };
        websocket.onclose = function (evt) {
            console.log("DISCONNECTED");
        };
        websocket.onmessage = function (evt) {
            console.log("Message received :", evt.data);
            __messageToUnity(evt.data);
        };
        websocket.onerror = function (evt) {
            console.log('ERROR: ' + evt.data);
        };
    } catch (exception) {
        console.log('ERROR: ' + exception);
    }
}

function wsStop() {
    if (websocket)
        websocket.close();
}

function wsCheckState() {
    if (websocket != null) {
        var stateStr;
        switch (websocket.readyState) {
            case 0: {
                stateStr = "CONNECTING";
                break;
            }
            case 1: {
                stateStr = "OPEN";
                break;
            }
            case 2: {
                stateStr = "CLOSING";
                break;
            }
            case 3: {
                stateStr = "CLOSED";
                break;
            }
            default: {
                stateStr = "UNKNOW";
                break;
            }
        }
        debug("WebSocket state = " + websocket.readyState + " ( " + stateStr + " )");
    } else {
        debug("WebSocket is null");
    }
}


function wsSendMessage(_message) {
    if (websocket != null) {
        websocket.send(_message);
    }
}

/*------------------------------------------------------------------------
 API : Unity - > Qt
------------------------------------------------------------------------*/
function __ready() {
    wsInitialize();
}

function __messageToQt(_message) {
    wsSendMessage(_message);
}

/*------------------------------------------------------------------------
 API : Qt -> Unity
------------------------------------------------------------------------*/
function __messageToUnity(_message) {
    gameInstance.SendMessage('__PLAYER__', 'JSMessage', _message);
}

