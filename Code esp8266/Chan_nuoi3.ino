#include <SoftwareSerial.h>

#define rxPin_sim D7
#define txPin_sim D8
#define rxPin_esp D5
#define txPin_esp D6

SoftwareSerial sim800(rxPin_sim, txPin_sim);
SoftwareSerial esp(rxPin_esp, txPin_esp);

const String PHONE = "+84364211384";
String smsStatus, senderNumber, receivedDate, msg, indexmess;
unsigned long previousMillis = 0;
String data_receive1;
bool stringComplete1 = false;
void setup() {
  Serial.begin(115200);

  sim800.begin(9600);
  sim800.println("AT+CMGF=1");

  esp.begin(9600);
  //esp.println("thang");
}

void loop() {
  // put your main code here, to run repeatedly:
  while (sim800.available()) {
    parseData(sim800.readString());
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
      //cho an 50
      Serial.println(msg);
      String msgNew = msg.substring(0, 6);
      Serial.println(msgNew);
      String senderNumbernew = senderNumber.substring(0, 12);
      Serial.println(senderNumbernew);
      if (senderNumbernew == PHONE && msg == "nhiet do, do am") {
        //Reply("thang");
        send_esp("nhiet do, do am");
        delay_(1000);
        String data_receive;
        bool stringComplete = false;
        while (true) {
          while (esp.available()) {
            char inChar = (char)esp.read();
            data_receive += inChar;
            if (inChar == '\n') {
              stringComplete = true;
            }
          }
          if (stringComplete) {
            Reply(data_receive);
            break;
          }
        }
      } else if (senderNumbernew == PHONE && msgNew == "cho an") {
        send_esp(msg);
        delay_(4000);
        Reply("Da cho an");
      } else if (senderNumbernew == PHONE && msg == "bat quat") {
        send_esp("bat quat");
        delay_(1000);
        Reply("Quat da bat");
      } else if (senderNumbernew == PHONE && msg == "bat dieu hoa") {
        send_esp("bat dieu hoa");
        delay_(1000);;
        Reply("Dieu hoa da bat");
      } else if (senderNumbernew == PHONE && msg == "bat may suoi") {
        send_esp("bat may suoi");
        delay_(1000);
        Reply("May suoi da bat");
      } else if (senderNumbernew == PHONE && msg == "bat den") {
        send_esp("bat den");
        delay_(1000);
        Reply("Den da bat");
      } else if (senderNumbernew == PHONE && msg == "bat phun suong") {
        send_esp("bat phun suong");
        delay_(1000);
        Reply("Phun suong da bat");
      } else if (senderNumbernew == PHONE && msg == "tat quat") {
        send_esp("tat quat");
        delay_(1000);
        Reply("Quat da tat");
      } else if (senderNumbernew == PHONE && msg == "tat dieu hoa") {
        send_esp("tat dieu hoa");
        delay_(1000);
        Reply("Dieu hoa da tat");
      } else if (senderNumbernew == PHONE && msg == "tat may suoi") {
        send_esp("tat may suoi");
        delay_(1000);
        Reply("May suoi da tat");
      } else if (senderNumbernew == PHONE && msg == "tat den") {
        send_esp("tat den");
        delay_(1000);
        Reply("Den da tat");
      } else if (senderNumbernew == PHONE && msg == "tat phun suong") {
        send_esp("tat phun suong");
        delay_(1000);
        Reply("Phun suong da tat");
      } else if (senderNumbernew == PHONE && msg == "trang thai") {
        //        String tt_quat, tt_dieuhoa, tt_maysuoi, tt_den, tt_phunsuong;
        //        if (digitalRead(quat) == 1) tt_quat = "bat"; else tt_quat = "tat";
        //        if (digitalRead(dieuhoa) == 1) tt_dieuhoa = "bat"; else tt_dieuhoa = "tat";
        //        if (digitalRead(maysuoi) == 1) tt_maysuoi = "bat"; else tt_maysuoi = "tat";
        //        if (digitalRead(den) == 1) tt_den = "bat"; else tt_den = "tat";
        //        if (digitalRead(phunsuong) == 1) tt_phunsuong = "bat"; else tt_phunsuong = "tat";
        //        String data_send = "Quat: " + tt_quat + "\n" + "Dieu hoa: " + tt_dieuhoa + "\n"
        //                           + "May suoi: " + tt_maysuoi + "\n" + "Den: " + tt_den + "\n" + "Phun suong: " + tt_phunsuong;
        send_esp("trang thai");
        delay_(1000);
        String data_receive;
        bool stringComplete = false;
        while (true) {
          while (esp.available()) {
            char inChar = (char)esp.read();
            data_receive += inChar;
            if (inChar == '\n') {
              stringComplete = true;
            }
          }
          if (stringComplete) {
            Reply(data_receive);
            break;
          }
        }

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
