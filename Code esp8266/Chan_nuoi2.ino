#include <ArduinoJson.h>


#include "DHT.h"
#include <Servo.h>
#include <HX711.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SimpleTimer.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

#define servoPin D3
#define DOUT D6
#define CLK D5
#define sda D2
#define scl D1
#define nhietdo_doam D4
#define DHTTYPE DHT11
#define rxPin D7
#define txPin D8

Servo myservo;
HX711 scale_;
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(nhietdo_doam, DHTTYPE);
WiFiUDP udp;
NTPClient realtime(udp);
SoftwareSerial sim800(rxPin, txPin);

char *ssid = "AndroidAP";
char *password = "123456789";
String host = "192.168.43.13";
String url_choan = "http://" + host + "/esp8266_channuoithongminh/getdata_choan.php";
String url_choan_cuoi = "http://" + host + "/esp8266_channuoithongminh/getdata_choan_cuoi.php";
String url_hengio = "http://" + host + "/esp8266_channuoithongminh/getdata_hengio.php";
int soluong_mysql;
const String PHONE = "+84971098681";

SimpleTimer timer;
int timerid1, timeridhengio, timeridcheck;
String smsStatus, senderNumber, receivedDate, msg;

void setup() {
  // put your setup code here, to run once:
  myservo.attach(servoPin);
  scale_.begin(DOUT, CLK);

  Wire.begin(sda, scl);
  sim800.begin(9600);
  sim800.println("AT+CMGF=1");
  lcd.clear();
  lcd.init();
  lcd.backlight();
  lcd.home();
  myservo.write(map(850, 0, 1023, 0, 180));

  dht.begin();
  realtime.begin();
  realtime.setTimeOffset(+7 * 60 * 60);

  Serial.begin(115200);

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

  timerid1 = timer.setInterval(1000, hienthinhietdo_doam_thoigian);
  timeridhengio = timer.setInterval(1000, hengiochoan);
  timeridcheck = timer.setInterval(1000, check_);
  //delay(6000);
  //choan(200);

  soluong_mysql = get_soluong_json(getdata(url_choan));
  //xoa_hengio("23");
  //Serial.println(soluong_mysql);
  //json(getdata(url_hengio));
  //check_choan();
}


