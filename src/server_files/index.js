var ws;

var html5VideoElement;
var webrtcPeerConnection;
var webrtcConfiguration;
var reportError;

function onLocalDescription(desc) {
    console.log("Local description: " + JSON.stringify(desc));
    webrtcPeerConnection.setLocalDescription(desc).then(function() {
        ws.send(JSON.stringify({ type: "sdp", "data": webrtcPeerConnection.localDescription }));
    }).catch(reportError);
}

function onIncomingSDP(sdp) {
    console.log("Incoming SDP: " + JSON.stringify(sdp));
    webrtcPeerConnection.setRemoteDescription(sdp).catch(reportError);
    webrtcPeerConnection.createAnswer().then(onLocalDescription).catch(reportError);
}

function onIncomingICE(ice) {
    var candidate = new RTCIceCandidate(ice);
    console.log("Incoming ICE: " + JSON.stringify(ice));
    webrtcPeerConnection.addIceCandidate(candidate).catch(reportError);
}

function onAddRemoteStream(event) {
    html5VideoElement.srcObject = event.streams[0];
}

function onIceCandidate(event) {
    if (event.candidate == null)
        return;

    console.log("Sending ICE candidate out: " + JSON.stringify(event.candidate));
    ws.send(JSON.stringify({ "type": "ice", "data": event.candidate }));
}

function onServerMessage(event) {
    console.log(event.data);
    if (event.data === "YOU_ARE_EXCESS")
        alert("You canNOT connect to the serever, you are an excess client.\nPlease wait till you be able to connect!");

    var msg;

    try {
        msg = JSON.parse(event.data);
    } catch (e) {
        return;
    }

    if (!webrtcPeerConnection) {
        webrtcPeerConnection = new RTCPeerConnection(webrtcConfiguration);
        webrtcPeerConnection.ontrack = onAddRemoteStream;
        webrtcPeerConnection.onicecandidate = onIceCandidate;
    }

    switch (msg.type) {
        case "sdp": onIncomingSDP(msg.data); break;
        case "ice": onIncomingICE(msg.data); break;
        default: break;
    }
}

function playStream(videoElement, configuration, reportErrorCB) {
    html5VideoElement = videoElement;
    webrtcConfiguration = configuration;
    reportError = (reportErrorCB != undefined) ? reportErrorCB : function(text) {};

    ws.addEventListener("message", onServerMessage);
}


var isQ_ButtonPushed = false;
var isE_ButtonPushed = false;
var isW_ButtonPushed = false;
var isA_ButtonPushed = false;
var isS_ButtonPushed = false;
var isD_ButtonPushed = false;

var isUP_ButtonPushed = false;
var isDOWN_ButtonPushed = false;
var isRIGHT_ButtonPushed = false;
var isLEFT_ButtonPushed = false;

var isANY_ButtonPushed = false;


function set_keydown(key_code) {
    if (key_code === 'ArrowUp')
        isUP_ButtonPushed = true;
    else if (key_code === 'ArrowDown')
        isDOWN_ButtonPushed = true;
    else if (key_code === 'ArrowRight')
        isRIGHT_ButtonPushed = true;
    else if (key_code === 'ArrowLeft')
        isLEFT_ButtonPushed = true;
    else if (key_code === 'q' || key_code === 'Q')
        isQ_ButtonPushed = true;
    else if (key_code === 'e' || key_code === 'E')
        isE_ButtonPushed = true;
    else if (key_code === 'w' || key_code === 'W')
        isW_ButtonPushed = true;
    else if (key_code === 's' || key_code === 'S')
        isS_ButtonPushed = true;
    else if (key_code === 'a' || key_code === 'A')
        isA_ButtonPushed = true;
    else if (key_code === 'd' || key_code === 'D')
        isD_ButtonPushed = true;

    if (isQ_ButtonPushed || isW_ButtonPushed || isE_ButtonPushed
        || isA_ButtonPushed || isS_ButtonPushed || isD_ButtonPushed
        || isUP_ButtonPushed || isDOWN_ButtonPushed || isLEFT_ButtonPushed || isRIGHT_ButtonPushed)
        isANY_ButtonPushed = true;
}

