#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
const int SWITCH_PIN = 4;
const int GREEN_LED = 18;
const int RED_LED = 19;
const int BUZZER = 23;
String towerID = "TL-07";

int faultCount = 0;
bool previousState = HIGH;

void showNormalDisplay() {

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(18,0);
  display.println("SMART GRID");

  display.drawLine(0,12,128,12,SSD1306_WHITE);

  display.setCursor(0,22);
  display.println("STATUS : NORMAL");

  display.setCursor(0,42);
  display.print("FAULT COUNT : ");
  display.println(faultCount);

  display.display();
}

void showFaultDisplay() {

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(18,0);
  display.println("SMART GRID");

  display.drawLine(0,12,128,12,SSD1306_WHITE);

  display.setCursor(0,22);
  display.println("STATUS : FAULT");

  display.setCursor(0,42);
  display.print("FAULT COUNT : ");
  display.println(faultCount);

  display.display();
}

void normalCondition() {

  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(RED_LED, LOW);

  ledcWriteTone(BUZZER, 0);

  showNormalDisplay();
  Serial.println();
  Serial.println("=======================================================");
  Serial.println("          TRANSMISSION LINE MONITORING SYSTEM");
  Serial.println("=======================================================");
  Serial.println();
  Serial.print("Tower ID        : ");
  Serial.println(towerID);
  Serial.println("Line Status     : HEALTHY");
  Serial.println("Power Supply    : NORMAL");
  Serial.print("Fault Count     : ");
  Serial.println(faultCount);
  Serial.println("Alarm           : OFF");
  Serial.println();
  Serial.println("System Status");
  Serial.println(" ------------------------------------------------------------");
  Serial.println(" ✓Line Healthy");
  Serial.println(" ✓Monitoring Active");
  Serial.println(" ✓Ready for Fault Detection");
  Serial.println(" System Status");
  Serial.println();
  Serial.println("==============================================================");
}

void faultCondition() {

  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, HIGH);

  showFaultDisplay();

  Serial.println();
  Serial.println("=======================================================");
  Serial.println("              TRANSMISSION LINE FAULT");
  Serial.println("=======================================================");
  Serial.print("Tower ID    : ");
  Serial.println(towerID);
  Serial.println("Line Status        : FAULT");
  Serial.println("Power Supply       : INTERRUPTED");
  Serial.print("Fault Count        : ");
  Serial.println(faultCount);
  Serial.println("Alarm            : ACTIVE");
  Serial.println();
  Serial.print("Action Taken");
  Serial.println(" ------------------------------------------------------------");
  Serial.println(" ✓Fault Detected");
  Serial.println(" ✓Tower Identified");
  Serial.println(" ✓Buzzer Activated");
  Serial.println(" ✓Alert Ready for Blynk Dashboard");
  Serial.println(" ✓ Maintenance Team Can Be Notified");
  Serial.println();
  Serial.println("=======================================================");

  ledcWriteTone(BUZZER, 1000);
  delay(250);

  ledcWriteTone(BUZZER, 0);
  delay(250);

  ledcWriteTone(BUZZER, 1000);
  delay(250);

  ledcWriteTone(BUZZER, 0);
}

void setup() {

  Serial.begin(115200);

  Wire.begin(21,22);

  pinMode(SWITCH_PIN, INPUT);

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  ledcAttach(BUZZER, 2000, 8);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

    Serial.println("OLED Initialization Failed");

    while (true);

  }

  display.clearDisplay();

  display.setTextSize(2);

  display.setTextColor(SSD1306_WHITE);

  display.setCursor(10,20);

  display.println("WELCOME");

  display.display();

  delay(2000);

  display.clearDisplay();
}

void loop() {

  bool currentState = digitalRead(SWITCH_PIN);

  // Count only when NORMAL -> FAULT
  if (previousState == HIGH && currentState == LOW) {

    faultCount++;

    Serial.println("==============================");
    Serial.println("NEW FAULT RECORDED");
    Serial.print("Fault Number : ");
    Serial.println(faultCount);
    Serial.println("==============================");
  }

  if (currentState == HIGH) {

    normalCondition();

  }
  else {

    faultCondition();

  }

  previousState = currentState;

  delay(500);
}
