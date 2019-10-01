#include "NexUploadWIFI.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"

// Create Nextion WiFi Uploader object
NexUploadWIFI nex_uploader(115200);

// Create Web Server
AsyncWebServer server(80);

// Credentials for WiFi connection
const char* ssid     = "ASUS";
const char* password = "special5";

// Async Web Server Requests Handler
void webServerHandler() {
  // Return web page for Firmware upload
  server.on("/f", HTTP_GET, [](AsyncWebServerRequest * request){
    AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->print("<!DOCTYPE html><html><head> <script type='text/javascript'>");
    
    response->print("var file,cmp,partSize=100,offset=0;function upload(){file=document.querySelector('input[name=file]').files[0],partSize=parseInt(document.querySelector('input[name=partSize]').value),cmp=document.getElementById('completed'),file?sendInfo(file.size):alert('Choose firmware file.')}function sendDataHandler(e){if(null==e.target.error){cmp.innerText=(100*offset/file.size).toFixed(1)+'%',offset+=e.target.result.byteLength;var t=new XMLHttpRequest;t.onreadystatechange=function(){4==t.readyState&&200==t.status&&(offset<file.size?sendData():cmp.innerText='All data was sent')},t.open('post','/u'),t.send(e.target.result)}else alert('Error: '+e.target.error)}function sendData(){var e=new FileReader,t=file.slice(offset,partSize+offset);e.onload=sendDataHandler,e.readAsArrayBuffer(t)}function sendInfo(e){var t=new XMLHttpRequest;t.onreadystatechange=function(){4==t.readyState&&200==t.status&&sendData()},t.open('post','/s'),t.send(e)}");
    
    response->print("</script> </head> <body> <input type='file' name='file'/> <input type='button' value='upload' onclick='upload()'/> <br/><label id='completed'></label><br/> Chunk size: <input type='text' name='partSize' value='1024' size='4'/> </body></html>");
    request->send(response);
  });

  // Recive Firmware file size
  server.on("/s", HTTP_POST, [](AsyncWebServerRequest * request){},
    NULL,
    [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {

      uint32_t fsize = atoi((const char*)data);
      nex_uploader.check(fsize);
 
      request->send(200);
  });

  // Recive Firmware cunks and flash Nextion
  server.on("/u", HTTP_POST, [](AsyncWebServerRequest * request){},
    NULL,
    [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
 
      nex_uploader.uploadTftFile(data, len);
 
      request->send(200);
  });
 
  server.begin();
}

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  webServerHandler();
}

void loop() {
  // put your main code here, to run repeatedly:
}
