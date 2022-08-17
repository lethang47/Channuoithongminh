
#include <SimpleTimer.h>
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>


#define quat D0
#define dieuhoa D1
#define maysuoi D2
#define den D3
#define phunsuong D6
#define nhietdo_doam D5
#define DHTTYPE DHT11
#define rxPin_esp D7
#define txPin_esp D8

DHT dht(nhietdo_doam, DHTTYPE);
SoftwareSerial esp(rxPin_esp, txPin_esp);
WiFiUDP udp;
NTPClient realtime(udp);

SimpleTimer timer_switch, timer_dht11, timer_thietlap, timer_esp;
int timerid_switch, timerid_dht11, timerid_thietlap, timerid_esp;
String smsStatus, senderNumber, receivedDate, msg, indexmess;
unsigned long previousMillis = 0;
String data_receive = "";
bool stringComplete = false;

char *ssid = "AndroidAP";
char *password = "123456789";
String host = "192.168.43.13";

void setup() {
  // put your setup code here, to run once:
  pinMode(quat, OUTPUT);
  pinMode(dieuhoa, OUTPUT);
  pinMode(maysuoi, OUTPUT);
  pinMode(den, OUTPUT);
  pinMode(phunsuong, OUTPUT);
  dht.begin();
  Serial.begin(115200);

  esp.begin(9600);

  Serial.println();
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  realtime.begin();
  realtime.setTimeOffset(+7 * 60 * 60);

  timerid_switch = timer_switch.setInterval(1000, switch_);
  timerid_dht11 = timer_dht11.setInterval(6000, insert_data_dht11);
  timerid_thietlap = timer_thietlap.setInterval(500, thietlap);
  //timerid_esp = timer_esp.setInterval(1000, check_esp);
}

void loop() {
  //  timer_thietlap.run();
  //  timer_switch.run();
  //  timer_dht11.run();
  //delay_(6000);
  insert_data_dht11();
  //delay_(1000);
  switch_();
  //delay_(500);
  thietlap();
  check_esp();
}
void check_esp() {

  while (esp.available()) {
    char inChar = (char)esp.read();
    //String instring = (String)inChar;
    data_receive += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
  if (stringComplete) {
    Serial.println(data_receive);
    data_receive.trim();
    String soluong = data_receive.substring(7, data_receive.length());
    String check_data_choan = data_receive.substring(0, 6);
    //Serial.println(data_receive.length());
    if (check_data_choan == "cho an"){
      insert_choan(soluong);
    } else if (data_receive == "bat quat") {
      Serial.println(data_receive);
      digitalWrite(quat, HIGH);
      update_switch("1", "1");
    } else if (data_receive == "bat dieu hoa") {
      digitalWrite(dieuhoa, HIGH);
      update_switch("1", "2");
    }  else if (data_receive == "bat may suoi") {
      digitalWrite(maysuoi, HIGH);
      update_switch("1", "3");
    } else if (data_receive == "bat den") {
      digitalWrite(den, HIGH);
      update_switch("1", "4");
    } else if (data_receive == "bat phun suong") {
      digitalWrite(phunsuong, HIGH);
      update_switch("1", "5");
    } else if (data_receive == "tat quat") {
      digitalWrite(quat, LOW);
      update_switch("0", "1");
    } else if (data_receive == "tat dieu hoa") {
      digitalWrite(dieuhoa, LOW);
      update_switch("0", "2");
    } else if (data_receive == "tat may suoi") {
      digitalWrite(maysuoi, LOW);
      update_switch("0", "3");
    } else if (data_receive == "tat den") {
      digitalWrite(den, LOW);
      update_switch("0", "4");
    } else if (data_receive == "tat phun suong") {
      digitalWrite(phunsuong, LOW);
      update_switch("0", "5");
    } else if (data_receive == "trang thai") {
      String tt_quat, tt_dieuhoa, tt_maysuoi, tt_den, tt_phunsuong;
      if (digitalRead(quat) == 1) tt_quat = "bat"; else tt_quat = "tat";
      if (digitalRead(dieuhoa) == 1) tt_dieuhoa = "bat"; else tt_dieuhoa = "tat";
      if (digitalRead(maysuoi) == 1) tt_maysuoi = "bat"; else tt_maysuoi = "tat";
      if (digitalRead(den) == 1) tt_den = "bat"; else tt_den = "tat";
      if (digitalRead(phunsuong) == 1) tt_phunsuong = "bat"; else tt_phunsuong = "tat";
      String data_send = "Quat: " + tt_quat + " " + "Dieu hoa: " + tt_dieuhoa + " "
                         + "May suoi: " + tt_maysuoi + " " + "Den: " + tt_den + " " + "Phun suong: " + tt_phunsuong;
      send_esp(data_send);
    } else if (data_receive == "nhiet do, do am") {
      float temperature = dht.readTemperature();
      float humidity = dht.readHumidity();
      if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Lỗi đọc DHT11");
      } else {
        String nhietdo = "";
        nhietdo.concat(temperature);
        String doam = "";
        doam.concat(humidity);
        String data_send = "Nhiet do: " + nhietdo + "/" + "Do am: " + doam;
        send_esp(data_send);
      }
    }
    data_receive = "";
    stringComplete = false;
  }
}
void switch_() {
  String url = "http://" + host + "/esp8266_channuoithongminh/getdata_switch.php";
  String data_quat = getdata(url, "1");
  //Serial.println(data_quat);
  if (data_quat == "1") digitalWrite(quat, HIGH); else digitalWrite(quat, LOW);
  String data_dieuhoa = getdata(url, "2");
  //Serial.println(data_dieuhoa);
  if (data_dieuhoa == "1") digitalWrite(dieuhoa, HIGH); else digitalWrite(dieuhoa, LOW);
  String data_maysuoi = getdata(url, "3");
  //Serial.println(data_maysuoi);
  if (data_maysuoi == "1") digitalWrite(maysuoi, HIGH); else digitalWrite(maysuoi, LOW);
  String data_den = getdata(url, "4");
  //Serial.println(data_den);
  if (data_den == "1") digitalWrite(den, HIGH); else digitalWrite(den, LOW);
  String data_phunsuong = getdata(url, "5");
  //Serial.println(data_phunsuong);
  if (data_phunsuong == "1") digitalWrite(phunsuong, HIGH); else digitalWrite(phunsuong, LOW);
}