function set_keyup(key_code) {
    if (key_code === 'ArrowUp')
        isUP_ButtonPushed = false;
    else if (key_code === 'ArrowDown')
        isDOWN_ButtonPushed = false;
    else if (key_code === 'ArrowRight')
        isRIGHT_ButtonPushed = false;
    else if (key_code === 'ArrowLeft')
        isLEFT_ButtonPushed = false;
    else if (key_code === 'q' || key_code === 'Q')
        isQ_ButtonPushed = false;
    else if (key_code === 'e' || key_code === 'E')
        isE_ButtonPushed = false;
    else if (key_code === 'w' || key_code === 'W')
        isW_ButtonPushed = false;
    else if (key_code === 's' || key_code === 'S')
        isS_ButtonPushed = false;
    else if (key_code === 'a' || key_code === 'A')
        isA_ButtonPushed = false;
    else if (key_code === 'd' || key_code === 'D')
        isD_ButtonPushed = false;

    if (!(isQ_ButtonPushed || isW_ButtonPushed || isE_ButtonPushed
        || isA_ButtonPushed || isS_ButtonPushed || isD_ButtonPushed
        || isUP_ButtonPushed || isDOWN_ButtonPushed || isLEFT_ButtonPushed || isRIGHT_ButtonPushed))
        isANY_ButtonPushed = false;
}

function send_control_message(){
    if(isANY_ButtonPushed){
        let json_str = "{\"comm\":["
        let isFirst = true;

        if(isQ_ButtonPushed) {
            ws.send("zmC_PLUS");
            json_str += "\"ZCP\"";
            isFirst = false;
        }
        if(isE_ButtonPushed) {
            ws.send("zmC_MINUS");
            if (isFirst) {
                json_str += "\"ZCM\"";
                isFirst = false;
            } else
                json_str += ",\"ZCM\"";
        }
        if(isW_ButtonPushed) {
            ws.send("mvC_UP");
            if (isFirst) {
                json_str += "\"MCU\"";
                isFirst = false;
            } else
                json_str += ",\"MCU\"";
        }
        if(isS_ButtonPushed) {
            ws.send("mvC_DOWN");
            if (isFirst) {
                json_str += "\"MCD\"";
                isFirst = false;
            } else
                json_str += ",\"MCD\"";

        }
        if(isA_ButtonPushed) {
            ws.send("mvC_LEFT");
            if (isFirst) {
                json_str += "\"MCL\"";
                isFirst = false;
            } else
                json_str += ",\"MCL\"";
        }
        if(isD_ButtonPushed) {
            ws.send("mvC_RIGHT");
            if (isFirst) {
                json_str += "\"MCR\"";
                isFirst = false;
            } else
                json_str += ",\"MCR\"";
        }

        if(isUP_ButtonPushed) {
            ws.send("mvT_UP");
            if (isFirst) {
                json_str += "\"MTU\"";
                isFirst = false;
            } else
                json_str += ",\"MTU\"";
        }
        if(isDOWN_ButtonPushed) {
            ws.send("mvT_DOWN");
            if (isFirst) {
                json_str += "\"MTD\"";
                isFirst = false;
            } else
                json_str += ",\"MTD\"";
        }
        if(isRIGHT_ButtonPushed) {
            ws.send("mvT_RIGHT");
            if (isFirst) {
                json_str += "\"MTR\"";
                isFirst = false;
            } else
                json_str += ",\"MTR\"";
        }
        if(isLEFT_ButtonPushed) {
            ws.send("mvT_LEFT");
            if (isFirst)
                json_str += "\"MTL\"";
            else
                json_str += ",\"MTL\"";
        }
        json_str += "]}";
        console.log(json_str);
    }
}

$(function begin() {
    document.addEventListener('keydown', (e) => {
        if (!e.repeat) {
            set_keydown(e.key);
            send_control_message();
        } else {
            send_control_message();
        }
    });

    document.addEventListener('keyup', (e) => {
       set_keyup(e.key);
    });

    $('#id_form_button_left').click(function onClick(e) {
        //ws.send("mvT_LEFT");
    });
    $('#id_form_button_right').click(function onClick(e) {
        //ws.send("mvT_RIGHT");
    });
    $('#id_form_button_up').click(function onClick(e) {
        //ws.send("mvT_UP");
    });
    $('#id_form_button_down').click(function onClick(e) {
        //ws.send("mvT_DOWN");
    });

    ws = new WebSocket('ws://' + document.location.host + '/chart', ['string', 'foo']);
    let vidstream = document.getElementById("stream");
    let config = { 'iceServers': [{ 'urls': 'stun:stun.l.google.com:19302' }] };

    ws.onopen = function () {
        console.log('onopen');
        //ws.send("GIVE_MQTT_STATUS")
    };

    ws.onclose = function () {
        console.log('onclose');
    };

    //ws.onmessage = function (message) {
    //
    //};

    ws.onerror = function (error) {
        //  add("ERROR: " + error);
    };

    playStream(vidstream, config, function (errmsg) { console.error(errmsg); });
});