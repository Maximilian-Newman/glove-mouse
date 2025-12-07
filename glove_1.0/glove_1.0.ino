#include "SparkFunLIS3DH.h"
#include "Wire.h"
#include <Mouse.h>
#include <Keyboard.h>



bool scroll = false;

int sensitivityF = 2;
float sensitivityS = 0.5;
float sensitivity = sensitivityS;
int Smargin = 7;
int Bmargin = 1;


long thumbPressTime = 0;
long indexPressTime = 0;
long pinkiePressTime = 0;


int totSavedPass = 3;
int autoPassStep = 0;



String APname1 = "Service 1";
String APuser1 = "Username";
String APpass1 = "Password";

String APname2 = "Service 2";
String APuser2 = "username";
String APpass2 = "password";

String APname3 = "Service 3";
String APuser3 = "username";
String APpass3 = "password";


bool autoPassActivate = false;

int IDnum;
String lastPrt;

int pressFinger() {
  if (digitalRead(A0) == 0){
    return 2;
  }
  else if (digitalRead(A1) == 0){
    return 3;
  }
  else if (digitalRead(A2) == 0){
    return 1;
  }
  else{
    return 0;
  }
}

void deleteM(int amount){
  Keyboard.begin();
  while (amount > 0){
    Keyboard.write(KEY_BACKSPACE);
    amount = amount - 1;
  }
  Keyboard.end();
}


void autoPassStart(){
  sensitivity = sensitivityS;
  autoPassStep = 0;
  autoPassActivate = true;
  Keyboard.begin();
  Keyboard.print("Auto Log In");
  delay(2000);
  deleteM(11);
  Keyboard.begin();
  Keyboard.print("thumb - select option");
  delay(2000);
  deleteM(21);
  Keyboard.begin();
  Keyboard.print("index - previous option");
  delay(2000);
  deleteM(23);
  Keyboard.begin();
  Keyboard.print("pinkie - next option");
  delay(2000);
  deleteM(20);
  Keyboard.end();
  
  delay(1000);
  IDnum = 0;
  lastPrt = "";
}

void autoPassDisplay1(){
  delay(10);
  if (IDnum == 0){
    deleteM(lastPrt.length());
    Keyboard.begin();
    Keyboard.print("Cancel");
    lastPrt = "Cancel";
    Keyboard.end();
  }
  if (IDnum == 1){
    deleteM(lastPrt.length());
    Keyboard.begin();
    Keyboard.print(APname1);
    lastPrt = APname1;
    Keyboard.end();
    
  }
  if (IDnum == 2){
    deleteM(lastPrt.length());
    Keyboard.begin();
    Keyboard.print(APname2);
    lastPrt = APname2;
    Keyboard.end();
    
  }
  if (IDnum == 3){
    deleteM(lastPrt.length());
    Keyboard.begin();
    Keyboard.print(APname3);
    lastPrt = APname3;
    Keyboard.end();
  }
}










void autoPassDisplay2(){
  if (IDnum == 0){
    deleteM(lastPrt.length());
    autoPassActivate = false;
    autoPassStep = 0;
    return;
  }

  
  if (IDnum == 1){
    Keyboard.begin();
    if (autoPassStep == 1){
      Keyboard.print(APuser1);
      delay(300);
      autoPassStep = 2;
    }
    if (pressFinger() == 1){
      Keyboard.print(APpass1);
      Keyboard.write(KEY_RETURN);
      autoPassActivate = false;
      return;
    }
    Keyboard.end();
  }
  
  if (IDnum == 2){
    Keyboard.begin();
    if (autoPassStep == 1){
      Keyboard.print(APuser2);
      delay(300);
      autoPassStep = 2;
    }
    if (pressFinger() == 1){
      Keyboard.print(APpass2);
      Keyboard.write(KEY_RETURN);
      autoPassActivate = false;
      return;
    }
    Keyboard.end();
  }
  
  if (IDnum == 3){
    Keyboard.begin();
    if (autoPassStep == 1){
      Keyboard.print(APuser3);
      delay(300);
      autoPassStep = 2;
    }
    if (pressFinger() == 1){
      Keyboard.print(APpass3);
      Keyboard.write(KEY_RETURN);
      autoPassActivate = false;
      return;
    }
    Keyboard.end();
  }
}









void autoPass() {
  if (!autoPassActivate){
    autoPassStart();
    autoPassDisplay1();
  }



  
  int fin = pressFinger();
  if (fin == 1 or autoPassStep > 0){
    if (autoPassStep == 0){
      deleteM(lastPrt.length());
      autoPassStep = 1;
      delay(250);
    }
    
    autoPassDisplay2();


  }
  else if (fin == 2){
    IDnum = IDnum - 1;
    autoPassDisplay1();
    delay(250);
  }
  else if (fin == 3){
    IDnum = IDnum + 1;
    autoPassDisplay1();
    delay(250);
  }


  if (IDnum < 0){
    IDnum = totSavedPass;
  }
  if (IDnum > totSavedPass){
    IDnum = 0;
  }
}