void loop() {
  timer.run();
  //Module sim800L
  while (sim800.available()) {
    parseData(sim800.readString());
  }
}
int get_cannang() {
  float nang;
  int cell;
  //for (int i = 0; i < 30; i++) {
  nang = scale_.read();//-22915
  nang = (nang + 22915) / 700.4f;
  nang = nang - 92.4;
  cell = round(nang);
  //}
  return cell;
}
void choan(int soluong) {
  if (soluong > 0) {
    myservo.write(map(740, 0, 1023, 0, 180));
    lcd.clear();
    timer.disable(timerid1);
    lcd.setCursor(0, 0);
    lcd.print("Cho an");
    int index = 6;
    int docannang = 0;
    while (docannang < soluong) {
      docannang = get_cannang();
      //Serial.println(docannang);
      lcd.setCursor(index, 0);
      lcd.print(".");
      //delay(2000);
      index++;

    }
    myservo.write(map(850, 0, 1023, 0, 180));
    check_choan();
    lcd.clear();
    lcd.print("Da cho an");
    delay(2000);
    lcd.clear();
    timer.enable(timerid1);
  }
}
void choan(String soluong) {
  int soluong_int = soluong.toInt();
  if (soluong_int > 0) {
    myservo.write(map(740, 0, 1023, 0, 180));
    lcd.clear();
    timer.disable(timerid1);
    lcd.setCursor(0, 0);
    lcd.print("Cho an");
    int index = 6;
    int docannang = 0;
    while (docannang < soluong_int) {
      docannang = get_cannang();
      //Serial.println(docannang);
      lcd.setCursor(index, 0);
      lcd.print(".");
      //delay(2000);
      index++;

    }
    myservo.write(map(850, 0, 1023, 0, 180));
    check_choan();
    lcd.clear();
    lcd.print("Da cho an");
    delay(2000);
    lcd.clear();
    timer.enable(timerid1);
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
  //String newtime = hour_ + ":" + minute_;
  return newtime;
}
void hienthinhietdo_doam_thoigian() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int temperature_ = round(temperature);
  int humidity_ = round(humidity);
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read DHT11");
  } else {
    lcd.setCursor(0, 0);
    String tem, hum;
    tem.concat(temperature_);
    hum.concat(humidity_);
    String hienthi = "Tem:"  + tem + "*C" + " " + "Hum:" + hum + "%";
    lcd.print(hienthi);
  }
  lcd.setCursor(0, 1);
  lcd.print(get_realtime());
}
String getdata(String url) {
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCodeGet = http.POST("");
  String dataGet = http.getString();
  return dataGet;
}
void xuly_data(int *array, String data_) {
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
int get_soluong_mysql(String data_) {
  //data_ = data_ + " ";
  String st = "";
  int index = 0;
  for (int i = 0; i < data_.length(); i++) {
    if (data_[i] != ' ') st = st + data_[i];
    else {
      index++;
      st = "";
    }
  }
  return index;
}
//void json(String data_) {
//  char txt[9000];
//  DynamicJsonBuffer jsonBuffer(data_.length() + 1);
//  data_.toCharArray(txt, data_.length() + 1);
//  JsonArray& root = jsonBuffer.parseArray(txt);
//  //String a = root[0]["Soluong"];
//  //Serial.println(a);
//  int index = 0;
//  while (root[index]) {
//    String a = root[index]["Soluong"];
//    //    int soluong = a.toInt();
//    //    array_[index] = soluong;
//    Serial.println(a);
//    index++;
//  }
//}
int get_soluong_json(String data_) {
  char txt1[data_.length() + 1];
  DynamicJsonBuffer jsonBuffer1(data_.length() + 1);
  data_.toCharArray(txt1, data_.length() + 1);
  JsonArray& root1 = jsonBuffer1.parseArray(txt1);
  int index = 0;
  while (root1[index]) {
    index++;
  }
  jsonBuffer1.clear();
  return index;
}
void hengiochoan() {
  String data_ = getdata(url_hengio);
  char txt1[data_.length() + 1];
  DynamicJsonBuffer jsonBuffer1(data_.length() + 1);
  data_.toCharArray(txt1, data_.length() + 1);
  JsonArray& root1 = jsonBuffer1.parseArray(txt1);
  int index = 0;
  while (root1[index]) {
    String time_ = root1[index]["Time"];
    String realtime = get_realtime();
    if (time_ == realtime) {
      int soluong = root1[index]["Soluong"];
      Serial.println("Thuc hien ham cho an");
      choan(soluong);
      String index1 = root1[index]["Id"];
      xoa_hengio(index1);
    }
    index++;
  }
}
void xoa_hengio(String id) {
  String url = "http://" + host + "/esp8266_channuoithongminh/xoa_hengio.php";
  WiFiClient client;
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }
  client.print(String("GET " + url + "?") +
               ("&id=") + id +
               " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  client.stop();
}
void check_choan() {
  String id = getdata(url_choan_cuoi);
  Serial.println(id);
  String url = "http://" + host + "/esp8266_channuoithongminh/update_choan.php";
  WiFiClient client;
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }
  client.print(String("GET " + url + "?") +
               ("&id=") + id +
               ("&check=") + "1" +
               " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  client.stop();
}
void check_() {
  int soluong_mysql_check = get_soluong_json(getdata(url_choan));
  if ( (soluong_mysql_check > 0) && (soluong_mysql > 0) ) {
    
    if (soluong_mysql > soluong_mysql_check) soluong_mysql = soluong_mysql_check;
    Serial.print(soluong_mysql);
    Serial.println(soluong_mysql_check);
    if (soluong_mysql < soluong_mysql_check) {
      Serial.println("Thuc hien ham cho an");

      String data_ = getdata(url_choan);
      char txt[data_.length() + 1];
      DynamicJsonBuffer jsonBuffer(data_.length() + 1);
      data_.toCharArray(txt, data_.length() + 1);
      JsonArray& root = jsonBuffer.parseArray(txt);

      int soluong_gam = root[soluong_mysql_check - 1]["Soluong"];
      choan(soluong_gam);
      jsonBuffer.clear();
      Serial.println(soluong_gam);
      soluong_mysql = soluong_mysql_check;
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
      //Lấy vị trí lưu tin nhắn
      index = buff.indexOf(",");
      String temp = buff.substring(index + 1, buff.length());
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
      String choan = msg.substring(0, 6);
      String soluong_string;
      if (senderNumbernew == PHONE && choan == "cho an") {
        for (int i = 7; i < msg.length(); i++)
        {
          soluong_string += msg[i];
        }
        //String soluong = msg.substring(7, msg.length());

        //        char txt1[100];
        //        soluong.toCharArray(txt1, soluong.length() + 1);
        //        int soluong_int;
        //        itoa(soluong_int,txt1,10);
        int soluong = soluong_string.toInt();
        if (soluong > 0) {
          myservo.write(map(740, 0, 1023, 0, 180));
          lcd.clear();
          timer.disable(timerid1);
          lcd.setCursor(0, 0);
          lcd.print("Cho an");
          int index = 6;
          int docannang = 0;
          while (docannang < soluong) {
            docannang = get_cannang();
            //Serial.println(docannang);
            lcd.setCursor(index, 0);
            lcd.print(".");
            //delay(2000);
            index++;

          }
          myservo.write(map(850, 0, 1023, 0, 180));
          check_choan();
          lcd.clear();
          lcd.print("Da cho an");
          delay(2000);
          lcd.clear();
          timer.enable(timerid1);
        }
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
        Reply("Cho an thanh cong");
        soluong_mysql = get_soluong_json(getdata(url_choan));
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
  sim800.print("AT+CMGF=1\r");
  delay(1000);
  sim800.print("AT+CMGS=\"" + PHONE + "\"\r");
  delay(1000);
  sim800.print(text);
  delay(100);
  sim800.write(0x1A);
  delay(1000);
  Serial.println("Đã gửi tin nhắn thành công");
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