void insert_data_dht11() {
  delay(3000);
  String url = "http://" + host + "/esp8266_channuoithongminh/insert_dht11_data.php";
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  if (isnan(temperature) || isnan(humidity)) {
    //Serial.println("Failed to read DHT11");
  } else {
    WiFiClient client;
    if (!client.connect(host, 80)) {
      Serial.println("connection failed");
      return;
    }
    client.print(String("GET " + url + "?") +
                 ("&temperature=") + temperature +
                 ("&humidity=") + humidity +
                 " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    client.stop();
  }
}
void update_switch(String value, String index) {
  String url = "http://" + host + "/esp8266_channuoithongminh/update_switch.php";
  WiFiClient client;
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }
  client.print(String("GET " + url + "?") +
               ("&id=") + index +
               ("&status=") + value +
               " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  client.stop();
}
void thietlap() {

  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  String url = "http://" + host + "/esp8266_channuoithongminh/getdata_thietlap.php";
  String dataGet = getdata(url, "1");
  if (dataGet != "") {
    float array_tem_hum[2];
    xuly_data(array_tem_hum, dataGet);

    float tem_data = array_tem_hum[0];
    float hum_data = array_tem_hum[1];
    //Serial.println(array_tem_hum[0]);
    //Serial.println(array_tem_hum[1]);
    if ((tem_data > temperature) && (tem_data != 0.00) ) {
      digitalWrite(maysuoi, HIGH);
      update_switch("1", "3");
      digitalWrite(dieuhoa, LOW);
      update_switch("0", "2");
    } else if ((tem_data < temperature) && (tem_data != 0.00)) {
      digitalWrite(maysuoi, LOW);
      update_switch("0", "3");
      digitalWrite(dieuhoa, HIGH);
      update_switch("1", "2");
    }
    if ((hum_data > humidity) && (hum_data != 0.00)) {
      digitalWrite(phunsuong, HIGH);
      update_switch("1", "5");
      digitalWrite(quat, LOW);
      update_switch("0", "1");
    } else if ((hum_data < humidity) && (hum_data != 0.00)) {
      digitalWrite(phunsuong, LOW);
      update_switch("0", "5");
      digitalWrite(quat, HIGH);
      update_switch("1", "1");
    }
  }
}
String get_realtime() {
  while (!realtime.update()) {
    realtime.forceUpdate();
  }
  //realtime.update();
  String time_ = realtime.getFormattedDate();
  //2022-08-03T00:40:52Z
  String day_, month_, year_, hour_, minute_;
  day_ = time_.substring(8, 10);
  month_ = time_.substring(5, 7);
  year_ = time_.substring(0, 4);
  hour_ = time_.substring(11, 13);
  minute_ = time_.substring(14, 16);
  String newtime = day_ + "-" + month_ + "-" + year_ + " " + hour_ + ":" + minute_;
  return newtime;
}
String getdata(String url, String id) {

  String getdata = "id=" + id;
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCodeGet = http.POST(getdata);
  String dataGet = http.getString();
  return dataGet;
}
void xuly_data(float * array, String data_) {
  data_ = data_ + " ";
  String st = "";
  int index = 0;
  for (int i = 0; i < data_.length(); i++) {
    if (data_[i] != ' ') st = st + data_[i];
    else {
      array[index] = st.toInt();
      index++;
      st = "";
    }
  }
}
void delay_(int number_mi) {
  while (true) {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= number_mi) {
      previousMillis = currentMillis;
      break;
    }
  }
}
void send_esp(String noidung) {
  esp.println(noidung);
  esp.flush();
}
String receive_esp() {
  String inputString = "";
  bool stringComplete = false;
  while (esp.available()) {
    char inChar = (char)esp.read();
    inputString += inChar;
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
  if (stringComplete) {
    return inputString;
  }
}
void insert_choan(String soluong) {
  String url = "http://" + host + "/esp8266_channuoithongminh/insert_choan.php";
  WiFiClient client;
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }
  client.print(String("GET " + url + "?") +
               ("&soluong=") + soluong +
               " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  client.stop();

}
