#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

#define MIN 0
#define MAX 1

SoftwareSerial mp3(0, 1); // RX, TX

RF24 radio(9, 10); // CE, CSN

const int button = 5;
const int ledButton =  A5; // desain lama D6/6
const int speaker = 7;

const int alamatSatu = 2;
//const int alamatDua = 3;
const int switchMinMax = 4;

const int pedRed = A1;
const int pedGreen = A0;
const int carRed = A4;
const int carYellow = A3;
const int carGreen =  A2;

byte address = 225;

bool buttonState = 0; 
bool minMaxState = MIN;
bool ledState = 0; // status enable tombol penyebrangan

byte counterSinyalLost = 0;
const byte batasSinyalLost = 4;


unsigned int count = 0;
unsigned long prevMillis = 0;
unsigned long curMillis = 0;
unsigned long hitMillis = 0;
const long interval = 1000;
int kuning = 1;

int sentMessage[1];
int dataMasuk[1] = {0};

//modul mp3
byte volume = 30;
byte trackMulai = 2;
byte trackStop = 30;  // track kendaraan berhenti
byte trackCD = 31;    // track waktu tunggu
byte trackA = 29;     // track pedestrian
byte trackB = 23;     // track pedestrian

//speaker
int length = 2; // the number of notes
char notes[] = "bg"; // a space represents a rest
int beats[] = { 1, 1};
int tempo = 100;

void setup() {
  pinMode(button, INPUT);
  pinMode(ledButton, OUTPUT);
  pinMode(speaker, OUTPUT);
  pinMode(alamatSatu, INPUT);
//  pinMode(alamatDua, INPUT);
  pinMode(switchMinMax, INPUT);
  pinMode(pedRed, OUTPUT);
  pinMode(pedGreen , OUTPUT);
  pinMode(carRed , OUTPUT);
  pinMode(carYellow , OUTPUT);
  pinMode(carGreen , OUTPUT);

  digitalWrite(button, LOW);
  digitalWrite(carYellow, HIGH);

  if (digitalRead(alamatSatu) == 1) {address = 224;}
//  else if (digitalRead(alamatDua) == 1) {address = 223;}
  minMaxState = digitalRead(switchMinMax);

// print serial dimatikan karena port digunakan untuk Audio
//  Serial.begin(9600);

  mp3.begin(9600);
  while (!mp3) {
      //wait for serial port to connect.
  }
  mp3_set_serial(mp3);
  delay(10);
  mp3_reset();
  delay(1000);
  mp3_set_volume(volume);
  delay(10);
   
  mode_receive();
}

void loop() {
  buttonState = digitalRead(button);
//  Serial.print("button state: ");
//  Serial.println(buttonState);

  if ( buttonState == HIGH ) {
    delay(100);
    buttonState = digitalRead(button);
    if ( buttonState == HIGH ) {
      tombolLED_on();
      mode_transmit();
  
      kirim_data(1);
      delay(300);
  
      mode_receive();
  
      tombol_ditekan();
    }
  } 

  if ( terima_data() != 0 && dataMasuk[0] != 1000 ) {
    tombolLED_on();
    Serial.println("perintah masuk");
    tombol_ditekan();
  }

  hitungMillis();
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
    Serial.print("alamat: ");
    Serial.println(address);
  } 
  else {
    digitalWrite(carYellow,HIGH);
    kuning = 0;
    Serial.print(hitMillis);
    Serial.println(" ON");
  }
}

void tombolLED_on()
{
  digitalWrite(ledButton, HIGH);
  ledState = 1;
  buttonState = 1;
}

void tombolLED_off()
{
  digitalWrite(ledButton, LOW);
  ledState = 0;
  buttonState = 0;
}

void mode_transmit()
{
  if (digitalRead(alamatSatu) == 1) {address = 224;}
  else { address = 225; }
  
  radio.begin();

  minMaxState = digitalRead(switchMinMax);
  
  radio.openWritingPipe(address);
  if ( minMaxState == MIN ) {
    radio.setPALevel(RF24_PA_MIN);
    Serial.print("min max State: ");
    Serial.println(minMaxState);
  } else {
    radio.setPALevel(RF24_PA_MAX);
    Serial.print("min max State: ");
    Serial.println(minMaxState);
  }
  radio.stopListening(); 
}

void kirim_data(int isiData)
{
  sentMessage[0] = isiData;
  radio.write(sentMessage, 2);

  Serial.print("hello ");
  Serial.println(count);
  Serial.print("sent Message:  ");
  Serial.println(sentMessage[0]);
}

