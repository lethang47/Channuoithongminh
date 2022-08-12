
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
#define rxPin D7
#define txPin D8

DHT dht(nhietdo_doam, DHTTYPE);
SoftwareSerial sim800(rxPin, txPin);
WiFiUDP udp;
NTPClient realtime(udp);

SimpleTimer timer_switch, timer_dht11, timer_thietlap;
int timerid_switch, timerid_dht11, timerid_thietlap;
String smsStatus, senderNumber, receivedDate, msg, indexmess;
unsigned long previousMillis = 0;

char *ssid = "D-Link";
char *password = "0988622764";
String host = "192.168.0.13";
const String PHONE = "+84971098681";
int kiemtra = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(quat, OUTPUT);
  pinMode(dieuhoa, OUTPUT);
  pinMode(maysuoi, OUTPUT);
  pinMode(den, OUTPUT);
  pinMode(phunsuong, OUTPUT);
  dht.begin();
  Serial.begin(115200);

  sim800.begin(9600);
  sim800.println("AT+CMGF=1");

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
}

void loop() {


  //timer_thietlap.run();
  timer_switch.run();
  

  //Module sim800L
  while (sim800.available()) {
    parseData(sim800.readString());
  }
  //  while (Serial.available())  {
  //    sim800.println(Serial.readString());
  //  }

  //  Serial.println(get_realtime());
  //  delay(1000);
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
void parseData(String buff) {
  
  Serial.println(buff);

  unsigned int len, index;
  //--------------------------------------------------------------------
  //Xóa lệnh AT từ chuỗi trả về.
  index = buff.indexOf("\r");
  buff.remove(0, index + 2);
  buff.trim();
  //--------------------------------------------------------------------

  //--------------------------------------------------------------------
  if (buff != "OK") {
    index = buff.indexOf(":");
    String cmd = buff.substring(0, index);
    cmd.trim();

    buff.remove(0, index + 2);

    //____________________________________________________________
    if (cmd == "+CMTI") {
      //timer_thietlap.disable(timerid_thietlap);
      //timer_switch.disable(timerid_switch);
      //Lấy vị trí lưu tin nhắn
      index = buff.indexOf(",");
      String temp = buff.substring(index + 1, buff.length());
      indexmess = temp;
      temp = "AT+CMGR=" + temp + "\r";
      //Lấy giá trị tin nhắn
      sim800.println(temp);
    }
    //____________________________________________________________
    else if (cmd == "+CMGR") {
      extractSms(buff);
      Serial.println(msg);
      String senderNumbernew = senderNumber.substring(0, 12);
      Serial.println(senderNumbernew);
      if (senderNumbernew == PHONE && msg == "nhiet do, do am") {
        //Reply("thang");
        float temperature = dht.readTemperature();
        float humidity = dht.readHumidity();
        String nhietdo = "";
        nhietdo.concat(temperature);
        String doam = "";
        doam.concat(humidity);
        if (isnan(temperature) || isnan(humidity)) {
          //Serial.println("Lỗi đọc DHT11");
        } else {
          String data_send = "Nhiet do: " + nhietdo + "\n" + "Do am: " + doam;
          Reply(data_send);
        }
      } else if (senderNumbernew == PHONE && msg == "bat quat") {
        digitalWrite(quat, HIGH);
        //update_switch("1", "1");
        Reply("Quat da bat");
      } else if (senderNumbernew == PHONE && msg == "bat dieu hoa") {
        digitalWrite(dieuhoa, HIGH);
        update_switch("1", "2");
        Reply("Dieu hoa da bat");
      } else if (senderNumbernew == PHONE && msg == "bat may suoi") {
        digitalWrite(maysuoi, HIGH);
        update_switch("1", "3");
        Reply("May suoi da bat");
      } else if (senderNumbernew == PHONE && msg == "bat den") {
        digitalWrite(den, HIGH);
        update_switch("1", "4");
        Reply("Den da bat");
      } else if (senderNumbernew == PHONE && msg == "bat phun suong") {
        digitalWrite(phunsuong, HIGH);
        update_switch("1", "5");
        Reply("Phun suong da bat");
      } else if (senderNumbernew == PHONE && msg == "tat quat") {
        digitalWrite(quat, LOW);
        update_switch("0", "1");
        Reply("Quat da tat");
      } else if (senderNumbernew == PHONE && msg == "tat dieu hoa") {
        digitalWrite(dieuhoa, LOW);
        update_switch("0", "2");
        Reply("Dieu hoa da tat");
      } else if (senderNumbernew == PHONE && msg == "tat may suoi") {
        digitalWrite(maysuoi, LOW);
        update_switch("0", "3");
        Reply("May suoi da tat");
      } else if (senderNumbernew == PHONE && msg == "tat den") {
        digitalWrite(den, LOW);
        update_switch("0", "4");
        Reply("Den da tat");
      } else if (senderNumbernew == PHONE && msg == "tat phun suong") {
        digitalWrite(phunsuong, LOW);
        update_switch("0", "5");
        Reply("Phun suong da tat");
      } else if (senderNumbernew == PHONE && msg == "trang thai") {
        String tt_quat, tt_dieuhoa, tt_maysuoi, tt_den, tt_phunsuong;
        if (digitalRead(quat) == 1) tt_quat = "bat"; else tt_quat = "tat";
        if (digitalRead(dieuhoa) == 1) tt_dieuhoa = "bat"; else tt_dieuhoa = "tat";
        if (digitalRead(maysuoi) == 1) tt_maysuoi = "bat"; else tt_maysuoi = "tat";
        if (digitalRead(den) == 1) tt_den = "bat"; else tt_den = "tat";
        if (digitalRead(phunsuong) == 1) tt_phunsuong = "bat"; else tt_phunsuong = "tat";
        String data_send = "Quat: " + tt_quat + "\n" + "Dieu hoa: " + tt_dieuhoa + "\n"
                           + "May suoi: " + tt_maysuoi + "\n" + "Den: " + tt_den + "\n" + "Phun suong: " + tt_phunsuong;
        Reply(data_send);
      }

    }
    //____________________________________________________________
    //--------------------------------------------------------------------
  }

  
}
void Reply(String text)
{
  if (text == "") {
    return;
  }
  Serial.println(text);
  sim800.print("AT+CMGDA=\"");
  sim800.println("DEL ALL\"");
  delay_(100);
  sim800.print("AT+CMGF=1\r");
  delay_(1000);
  sim800.print("AT+CMGS=\"" + PHONE + "\"\r");
  delay_(1000);
  sim800.print(text);
  delay_(100);
  sim800.write(0x1A);
  delay_(1000);
  Serial.println("Da gui tin nhan thanh cong");
  //  sim800.print("AT+CMGF=1\r");
  //  delay_(100);
}
void extractSms(String buff) {
  unsigned int index;

  index = buff.indexOf(",");
  smsStatus = buff.substring(1, index - 1);
  buff.remove(0, index + 2);

  senderNumber = buff.substring(0, 13);
  buff.remove(0, 19);

  receivedDate = buff.substring(0, 20);
  buff.remove(0, buff.indexOf("\r"));
  buff.trim();

  index = buff.indexOf("\n\r");
  buff = buff.substring(0, index);
  buff.trim();
  msg = buff;
  buff = "";
  msg.toLowerCase();
  Serial.println(msg);
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
void xuly_data(float *array, String data_) {
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
