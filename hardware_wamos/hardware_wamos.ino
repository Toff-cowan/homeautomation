
#include <SoftwareSerial.h>
// IMPORT ALL REQUIRED LIBRARIES
#include <NewPing.h>
#include <ArduinoJson.h>
#include <math.h>
   
//**********ENTER IP ADDRESS OF SERVER******************//

#define HOST_IP     "172.17.0.1"       // REPLACE WITH IP ADDRESS OF SERVER ( IP ADDRESS OF COMPUTER THE BACKEND IS RUNNING ON) 
#define HOST_PORT   "8080"            // REPLACE WITH SERVER PORT (BACKEND FLASK API PORT)
#define route       "api/update"      // LEAVE UNCHANGED 
#define idNumber    "620164419"       // REPLACE WITH YOUR ID NUMBER 

// WIFI CREDENTIALS
#define SSID        "MonaConnect"      // "REPLACE WITH YOUR WIFI's SSID"   
#define password    ""  // "REPLACE WITH YOUR WiFi's PASSWORD" 

#define stay        100
 
//**********PIN DEFINITIONS******************//

 
#define espRX         10
#define espTX         11
#define espTimeout_ms 300

#define TRIG 4  // Trigger pin
#define ECHO    5 // Echo pin
#define max 77.763 // Maximum water level/height from the base of the tank
#define cap  1000 // Maximum capacity of the tank in US Gallons
#define tankHeight 94.5 // Ultrasonic sensor height from the base of the tank
#define diameter 61.5 // Diameter of the tank
 
 
/* Declare your functions below */
void espSend(char command[]);
void espUpdate(char mssg[]);
void espInit();
double getWaterHeight(double distance);
double getReserve(double height);

SoftwareSerial esp(espRX, espTX);
NewPing sonar(TRIG, ECHO, max);  
 

void setup(){

  Serial.begin(115200); 
  // Configure GPIO pins here
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
 

  espInit();  
 
}

void loop(){ 
  unsigned int distance = sonar.ping_in();
  Serial.println(distance);
  double waterHeight = getWaterHeight(distance);
  double reserve = getReserve(waterHeight);
  double percentage = waterHeight / max * 100;
  // send updates with schema ‘{"id": "student_id", "type": "ultrasonic", "radar": 0, "waterheight": 0, "reserve": 0, "percentage": 0}’

  JsonDocument doc; // Create JSon object
      
  // 2. Create message buffer/array to store serialized JSON object
  char message[290]  = {0};

  doc["id"] = "620162206";
  doc["type"] = "ultrasonic";
  doc["radar"] = distance;
  doc["waterheight"] = waterHeight;
  doc["reserve"] = reserve;
  doc["percentage"] = percentage;

  serializeJson(doc, message);

  espUpdate(message);

  delay(1000);  
}

 
void espSend(char command[] ){   
    esp.print(command); // send the read character to the esp    
    while(esp.available()){ Serial.println(esp.readString());}    
}


void espUpdate(char mssg[]){ 
    char espCommandString[50] = {0};
    char post[290]            = {0};

    snprintf(espCommandString, sizeof(espCommandString),"AT+CIPSTART=\"TCP\",\"%s\",%s\r\n",HOST_IP,HOST_PORT); 
    espSend(espCommandString);    //starts the connection to the server
    delay(stay);

    // GET REQUEST 
    // snprintf(post,sizeof(post),"GET /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/x-www-form-urlencoded\r\nContent-Length: %d\r\n\r\n%s\r\n\r\n",route,HOST_IP,strlen(mssg),mssg);

    // POST REQUEST
    snprintf(post,sizeof(post),"POST /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: application/json\r\nContent-Length: %d\r\n\r\n%s\r\n\r\n",route,HOST_IP,strlen(mssg),mssg);
  
    snprintf(espCommandString, sizeof(espCommandString),"AT+CIPSEND=%d\r\n", strlen(post));
    espSend(espCommandString);    //sends post length
    delay(stay);
    Serial.println(post);
    espSend(post);                //sends POST request with the parameters 
    delay(stay);
    espSend("AT+CIPCLOSE\r\n");   //closes server connection
   }

void espInit(){
    char connection[100] = {0};
    esp.begin(115200); 
    Serial.println("Initiallizing");
    esp.println("AT"); 
    delay(1000);
    esp.println("AT+CWMODE=1");
    delay(1000);
    while(esp.available()){ Serial.println(esp.readString());} 

    snprintf(connection, sizeof(connection),"AT+CWJAP=\"%s\",\"%s\"\r\n",SSID,password);
    esp.print(connection);

    delay(3000);  //gives ESP some time to get IP

    if(esp.available()){   Serial.print(esp.readString());}
    
    Serial.println("\nFinish Initializing");    
   
}

//***** Design and implement all util functions below ******
double getWaterHeight(double distance){
  return tankHeight - distance + 16.737;
}

double getReserve(double height){
  return M_PI * pow(diameter / 2.0, 2) * height / 231.0;
}
