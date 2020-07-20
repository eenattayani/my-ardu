#include <LiquidCrystal_I2C.h>
#include <TM1637Display.h>
#include <Keypad.h>
#include <EEPROM.h>

// #include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// modul 7 Segmen 
#define CLK 32 
#define DIO 34

#define MIN 0
#define MAX 1

#define mp3 Serial1 

TM1637Display display(CLK, DIO);
LiquidCrystal_I2C lcd(0x27, 16, 2);  //0x27 0x3F  // Mega(SDA, SCL) = 20, 21 

RF24 radio(48,49); // CE, CSN

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'d','#','0','*'},
  {'c','9','8','7'},
  {'b','6','5','4'},
  {'a','3','2','1'}
};

byte rowPins[ROWS] = {37,35,33,31};
byte colPins[COLS] = {29,27,25,23};

Keypad keypad = Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);

const int addrPelican = 0;
const int addrTunggu = 1;
const int addrJeda = 2;

const int button = 2;
const int ledButton = 3;
const int speaker = 41;

const int alamatSatu = 6;
//const int alamatDua = 7;
const int switchMinMax = 8;

const int pedRed = 10;
const int pedGreen = 9;
const int carRed = 13;
const int carYellow = 12;
const int carGreen =  11;

byte address = 225;

int durPelican = 15;
int durTunggu = 10;
int durJeda = 30;
int durasiAllRed = 3000;
int durasiKuning = 2000;
int durasiHijau = 20000;
int durasiAutoOut = 60;
byte volume = 30;
byte trackMulai = 2;
byte trackStop = 30;

bool minMaxState = MIN;
unsigned int count = 0;
unsigned long curMillis = 0;
unsigned long hitMillis = 0;
unsigned long prevMillis_1 = 0;
unsigned long prevMillis_2 = 0;
const int interval_1 = 1000;
const int interval_2 = 1000;
int kuning = 1;

String menu = "";
bool statusSimpan = false;
byte holdKeyState = 0;
bool buttonState = LOW;
int dataMasuk[1] = {0};
int sentMessage[1];

unsigned long changeTime;

char tombol;
int statusTombol = 1; // keypad

int length = 2; // the number of notes
char notes[] = "bg"; // a space represents a rest
int beats[] = { 1, 1};
int tempo = 100;



void setup() 
{
  pinMode(button, INPUT);
  pinMode(speaker, OUTPUT);
  pinMode(alamatSatu, INPUT);
//  pinMode(alamatDua, INPUT);
  pinMode(switchMinMax, INPUT);
  pinMode(pedRed, OUTPUT);
  pinMode(pedGreen , OUTPUT);
  pinMode(carRed , OUTPUT);
  pinMode(carYellow , OUTPUT);
  pinMode(carGreen , OUTPUT);
  pinMode(ledButton, OUTPUT);
  pinMode(button, INPUT);
  
  digitalWrite(button, LOW);

  digitalWrite(carYellow,HIGH);

  if (digitalRead(alamatSatu) == 1) {address = 224;}
//  else if (digitalRead(alamatDua) == 1) {address = 223;}
  minMaxState = digitalRead(switchMinMax);
  

  durPelican = EEPROM.read(addrPelican);
  durTunggu = EEPROM.read(addrTunggu);
  durJeda = EEPROM.read(addrJeda);

   Serial.begin(9600);
   while (!Serial) {
     // wait for serial port to connect. Needed for native USB port only
   }

   mp3.begin(9600);
   while (!mp3) {
     // wait for serial port to connect.
   }
   mp3_set_serial(mp3);
   delay(10);
   mp3_reset();
   delay(1000);
   mp3_set_volume(volume);
   delay(10);

  if(durPelican > 200) {durPelican = 20;}
  if(durTunggu > 200) {durTunggu = 20;}
  if(durJeda > 200) {durJeda = 20;}
  
  lcd.begin();
  lcd.setCursor(0,0);
  lcd.print("dur pelican: ");
  lcd.setCursor(13,0);
  lcd.print(durPelican);
  lcd.setCursor(0,1);
  lcd.print("dur tunggu :  ");
  lcd.setCursor(13,1);
  lcd.print(durTunggu);
  lcd.setCursor(0,2);
  lcd.print("dur jeda   :");
  lcd.setCursor(13,2);
  lcd.print(durJeda);
  
  keypad.setHoldTime(500);
  
  display.setBrightness(0x0f);

  mp3_play(trackMulai);

  modeTransmit();
  delay(500);
  kirimData(1000);
  delay(500);  
  
  modeReceive();
  delay(1000);

  lcd.clear();
  tampilanAwal();
}

