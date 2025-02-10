#include <LiquidCrystal.h>
#include <Stepper.h>

const int relay1 = 22; 
const int relay2 = 24; 
const int relay3 = 26;

const int trigPin = 28;
const int echoPin = 30;

const int button1 = 32; 
const int button2 = 34; 
const int button3 = 36; 
const int button4 = 38; 
const int button5 = 40; 
const int button6 = 42; 

const int rs = 44, en = 46, d4 = 48, d5 = 50, d6 = 52, d7 = 54;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int stepsPerRevolution = 2048; 
Stepper myStepper(stepsPerRevolution, 37, 39, 41, 43);


bool isReady = false; 
int currentRecipe = 0; 
int distanceToCup = 0; 

void setup() {
  Serial.begin(9600);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  pinMode(button4, INPUT_PULLUP);
  pinMode(button5, INPUT_PULLUP);
  pinMode(button6, INPUT_PULLUP);

  lcd.begin(16, 2);
  lcd.print("Добро пожаловать!");
  lcd.setCursor(0, 1);
  lcd.print("Выберите рецепт:");

  myStepper.setSpeed(15);
}

void loop() {
  if (!digitalRead(button1)) selectRecipe(1);
  else if (!digitalRead(button2)) selectRecipe(2);
  else if (!digitalRead(button3)) selectRecipe(3);
  else if (!digitalRead(button4)) selectRecipe(4);
  else if (!digitalRead(button5)) selectRecipe(5);
  else if (!digitalRead(button6)) selectRecipe(6);

  if (currentRecipe != 0 && !isReady) {
    prepareDrink(currentRecipe);
    isReady = true;
  }

  if (isReady) {
    deliverOrder();
    isReady = false;
    currentRecipe = 0;
    lcd.clear();
    lcd.print("Готово! Заберите");
    lcd.setCursor(0, 1);
    lcd.print("заказ.");
  }
}

void selectRecipe(int recipe) {
  currentRecipe = recipe;
  lcd.clear();
  lcd.print("Вы выбрали рецепт ");
  lcd.print(recipe);
  lcd.setCursor(0, 1);
  lcd.print("Приготовим...");
  
  Serial.print("Вы выбрали рецепт ");
  Serial.println(recipe);
}

void prepareDrink(int recipe) {
  switch (recipe) {
    case 1:
      dispenseFluid(relay1, 50);
      break;
    case 2: 
      dispenseFluid(relay2, 10);
      break;
    case 3: 
      dispenseFluid(relay3, 40);
      break;
    case 4: 
      dispenseFluid(relay1, 80);
      dispenseFluid(relay2, 20);
      break;
    case 5:
      dispenseFluid(relay1, 30);
      dispenseFluid(relay3, 50);
      break;
    case 6:
        (relay1, 35);
      dispenseFluid(relay3, 45);
      dispenseFluid(relay2, 10);
      break;
  }
}

void dispenseFluid(int relayPin, int volume) {
  digitalWrite(relayPin, HIGH); 
  delay(volume * 50); 
  digitalWrite(relayPin, LOW); 
}

void deliverOrder() {
  while (distanceToCup > 10) { 
    myStepper.step(-stepsPerRevolution / 4); 
    measureDistance(); 
  }
}

void measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  distanceToCup = duration * 0.034 / 2;
}-