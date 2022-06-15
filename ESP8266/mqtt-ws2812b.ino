#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <PubSubClient.h>

#define LED_PIN    D5
#define LED_COUNT 256
#define mqtt_server "broker.hivemq.com"
#define DEFAULT_ROW 16
const uint16_t mqtt_port = 1883;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
ESP8266WiFiMulti WiFiMulti;
WiFiClient espClient;
PubSubClient client(espClient);

class Vecto{
  public:
    int v[256] = { 0 };
  public:
    void setElement(byte i, int j);
    int getElement(byte i);
};
void Vecto::setElement(byte i, int j){
  v[i] = j;
}
int Vecto::getElement(byte i){
  return v[i];
}

class Display{
  public:
    byte row = DEFAULT_ROW;
    byte col = 16;
    int table[DEFAULT_ROW][256] = { 0x000000 };
  public:
    Display();
    Display(byte c);
    void setColumn(byte c);
    void setColor(byte i, byte j, int color);
    void resetColor();
    void decrease();
    Vecto convert();
};
Display::Display(){
  col = 96;
  table[1][1]=0xffff00;
  table[1][2]=0xffff00;
  table[1][3]=0xffff00;
  table[1][4]=0xffff00;
  table[1][5]=0xffff00;
  table[1][6]=0xffff00;
  table[1][7]=0xffff00;
  table[1][9]=0xffff00;
  table[1][16]=0xff0000;
  table[1][17]=0xff0000;
  table[1][19]=0xff0000;
  table[1][20]=0xff0000;
  table[1][45]=0xff0000;
  table[1][46]=0xff0000;
  table[1][48]=0xff0000;
  table[1][49]=0xff0000;
  table[2][4]=0xffff00;
  table[2][9]=0xffff00;
  table[2][15]=0xff0000;
  table[2][16]=0xff0000;
  table[2][17]=0xff0000;
  table[2][18]=0xff0000;
  table[2][19]=0xff0000;
  table[2][20]=0xff0000;
  table[2][21]=0xff0000;
  table[2][44]=0xff0000;
  table[2][45]=0xff0000;
  table[2][46]=0xff0000;
  table[2][47]=0xff0000;
  table[2][48]=0xff0000;
  table[2][49]=0xff0000;
  table[2][50]=0xff0000;
  table[3][4]=0xffff00;
  table[3][8]=0xffff00;
  table[3][15]=0xff0000;
  table[3][16]=0xff0000;
  table[3][17]=0xff0000;
  table[3][18]=0xff0000;
  table[3][19]=0xff0000;
  table[3][20]=0xff0000;
  table[3][21]=0xff0000;
  table[3][34]=0xffff00;
  table[3][44]=0xff0000;
  table[3][45]=0xff0000;
  table[3][46]=0xff0000;
  table[3][47]=0xff0000;
  table[3][48]=0xff0000;
  table[3][49]=0xff0000;
  table[3][50]=0xff0000;
  table[3][54]=0xffff00;
  table[4][4]=0xffff00;
  table[4][15]=0xff0000;
  table[4][16]=0xff0000;
  table[4][17]=0xff0000;
  table[4][18]=0xff0000;
  table[4][19]=0xff0000;
  table[4][20]=0xff0000;
  table[4][21]=0xff0000;
  table[4][34]=0xffff00;
  table[4][44]=0xff0000;
  table[4][45]=0xff0000;
  table[4][46]=0xff0000;
  table[4][47]=0xff0000;
  table[4][48]=0xff0000;
  table[4][49]=0xff0000;
  table[4][50]=0xff0000;
  table[4][54]=0xffff00;
  table[5][4]=0xffff00;
  table[5][16]=0xff0000;
  table[5][17]=0xff0000;
  table[5][18]=0xff0000;
  table[5][19]=0xff0000;
  table[5][20]=0xff0000;
  table[5][34]=0xffff00;
  table[5][45]=0xff0000;
  table[5][46]=0xff0000;
  table[5][47]=0xff0000;
  table[5][48]=0xff0000;
  table[5][49]=0xff0000;
  table[5][54]=0xffff00;
  table[6][4]=0xffff00;
  table[6][17]=0xff0000;
  table[6][18]=0xff0000;
  table[6][19]=0xff0000;
  table[6][34]=0xffff00;
  table[6][46]=0xff0000;
  table[6][47]=0xff0000;
  table[6][48]=0xff0000;
  table[6][54]=0xffff00;
  table[7][4]=0xffff00;
  table[7][18]=0xff0000;
  table[7][34]=0xffff00;
  table[7][47]=0xff0000;
  table[7][54]=0xffff00;
  table[8][4]=0xffff00;
  table[8][9]=0xffff00;
  table[8][11]=0xffff00;
  table[8][13]=0xffff00;
  table[8][20]=0xffff00;
  table[8][21]=0xffff00;
  table[8][22]=0xffff00;
  table[8][25]=0xffff00;
  table[8][26]=0xffff00;
  table[8][27]=0xffff00;
  table[8][34]=0xffff00;
  table[8][37]=0xffff00;
  table[8][38]=0xffff00;
  table[8][39]=0xffff00;
  table[8][42]=0xffff00;
  table[8][44]=0xffff00;
  table[8][45]=0xffff00;
  table[8][49]=0xffff00;
  table[8][50]=0xffff00;
  table[8][51]=0xffff00;
  table[8][54]=0xffff00;
  table[8][56]=0xffff00;
  table[8][60]=0xffff00;
  table[9][4]=0xffff00;
  table[9][9]=0xffff00;
  table[9][10]=0xffff00;
  table[9][11]=0xffff00;
  table[9][12]=0xffff00;
  table[9][13]=0xffff00;
  table[9][19]=0xffff00;
  table[9][24]=0xffff00;
  table[9][28]=0xffff00;
  table[9][34]=0xffff00;
  table[9][36]=0xffff00;
  table[9][40]=0xffff00;
  table[9][42]=0xffff00;
  table[9][43]=0xffff00;
  table[9][46]=0xffff00;
  table[9][48]=0xffff00;
  table[9][52]=0xffff00;
  table[9][54]=0xffff00;
  table[9][56]=0xffff00;
  table[9][60]=0xffff00;
  table[10][4]=0xffff00;
  table[10][9]=0xffff00;
  table[10][11]=0xffff00;
  table[10][13]=0xffff00;
  table[10][20]=0xffff00;
  table[10][21]=0xffff00;
  table[10][24]=0xffff00;
  table[10][28]=0xffff00;
  table[10][34]=0xffff00;
  table[10][36]=0xffff00;
  table[10][40]=0xffff00;
  table[10][42]=0xffff00;
  table[10][46]=0xffff00;
  table[10][48]=0xffff00;
  table[10][49]=0xffff00;
  table[10][50]=0xffff00;
  table[10][51]=0xffff00;
  table[10][54]=0xffff00;
  table[10][56]=0xffff00;
  table[10][60]=0xffff00;
  table[11][4]=0xffff00;
  table[11][9]=0xffff00;
  table[11][11]=0xffff00;
  table[11][13]=0xffff00;
  table[11][22]=0xffff00;
  table[11][24]=0xffff00;
  table[11][28]=0xffff00;
  table[11][34]=0xffff00;
  table[11][36]=0xffff00;
  table[11][40]=0xffff00;
  table[11][42]=0xffff00;
  table[11][46]=0xffff00;
  table[11][48]=0xffff00;
  table[11][54]=0xffff00;
  table[11][56]=0xffff00;
  table[11][60]=0xffff00;
  table[12][1]=0xffff00;
  table[12][2]=0xffff00;
  table[12][3]=0xffff00;
  table[12][4]=0xffff00;
  table[12][5]=0xffff00;
  table[12][6]=0xffff00;
  table[12][7]=0xffff00;
  table[12][9]=0xffff00;
  table[12][11]=0xffff00;
  table[12][13]=0xffff00;
  table[12][19]=0xffff00;
  table[12][20]=0xffff00;
  table[12][21]=0xffff00;
  table[12][25]=0xffff00;
  table[12][26]=0xffff00;
  table[12][27]=0xffff00;
  table[12][34]=0xffff00;
  table[12][37]=0xffff00;
  table[12][38]=0xffff00;
  table[12][39]=0xffff00;
  table[12][42]=0xffff00;
  table[12][46]=0xffff00;
  table[12][49]=0xffff00;
  table[12][50]=0xffff00;
  table[12][51]=0xffff00;
  table[12][52]=0xffff00;
  table[12][54]=0xffff00;
  table[12][57]=0xffff00;
  table[12][58]=0xffff00;
  table[12][59]=0xffff00;
  table[12][60]=0xffff00;
  table[13][60]=0xffff00;
  table[14][56]=0xffff00;
  table[14][60]=0xffff00;
  table[15][57]=0xffff00;
  table[15][58]=0xffff00;
  table[15][59]=0xffff00;
}
Display::Display(byte c){
  col = c;
}
void Display::setColumn(byte c){
  if (c>256){
    String alert = "Out of column range";
  }else col = c;
}
void Display::setColor(byte i, byte j, int color){
  table[i][j] = color;
}
void Display::resetColor(){
  for (byte i=0; i<row; i++)
    for (byte j=0; j<col; j++) table[i][j] = 0x000000;
}
void Display::decrease(){
  int tmp[16];
  for (byte i=0; i<row; i++) tmp[i] = table[i][0];
  for (byte j=1; j<col; j++)
    for (byte i=0; i<row; i++) table[i][j-1] = table[i][j];
  for (byte i=0; i<row; i++) table[i][col-1] = tmp[i];
}
Vecto Display::convert(){
  Vecto v;
  for (byte i=0; i<16; i++)
    for (byte j=0; j<16; j++)
      if (i%2==0) v.setElement(16*i+j,table[i][15-j]);
      else v.setElement(16*i+j,table[i][j]);
  return v;
}