//**************************UTAMA*******************************

void loop()
{
  buttonState = digitalRead(button);
   
  if ( buttonState == HIGH ) {
    tombolDitekan();
  } else {

    if ( terimaData() == 1 ) {
      tombolDitekan();
    }

    dataMasuk[0] = 0;
  }

  curMillis = millis();

  if ( curMillis - prevMillis_1 > interval_1 ) {
      hitMillis = hitMillis + 1;
      prevMillis_1 = curMillis;
    }
  
  if ( hitMillis > durasiAutoOut ) {
        lcd.noBacklight();
        hitMillis = 0;
    }

  if ( curMillis - prevMillis_2 > interval_2 ) {
    kuning_flashing();
    prevMillis_2 = curMillis;
  }

  
  
  tombol = keypad.getKey();
  if ( tombol != NO_KEY ) {
    hitMillis = 0;
    lcd.backlight();
    while( millis() - 1000 < curMillis ){
      keypad.getKey();
      holdKeyState = keypad.getState();
    }

    if ( holdKeyState == 2 ) {
        digitalWrite(carYellow,LOW);
        menuPengaturan();
        tombol = "";
      }
  }
   
}

//********************************************************************






//******FUNGSI*****************
void tampilanAwal() 
{
  lcd.setCursor(0,0);
  lcd.print("Pelican Cross...");
}

void tombolDitekan() 
{
    digitalWrite(ledButton, HIGH);
    modeTransmit();
    delay(100);

    kirimData(100);

    hitMillis = 0;
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0,0);
    lcd.print("Tombol ditekan  ");
    lcd.setCursor(0,1);
    lcd.print("Silahkan Tunggu..");
    delay(1000);

    countdownAktif();   
    changeLights();

    modeReceive();
    digitalWrite(ledButton, LOW);
}

void kuning_flashing()
{
  if ( kuning == 0 ) {
    digitalWrite(carYellow,LOW);
    kuning = 1;
    lcd.setCursor(0,1);
    lcd.print("            ");

    Serial.print("alamat: ");
    Serial.println(address);
    Serial.print("min max state: ");
    Serial.println(minMaxState);
  } 
  else {
    digitalWrite(carYellow,HIGH);
    kuning = 0;
    lcd.setCursor(0,1);
    lcd.print("Ready...    ");
  }
}

void countdownAktif()
{
  digitalWrite(carYellow,LOW);
  digitalWrite(carGreen,HIGH);
  digitalWrite(pedRed,HIGH);
  kirimData(13);

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("siap menyebrang");
  
    //counterdown aktif
    for ( int x = durTunggu; x > 0; x-- ) {  
      lcd.setCursor(0,1);
      lcd.print("       ");
      lcd.print(x);
      lcd.print("    ");
      display.showNumberDec(x);
      
      kirimData(31);
      
      playNote(notes[1], 200);
      delay(tempo / 1);
      delay(500);
      
    }

  lcd.setCursor(0,1);
  lcd.print("       ");
  lcd.print(0);
  lcd.print("    ");
  display.showNumberDec(0);
  // delay(1000);
    
}

