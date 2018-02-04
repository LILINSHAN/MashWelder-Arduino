#include<EEPROM.h>
#include<LiquidCrystal.h>

LiquidCrystal LCD(12, 11, 5, 4, 3, 2); // RS E D4 D5 D6 D7
int up = 6; //向上
int down = 7; //向下
int chage = 8; //模式切换
int in = 9; //控制开关
int out = 10; //输出
int setin = 1;

int mode = 0;
int times = 1;
int set = 2;
int before = 3;
int middle = 4;


int _times = 0;
void setup() {
  // put your setup code here, to run once:
  LCD.begin(16, 2);
  pinMode(1, INPUT);
  for (int i = 6; i <= 9; i++)
  {
    pinMode(i, INPUT);
  }
  
  pinMode(10, OUTPUT);

  if (EEPROM.read(before) > 200)
  {
    EEPROM.write(before, 0);
  }

  if (EEPROM.read(middle) > 200)
  {
    EEPROM.write(middle, 0);
  }
  
  EEPROM.write(set, 0);
  
  if (EEPROM.read(mode) > 1)
  {
    EEPROM.write(mode, 0);
  }
  if (EEPROM.read(times) > 250 || EEPROM.read(times) < 1)
  {
    EEPROM.write(times, 1);
  } 
  LCD.clear();
  displayMode();
  _times = EEPROM.read(times) * 8;
  digitalWrite(out, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(100);
  if (digitalRead(in) == HIGH && EEPROM.read(set) == 0)
  {
    
    if (EEPROM.read(mode) == 0)
    {
      digitalWrite(out, HIGH);
      delay(EEPROM.read(before));
      digitalWrite(out, LOW);
      delay(EEPROM.read(middle));
      digitalWrite(out, HIGH);
      delay(EEPROM.read(times) * 8);
      digitalWrite(out, LOW);
      while(digitalRead(in) == HIGH)
      {
        delay(4);
      }
      delay(500);
    }
    else
    {
      while (digitalRead(in) == HIGH)
      {
        digitalWrite(out, HIGH);
        delay(4);
      }
      digitalWrite(out, LOW);
      delay(500);
    }
  }
  
  else if (digitalRead(setin) == HIGH && EEPROM.read(mode) == 0)
  {
    EEPROM.write(set, EEPROM.read(set) + 1);
    if (EEPROM.read(set) > 3)
    {
      EEPROM.write(set, 0);
    }
    int reads = EEPROM.read(set);
    if (reads == 0)
      {
        LCD.setCursor(1,1);
        LCD.print(" ");
        LCD.setCursor(6,1);
        LCD.print(" ");
        LCD.setCursor(11,1);
        LCD.print(" ");
      }
      else if (reads == 1)
      {
        LCD.setCursor(1,1);
        LCD.print("@");
        LCD.setCursor(6,1);
        LCD.print(" ");
        LCD.setCursor(11,1);
        LCD.print(" ");
      }
       else if (reads == 2)
      {
        LCD.setCursor(1,1);
        LCD.print(" ");
        LCD.setCursor(6,1);
        LCD.print("@");
        LCD.setCursor(11,1);
        LCD.print(" ");
      }
      else if (reads == 3)
      {
        LCD.setCursor(1,1);
        LCD.print(" ");
        LCD.setCursor(6,1);
        LCD.print(" ");
        LCD.setCursor(11,1);
        LCD.print("@");
      }
    
    while (digitalRead(setin) == HIGH)
    {
      delay(4);
    }
  }
  
  else if (digitalRead(up) == HIGH && EEPROM.read(mode) == 0)
  {
    if (EEPROM.read(set) == 1)
    {
      if (EEPROM.read(before) < 200)
      {
        EEPROM.write(before, EEPROM.read(before) + 1);
      }
      displayTimes();
    }
    if (EEPROM.read(set) == 2)
    {
      if (EEPROM.read(middle) < 200)
      {
        EEPROM.write(middle, EEPROM.read(middle) + 1);
      }
      displayTimes();
    }
    if (EEPROM.read(set) == 3)
    {
      if (EEPROM.read(times) < 250)
      {
        EEPROM.write(times, EEPROM.read(times) + 1);
      }
      displayTimes();
    }
    
  }
  else if (digitalRead(down) == HIGH && EEPROM.read(mode) == 0)
  {
    if (EEPROM.read(set) == 1)
    {
      if (EEPROM.read(before) > 0)
      {
        EEPROM.write(before, EEPROM.read(before) - 1);
      }
      displayTimes();
    }
    if (EEPROM.read(set) == 2)
    {
      if (EEPROM.read(middle) > 0)
      {
        EEPROM.write(middle, EEPROM.read(middle) - 1);
      }
      displayTimes();
    }
    if (EEPROM.read(set) == 3)
    {
      if (EEPROM.read(times) > 0)
      {
        EEPROM.write(times, EEPROM.read(times) - 1);
      }
      displayTimes();
    }
  }
  else if (digitalRead(chage) == HIGH && EEPROM.read(set) == 0)
  {
    bool _mode = EEPROM.read(mode) % 2;
    _mode = !_mode;
    EEPROM.write(mode, _mode);
    displayMode();
    delay(500);
  }
}

void displayMode(){
  LCD.setCursor(0,0);
  if (EEPROM.read(mode) == 0)
  {
    LCD.print("      AUTO      ");
    LCD.setCursor(0,1);
    LCD.print("                ");
    displayTimes();
  }
  else
  {
    LCD.print("     MANUAL     ");
    dosplayNoTimes();
  }
}

void displayTimes(){
  
  _times = EEPROM.read(times) * 8;
  LCD.setCursor(2,1);
  LCD.print((EEPROM.read(before) / 100) % 10, DEC);
  LCD.setCursor(3,1);
  LCD.print((EEPROM.read(before) / 10) % 10, DEC);
  LCD.setCursor(4,1);
  LCD.print(EEPROM.read(before) % 10, DEC);
  
  LCD.setCursor(7,1);
  LCD.print((EEPROM.read(middle) / 100) % 10, DEC);
  LCD.setCursor(8,1);
  LCD.print((EEPROM.read(middle) / 10) % 10, DEC);
  LCD.setCursor(9,1);
  LCD.print(EEPROM.read(middle) % 10, DEC);
  
  LCD.setCursor(12,1);
  LCD.print((_times / 1000) % 10, DEC);
  LCD.setCursor(13,1);
  LCD.print((_times / 100) % 10, DEC);
  LCD.setCursor(14,1);
  LCD.print((_times / 10) % 10, DEC);
  LCD.setCursor(15,1);
  LCD.print(_times % 10, DEC);
}

void dosplayNoTimes(){
  LCD.setCursor(0,1);
  LCD.print("     ------     ");
}



