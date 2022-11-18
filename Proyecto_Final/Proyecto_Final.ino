#include <analogWrite.h>
#include <WiFi.h>
#include <FirebaseESP32.h>

//============================================= Usuario y Contraseña WIFI
#define ssid "ORT-IoT"
#define pass "OrtIOTnew22$2"
//=============================================

//================================================================== FIREBASE
#define url "projectofinal-f4108-default-rtdb.firebaseio.com/"
#define secreto "E8153b8tbwFxpFIFQiv14riHICsf2VlTtIwq6q0m"
//================================================================== 

#define ledINT 2 //led integrado

#define MOTOR_VENTILADOR_1 33                
#define MOTOR_VENTILADOR_2 32

#define POT_VENTILADOR 50

#define MOTOR_PERSIANA_2 26                  
#define MOTOR_PERSIANA_1 27
#define MOTOR_PERSIANA_PWM 25
#define POT_PERSIANA 150
#define TIEMPO_PERSIANA 250

#define LUZ1 14
#define LUZ2 13
#define LUZ3 12

#define PIN_AUXILIAR_1 5
#define PIN_AUXILIAR_2 17

FirebaseData myFireBaseData;
FirebaseJson myJson;
FirebaseJsonData myJsonData;

boolean estadoLed3, estadoLedINT, estadoLed1, estadoLed2, estadoLed4, estadoVentilador, estadoPersianaSube, estadoPersianaBaja;  

String estados;


void setup() {                          //----------------------------------------------------------------------------- setup
  Serial.begin(115200);

  WiFi.begin(ssid, pass);
  Firebase.begin(url, secreto);
  Firebase.reconnectWiFi(true);

  pinMode(ledINT, OUTPUT);

  pinMode(MOTOR_VENTILADOR_1, OUTPUT);
  pinMode(MOTOR_VENTILADOR_2, OUTPUT);

  pinMode(MOTOR_PERSIANA_1, OUTPUT);
  pinMode(MOTOR_PERSIANA_2, OUTPUT);
  pinMode(MOTOR_PERSIANA_PWM, OUTPUT);

  pinMode(LUZ1, OUTPUT);
  pinMode(LUZ2, OUTPUT);
  pinMode(LUZ3, OUTPUT);
}

void loop() {                           //----------------------------------------------------------------------------- loop

  Firebase_Luz1 ();
  Firebase_Luz2 ();
  Firebase_Luz3 ();

  Firebase_ventilador ();
  Firebase_persiana ();
}

//----------------------------------------------- ventilador
void ventiladorON()
{
  analogWrite(MOTOR_VENTILADOR_1, POT_VENTILADOR);
  digitalWrite(MOTOR_VENTILADOR_2, LOW);
}

void ventiladorOFF()
{
  analogWrite(MOTOR_VENTILADOR_1, 0);
  digitalWrite(MOTOR_VENTILADOR_2, LOW);
}

//----------------------------------------------- persiana
void persianaUP() {
  digitalWrite(MOTOR_PERSIANA_1, HIGH);
  digitalWrite(MOTOR_PERSIANA_2, LOW);

  analogWrite(MOTOR_PERSIANA_PWM, POT_PERSIANA);
}

void persianaDOWN() {
  digitalWrite(MOTOR_PERSIANA_1, LOW);
  digitalWrite(MOTOR_PERSIANA_2, HIGH);

  analogWrite(MOTOR_PERSIANA_PWM, POT_PERSIANA);
}

void persianaSTOP() {
  digitalWrite(MOTOR_PERSIANA_1, LOW);
  digitalWrite(MOTOR_PERSIANA_2, LOW);

  analogWrite(MOTOR_PERSIANA_PWM, POT_PERSIANA);
}

//----------------------------------------------- luces
void luz1_ON () {
  digitalWrite(LUZ1, HIGH);
}
void luz1_OFF () {
  digitalWrite(LUZ1, LOW);
}
//------------------------
void luz2_ON () {
  digitalWrite(LUZ2, HIGH);
}
void luz2_OFF () {
  digitalWrite(LUZ2, LOW);
}
//------------------------
void luz3_ON () {
  digitalWrite(LUZ3, HIGH);
}
void luz3_OFF () {
  digitalWrite(LUZ3, LOW);
}



//------------------------------------------------------------------------------------------------------- Firebase
//----------------------------------------------- firebase LED-INT
void Firebase_ledINT () {
  Firebase.get(myFireBaseData, "/ledInt");
  estadoLedINT = myFireBaseData.boolData();

  if (estadoLedINT == true) {
    digitalWrite(ledINT, HIGH);
  }
  else {
    digitalWrite(ledINT, LOW);
  }
}

//----------------------------------------------- firebase LUZ1
void Firebase_Luz1 () {
  Firebase.get(myFireBaseData, "/led");
  estadoLed1 = myFireBaseData.boolData();

  if (estadoLed1 == true) {
    luz1_ON();
  }
  else {
    luz1_OFF();
  }
}

//----------------------------------------------- firebase LUZ2
void Firebase_Luz2 () {
  Firebase.get(myFireBaseData, "/led1");
  estadoLed2 = myFireBaseData.boolData();

  if (estadoLed2 == true) {
    luz2_ON();
  }
  else {
    luz2_OFF();
  }
    
}

//----------------------------------------------- firebase LUZ3
void Firebase_Luz3 () {
  Firebase.get(myFireBaseData, "/led3");
  estadoLed3 = myFireBaseData.boolData();


  if (estadoLed3 == true) {
    luz3_ON();
  }
  else {
    luz3_OFF();
  }
}

//----------------------------------------------- firebase Ventilador
void Firebase_ventilador () {
  Firebase.get(myFireBaseData, "/ventilador");
  estadoVentilador = myFireBaseData.boolData();

  if (estadoVentilador == true) {
    ventiladorON();
  }
  else {
    ventiladorOFF();
  }
}

//----------------------------------------------- firebase Persiana
void Firebase_persiana () {
  Firebase.get(myFireBaseData, "/persianasube");
  estadoPersianaSube = myFireBaseData.boolData();

  Firebase.get(myFireBaseData, "/persianabaja");
  estadoPersianaBaja = myFireBaseData.boolData();

  if (estadoPersianaSube == false && estadoPersianaBaja == false) {
    persianaSTOP();
  }
  else if (estadoPersianaSube == true && estadoPersianaBaja == false) {
    persianaUP();
    delay(TIEMPO_PERSIANA);
    persianaSTOP();
    
    Firebase.set(myFireBaseData, "/persianasube", false);
    Firebase.set(myFireBaseData, "/estadopersiana", true);
  }
  else if (estadoPersianaBaja == true && estadoPersianaSube == false) {
    persianaDOWN();
    delay(TIEMPO_PERSIANA);
    persianaSTOP();
    
    Firebase.set(myFireBaseData, "/persianabaja", false);
    Firebase.set(myFireBaseData, "/estadopersiana", false);
  }
  else if (estadoPersianaBaja == true && estadoPersianaSube == true) { 
    Firebase.set(myFireBaseData, "/persianabaja", false);
    Firebase.set(myFireBaseData, "/persianasube", false);
  }

}

//-----------------------------------------------

//-----------------------------------------------