void textWipe(Vecto v) {
  for (short i=0; i<=255; i++){
    int rgb = v.getElement(i);
    
    int red = rgb >> 16 ;
    int green = (rgb & 0x00ff00) >> 8;
    int blue = (rgb & 0x0000ff);
    
    rgb = 0;
    
    rgb |= red <<16;
    rgb |= blue <<8;
    rgb |= green;
    strip.setPixelColor(i, strip.Color(red, green, blue));
  }
  strip.show();
}

void setup_wifi(){
  WiFiMulti.addAP("Darkin_Wifi", "11042003");
  WiFiMulti.addAP("Darkin_Phone", "11042003");
  while(WiFiMulti.run() != WL_CONNECTED) {
      delay(500);
      for (int i=0; i<10; i++){
        digitalWrite(D4, HIGH);
        delay(500);
        digitalWrite(D4, LOW);
        delay(500);
      }
  }
}

byte count;
byte func = 0;
bool flagRecievedData[21] = { false };

void callback(char* topic, byte* payload, unsigned int length) 
{
  char output[length] = "";
  for (int i = 0; i < length; i++){
    output[i] = (char)payload[i];
  }
  String oStr = String(output);
  if (oStr.indexOf("Default")!=-1){
    func = 0;
  }
  if (oStr.indexOf("Heart")!=-1){
    func = 1;
  }
  if (oStr.indexOf("Friendzone")!=-1){
    func = 2;
  }
  
}
void reconnect() 
{
  while (!client.connected())
  {
    if (client.connect("ESP8266_id1","espData",0,0,"offline")){
      client.subscribe("colorData", 0);
    }else{
      for (byte i=0; i<20; i++){
        digitalWrite(D4, HIGH);
        delay(200);
        digitalWrite(D4, LOW);
        delay(200);
      }
      delay(2000);
    }
  }
}

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D4, OUTPUT);
  setup_wifi();

  client.setServer(mqtt_server, mqtt_port); 
  client.setCallback(callback);
  
  strip.begin();
  strip.show();
  strip.setBrightness(50);
  
  strip.setPixelColor(0, strip.Color(255, 0, 0));
  strip.show();
  
}
Display d0,d1(16); //FIXED BUG FROM DESTROYING OBJECT

