#include <EEPROM.h>
#include <LiquidCrystal.h>
#include "Arduino.h"
#include <Streaming.h>
#include "avdweb_Switch.h"


#define HOME 0
#define PITIDO 1
#define BRILLO 2
#define CONTRASTE 3
#define CAPACIMETRO 4
#define CAMBIAR_BRILLO 5

const byte botonUpPin = 12;
const byte botonDownPin = 3;
const byte botonLeftPin = 2;
const byte botonRightPin = 11;

const int entrada = 9;  //El pin 9 recibe la onda rectangular proveniente del 555.
const int buzzer = 10;  //Pin 10 como salida para el buzzer.
float cap = 0.;        //Variable donde se almacenan el resultado del cálculo.
float ref = 0.;    //Variable utilizada para calibrar el cero.
float dato = 0.;       //Variable que finalmente se envía mediante BT.
boolean flag = false;
boolean inicio = true;
int flagPitido;
char* pitidoOnOff[]={"OFF", "ON"};
int brillo;


Switch botonUp = Switch(botonUpPin);
Switch botonDown = Switch(botonDownPin);
Switch botonLeft = Switch(botonLeftPin);
Switch botonRight = Switch(botonRightPin);

const int rs = 8, en = 13, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

volatile int estado = HOME;

/*Función que emite un sonido cuando se realiza una lectura*/
void sonidoConecta(){
  if(flagPitido == 1)
  {
    tone(buzzer,3951);
    delay(50);
    noTone(buzzer);
    flag = true;
  }
}

/*Función que emite un sonido cuando se desconecta el capacitor*/
void sonidoDesconecta(){
  if(flagPitido)
  {
    tone(buzzer,4186);
    delay(50);
    noTone(buzzer);

    flag = false;
  }
}
void Volver()
{
  estado = HOME;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("->Capacimetro");
  lcd.setCursor(0, 1);
  lcd.print("  Pitido: ");
  lcd.print(pitidoOnOff[flagPitido]);
  inicio = true;
}


void SonidoTeclas(){
  if(flagPitido == 1)
  {
    tone(buzzer,4186);
    delay(25);
    noTone(buzzer);
  }
}




/*==================================================================================*/
void setup()
{
  pinMode(entrada,INPUT);
  attachInterrupt(digitalPinToInterrupt(2), Volver, LOW);
  flagPitido = EEPROM.read(0);
  brillo = EEPROM.read(1);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("->Capacimetro");
  lcd.setCursor(0, 1);
  lcd.print("  Pitido: ");
  lcd.print(pitidoOnOff[flagPitido]);

}

void loop()
{
  botonUp.poll();
  botonDown.poll();
  botonLeft.poll();
  botonRight.poll();

  switch (estado) {
    case HOME:
    if(botonUp.pushed())
    {
      SonidoTeclas();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("->Opcion 3");
      lcd.setCursor(0, 1);
      lcd.print("  Capacimetro");
      estado = CONTRASTE;
    }
    if(botonDown.pushed())
    {
      SonidoTeclas();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  Capacimetro");
      lcd.setCursor(0, 1);
      lcd.print("->Pitido: ");
      lcd.print(pitidoOnOff[flagPitido]);
      estado = PITIDO;
    }
    if(botonRight.pushed())
    {
      SonidoTeclas();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Listo para Medir");
      estado = CAPACIMETRO;
    }
    break;
    case PITIDO:
    if(botonUp.pushed())
    {
      SonidoTeclas();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("->Capacimetro");
      lcd.setCursor(0, 1);
      lcd.print("  Pitido: ");
      lcd.print(pitidoOnOff[flagPitido]);
      estado = HOME;
    }
    if(botonRight.pushed())
    {
      if(EEPROM.read(0) != 0)
      {
        EEPROM.write(0,0);
      }else
      {
        EEPROM.write(0,1);
      }
      flagPitido = EEPROM.read(0);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  Capacimetro");
      lcd.setCursor(0, 1);
      lcd.print("->Pitido: ");
      lcd.print(pitidoOnOff[flagPitido]);
    }
    if(botonDown.pushed())
    {
      SonidoTeclas();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  Pitido: ");
      lcd.print(pitidoOnOff[flagPitido]);
      lcd.setCursor(0, 1);
      lcd.print("->Opcion 2");
      estado = BRILLO;
    }
    break;
    case BRILLO:
    if(botonUp.pushed())
    {
      SonidoTeclas();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("->Pitido: ");
      lcd.print(pitidoOnOff[flagPitido]);
      lcd.setCursor(0, 1);
      lcd.print("  Opcion 2");
      estado = PITIDO;
    }
    if(botonDown.pushed())
    {
      SonidoTeclas();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("  Opcion 2");
      lcd.setCursor(0, 1);
      lcd.print("->Opcion 3");
      estado = CONTRASTE;
    }
    break;
    case CONTRASTE:
    if(botonUp.pushed())
    {
      SonidoTeclas();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("->Opcion 2");
      lcd.setCursor(0, 1);
      lcd.print("  Opcion 3");
      estado = BRILLO;
    }
    if(botonDown.pushed())
    {
      SonidoTeclas();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("->Capacimetro");
      lcd.setCursor(0, 1);
      lcd.print("  Pitido: ");
      lcd.print(pitidoOnOff[flagPitido]);
      estado = HOME;
    }
    break;
    case CAMBIAR_BRILLO:
    if(botonUp.pushed())
    {
      SonidoTeclas();
    }
    if(botonDown.pushed())
    {

    }
    break;
    case CAPACIMETRO:
    delay(50);

/*Se realiza el calculo del valor del capacitor
con la conversión a segundos del tiempo.*/
    cap = ((pulseIn(entrada,HIGH,20000000)*1E-6))/(log(2)*14550);

  /*Se realiza la calibración con el valor
  que se esta leyendo sin haber conectado un capacitor.
  (se colocó un capacitor de valor muy bajo para evitar falsas lecturas)*/
    if (inicio == true){
      ref = cap+0.0000000004;
    }
    inicio = false;
    cap -= ref;
    dato = cap;

//////////////////////////////////////////////////////////////////////////////////////
/*Dependiendo el rango en el
que se encuentra el capacitor conectado, se
hace la vonversión correspondiente*/  
    if(cap < 1E-3 && cap >= 1E-6){
      if (flag == false){
        sonidoConecta();
      }
      dato = (cap*1E6);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Midiendo...");
      lcd.setCursor(5, 1);
      lcd.print(dato,2);
      lcd.print("uF");

    }

    if(cap < 1E-6 && cap >= 1E-9){
      if (flag == false){
        sonidoConecta();
      }
      dato = (cap*1E9);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Midiendo...");
      lcd.setCursor(5, 1);
      lcd.print(dato,2);
      lcd.print("nF");
    }

    if(cap < 1E-9 && cap >= 1E-12){
      if (flag == false){
        sonidoConecta();
      }
      dato = (cap*1E12);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Midiendo...");
      lcd.setCursor(5, 1);
      lcd.print(dato,2);
      lcd.print("pF");
    }
///////////////////////////////////////////////////////////////////////////////////////////


    if(dato<=0.0000000000 && estado == CAPACIMETRO){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Listo para Medir");
    }

//Emite un sonido al desconectar el capacitor medido.
    if (dato<=0.000 && flag == true){
      sonidoDesconecta();
    }

    break;
    default:
    estado = HOME;
  }
  
}
/*=================================================================*/