/*
   Copyright (c) 2020 Peter Geher.  All rights reserved.
   See the bottom of this file for the license terms.
   peter.geher.dev@nsa2go.de
*/

/*
   Wifi-TEA5767 Controller with OTA (Over The Air)-Update
*/

#include <TEA5767.h>
#include <ESP8266WiFi.h>  //Wifi

// Enable oder disable OTA, maybe it works.
// #define OTA
#ifdef OTA
#include <ESP8266mDNS.h>  //OTA
#include <WiFiUdp.h>      //OTA
#include <ArduinoOTA.h>   //OTA
#endif

// Enable oder Disable Serial-Output
// #define serial


// Local Frequencys
float einslive = 106.7; // EinsLive
float wdr2 = 99.20; // WDR2
float wdr3 = 95.10; // WDR3
float wdr4 = 101.30; // WDR4
float wdr5 = 88.80; // WDR5
float cosmo = 103.30; // Cosmo
float radioen = 105.70; // WDR4


// SSID and Password from your Wireless Network
const char* ssid = "SSID";
const char* password = "PASSWORD";

String MuteState = "off";


TEA5767 radio = TEA5767();

WiFiServer server(80);    // Listen http-Port

void setup() {
  Wire.begin();


  // Enable or disable Serial-Output
#ifdef serial
  Serial.begin(115200);
#endif

  radio.setFrequency(wdr4); // Startup-Station

  WiFi.mode(WIFI_STA);    //OTA
  delay(10);

  // Connect to network...
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(250);         // Print dots while waiting for connection :-)
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connectet to: ");
  Serial.println(ssid);

  // Start the server
  server.begin();
  Serial.println("Server started successfully!");

  // Print IP-Address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
#ifdef OTA
  ArduinoOTA.begin();     //OTA
#endif
}

void loop() {
#ifdef OTA
  ArduinoOTA.handle();    //OTA
#endif

  // New Client
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Waiting...
  Serial.println("Client Connected:");
  while (!client.available()) {
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  // requests...
  if (request.indexOf("/einslive") != -1)  {
    Serial.print("Requested from: ");
    Serial.print(client.remoteIP());
    Serial.println(" ");
    radio.setFrequency(einslive);
    server.stop();
    delay(10);
    server.begin();

  }

  if (request.indexOf("/wdr2") != -1)  {
    Serial.print("Requested from: ");
    Serial.print(client.remoteIP());
    Serial.println(" ");
    radio.setFrequency(wdr2);
    server.stop();
    delay(10);
    server.begin();
  }

  if (request.indexOf("/wdr3") != -1)  {
    Serial.print("Requested from: ");
    Serial.print(client.remoteIP());
    Serial.println(" ");
    radio.setFrequency(wdr3);
    server.stop();
    delay(10);
    server.begin();
  }

  if (request.indexOf("/wdr4") != -1)  {
    Serial.print("Requested from: ");
    Serial.print(client.remoteIP());
    Serial.println(" ");
    radio.setFrequency(wdr4);
    server.stop();
    delay(10);
    server.begin();
  }

  if (request.indexOf("/wdr5") != -1)  {
    Serial.print("Requested from: ");
    Serial.print(client.remoteIP());
    Serial.println(" ");
    radio.setFrequency(wdr5);
    server.stop();
    delay(10);
    server.begin();
  }

  if (request.indexOf("/cosmo") != -1)  {
    Serial.print("Requested from: ");
    Serial.print(client.remoteIP());
    Serial.println(" ");
    radio.setFrequency(cosmo);
    server.stop();
    delay(10);
    server.begin();
  }


  if (request.indexOf("/radioen") != -1)  {
    Serial.print("Requested from: ");
    Serial.print(client.remoteIP());
    Serial.println(" ");
    radio.setFrequency(radioen);
    server.stop();
    delay(10);
    server.begin();
  }

  if (request.indexOf("/stumm") != -1)  {
    Serial.print("Requested from: ");
    Serial.print(client.remoteIP());
    Serial.println(" ");
    radio.setMuted(true);
    MuteState = "on";
    server.stop();
    delay(10);
    server.begin();
  }

  if (request.indexOf("/normal") != -1)  {
    Serial.print("Requested from: ");
    Serial.print(client.remoteIP());
    Serial.println(" ");
    radio.setMuted(false);
    MuteState = "off";
    server.stop();
    delay(10);
    server.begin();
  }


  // Build the "WebInterface"
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  Must be important!...
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<title>Bad-Radio</title>");

  // Should still be tidied up a little and sorted. But ... it works for me :-)
  client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
  client.println("<link rel=\"icon\" href=\"data:,\">");

  client.println("<style>html { font-family: Helvetica; background-color: grey; display: inline-block; margin: 0px auto; text-align:}");
  client.println(".button { background-color: #1C91FF; border: none; color: white; padding: 25px 80px;");
  client.println("text-decoration: none; font-size: 50px; margin: 2px; cursor: pointer;}");
  client.println(".buttoncosmo {background-color: #1C91FF; padding: 25px 59px;}");
  client.println(".buttonmuteon {background-color: #FF0000; padding: 25px 86px;}");
  client.println(".buttonmuteoff {background-color: #1C91FF; padding: 25px 78px;}");
  client.println(".buttonlive {background-color: #1C91FF; padding: 25px 67px;}");
  client.println(".buttonren {background-color: #1C91FF; padding: 25px 62px;}");
  client.println("</style></head>");

  // Einslive
  client.println("<p><a href=\"/einslive\"><button class=\"button buttonlive\">Eins Live</button></a></p>");

  // WDR 2
  client.println("<p><a href=\"/wdr2\"><button class=\"button\">W  D  R  2</button></a></p>");

  // WDR3
  client.println("<p><a href=\"/wdr3\"><button class=\"button\">W  D  R  3</button></a></p>");

  // WDR4
  client.println("<p><a href=\"/wdr4\"><button class=\"button\">W  D  R  4</button></a></p>");

  // WDR5
  client.println("<p><a href=\"/wdr5\"><button class=\"button\">W  D  R  5</button></a></p>");

  // WDR Cosmo
  client.println("<p><a href=\"/cosmo\"><button class=\"button buttoncosmo\">C o s m o</button></a></p>");

  // Radio EN
  client.println("<p><a href=\"/radioen\"><button class=\"button buttonren\">Radio EN</button></a></p>");

  // Muter
  if (MuteState == "off") {
    client.println("<p><a href=\"/stumm\"><button class=\"button buttonmuteoff\">Ton aus</button></a></p>");
  } else {
    client.println("<p><a href=\"/normal\"><button class=\"button buttonmuteon\">Ton ein</button></a></p>");
  }

  client.println();
  delay(1);
  short lvl = radio.getSignalLevel();
  Serial.println("Client disconnected");
  client.println("<br>");
  delay(250);

  client.print("&nbsp;  &nbsp;  &nbsp;  &nbsp; &nbsp;  &nbsp; Signalst&auml;rke: ");
  client.print(lvl);
  client.println("<a href=\"/\"\"></a>");
  client.println("</html>");
}


/*
   Copyright (c) 2020 Peter Geher

   This Code is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This Code is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this Code; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/
