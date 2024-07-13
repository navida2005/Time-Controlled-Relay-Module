#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin Variables
int relay[] = {4, 5, 6, 7};
int up = 8;
int down = 9;
int set = 10;
int reset = 11;
int buzzer = 12;
int slots = 0;
int time = 0;
int gap = 0;
int set_btn = 1;

void setup() {
  // Outputs
  pinMode(relay[0], OUTPUT);
  pinMode(relay[1], OUTPUT);
  pinMode(relay[2], OUTPUT);
  pinMode(relay[3], OUTPUT);
  pinMode(buzzer, OUTPUT);

  // Inputs
  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(set, INPUT_PULLUP);
  pinMode(reset, INPUT_PULLUP);

  // Initializing relays to HIGH
  for (int i = 0; i < 4; i++) {
    digitalWrite(relay[i], HIGH);
  }

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Welcome!!!");
  lcd.setCursor(0, 1);
  Buzzerout();
  lcd.print("Initializing...");
  delay(3000);
  Serial.begin(9600);
}

void loop() {
  Modules();
  if (slots != 0){
    Activate();
    Module_gap();
    Launch();
  }
}

void Buzzerout() {
  tone(buzzer, 2300);
  delay(500);
  noTone(buzzer);
  delay(500);
  tone(buzzer, 2300);
  delay(500);
  noTone(buzzer);
}

void Buzzerout_launch(){
  tone(buzzer, 2300);
  delay(3000);
  noTone(buzzer);
}

void Modules() {
  lcd.clear();
  set_btn = 1;
  while (set_btn == 1) {
    int up_btn = digitalRead(up);
    int down_btn = digitalRead(down);
    int reset_btn = digitalRead(reset);

    if (up_btn == 0 && slots < 4) {
      slots++;
    }
    if (down_btn == 0 && slots > 0) {
      slots--;
    }
    if (reset_btn == 0) {
      Reset_var();
      break;
    }

    lcd.setCursor(0, 0);
    lcd.print("No Of Slots: ");
    lcd.print(slots);
    set_btn = digitalRead(set);
    delay(110);
  }
}

void Activate() {
  set_btn = 1;
  lcd.clear();
  while (set_btn == 1) {
    int up_btn = digitalRead(up);
    int down_btn = digitalRead(down);
    int reset_btn = digitalRead(reset);

    if (up_btn == 0 && time < 1000) {
      time++;
    }
    if (down_btn == 0 && time > 0) {
      time--;
    }
    if (reset_btn == 0) {
      Reset_var();
      break;
    }

    lcd.setCursor(0, 0);
    lcd.print("Set timer: ");
    lcd.print(time);
    lcd.print("s   ");
    set_btn = digitalRead(set);
    delay(110);
  }
}

void Module_gap() {
  set_btn = 1;
  lcd.clear();
  while (set_btn == 1 && slots > 1) {
    int up_btn = digitalRead(up);
    int down_btn = digitalRead(down);
    int reset_btn = digitalRead(reset);

    if (up_btn == 0 && gap < 1000) {
      gap++;
    }
    if (down_btn == 0 && gap > 0) {
      gap--;
    }
    if (reset_btn == 0) {
      Reset_var();
      break;
    }

    lcd.setCursor(0, 0);
    lcd.print("Gap time: ");
    lcd.print(gap);
    lcd.print("s   ");
    set_btn = digitalRead(set);
    delay(110);
  }
}

void Reset_var() {
  slots = 0;
  time = 0;
  gap = 0;
  for (int i = 0; i < 4; i++) {
    digitalWrite(relay[i],HIGH);
  }

}

void Launch() {
  delay(1000);
  if (slots != 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Setting up...");
    delay(3000);

    lcd.clear();
    lcd.setCursor(0, 0); // Centered position for line 1
    lcd.print("-Activating  In-");
    for (int i = 5; i > 0; i--) {
      lcd.setCursor(6, 1); // Centered position for line 2
      lcd.print(i);
      lcd.print("s   ");
      delay(1000);
    }
    lcd.clear();
    lcd.setCursor(0, 0); // Adjusted position for centered display
    lcd.print("-Launch No 1 In-");
    for (int i = time; i > 0; i--) {
      lcd.setCursor(6, 1); // Adjusted position for centered display
      lcd.print(i);
      lcd.print("s   ");
      delay(1000);
      lcd.setCursor(6, 1);
      lcd.print("    "); // Clear previous number
    }
    digitalWrite(relay[0], LOW);
    Buzzerout_launch();
    digitalWrite(relay[0], HIGH);
    for (int i = 1; i < slots; i++) {
      lcd.clear();
      lcd.setCursor(0, 0); // Adjusted position for centered display
      lcd.print("-Launch No ");
      lcd.print(i + 1);
      lcd.print(" In-");
      for (int j = gap; j > 0; j--) {
        lcd.setCursor(6, 1); // Adjusted position for centered display
        lcd.print(j);
        lcd.print("s   ");
        delay(1000);
        lcd.setCursor(6, 1);
        lcd.print("    "); // Clear previous number
      }
      digitalWrite(relay[i], LOW);
      Buzzerout_launch();
      digitalWrite(relay[i], HIGH);
    }
    lcd.clear();
    delay(5000);
    lcd.setCursor(0, 0);
    lcd.print("Launch Complete");
    delay(2000);
    Reset_var();
  }
}

