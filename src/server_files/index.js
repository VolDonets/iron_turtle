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

function button_handler(key_code) {
    if (key_code === 'ArrowUp') {
        ws.send("mvT_UP");
    } else if (key_code === 'ArrowDown') {
        ws.send("mvT_DOWN");
    } else if (key_code === 'ArrowRight') {
        ws.send("mvT_RIGHT");
    } else if (key_code === 'ArrowLeft') {
        ws.send("mvT_LEFT");
    } else if (key_code === 'q' || key_code === 'Q') {
        ws.send("zmC_MINUS")
    } else if (key_code === 'e' || key_code === 'E') {
        ws.send("zmC_PLUS");
    } else if (key_code === 'w' || key_code === 'W') {
        ws.send("mvC_UP");
    } else if (key_code === 's' || key_code === 'S') {
        ws.send("mvC_DOWN");
    } else if (key_code === 'a' || key_code === 'A') {
        ws.send("mvC_LEFT");
    } else if (key_code === 'd' || key_code === 'D') {
        ws.send("mvC_RIGHT");
    }
}

$(function begin() {
    document.addEventListener('keydown', (e) => {
        if (!e.repeat)
            button_handler(e.key);
        else
            button_handler(e.key);
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