void loop() {
  if (!client.connected()){
    reconnect();
  }
  client.loop();

  switch (func){
    case 0:
      if (!flagRecievedData[0]){
        digitalWrite(D0, HIGH);
        digitalWrite(D1, LOW);
        digitalWrite(D2, LOW);
        digitalWrite(D4, HIGH);
        flagRecievedData[0] = true;
        flagRecievedData[1] = false;
        flagRecievedData[2] = false;
      }
      break;
    case 1:
      if (!flagRecievedData[1]){
        digitalWrite(D1, HIGH);
        digitalWrite(D0, LOW);
        digitalWrite(D2, LOW);
        digitalWrite(D4, HIGH);
        flagRecievedData[1] = true;
        flagRecievedData[0] = false;
        flagRecievedData[2] = false;

        d1.resetColor();
        d1.setColor(2,2,0xff0000);
        d1.setColor(2,3,0xff0000);
        d1.setColor(2,4,0xff0000);
        d1.setColor(2,5,0xff0000);
        d1.setColor(2,9,0xff0000);
        d1.setColor(2,10,0xff0000);
        d1.setColor(2,11,0xff0000);
        d1.setColor(2,12,0xff0000);
        d1.setColor(3,1,0xff0000);
        d1.setColor(3,2,0xff0000);
        d1.setColor(3,3,0xff0000);
        d1.setColor(3,4,0xff0000);
        d1.setColor(3,5,0xff0000);
        d1.setColor(3,6,0xff0000);
        d1.setColor(3,8,0xff0000);
        d1.setColor(3,9,0xff0000);
        d1.setColor(3,10,0xff0000);
        d1.setColor(3,11,0xff0000);
        d1.setColor(3,12,0xff0000);
        d1.setColor(3,13,0xff0000);
        d1.setColor(4,0,0xff0000);
        d1.setColor(4,1,0xff0000);
        d1.setColor(4,2,0xff0000);
        d1.setColor(4,3,0xff0000);
        d1.setColor(4,4,0xff0000);
        d1.setColor(4,5,0xff0000);
        d1.setColor(4,6,0xff0000);
        d1.setColor(4,7,0xff0000);
        d1.setColor(4,8,0xff0000);
        d1.setColor(4,9,0xff0000);
        d1.setColor(4,10,0xff0000);
        d1.setColor(4,11,0xff0000);
        d1.setColor(4,12,0xff0000);
        d1.setColor(4,13,0xff0000);
        d1.setColor(4,14,0xff0000);
        d1.setColor(5,0,0xff0000);
        d1.setColor(5,1,0xff0000);
        d1.setColor(5,2,0xff0000);
        d1.setColor(5,3,0xff0000);
        d1.setColor(5,4,0xff0000);
        d1.setColor(5,5,0xff0000);
        d1.setColor(5,6,0xff0000);
        d1.setColor(5,7,0xff0000);
        d1.setColor(5,8,0xff0000);
        d1.setColor(5,9,0xff0000);
        d1.setColor(5,10,0xff0000);
        d1.setColor(5,11,0xff0000);
        d1.setColor(5,12,0xff0000);
        d1.setColor(5,13,0xff0000);
        d1.setColor(5,14,0xff0000);
        d1.setColor(6,0,0xff0000);
        d1.setColor(6,1,0xff0000);
        d1.setColor(6,2,0xff0000);
        d1.setColor(6,3,0xff0000);
        d1.setColor(6,4,0xff0000);
        d1.setColor(6,5,0xff0000);
        d1.setColor(6,6,0xff0000);
        d1.setColor(6,7,0xff0000);
        d1.setColor(6,8,0xff0000);
        d1.setColor(6,9,0xff0000);
        d1.setColor(6,10,0xff0000);
        d1.setColor(6,11,0xff0000);
        d1.setColor(6,12,0xff0000);
        d1.setColor(6,13,0xff0000);
        d1.setColor(6,14,0xff0000);
        d1.setColor(7,0,0xff0000);
        d1.setColor(7,1,0xff0000);
        d1.setColor(7,2,0xff0000);
        d1.setColor(7,3,0xff0000);
        d1.setColor(7,4,0xff0000);
        d1.setColor(7,5,0xff0000);
        d1.setColor(7,6,0xff0000);
        d1.setColor(7,7,0xff0000);
        d1.setColor(7,8,0xff0000);
        d1.setColor(7,9,0xff0000);
        d1.setColor(7,10,0xff0000);
        d1.setColor(7,11,0xff0000);
        d1.setColor(7,12,0xff0000);
        d1.setColor(7,13,0xff0000);
        d1.setColor(7,14,0xff0000);
        d1.setColor(8,1,0xff0000);
        d1.setColor(8,2,0xff0000);
        d1.setColor(8,3,0xff0000);
        d1.setColor(8,4,0xff0000);
        d1.setColor(8,5,0xff0000);
        d1.setColor(8,6,0xff0000);
        d1.setColor(8,7,0xff0000);
        d1.setColor(8,8,0xff0000);
        d1.setColor(8,9,0xff0000);
        d1.setColor(8,10,0xff0000);
        d1.setColor(8,11,0xff0000);
        d1.setColor(8,12,0xff0000);
        d1.setColor(8,13,0xff0000);
        d1.setColor(9,2,0xff0000);
        d1.setColor(9,3,0xff0000);
        d1.setColor(9,4,0xff0000);
        d1.setColor(9,5,0xff0000);
        d1.setColor(9,6,0xff0000);
        d1.setColor(9,7,0xff0000);
        d1.setColor(9,8,0xff0000);
        d1.setColor(9,9,0xff0000);
        d1.setColor(9,10,0xff0000);
        d1.setColor(9,11,0xff0000);
        d1.setColor(9,12,0xff0000);
        d1.setColor(10,3,0xff0000);
        d1.setColor(10,4,0xff0000);
        d1.setColor(10,5,0xff0000);
        d1.setColor(10,6,0xff0000);
        d1.setColor(10,7,0xff0000);
        d1.setColor(10,8,0xff0000);
        d1.setColor(10,9,0xff0000);
        d1.setColor(10,10,0xff0000);
        d1.setColor(10,11,0xff0000);
        d1.setColor(11,4,0xff0000);
        d1.setColor(11,5,0xff0000);
        d1.setColor(11,6,0xff0000);
        d1.setColor(11,7,0xff0000);
        d1.setColor(11,8,0xff0000);
        d1.setColor(11,9,0xff0000);
        d1.setColor(11,10,0xff0000);
        d1.setColor(12,5,0xff0000);
        d1.setColor(12,6,0xff0000);
        d1.setColor(12,7,0xff0000);
        d1.setColor(12,8,0xff0000);
        d1.setColor(12,9,0xff0000);
        d1.setColor(13,6,0xff0000);
        d1.setColor(13,7,0xff0000);
        d1.setColor(13,8,0xff0000);
        d1.setColor(14,7,0xff0000);
        Vecto arr1 = d1.convert();
        textWipe(arr1);
      }
      break;
    case 2:
      if (!flagRecievedData[2]){
        digitalWrite(D2, HIGH);
        digitalWrite(D0, LOW);
        digitalWrite(D1, LOW);
        digitalWrite(D4, HIGH);
        flagRecievedData[2] = true;
        flagRecievedData[0] = false;
        flagRecievedData[1] = false;
        
        d1.resetColor();
        d1.setColor(2,4,0x0000ff);
        d1.setColor(2,5,0x0000ff);
        d1.setColor(2,11,0xff0000);
        d1.setColor(2,12,0xff0000);
        d1.setColor(2,13,0xff0000);
        d1.setColor(3,3,0x0000ff);
        d1.setColor(3,5,0x0000ff);
        d1.setColor(3,10,0xff0000);
        d1.setColor(3,11,0xff0000);
        d1.setColor(3,12,0xff0000);
        d1.setColor(3,13,0xff0000);
        d1.setColor(3,14,0xff0000);
        d1.setColor(4,2,0x0000ff);
        d1.setColor(4,5,0x0000ff);
        d1.setColor(4,9,0xff0000);
        d1.setColor(4,10,0xff0000);
        d1.setColor(4,11,0xff0000);
        d1.setColor(4,12,0xff0000);
        d1.setColor(4,13,0xff0000);
        d1.setColor(4,14,0xff0000);
        d1.setColor(4,15,0xff0000);
        d1.setColor(5,1,0x0000ff);
        d1.setColor(5,4,0x0000ff);
        d1.setColor(5,9,0xff0000);
        d1.setColor(5,10,0xff0000);
        d1.setColor(5,11,0xff0000);
        d1.setColor(5,12,0xff0000);
        d1.setColor(5,13,0xff0000);
        d1.setColor(5,14,0xff0000);
        d1.setColor(5,15,0xff0000);
        d1.setColor(6,0,0x0000ff);
        d1.setColor(6,3,0x0000ff);
        d1.setColor(6,4,0x0000ff);
        d1.setColor(6,5,0x0000ff);
        d1.setColor(6,6,0x0000ff);
        d1.setColor(6,9,0xff0000);
        d1.setColor(6,10,0xff0000);
        d1.setColor(6,11,0xff0000);
        d1.setColor(6,12,0xff0000);
        d1.setColor(6,13,0xff0000);
        d1.setColor(6,14,0xff0000);
        d1.setColor(6,15,0xff0000);
        d1.setColor(7,0,0x0000ff);
        d1.setColor(7,7,0x0000ff);
        d1.setColor(7,9,0xff0000);
        d1.setColor(7,10,0xff0000);
        d1.setColor(7,11,0xff0000);
        d1.setColor(7,12,0xff0000);
        d1.setColor(7,13,0xff0000);
        d1.setColor(7,14,0xff0000);
        d1.setColor(7,15,0xff0000);
        d1.setColor(8,0,0x0000ff);
        d1.setColor(8,5,0x0000ff);
        d1.setColor(8,6,0x0000ff);
        d1.setColor(8,7,0x0000ff);
        d1.setColor(8,9,0xff0000);
        d1.setColor(8,10,0xff0000);
        d1.setColor(8,11,0xff0000);
        d1.setColor(8,12,0xff0000);
        d1.setColor(8,13,0xff0000);
        d1.setColor(8,14,0xff0000);
        d1.setColor(9,0,0x0000ff);
        d1.setColor(9,7,0x0000ff);
        d1.setColor(9,9,0xff0000);
        d1.setColor(9,10,0xff0000);
        d1.setColor(9,11,0xff0000);
        d1.setColor(9,12,0xff0000);
        d1.setColor(9,13,0xff0000);
        d1.setColor(10,0,0x0000ff);
        d1.setColor(10,5,0x0000ff);
        d1.setColor(10,6,0x0000ff);
        d1.setColor(10,7,0x0000ff);
        d1.setColor(10,9,0xff0000);
        d1.setColor(10,10,0xff0000);
        d1.setColor(10,11,0xff0000);
        d1.setColor(10,12,0xff0000);
        d1.setColor(11,0,0x0000ff);
        d1.setColor(11,7,0x0000ff);
        d1.setColor(11,9,0xff0000);
        d1.setColor(11,10,0xff0000);
        d1.setColor(11,11,0xff0000);
        d1.setColor(12,0,0x0000ff);
        d1.setColor(12,5,0x0000ff);
        d1.setColor(12,6,0x0000ff);
        d1.setColor(12,7,0x0000ff);
        d1.setColor(12,9,0xff0000);
        d1.setColor(12,10,0xff0000);
        d1.setColor(13,0,0x0000ff);
        d1.setColor(13,7,0x0000ff);
        d1.setColor(13,9,0xff0000);
        d1.setColor(14,1,0x0000ff);
        d1.setColor(14,2,0x0000ff);
        d1.setColor(14,3,0x0000ff);
        d1.setColor(14,4,0x0000ff);
        d1.setColor(14,5,0x0000ff);
        d1.setColor(14,6,0x0000ff);
        Vecto arr1 = d1.convert();
        textWipe(arr1);
      }
      break;
    default:
      {
        digitalWrite(D4, LOW);
        digitalWrite(D0, LOW);
        digitalWrite(D1, LOW);
        digitalWrite(D2, LOW);
      }
      break;
  }
  if (func == 0){
    Vecto arr0 = d0.convert();
    textWipe(arr0);
    delay(120);
    d0.decrease();
    count++;
    if (count==96){
      count=0;
    }
  }
}
