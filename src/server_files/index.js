var ws;

$(function begin() {
    $('#id_form_button_left').click(function onClick(e) {
        ws.send("mvT_LEFT");
    });
    $('#id_form_button_right').click(function onClick(e) {
        ws.send("mvT_RIGHT");
    });
    $('#id_form_button_up').click(function onClick(e) {
        ws.send("mvT_UP");
    });
    $('#id_form_button_down').click(function onClick(e) {
        ws.send("mvT_DOWN");
    });

    ws = new WebSocket('ws://' + document.location.host + '/chart', ['string', 'foo']);
    ws.onopen = function () {
        console.log('onopen');
        //ws.send("GIVE_MQTT_STATUS")
    };

    ws.onclose = function () {
        console.log('onclose');
    };

    ws.onmessage = function (message) {
        /*var message_data = message.data;
        if (message_data === "MQTT_STATUS_ENABLE") {
            isEnabledMQTT = true;
            buttonStatusMQTT.value = "DISABLE MQTT (actually ENABLED)"
        } if(message_data === "MQTT_STATUS_DISABLE"){
            isEnabledMQTT = false;
            buttonStatusMQTT.value = "ENABLE MQTT (actually DISABLED)"
        }else {

            var json_obj = JSON.parse(message_data);

            let valueAccelX = json_obj.accelX;
            let valueAccelY = json_obj.accelY;
            let valueAccelZ = json_obj.accelZ;
            if (Number.isFinite(valueAccelX) && Number.isFinite(valueAccelY) && Number.isFinite(valueAccelZ)) {
                dataAccelX.unshift(valueAccelX);
                dataAccelY.unshift(valueAccelY);
                dataAccelZ.unshift(valueAccelZ);
                if (dataAccelX > 20) {
                    dataAccelX.pop();
                    dataAccelY.pop();
                    dataAccelZ.pop();
                }
                chartAccel.update();
            }

            let valueGyroX = json_obj.gyroX;
            let valueGyroY = json_obj.gyroY;
            let valueGyroZ = json_obj.gyroZ;
            if (Number.isFinite(valueGyroX) && Number.isFinite(valueGyroY) && Number.isFinite(valueGyroZ)) {
                dataGyroX.unshift(valueGyroX);
                dataGyroY.unshift(valueGyroY);
                dataGyroZ.unshift(valueGyroZ);
                if (dataGyroX > 20) {
                    dataGyroX.pop();
                    dataGyroY.pop();
                    dataGyroZ.pop();
                }
                chartGyro.update();
            }

            let valueT = json_obj.temperature;
            if (Number.isFinite(valueT)) {
                dataTemperature.unshift(valueT);
                if (dataTemperature.length > 20) {
                    dataTemperature.pop();
                }
                chartTemperature.update();
            }

            let valueP = json_obj.pressure;
            if (Number.isFinite(valueP)) {
                dataPressure.unshift(valueP);
                if (dataPressure.length > 20) {
                    dataPressure.pop();
                }
                chartPressure.update();
            }

            let valueH = json_obj.humidity;
            if (Number.isFinite(valueH)) {
                dataHumidity.unshift(valueH);
                if (dataPressure.length > 20) {
                    dataHumidity.pop();
                }
                chartHumidity.update();
            }

            let valueA = json_obj.altitude;
            if (Number.isFinite(valueA)) {
                dataAltitude.unshift(valueA);
                if (dataAltitude.length > 20) {
                    dataAltitude.pop();
                }
                chartAltitude.update();
            }
        }*/
    };

    ws.onerror = function (error) {
        //  add("ERROR: " + error);
    };
});