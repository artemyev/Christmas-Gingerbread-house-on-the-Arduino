// ATtiny85    -------------- Arduino Leonardo
// MISO  pin 6 -------------- ICSP pin 1 MISO
// MOSI  pin 5 -------------- ICSP pin 4 MOSI
// SCK   pin 7 -------------- ICSP pin 3 SCK
// RESET pin 1 -------------- digital pin 10

const int ledPin[] = {0,1,2,3,4}; // AVR pins 5,6,7,2,3
int count = 0;
int def1 = 0;
int def2 = 0;

const int cfg_shutdown = 3000; // Через сколько циклов выключать вывеску, елку и мяч
const int cfg_restart = 3700; // Через сколько циклов перезапускать программу

void setup() {
  for (int z=1;z<6;z++) {
    pinMode(ledPin[z], OUTPUT);
  }
}

void loop() {
  count++;
  def1 = random(1000,1500);
  def2 = random(1600,2000);
  if (count==1) {
    // Включаем огни в окнах	
    for (int x=0;x<255;x++) {
      spwm(x,ledPin[0],1);
      spwm(x,ledPin[1],1);
    }
  }
  // Имитируем эффект горения свечи в окнах
  Fire_In_Windows(100,140,50);

  // Включаем елку и мяч под окном
  if (count==5) {
    analogWrite(ledPin[4], 255);
    delay(random(20));
    analogWrite(ledPin[3], 255);
  }

  // Запускаем вывеску над входом
  if (count==6) {
    Start_Strobos(ledPin[2],10);
  }

  // Выключаем
  if (count==def1) {
    // Вывеску
    for (int x=254;x>0;x--) {
      spwm(x,ledPin[2],3);
    }
  }

  // Запускаем вывеску над входом
  if (count==def2) {
    Start_Strobos(ledPin[2],10);
  }

  // Выключаем
  if (count==cfg_shutdown) {
    // Вывеску
    for (int x=254;x>0;x--) {
      spwm(x,ledPin[2],3);
    }
    delay(random(20));
    // Елку мяч
    analogWrite(ledPin[3], 0);
    delay(random(20));
    analogWrite(ledPin[4], 0);
  }
  // Перезапускаем программу
  if (count==cfg_restart) {
     count=3; // Не полностью :)
  }
}

// Функция имитации горения свечи в окнах
void Fire_In_Windows(int start,int finish,int pause) {
  int level = random(start,finish);
  int levelmin = 255 - level;
  analogWrite(ledPin[0], random(level)+levelmin);
  analogWrite(ledPin[1], random(level)+levelmin);
  delay(random(pause));
}

// Функция включения эффектом стартера (как лампа ДС)
void Start_Strobos(int spin, int cn) {
  for (int x=0;x<cn;x++) {
    digitalWrite(spin,HIGH);
    delay(x);
    digitalWrite(spin,LOW);
    delay(x);
    // Включаем
    if(x==cn-1){
      digitalWrite(spin,HIGH);
    }
  }
}

// Функция плавного включения/выключения (диммер)
void spwm(int freq,int spin,int sp) {
  // on
  digitalWrite(spin,HIGH);
  delayMicroseconds(sp*freq);
  // off
  digitalWrite(spin,LOW);
  delayMicroseconds(sp*(255-freq));
}
