#include "FastLED.h"            // 此示例程序需要使用FastLED库
#include <WiFi.h>
#include <HTTPClient.h>
#include <LiquidCrystal_I2C.h>
#define NUM_LEDS 183          // LED灯珠数量
#define LED_DT 13               // Arduino输出控制信号引脚
#define LED_TYPE WS2812         // LED灯带型号
#define COLOR_ORDER GRB         // RGB灯珠中红色、绿色、蓝色LED的排列顺序
uint8_t max_bright = 30;       // LED亮度控制变量，可使用数值为 0 ～ 255， 数值越大则光带亮度越高
CRGB leds[NUM_LEDS];            // 建立光带leds

bool SendData = false;
TaskHandle_t doTheUpload;

String tmpa = "";
int lcdColumns = 16;
int lcdRows = 2;
LiquidCrystal_I2C lcd(0x20, lcdColumns, lcdRows);  

const char ssid[]="EdNetwork"; 
const char pwd[]="asd12345678"; 
const char* serverName = "http://210.70.74.222:30008/door/api/values/AddStuEntry";

uint8_t stu_count = 0;
char stu_id[6] = {0,0,0,0,0,0};
long long lastPress = 0;
long long last_tmp = 0;

const int trigPin = 2;
const int echoPin = 15;
const int motorPin = 4;
long duration;
double distance;

const byte ROWS =4; //設定列數為4
const byte COLS = 4; //設定行數為4
char key;
//char keys[ROWS][COLS] ={ //軟鍵盤對應輸出字元
//{'1' , '2' , '3' , 'A' },
//{'4' , '5' , '6' , 'B' },
//{'7' , '8' , '9' , 'C' },
//{'*' , '0' , '#' , 'D' }
//};
char keys[ROWS][COLS] ={ //硬鍵盤對應輸出字元
{'1' , '4' , '7' , '*' },
{'2' , '5' , '8' , '0' },
{'3' , '6' , '9' , '#' },
{'A' , 'B' , 'C' , 'D' }
};
byte rowPins[ROWS] = {32,33,25,26}; //列1~4的PIN腳
byte colPins[COLS] = {27,14,12,35}; //行1~4的PIN腳
//掃描碼，每掃一次僅有一個為＂0"
byte scanCode[ROWS][COLS]={
{0,1,1,1},
{1,0,1,1},
{1,1,0,1},
{1,1,1,0}
};

char keyScan(){ 
  //鍵盤掃描函數
for(int scan=0;scan<4;scan++){ 
  //分4次送出掃描碼
for(int i=0; i<4; i++){ 
  //將掃描碼寫入ROW
  digitalWrite(rowPins[i], scanCode[scan][i]);
}
    for(int j=0;j<4;j++){ 
      //讀取COLUMN的值，若為＂0"表示該鍵被按下
      if(digitalRead(colPins[j])==0){
      delay(10); //消除開關彈跳
        if(digitalRead(colPins[j])==0){
          while(digitalRead(colPins[j])==0); //等待按鍵放開
          return keys[scan][j];//返回按鍵值
        }
      }
    }
}
    return 'X'; //若無任何按鍵盤被按下，則返回字元＂X"
}

void doTheUpload_senddata(void * pvParameters ) {
  for (;;) {
    if(SendData){
      Serial.println("GOGO Upload");
      String tmp = "";
      for (auto i : stu_id){
        tmp+= i;
      }
      char PostData[500];
      if(WiFi.status()== WL_CONNECTED){
        WiFiClient client;
        HTTPClient http;
        http.begin(client, serverName);
        http.addHeader("Content-Type", "application/json");
        sprintf(PostData,"{\"api_key\":\"%s\",\"door_id\":\"%s\",\"stu_id\":\"%s\"}","thisisatestkey","A01",tmp.c_str());
        Serial.println(PostData);
        int httpResponseCode = http.POST(PostData);
        Serial.println(httpResponseCode);
        http.end();

        if(httpResponseCode == 200){
          Serial.println("OK Send");
        }
        else{
          Serial.println("Faild Send");
        }

        SendData = false;
        http.end();
      }
    }
    
    else{
      delay(1);
    }

  }
}

