#include <HID-Project.h>
int LedA = 9;
int LedB = 10;
enum BlinkType {
  NUM_LOCK,
  CAPS_LOCK,
  SCROLL_LOCK,
  COMPOSE,
  KANA,
  ANY,
  None
};
bool CapsLock = false;
bool NumLock = false;

struct BlinkResult {
  BlinkType LedType;
  int Blink;
  bool TimeOut;
};
struct LedStatus {
  int leds;
  bool NumLockPrev, CapsLockPrev;
  int Blinks[5] = {0, 0, 0, 0, 0}; //NUM_LOCK,CAPS_LOCK,SCROLL_LOCK,COMPOSE,KANA,
  long StartTime;
  long CurrentTime;
  long WaitTime;
  bool Started = false;

  void init() {
    Keyboard.begin();
    AbsoluteMouse.begin();
    pinMode(LedB, OUTPUT);
    pinMode(LedA, OUTPUT);
  }

  bool Update(long WaitTime) {
    bool TimeOut;
    leds = BootKeyboard.getLeds();
    //Serial.println(leds);
    NumLockPrev = NumLock;
    CapsLockPrev = CapsLock;
    CurrentTime = millis();
    //Serial.println(" started:"+String(Started));
    if (!Started) {
      for (int i = 0; i < 5; i++) Blinks[i] = 0;
      Started = true;
      StartTime = millis();
      TimeOut = false;
      //Serial.println("start");
      //Serial.println("start:"+String(StartTime)+" started:"+String(Started));
    }
    else {
      if (CurrentTime >= StartTime + WaitTime){
        TimeOut = true;
        //Serial.println("stop");
      }
      else TimeOut = false;
      
    }
    if (leds & (1 << NUM_LOCK)) NumLock = true;
    else NumLock = false;
    if (leds & (1 << CAPS_LOCK)) CapsLock = true;
    else CapsLock = false;
    //Serial.println(String(NumLockPrev)+"  "+String(NumLock));
    if (NumLockPrev != NumLock) Blinks[0]++;
    if (CapsLockPrev != CapsLock) Blinks[CAPS_LOCK]++;
    digitalWrite(LedA, NumLock);
    digitalWrite(LedB, CapsLock);
    //for(int i=0;i<4;i++) Serial.print(String(Blinks[i])+",");
    //Serial.println(String(StartTime) + " " + String(CurrentTime) + " " + String(Started));
    return TimeOut;
  }

  BlinkResult GetLedBlink(BlinkType Type, long WaitTime) {
    BlinkResult result;
    result.TimeOut = Update(WaitTime);
    if(result.TimeOut) Started = false;
    switch (Type) {
      case CAPS_LOCK:
        result.Blink = Blinks[CAPS_LOCK];
        result.LedType = CAPS_LOCK;
        //Serial.println("Caps lock blink: " + String(Blink));
        break;
      case NUM_LOCK:
        result.Blink = Blinks[NUM_LOCK];
        result.LedType = NUM_LOCK;
        //Serial.println("Num lock blink: " + String(Blink));
        break;
      default:
        result.Blink = -1;
        result.LedType = None;
        break;
    }
    //Serial.println(String(TimeIndex) + "  " + String(Blink));
    return result;
  }

  BlinkResult WaitLedBlink(BlinkType Type, long WaitTime) {
    int Blink;
    BlinkResult result;
    result.TimeOut = Update(WaitTime);
    //Serial.println(String(NumLockBlink)+" "+String(CapsLockBlink)+" "+String(TimeIndex));
    switch (Type) {
      case CAPS_LOCK:
        if (Blinks[CAPS_LOCK] > 0) {
          result.LedType = CAPS_LOCK;
          Started = false;
        }
        else result.LedType = None;
        break;
      case NUM_LOCK:
        if (Blinks[NUM_LOCK] > 0) {
          result.LedType = NUM_LOCK;
          Started = false;
        }
        else result.LedType = None;
        break;
      case ANY:
        if (Blinks[NUM_LOCK] > 0 || Blinks[CAPS_LOCK] > 0) {
          if (Blinks[NUM_LOCK] >= Blinks[CAPS_LOCK]) {
            result.LedType = NUM_LOCK;
            Started = false;
          }
          else {
            result.LedType = CAPS_LOCK;
            Started = false;
          }
        }
        else result.LedType = None;;
        break;
      default:
        result.LedType = None;
        break;

    }
    //Serial.println(String(TimeIndex) + "  " + String(Blink));
    //Serial.println(String(result.TimeOut));
    return result;
  }
};
