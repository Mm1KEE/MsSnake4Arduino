long CanvasWidth = 1920;
long CanvasHeight = 1080;
long CanvasStartX = 5;
long CanvasStartY = 55;

struct Player {
  float speed = 100;
  float StepDelay=100;
  float dir = HALF_PI/2;
  //float p[2] = {0.5 * CanvasWidth + CanvasStartX, 0.5 * CanvasHeight + CanvasStartY}; //{x,y}
  float p[2] = {0, 0};
  float v[2] = {0, 0}; //{x,y}
  //float border[4] = {1*CanvasWidth+CanvasStartX, 1*CanvasHeight+CanvasStartY, 0.2*CanvasWidth+CanvasStartX, 0.2*CanvasHeight+CanvasStartY}; //{lx,ly,hx,hy}
  float border[4] = { -10000, -10000, 10000, 10000};
  bool pressed = false;
  bool Step() {
    delay(StepDelay);
    GetVec();
    p[0] += v[0];
    p[1] += v[1];
    Serial.println("p:"+String(p[0])+","+String(p[1])+" v:"+String(v[0])+","+String(v[1])+" dir:"+String(dir)+","+String(sin(dir))+","+String(cos(dir)));
    if (p[0] < border[0] ||
        p[0] > border[2] ||
        p[1] < border[1] ||
        p[1] > border[3] )
      return false;
    MoveMouse();
    return true;
  }
  void GetVec() {
    v[0] = sin(dir) * speed;
    v[1] = cos(dir) * speed;
  }
  void MoveMouse() {
    AbsoluteMouse.moveTo(p[0], p[1]);
  }
  void ToggleButton() {
    if (pressed) AbsoluteMouse.click();
    else AbsoluteMouse.release();
    pressed = !pressed;
  }
};
