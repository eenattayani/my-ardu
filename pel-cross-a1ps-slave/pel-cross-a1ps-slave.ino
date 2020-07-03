#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define MIN 0
#define MAX 1

RF24 radio(9, 10); // CE, CSN

const int button = 2;
const int ledButton =  3;
const int speaker = 5;

const int alamatSatu = 6;
const int alamatDua = 7;
const int switchMinMax = 8;

const int pedRed = A0;
const int pedGreen = A1;
const int carRed = A2;
const int carYellow = A3;
const int carGreen =  A4;

byte address = 225;

bool buttonState = 0; 
bool minMaxState = MIN;
bool ledState = 0;

unsigned int count = 0;
unsigned long prevMillis = 0;
unsigned long curMillis = 0;
unsigned long hitMillis = 0;
const long interval = 1000;
int kuning = 1;

int sentMessage[1];
int dataMasuk[1] = {0};

void setup() {
  pinMode(button, INPUT);
  pinMode(ledButton, OUTPUT);
  pinMode(speaker, OUTPUT);
  pinMode(alamatSatu, INPUT);
  pinMode(alamatDua, INPUT);
  pinMode(switchMinMax, INPUT);
  pinMode(pedRed, OUTPUT);
  pinMode(pedGreen , OUTPUT);
  pinMode(carRed , OUTPUT);
  pinMode(carYellow , OUTPUT);
  pinMode(carGreen , OUTPUT);

  digitalWrite(button, LOW);
  digitalWrite(carYellow, HIGH);

  if (digitalRead(alamatSatu) == 1) {address = 224;}
  else if (digitalRead(alamatDua) == 1) {address = 223;}
  
  Serial.begin(9600);
  
  mode_receive();
}

void loop() {
  buttonState = digitalRead(button);
  if (buttonState == HIGH) {
    digitalWrite(ledButton, HIGH);
    ledState = 1;
    mode_transmit();
    while (buttonState == HIGH) {
      
      kirim_data(1);
      
      delay(300);
      buttonState = digitalRead(button);
   }
   mode_receive();
  } else {
    switch ( terima_data() ) {
        case 100:
          ledState = 1;
          digitalWrite(ledButton, HIGH);
          Serial.println("Tombol Ditekan");
        break;
        case 1000 :
          Serial.println("Semua Padam");
          
          digitalWrite(pedRed, LOW);
          digitalWrite(pedGreen, LOW);
          digitalWrite(carRed, LOW);
          digitalWrite(carYellow, LOW);
          digitalWrite(carGreen, LOW);
          
          digitalWrite(ledButton, LOW);
          ledState = 0;
        break;
        case 10 : 
          Serial.println("Kendaraan Padam");
          
          digitalWrite(carRed, LOW);
          digitalWrite(carYellow, LOW);
          digitalWrite(carGreen, LOW);
        break;
        case 11 :
          Serial.println("Kendaraan Merah");
          
          digitalWrite(carRed, HIGH);
          digitalWrite(carYellow, LOW);
          digitalWrite(carGreen, LOW);
        break;
        case 12 :
          Serial.println("Kendaraan Kuning");
          
          digitalWrite(carRed, LOW);
          digitalWrite(carYellow, HIGH);
          digitalWrite(carGreen, LOW);
        break;
        case 13 :
          Serial.println("Kendaraan Hijau");

          digitalWrite(carRed, LOW);
          digitalWrite(carYellow, LOW);
          digitalWrite(carGreen, HIGH);
        break;
        case 14 :
          Serial.println("Kendaraan Merah Kuning");
          digitalWrite(carRed, HIGH);
          digitalWrite(carYellow, HIGH);
          digitalWrite(carGreen, LOW);
        break;
        case 20 :
          Serial.println("Pedestrian Padam");

          digitalWrite(pedRed, LOW);
          digitalWrite(pedGreen, LOW);
        break; 
        case 21 :
          Serial.println("Pedestrian Merah");
          
          digitalWrite(pedRed, HIGH);
          digitalWrite(pedGreen, LOW);
        break;
        case 23 :
          Serial.println("Pedestrian Hijau");

          digitalWrite(pedRed, LOW);
          digitalWrite(pedGreen, HIGH);
        break;
        default:

        break;
      }
  }  

  if ( ledState == 0 ) { hitungMillis();}
}



//******** FUNGSI-FUNGSI ************//

void hitungMillis()
{
  curMillis = millis();

  if ( curMillis - prevMillis > interval ) {
    kuning_flashing();
    hitMillis = hitMillis + 1;
    prevMillis = curMillis;
  }
}

void kuning_flashing()
{
  if ( kuning == 0 ) {
    digitalWrite(carYellow,LOW);
    kuning = 1;
    Serial.print(hitMillis);
    Serial.println(" OFF");
  } 
  else {
    digitalWrite(carYellow,HIGH);
    kuning = 0;
    Serial.print(hitMillis);
    Serial.println(" ON");
  }
}

void mode_transmit(){
  radio.begin();
  radio.openWritingPipe(address);
  if ( minMaxState == MIN ) {
    radio.setPALevel(RF24_PA_MIN);
  } else {
    radio.setPALevel(RF24_PA_MAX);
  }
  radio.stopListening(); 
}

void kirim_data(int isiData){
  sentMessage[0] = isiData;
  radio.write(sentMessage, 2);

  Serial.print("hello ");
  Serial.println(count);
  Serial.print("sent Message:  ");
  Serial.println(sentMessage[0]);
}

void mode_receive(){
  radio.begin();
  radio.openReadingPipe(0, address);
  if ( minMaxState == MIN ) {
    radio.setPALevel(RF24_PA_MIN);
  } else {
    radio.setPALevel(RF24_PA_MAX);
  }
  radio.startListening();
}

int terima_data(){
  dataMasuk[0] = 0;
  if (radio.available()) {
    
    radio.read(dataMasuk, 2);
    
    Serial.print(dataMasuk[0]);
    Serial.print(" ");
    count += 1;
    Serial.print(count);
    Serial.print(" ");
  }

  return dataMasuk[0];
}