void mode_receive()
{
  if (digitalRead(alamatSatu) == 1) {address = 224;}
  else { address = 225; }
  
  radio.begin();

  minMaxState = digitalRead(switchMinMax);
  
  radio.openReadingPipe(0, address);
  if ( minMaxState == MIN ) {
    radio.setPALevel(RF24_PA_MIN);
    Serial.print("min max State: ");
    Serial.println(minMaxState);
  } else {
    radio.setPALevel(RF24_PA_MAX);
    Serial.print("min max State: ");
    Serial.println(minMaxState);
  }
  radio.startListening();
}

int terima_data()
{

  dataMasuk[0] = 0;
  if (radio.available()) {

    counterSinyalLost = 0;
    
    radio.read(dataMasuk, 2);
    
    Serial.print(dataMasuk[0]);
    Serial.print(" ");
    count += 1;
    Serial.print(count);
    Serial.print(" ");
  }

  return dataMasuk[0];
}

//*********Buzzer*******
void playTone(int tone, int duration) {
  for ( long i = 0; i < duration * 1000L; i += tone * 2 ) {
    digitalWrite(speaker, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speaker, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for ( int i = 0; i < 8; i++ ) {
    if ( names[i] == note ) {
      playTone(tones[i], duration);
      delay(200);
    }
  }
}

void suara_counter()
{
  playNote(notes[1], 200);
  delay(tempo / 1);
}

void suara_hijau_awal()
{
//  playNote(notes[0], 200);
//    delay(tempo / 1);
//  playNote(notes[1], 200);
//    delay(tempo / 1);

  mp3_play(trackA);
  delay(350);
  mp3_stop();
  delay(150);
  mp3_play(trackB);
  delay(350);
  mp3_stop();
}

void suara_hijau_akhir()
{
//  playNote(notes[0], 100);
//    delay(tempo / 0.5);
//  playNote(notes[1], 100);
//    delay(tempo / 0.5);
  mp3_play(trackA);
  delay(200);
  mp3_stop();
  delay(300);
  mp3_play(trackB);
  delay(200);
  mp3_stop();
}

void tombol_ditekan()
{
  do{
    switch ( terima_data() ) {
      case 100:
        tombolLED_on();
        Serial.println("Tombol Ditekan");
      break;
      case 1000 :
        Serial.println("Semua Padam");
        
        digitalWrite(pedRed, LOW);
        digitalWrite(pedGreen, LOW);
        digitalWrite(carRed, LOW);
        digitalWrite(carYellow, LOW);
        digitalWrite(carGreen, LOW);
        
        tombolLED_off();
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

        mp3_play(trackStop);
      break;
      case 13 :
        Serial.println("Kendaraan Hijau");

        digitalWrite(carRed, LOW);
        digitalWrite(carYellow, LOW);
        digitalWrite(carGreen, HIGH);

        digitalWrite(pedRed, HIGH);
        digitalWrite(pedGreen, LOW);
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

        digitalWrite(carRed, HIGH);
        digitalWrite(carGreen, LOW);
      break;
      case 30 :
       //suara mulai lewat modul mp3 
       Serial.println("play Suara mulai");
       
      break;
      case 31 :
        //suara ketika counterdown waktu tunggu aktif
        Serial.println("play Suara counterdown");
        
        //suara_counter();
        mp3_play(trackCD);
      break;
      case 32 :
        //suara ketika pedestrian ketika menyebrang   
        Serial.println("play Suara hijau awal");
           
        suara_hijau_awal();
      break;
      case 33 :
        // suara ketika pedestrian sisa 5 detik
        Serial.println("play Suara hijau akhir");
        suara_hijau_akhir();
      break;
      case 40 :
        // ketika all Red kendaraan stop
        mp3_play(trackStop);
      break;
      default:
        curMillis = millis();

        if ( curMillis - prevMillis > interval ) {
          counterSinyalLost = counterSinyalLost + 1;
          prevMillis = curMillis;
           Serial.print("counter sinyal lost: ");
           Serial.println(counterSinyalLost);

          if ( counterSinyalLost > batasSinyalLost ) {
            counterSinyalLost = 0;
            Serial.println("Semua Padam");
      
            digitalWrite(pedRed, LOW);
            digitalWrite(pedGreen, LOW);
            digitalWrite(carRed, LOW);
            digitalWrite(carYellow, LOW);
            digitalWrite(carGreen, LOW);
            
            tombolLED_off();
          }
        }
      break;
    }
    
  } while ( ledState == 1 );
} 
