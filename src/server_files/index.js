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
        alert("You canNOT connect to the server, you are an excess client.\nPlease wait till you be able to connect!");

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
var isZERO_ButtonPushed = false;

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
    else if (key_code === '0')
        isZERO_ButtonPushed = true;

    if (isQ_ButtonPushed || isW_ButtonPushed || isE_ButtonPushed
        || isA_ButtonPushed || isS_ButtonPushed || isD_ButtonPushed
        || isUP_ButtonPushed || isDOWN_ButtonPushed || isLEFT_ButtonPushed
        || isRIGHT_ButtonPushed || isZERO_ButtonPushed)
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
    else if (key_code === '0')
        isZERO_ButtonPushed = false;

    if (!(isQ_ButtonPushed || isW_ButtonPushed || isE_ButtonPushed
        || isA_ButtonPushed || isS_ButtonPushed || isD_ButtonPushed
        || isUP_ButtonPushed || isDOWN_ButtonPushed || isLEFT_ButtonPushed
        || isRIGHT_ButtonPushed || isZERO_ButtonPushed))
        isANY_ButtonPushed = false;
}

function send_control_message(){
    if(isANY_ButtonPushed){
        let json_str = "{\"comm\":["
        let isFirst = true;

        if(isQ_ButtonPushed) {
            json_str += "\"ZCP\"";
            isFirst = false;
        }
        if(isE_ButtonPushed) {
            if (isFirst) {
                json_str += "\"ZCM\"";
                isFirst = false;
            } else
                json_str += ",\"ZCM\"";
        }
        if(isW_ButtonPushed) {
            if (isFirst) {
                json_str += "\"MCU\"";
                isFirst = false;
            } else
                json_str += ",\"MCU\"";
        }
        if(isS_ButtonPushed) {
            if (isFirst) {
                json_str += "\"MCD\"";
                isFirst = false;
            } else
                json_str += ",\"MCD\"";

        }
        if(isA_ButtonPushed) {
            if (isFirst) {
                json_str += "\"MCL\"";
                isFirst = false;
            } else
                json_str += ",\"MCL\"";
        }
        if(isD_ButtonPushed) {
            if (isFirst) {
                json_str += "\"MCR\"";
                isFirst = false;
            } else
                json_str += ",\"MCR\"";
        }

        if(isUP_ButtonPushed) {
            if (isFirst) {
                json_str += "\"MTU\"";
                isFirst = false;
            } else
                json_str += ",\"MTU\"";
        }
        if(isDOWN_ButtonPushed) {
            if (isFirst) {
                json_str += "\"MTD\"";
                isFirst = false;
            } else
                json_str += ",\"MTD\"";
        }
        if(isRIGHT_ButtonPushed) {
            // here inverse button behavior, so if push the right button the iron turtle moves left,
            // so we should send here the 'left' command
            if (isFirst) {
                // json_str += "\"MTR\"";
                json_str += "\"MTL\"";
                isFirst = false;
            } else {
                // json_str += ",\"MTR\"";
                json_str += ",\"MTL\"";
            }
        }
        if(isLEFT_ButtonPushed) {
            // here inverse button behavior, so if push the left button the iron turtle moves right,
            // so we should send here the 'right' command
            if (isFirst) {
                //json_str += "\"MTL\"";
                json_str += "\"MTR\"";
                isFirst = false;
            } else {
                //json_str += ",\"MTL\"";
                json_str += ",\"MTR\"";
            }
        }
        if(isZERO_ButtonPushed) {
            if (isFirst) {
                json_str += "\"MTS\"";
            } else {
                json_str += ",\"MTS\"";
            }
        }
        json_str += "]}";
        ws.send(json_str);
    }
}

function scheduled_sender(){
    ws.send("{\"comm\":[\"OK\"]}");
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
    };

    ws.onclose = function () {
        console.log('onclose');
    };

    ws.onerror = function (error) {
    };

    playStream(vidstream, config, function (errmsg) { console.error(errmsg); });
    let messageSender = setInterval(scheduled_sender, 100);
});