void switchSensitivity(){
  if (sensitivity == sensitivityS){
    sensitivity = sensitivityF;
  }
  else{
    sensitivity = sensitivityS;
  }
}

LIS3DH myIMU(I2C_MODE, 0x18);
float calibx;
float caliby;
float calibz;

void mouseMove(int x, int y, int scroll){
  Mouse.begin();
  Mouse.move(x, y, scroll);
  Mouse.end();
}
void mouseClick(char button){
  Mouse.begin();
  Mouse.click(button);
  Mouse.end();
}


void recalibrate(){
  calibx = myIMU.readFloatAccelX();
  caliby = myIMU.readFloatAccelY();
  calibz = myIMU.readFloatAccelZ();
}

float accelx() {return myIMU.readFloatAccelX() - calibx;}
float accely() {return myIMU.readFloatAccelY() - caliby;}
float accelz() {return myIMU.readFloatAccelZ() - calibz;}

void setup() {
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  pinMode(6, OUTPUT);
  digitalWrite(6, HIGH);
  
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);

  
  
  Serial.begin(9600);
  
  myIMU.begin();
  myIMU.settings.accelRange = 16;
  myIMU.settings.xAccelEnabled = 1;
  myIMU.settings.yAccelEnabled = 1;
  myIMU.settings.zAccelEnabled = 1;
  myIMU.settings.adcEnabled = 0;

  delay(5000);
  recalibrate();
  delay(500);

}









void loop(){
  if (autoPassActivate){
    autoPass();
  }
  


  



  if (digitalRead(A2) == 0 and digitalRead(A0) == 1 and digitalRead(A1) == 1){
    if (thumbPressTime == 0){
      thumbPressTime = millis();
    }
    else if (millis() - thumbPressTime > 1000){
      Mouse.press(MOUSE_LEFT);
    }
  }

  if (digitalRead(A2) == 1){
    if (thumbPressTime != 0){
      if (millis() - thumbPressTime < 1000){
        switchSensitivity();
        while(digitalRead(A1) == 0);
        delay(50);
      }
      thumbPressTime = 0;
      Mouse.release(MOUSE_LEFT);
    }
  }



  if (digitalRead(A1) == 0){
    if (pinkiePressTime == 0){
      Mouse.press(MOUSE_RIGHT);
      pinkiePressTime = millis();
    }

    
    if (millis() - pinkiePressTime > 1000){
      recalibrate();
    }

  }
  else{
    Mouse.release(MOUSE_RIGHT);
  }




  if (digitalRead(A0) == 1 and indexPressTime > 0){
    if (millis() - indexPressTime < 2000){
      if (scroll){
        scroll = false;
      }
      else{
        scroll = true;
      }
      while (digitalRead(A0) == 0){
        delay(30);
      }
    }
    else{
      autoPass();
    }
    indexPressTime = 0;
  }


  if (digitalRead(A0) == 0 and indexPressTime == 0){
    indexPressTime = millis();
  }





  
  
  if (accely() < -10){
    mouseClick(MOUSE_LEFT);
    while(accely() < 0);
    delay(50);
  }



  


  if (accelx() < -Smargin){
    int xmotion = accelx();
    while(accelx() < Bmargin){
      mouseMove(- xmotion * sensitivity, 0, 0);
      delay(20);
    }
    while(accelx() > Bmargin);
   delay(20);
  }

  if (accelx() > Smargin){
    int xmotion = accelx();
    while(accelx() > -Bmargin){
      mouseMove(- xmotion * sensitivity, 0, 0);
      delay(20);
    }
    while(accelx() < -Bmargin);
   delay(20);
  }
  
  if (accelz() > Smargin){
    int zmotion = accelz();
    while(accelz() > -Bmargin){
      if (scroll){
        Mouse.move(0, 0, 1);
        delay(5);
      }
      else {
        mouseMove(0,- zmotion * sensitivity, 0);
      }
      delay(20);
    }
    while(accelz() < -Bmargin);
   delay(20);
  }
  
  if (accelz() < -Smargin){
    int zmotion = accelz();
    while(accelz() < Bmargin){
      if (scroll){
        Mouse.move(0, 0, -1);
        delay(5);
      }
      else{
        mouseMove(0,- zmotion * sensitivity, 0);
      }
      delay(20);
    }
    while(accelz() > Bmargin);
   delay(20);
  }


  delay(10);
}
