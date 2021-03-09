#ifndef WEBSERVERZ_H
#define WEBSERVERZ_H

#include <WebServer.h>

char webpage[] PROGMEM = R"=====(
<html>
<!-- Adding a data chart using Chart.js -->
<head>
  <!--script src='https://cdnjs.cloudflare.com/ajax/libs/Chart.js/2.5.0/Chart.min.js'></script-->
</head>
<body onload="javascript:init()">

  
<button onclick="fetch('/webRadio');">Web radio</button>
<button onclick="fetch('/next');">Next stream</button>
<button onclick="fetch('/prev');">Previous stream</button>
<div>
    <label >Audio volume</label>
    <input type="range" min="0" max="21" value="10"
        onchange="fetch('/volume',{method: 'POST', body: this.value});">
</div>
<div>
  <label >add station</label>
  <input type="text"
      onchange="fetch('/station',{method: 'POST', body: this.value});">
</div>

<button onclick="fetch('/bluetooth');">Bluetooth</button>
<button onclick="fetch('/nextPalette');">Next palette</button>
<button onclick="fetch('/prevPalette');">Previous palette</button>
<button onclick="fetch('/fft',{method: 'POST', body: 'false'});">Normal</button>
<button onclick="fetch('/fft',{method: 'POST', body: 'true'});">Fft</button>




<!--h2>Browser Based ESP32-EYE Spectrum Analyzer</h2>
<div>
  <canvas id="chart" width="600" height="400"></canvas>
</div-->
<!-- Adding a websocket to the client (webpage) -->
<script>
  var webSocket, dataPlot;
  var maxDataPoints = 20;
  const maxValue = 200000000;
  const maxLow = maxValue * 0.5;
  const maxMedium = maxValue * 0.2;
  const maxHigh = maxValue * 0.3;
  function init() {
    webSocket = new WebSocket('ws://' + window.location.hostname + ':81/');
    /*dataPlot = new Chart(document.getElementById("chart"), {
      type: 'bar',
      data: {
        labels: [],
        datasets: [{
          data: [],
          label: "Low",
          backgroundColor: "#D6E9C6"
        },
        {
          data: [],
          label: "Moderate",
          backgroundColor: "#FAEBCC"
        },
        {
          data: [],
          label: "High",
          backgroundColor: "#EBCCD1"
        },
        ]
      }, 
      options: {
          responsive: false,
          animation: false,
          scales: {
              xAxes: [{ stacked: true }],
              yAxes: [{
                  display: true,
                  stacked: true,
                  ticks: {
                    beginAtZero: true,
                    steps: 1000,
                    stepValue: 500,
                    max: maxValue
                  }
              }]
           }
       }
    });*/
    webSocket.onmessage = function(event) {
      var data = JSON.parse(event.data);
      console.log(data);
      /*dataPlot.data.labels = [];
      dataPlot.data.datasets[0].data = [];
      dataPlot.data.datasets[1].data = [];
      dataPlot.data.datasets[2].data = [];
      
      data.forEach(function(element) {
        dataPlot.data.labels.push(element.bin);
        var lowValue = Math.min(maxLow, element.value);
        dataPlot.data.datasets[0].data.push(lowValue);
        
        var mediumValue = Math.min(Math.max(0, element.value - lowValue), maxMedium);
        dataPlot.data.datasets[1].data.push(mediumValue);
        
        var highValue = Math.max(0, element.value - lowValue - mediumValue);
        dataPlot.data.datasets[2].data.push(highValue);
      });
      dataPlot.update();*/
    }
  }
</script>
</body>
</html>
)=====";



class WebServerz {
    WebServer server;
    // char * webpage;
public:

    WebServerz() {
      // server = WebServer(80);
    }

    void begin(/* void (*handleVolume)(uint8_t), uint8_t (*handleNext)(), uint8_t (*handlePrev)() */) {
        Serial.println("Starting HTTP server");

        server.on("/",[this](){
          Serial.println("HTTP server root requested");
          server.send_P(200, "text/html", webpage);
        });

/*         server.on("/volume", [handleVolume, this]() {
            String volume = server.arg(0);
            handleVolume(volume.toInt());
            server.send(200, "text/plain", "Volume changed: " + volume);
        });
        server.on("/next", [handleNext, this]() {
            uint8_t station = handleNext();
            server.send(200, "text/plain", "New station: " + station); // FIXME conversion
        });
        server.on("/prev", [handlePrev, this]() {
            uint8_t station = handlePrev();
            server.send(200, "text/plain", "New station: " + station); // FIXME conversion
        }); */

        server.begin();

        Serial.println("HTTP server started");
    }

    void on(String path, String (*handler)(String)) {
        server.on(path,[this, handler]() {
          String body = server.arg(0);
          String response = handler(body);
          server.send(200, "text/plain", response);
        });
    }

    void handleClient() {
        server.handleClient();
    }

};

#endif