void changeLights()
{
  kirimData(40);
  mp3_play(trackStop);

  digitalWrite(carGreen,LOW);
  digitalWrite(carYellow,HIGH);
  kirimData(12);
  lcd.setCursor(0,1);
  lcd.print("Kendaraan STOP..");
  delay(durasiKuning);

  digitalWrite(carYellow,LOW);
  digitalWrite(carRed,HIGH);
  kirimData(11);
  delay(durasiAllRed);
  
  lcd.setCursor(0,0);
  lcd.print("Pedestrian Jalan");

  digitalWrite(pedRed,LOW);
  digitalWrite(pedGreen,HIGH);
  kirimData(23);

  for ( int x=durPelican; x >= 6; x-- ) {
    lcd.setCursor(0,1);
    lcd.print("       ");
    lcd.print(x);
    lcd.print("       ");
    display.showNumberDec(x);
    
    kirimData(32);
    playNote(notes[0], 200);
      delay(tempo / 1);
    playNote(notes[1], 200);
      delay(tempo / 1);
  }
  
  //flashing lampu hijau penyebrangan ; status = off
  for ( int x = 5; x>=0; x-- ) {
    lcd.setCursor(0,1);
    lcd.print("       ");
    lcd.print(x);
    lcd.print("       ");
    display.showNumberDec(x);
    
    kirimData(33);
    playNote(notes[0], 100);
      delay(tempo / 0.5);
    playNote(notes[1], 100);
      delay(tempo / 0.5);
  }

  digitalWrite(pedGreen,LOW);
  digitalWrite(pedRed,HIGH);
  kirimData(21);
  lcd.setCursor(0,0);
  lcd.print("Pedestrian STOP  ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  delay(durasiAllRed);

  digitalWrite(carYellow,HIGH);
  kirimData(14);
  lcd.setCursor(0,1);
  lcd.print("Kendaraan SIAP..");
  delay(durasiKuning);

  digitalWrite(carRed,LOW);
  digitalWrite(carYellow,LOW);
  digitalWrite(carGreen,HIGH);
  kirimData(13);
  lcd.setCursor(0,0);
  lcd.print("Kendaraan Jalan  ");
  for ( int x=durJeda; x>=0; x-- ) {
    lcd.setCursor(0,1);
    lcd.print("       ");
    lcd.print(x);
    lcd.print("       ");
    display.showNumberDec(x);
    delay(1000);

    kirimData(13);
  }
  

  digitalWrite(carGreen,LOW);
  digitalWrite(pedRed,LOW);
  kirimData(1000);
  
  tampilanAwal();
  
  changeTime = millis();
}

//*********Buzzer*******
void playTone(int tone, int duration) 
{
  for ( long i = 0; i < duration * 1000L; i += tone * 2 ) {
    digitalWrite(speaker, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speaker, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) 
{
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

void speaker_aktif()
{
  for ( int i = 0; i < length; i++ ) {
    if ( notes[i] == ' ' ) {
      delay(beats[i] * tempo); // rest
    } else {
      //playNote(notes[i], beats[i] * tempo);
      playNote(notes[i], 100);
    }

    // pause between notes
    delay(tempo / 1);
  }
}




//********Menu Pengaturan*******
void tampilMenu()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("1.PELICAN");
  lcd.setCursor(0,1);
  lcd.print("2.TUNGGU  3.JEDA");
}

void masukMenu(String namaMenu = "menu") 
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(namaMenu);
  lcd.setCursor(6,1);
  for(int x=0; x<3; x++){  
    lcd.print(".");
    delay(700);
  }
}

void menuPengaturan()
{
  hitMillis = 0;
  
  masukMenu("Menu Pengaturan");
  tampilMenu();  
  
  do
  {
    tombol = keypad.getKey();
    if ( tombol == 'a' ) {
      statusTombol = 1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("   Kembali...");
    }
    else if ( tombol == 'b' ) {
      statusTombol = 1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("    Kembali...");
    }
    else if ( tombol == '1' ) {
        statusTombol = 0;
        aturPelican();
        statusTombol = 0;
        tampilMenu();
    }
    else if ( tombol == '2' ) {
        statusTombol = 0;
        aturTunggu();
        statusTombol = 0;
        tampilMenu();
    }
    else if ( tombol == '3' ) {
        statusTombol = 0;
        aturJeda();
        statusTombol = 0;
        tampilMenu();
    }
    else{
        statusTombol = 0;
    }

    curMillis = millis();
    
    if ( curMillis % 1000 == 0 ) {
      hitMillis = hitMillis + 1;
      holdKeyState = keypad.getState();
      delay(1);  
    }

    if ( hitMillis > durasiAutoOut ) {
      statusTombol = 1;
      hitMillis = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("   Kembali...");
    }
              
    if( tombol != NO_KEY ) {
      hitMillis = 0;
    }
  }    
  while( statusTombol != 1 ); 
  
  delay(1500);
  tampilanAwal();
}

void aturTunggu() 
{
  menu = "Tunggu";

  durTunggu = menuPengaturanDurasi(durTunggu);

  if ( statusSimpan == true ) {
    EEPROM.update(addrTunggu,durTunggu);
    statusSimpan = false;
  }
}



void aturPelican() 
{
  menu = "Pelican";
  
  durPelican = menuPengaturanDurasi(durPelican);

  if ( statusSimpan == true ) {
    EEPROM.update(addrPelican,durPelican);
    statusSimpan = false;
  }
  
}


void aturJeda()
{
  menu = "jeda";

  durJeda = menuPengaturanDurasi(durJeda);

  if ( statusSimpan == true ) {
    EEPROM.update(addrJeda,durJeda);
    statusSimpan = false;
  }

}

int menuPengaturanDurasi(int data_durasi)
{
  int durasi = data_durasi;

  hitMillis = 0;
    
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SET");
  lcd.setCursor(4,0);
  lcd.print(menu);
  lcd.setCursor(13,0);
  lcd.print("#:+");
  lcd.setCursor(13,1);
  lcd.print("*:-");
  lcd.setCursor(0,1);
  lcd.print("Durasi = ");
  lcd.setCursor(9,1);
  lcd.print(durasi);

  do{
    curMillis = millis();
    
    if ( curMillis % 1000 == 0 ) {
      hitMillis = hitMillis + 1;
      delay(1);
    }
    
    tombol = keypad.getKey();
    if ( tombol == 'a' ) {  
        durasi = data_durasi;
        statusSimpan = false;          //cancel
        statusTombol = 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("   Cancel...");
        delay(1000);
      }
    else if ( tombol == 'b' ) {        //simpan
        statusSimpan = true;
        statusTombol = 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("    Saved...");
        delay(1000);
    }
    else if ( tombol == '#' || tombol == 'c' ) {      //tambah durasi ++
      statusTombol = 0;
      durasi += 1;
      if ( durasi > 120 ) { durasi = 120; }
      lcd.setCursor(9,1);
      lcd.print(durasi);
      lcd.print(" ");
    }
    else if ( tombol == '*' || tombol == 'd' ) {       //kurangi durasi --
      statusTombol = 0;
      durasi -= 1;
      if ( durasi < 0 ) { durasi = 0; }
      lcd.setCursor(9,1);
      lcd.print(durasi);
      lcd.print(" ");
    }
    else {
        statusTombol = 0;
    }

    if ( hitMillis > durasiAutoOut ) {  // auto cancel...
        durasi = data_durasi;
        statusSimpan = false;
        statusTombol = 1;
        hitMillis = 0;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("   Kembali...");
        delay(1000);
     }
           
    if ( tombol != NO_KEY ) {
      hitMillis = 0;
    }
     
  } while ( statusTombol != 1 );

  return durasi;
}





void modeTransmit()
{
  radio.begin();
  radio.openWritingPipe(address);
  if ( minMaxState == MIN ) {
    radio.setPALevel(RF24_PA_MIN);
  } else {
    radio.setPALevel(RF24_PA_MAX);
  }
  radio.stopListening();
}

void kirimData(int isiData)
{
  sentMessage[0] = isiData;
  radio.write(sentMessage, 2);

  count +=1;
}

void modeReceive()
{
  radio.begin();
  radio.openReadingPipe(0, address);
  if ( minMaxState == MIN ) {
    radio.setPALevel(RF24_PA_MIN);
  } else {
    radio.setPALevel(RF24_PA_MAX);
  }
  radio.startListening();
}

int terimaData() 
{
  if ( radio.available() ) {

    radio.read(dataMasuk, 2);

    count += 1;
    lcd.clear();
    lcd.setCursor(2,3);
    lcd.print("received: ");
    lcd.print(dataMasuk[0]);
    lcd.print(" ");
    lcd.print(count);
    
    Serial.print("terima: ");
    Serial.print(dataMasuk[0]);
    Serial.print(" ");
    Serial.println(count);
    
  }

  return dataMasuk[0];
}
