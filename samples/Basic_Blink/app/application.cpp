#include <user_config.h>
#include <SmingCore.h>

#ifndef WIFI_SSID
    #define WIFI_SSID "" // Put you SSID and Password here
    #define WIFI_PWD ""
#endif

HttpClient myhttpclient;

void onSecondRequest (HttpClient& client, bool successful)
{
    if (!successful) { Serial.println("Failed"); return; }

    String response = client.getResponseString();
    Serial.println("onSecondRequest - Server response: '" + response.substring(1,30) + "'\n\r");
}

void onFirstRequest(HttpClient& client, bool successful) {
    if (!successful) { Serial.println("Failed"); return; }

    String response = client.getResponseString();
    Serial.println("onFirstRequest - Server response: '" + response.substring(1,30) + "'\n\r");


    /*********This will show that it has finished processing*************/
    Serial.println ("onFirstRequest - isProcessing: " + String(client.isProcessing() ));



    /*********The following request will cause exception***********/
    Serial.println("Sending second request");
    myhttpclient.downloadString("http://mail.tauri.nl", onSecondRequest);
}


void connectOk() {
    Serial.println("Sending first request");
    myhttpclient.downloadString("http://mail.tauri.nl", onFirstRequest);
}



void init() {
    Serial.begin(SERIAL_BAUD_RATE);
    Serial.systemDebugOutput(true);

    WifiAccessPoint.enable(false);

    WifiStation.enable(true);
    WifiStation.config(WIFI_SSID, WIFI_PWD); // Put you SSID and Password here

    WifiStation.waitConnection(connectOk);
}