void lcd_current(char key){
  //lcd.setCursor(0, 0);
  //lcd.print("Pls enter STUID");

  
  lcd.setCursor(0, 0);
  lcd.print("You Have Entered : ");
  tmpa += key;
  lcd.setCursor(0, 1);
  Serial.println("UC");
  Serial.println(tmpa);
  lcd.print(tmpa);

}

void let_green(){
  fill_solid(leds, NUM_LEDS, CRGB::Green);
  FastLED.show();
  delay(50);
}

void let_red(uint8_t dms){
  fill_solid(leds, NUM_LEDS, CRGB::Red);
  FastLED.show();
  delay(dms);
}

void ultra_sensor(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(1);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); 
  distance = duration*0.034/2;
  //Serial.print("Distance: ");
  //Serial.print(distance);
  //Serial.println("cm");
  delay(1);
}

void stopall(){
    let_green();
    digitalWrite(motorPin, 0);
    stu_count = 0;
    lcd.clear();
    tmpa = "";
}

void setup(){
  Serial.begin(115200);
  for(int i = 0; i<4; i++){
  //設定COLUMNS為輸入且為PULL UP
    pinMode(colPins[i],INPUT_PULLUP);
  //設定ROWS為輸出，作為輸出掃描碼PIN腳
    pinMode(rowPins[i], OUTPUT);
  }
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motorPin, OUTPUT);
  //WIFI Section
  WiFi.mode(WIFI_STA); //設置WiFi模式
  WiFi.begin(ssid,pwd);
  Serial.print("WiFi connecting");
  while(WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);   
  }
  Serial.println("");
  Serial.print("IP位址:");
  Serial.println(WiFi.localIP()); //讀取IP位址
  Serial.print("WiFi RSSI:");
  Serial.println(WiFi.RSSI()); //讀取WiFi強度
  
  LEDS.addLeds<LED_TYPE, LED_DT, COLOR_ORDER>(leds, NUM_LEDS);  // 初始化光带  
  FastLED.setBrightness(max_bright);// 设置光带亮度

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();


  
  let_green();
  digitalWrite(motorPin, 0);
  //在核心0啟動任務1
  xTaskCreatePinnedToCore(
  doTheUpload_senddata, /*任務實際對應的Function*/
  "doTheUpload", /*任務名稱*/
  10000, /*堆疊空間*/
  NULL, /*無輸入值*/
  0, /*優先序0*/
  &doTheUpload, /*對應的任務變數位址*/
  1); /*指定在核心0執行 */
}

void loop(){
  key = keyScan();
  ultra_sensor();
  if (distance <= 55){
    Serial.println(distance);
    stopall();
  }
  if(key == 'X') return;
  Serial.print("key = ");
  Serial.println(key);
  Serial.print(stu_count);
  
  if( key!='#' && key!='*' && stu_count>=0 && stu_count<7){
    last_tmp = millis();
    if (last_tmp - lastPress > 3000){
      stu_count = 0;
      Serial.println("Timeout Clean I/O");
    }
    lastPress = last_tmp;
    stu_id[stu_count] = key; 
    stu_count += 1;
    lcd_current(key);
    if(stu_count >= 6){
      let_red(10);  
      digitalWrite(motorPin, 1);
      SendData = true;
      stu_count = 0;
      lcd.clear();
      tmpa = "";
      for(int j=0;j<6;j++){
        Serial.print(stu_id[j]);
      }
        Serial.println("...");  
    }

  }
  else if (key=='#'){
    if(stu_count != 0){
      stu_count = stu_count - 1;
      tmpa = "";
      for(int k;k<stu_count-1;k++){
        tmpa += stu_id[k];
      }
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("You Have Entered :: ");
      lcd.setCursor(0, 1);
      Serial.println("DEC");
      Serial.println(tmpa);
      lcd.print(tmpa);
    }

  }
  else if (key == '*'){
    stopall();
  }
  

}
