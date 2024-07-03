#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <HTTPSRedirect.h>

const char *GScriptId = "AKfycbw1pP76DxM-ZzXlCO7IDU8d5p9SAMcaEI1CAVVaiy39Sek894LTwq64mM9_QK7FhAwR";

const char* ssid     = "IOT-PROJECTS";
const char* password = "IOT@12345";

String payload_base =  "{\"command\": \"insert_row\", \"sheet_name\": \"Sheet1\", \"values\": ";
String payload = "";

const char* host        = "script.google.com";
const int   httpsPort   = 443;
const char* fingerprint = "";
String url = String("/macros/s/") + GScriptId + "/exec";
HTTPSRedirect* client = nullptr;
//------------------------------------------------------------
// Declare variables that will be published to Google Sheets


void setup() {
  //----------------------------------------------------------
  Serial.begin(9600);        
  delay(10);
  Serial.println('\n');
  WiFi.begin(ssid, password);             
  Serial.print("Connecting to ");
  Serial.print(ssid); 
  Serial.println(" ...");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
  
  Serial.print("Connecting to ");
  Serial.println(host);
  bool flag = false;
  for(int i=0; i<5; i++){ 
    int retval = client->connect(host, httpsPort);
    if (retval == 1){
      flag = true;
      String msg = "Connected To Google Spreadsheet. OK";
      Serial.println(msg);
      break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }
  if (!flag){
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    delay(5000);
    return;
  }
  delete client;
  client = nullptr; 
}

bool srlprt=false;

void sendToGoogleSheet(String cardHolderID)
{
  static bool flag = false;
  if (!flag){
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    flag = true;
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
  }
  if (client != nullptr){
    if (!client->connected())
      {client->connect(host, httpsPort);}
  }
  else{Serial.println("Error creating client object!");}

    url =  "id=" + cardHolderID;
    // Create json object string to send to Google Sheets
    // values = "\"" + value0 + "," + value1 + "," + value2 + "\"}"
    Serial.println("Publishing data...");
    Serial.println(payload);
    if(client->GET(url, host)){ 
      Serial.println("Succesfully Uploaded");
    }
    else{
      Serial.println("Error while connecting");
    }
}

void loop() {
  
  String cardHolderName=String('\0');
  
  if(!srlprt)
  {
    Serial.println("Place The Tag");
    srlprt=true;
  }
  if (Serial.available() > 0) {
    cardHolderName = Serial.readString();
    Serial.println(cardHolderName);
    //delay(200);
  }
  if(cardHolderName.length()==12)
  {
    Serial.println("Sending To Google Sheet.......Pls Wait");
    sendToGoogleSheet(cardHolderName);
    cardHolderName=String('\0');
    srlprt=false;
    //delay(2000);
  }
  
}
