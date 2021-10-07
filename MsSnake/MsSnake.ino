#include "LedFunction.h"
#include "Player.h"
//#define NONE_LOCK -1
//#define NUM_LOCK 0
//#define CAPS_LOCK 1
//#define SCROLL_LOCK 2

long delaytime = 200;
long l = 10000;
LedStatus Led;
Player player;
BlinkResult result;

void setup() {
  //init
  Led.init();
  Serial.begin(115200);
  delay(1000);
  //wait num lock blink
  while (1) {
    //Led.Update(50);
    //Serial.println(String(Led.StartTime)+" "+String(Led.CurrentTime));
    //Serial.println(Led.GetLedBlink(NUM_LOCK,500).Blink);
    result =Led.GetLedBlink(NUM_LOCK, 1000);
    //Serial.println("blink:"+String(result.Blink)+" "+String(Led.StartTime)+" "+String(Led.CurrentTime));
    if (result.Blink > 3) break;
  }
  //open mspaint
  Keyboard.press(KEY_LEFT_GUI);
    delay(delaytime);
    Keyboard.press('r');
    delay(delaytime);
    Keyboard.releaseAll();
    delay(delaytime);
    Keyboard.println("mspaint\n");
    //set res
    Keyboard.press(KEY_LEFT_CTRL);
    delay(delaytime);
    Keyboard.press('e');
    delay(delaytime);
    Keyboard.releaseAll();
    delay(500);
    Keyboard.print(String(CanvasWidth));
    delay(delaytime);
    Keyboard.press(KEY_TAB);
    Keyboard.print(String(CanvasHeight));
    delay(delaytime);
    Keyboard.press(KEY_RETURN);
    Keyboard.releaseAll();
    //maximize mspaint
    Keyboard.press(KEY_LEFT_GUI);
    delay(delaytime);
    Keyboard.press(KEY_UP_ARROW);
    delay(delaytime);
    Keyboard.releaseAll();
    delay(delaytime);
  //draw border
  //Serial.println(String(player.border[0]) + " " + String(player.border[1]) + " " + String(player.border[2]) + " " + String(player.border[3]) + " ");
  AbsoluteMouse.moveTo(player.border[0], player.border[1]);
  AbsoluteMouse.press();
  AbsoluteMouse.moveTo(player.border[2], player.border[1]);
  delay(100);
  AbsoluteMouse.moveTo(player.border[2], player.border[3]);
  delay(100);
  AbsoluteMouse.moveTo(player.border[0], player.border[3]);
  delay(100);
  AbsoluteMouse.moveTo(player.border[0], player.border[1]);
  delay(100);
  AbsoluteMouse.release();
  AbsoluteMouse.press();
}
void loop() {
  float turn = HALF_PI / 2;
  while (1) {
    result = Led.WaitLedBlink(ANY, 100);
    //Serial.println(String(result.TimeOut));
    if (result.LedType == None) {
      //Serial.println("timeout");
      if (!player.Step()) {
        AbsoluteMouse.release();
        break;
      }
    }
    else {
      switch (result.LedType) {
        case CAPS_LOCK:
          Serial.println("caps");
          player.dir -=  turn;
          //_x+=50;
          break;
        case NUM_LOCK:
          Serial.println("num");
          player.dir += turn;
          //_y+=50;
          break;
      }

    }
    //Serial.println(String(result.LedType)+" "+String(result.TimeOut));
  }
  while(1){}
  //AbsoluteMouse.release();
  //AbsoluteMouse.stop();
}
