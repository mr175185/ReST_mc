#include <LiquidCrystal.h>
#define s_on    9
#define but1    2
#define but2    3
#define rest    4
#define start   5
#define endless 6

LiquidCrystal lcd(15, 16, 14, 17, 19, 18);

unsigned long ontime;
int second = 0;
int count = 0;
int skip = 0;
int escape = 0;
int times;
int minus;

void setup() {
  for (int i = 2; i <= 6; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  pinMode(s_on, OUTPUT);

  lcd.begin(8, 2);
  lcd.print("welcome");
  lcd.setCursor(0, 1);
  lcd.print("to CR4'.");
  delay(2000);
  lcd.clear();
}

void loop() {
  lcd.setCursor(0, 0);                            //メイン（設定画面）表示用
  lcd.print(second);
  lcd.print(" sec");
  lcd.setCursor(0, 1);
  lcd.print(count);
  lcd.print(" times");

  if (digitalRead(but1) == LOW) {                 //ボタンアクション（時間設定
    second++;
    delay(100);
  } else if (digitalRead(but2) == LOW) {          //ボタンアクション（回数設定
    if (skip == 0) {                              //mills未読み込みの場合
      skip = 1;
      ontime = millis();
    } else {
      if (millis() - ontime > 6150) {             //押し始めてから6秒（+100
        count = count + 100;
        delay(1000);
      } else if (millis() - ontime > 2150) {      //押し始めてから2秒（+20
        count = count + 10;
        delay(500);
      } else {                                    //2秒も経っていない時（+1
        count++;
        delay(100);
      }
    }
  } else if (digitalRead(rest) == LOW) {          //ボタンアクション（リセット
    if (count == 0 && second == 0) {              //既にリセット済みの場合
      lcd.clear();
      lcd.print("variable");
      lcd.setCursor(0, 1);
      lcd.print(" clead!");
      delay(1000);
      lcd.clear();
    } else {                                      //リセット
      lcd.clear();
      count = 0;
      second = 0;
      delay(250);                                 //リセット直後にリセット済みが出ないように
    }
  } else {                                        //ボタンアクション無しの場合（設定系
    skip = 0;                                     //回数スキップ無効化
    ontime = 0;                                   //mills初期化（0にしてるだけ
  }

  if (digitalRead(start) == LOW) {                //ボタンアクション（回数指定スタート
    if (second == 0) {                            //時間未設定の場合
      lcd.clear();
      lcd.print("need set");
      lcd.setCursor(0, 1);
      lcd.print(" delay!");
      delay(1500);
      lcd.clear();
    } else if (count == 0) {                      //回数未設定の場合
      lcd.clear();
      lcd.print("need set");
      lcd.setCursor(0, 1);
      lcd.print(" count!");
      delay(1500);
      lcd.clear();
    } else {                                      //実行ここから
      times = count - 1;                          //カウントダウン用
      minus = 50 / second;                        //delay調節用

      for (int i = 1; i <= count; i++) {          //設定回数回繰り返し
        if (escape == 1) {                        //途中停止が有効の場合
          escape = 0;                             //途中停止をリセット
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("work is");
          lcd.setCursor(0, 1);
          lcd.print("stoped!");
          delay(1500);
          lcd.clear();
          break;                                  //脱出 to [メインループ]
        } else {                                  //DO
          lcd.clear();                            //設定表示+残回数カウントダウン
          lcd.setCursor(0, 0);
          lcd.print(second);
          lcd.print("s * ");
          lcd.print(count);
          lcd.print(".");
          lcd.setCursor(0, 1);
          lcd.print("last ");
          lcd.print(times);
          lcd.print(".");
          times = times - 1;                      //一回表示したら-1

          digitalWrite(s_on, HIGH);               //シャッター出力
          delay(50);                              //delay（カメラの反応する時間
          digitalWrite(s_on, LOW);                //出力断

          for (int n = 1; n <= second; n++) {     //指定秒数wait用（1秒毎に脱出確認
            if (digitalRead(rest) == LOW) {       //リセットが押されている場合
              escape = 1;                         //途中停止を有効に
              break;                              //脱出 to [start]ループ
            } else {
              delay(1000 - minus);                //1秒(-delay/second)休む * 秒数
            }
          }
        }
      }
      lcd.clear();
    }
  }

  if (digitalRead(endless) == LOW) {              //ボタンアクション（エンドレススタート
    if (second == 0) {                            //時間未設定の場合
      lcd.clear();
      lcd.print("need set");
      lcd.setCursor(0, 1);
      lcd.print(" delay!");
      delay(1500);
      lcd.clear();
    } else {                                      //実行ここから
      lcd.clear();                                //設定表示（カウントダウン無し
      lcd.setCursor(0, 0);
      lcd.print("delay ");
      lcd.print(second);
      lcd.print(".");
      lcd.setCursor(0, 1);
      lcd.print("endless!");
      minus = 50 / second;                        //delay調節用

      while (true) {                              //k本的に無限ループ
        if (escape == 1) {                        //途中停止が有効の場合（上同
          escape = 0;                             //途中停止をリセット
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("work is");
          lcd.setCursor(0, 1);
          lcd.print("stoped!");
          delay(1500);
          lcd.clear();
          break;                                  //脱出 to [メインループ]
        } else {                                  //DO
          digitalWrite(s_on, HIGH);
          delay(50);
          digitalWrite(s_on, LOW);

          for (int n = 1; n <= second; n++) {     //指定秒数wait用（1秒毎に脱出確認
            if (digitalRead(rest) == LOW) {       //リセットが押されている場合（上同
              escape = 1;                         //途中停止を有効に
              break;                              //脱出 to [endless]ループ
            } else {
              delay(1000 - minus);                //1秒(-delay/second)休む * 秒数
            }
          }
        }
      }
    }
  }
}
