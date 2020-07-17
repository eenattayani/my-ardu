// bel sekolah 17-06-2020

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <virtuabotixRTC.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

#define jumlah_jadwal 20
#define jumlah_kode 5

//address EEPROM 0 - 1023
const byte addrVolume = 0;
const byte addrTrack = 1;

const byte addrJamA = 100;
const byte addrMenitA = 120;
const byte addrTrackA = 10;

const byte addrJamB = 140;
const byte addrMenitB = 160;
const byte addrTrackB = 30;

const byte addrJamC = 180;
const byte addrMenitC = 200;
const byte addrTrackC = 50;

const byte addrJamD = 220;
const byte addrMenitD = 240;
const byte addrTrackD = 70; 

const int addrKodeJadwal = 300; //[7]

// const int addrJamRutin[] = {10,11,12,13,14,15,16,17,18,19,20,21};


LiquidCrystal_I2C lcd(0x3F, 16, 2); //0x27  0x3F

//Inisialisasi pin RTC (CLK, DAT, RST) DS1302
virtuabotixRTC myRTC(11,12,13);

SoftwareSerial mySerial(9,10); //MP3

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {8,7,6,5};
byte colPins[COLS] = {4,3,2,1};

Keypad keypad = Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);

String nama_hari[] = {"AHAD","SENIN","SELASA","RABU","KAMIS","JUMAT","SABTU"};
// String nama_bulan[] = {"Januari","Februari","Maret","April","Mei","Juni","Juli","Agustus","September","Oktober","November","Desember"};
int hari,tanggal, bulan, tahun, jam, menit, detik;

unsigned long int previousMillis = millis(); 
unsigned long int currentMillis;
const int jeda = 1000;
const byte autoOut = 60;
bool keluar = false;
// String menu = "";
String mode = "";
String jam_pelajaran = "OFF";
String status_jadwal = "A";
String tanda = "->";
String data_jam_pelajaran[] = {};
byte counter = 0;
char hold_tombol;

byte kode_jadwal[7] = { 0, 2, 1, 1, 1, 3, 0};
String label_kode_jadwal[jumlah_kode] = {"OFF", "A", "B", "C", "D"};

byte jam_A[jumlah_jadwal] =   { 7,   7,    8,   9,   9,  10,  11,   11,   12,   12,  13};
byte menit_A[jumlah_jadwal] = { 0,  45,   30,  15,  30,  15,   0,   45,    0,   45,  30};
byte track_A[jumlah_jadwal] = { 1,   5,    5,   4,   1,   5,   5,    4,    1,    5,   8};

byte jam_B[jumlah_jadwal] =   { 7,   7,    8,   9,   9,  10,  11,   11,   12,   12,  13};
byte menit_B[jumlah_jadwal] = { 0,  45,   30,  15,  30,  15,   0,   45,    0,   45,  30};
byte track_B[jumlah_jadwal] = { 1,   5,    5,   4,   1,   5,   5,    4,    1,    5,   8};

byte jam_C[jumlah_jadwal] =   { 7,   7,    8,   9,   9,  10,  11,    0,    0,    0,   0};
byte menit_C[jumlah_jadwal] = { 0,  45,   30,  15,  30,  15,   0,   45,    0,   45,  30};
byte track_C[jumlah_jadwal] = { 1,   5,    5,   4,   1,   5,   5,    4,    1,    5,   8};

byte jam_D[jumlah_jadwal] =   { 7,   7,    8,   9,   9,  10,  11,    0,    0,    0,   0};
byte menit_D[jumlah_jadwal] = { 0,  45,   30,  15,  30,  15,   0,   45,    0,   45,  30};
byte track_D[jumlah_jadwal] = { 1,   5,    5,   4,   1,   5,   5,    4,    1,    5,   8};

byte jam_jadwal[jumlah_jadwal] =   { 7,   7,    8,   9,   9,  10,  11,   11,   12,   12,  13};
byte menit_jadwal[jumlah_jadwal] = { 0,  45,   30,  15,  30,  15,   0,   45,    0,   45,  30};
byte nada[] =         { 1,   5,    5,   4,   1,   5,   5,    4,    1,    5,   8};
// String status_jam_ke[] =    {"I","II","III","IV","V","VI","VII","VIII","IX", "X", "XI" , "XII", "XIII", "XIV"};
byte arraySize = sizeof(jam_jadwal) / sizeof(jam_jadwal[0]);

byte volume = EEPROM.read(addrVolume);
byte track = EEPROM.read(addrTrack);


void setup()
{
  Serial.begin(9600);
  while (!Serial)
  {
    /* code */
  }

  // KODE JADWAL
  for ( int i = 0; i < 7; i++  ){
    kode_jadwal[i] = EEPROM.read(addrKodeJadwal + 1 + i);
    if ( kode_jadwal[i] == 255 ) { kode_jadwal[i] = 0; }
    
    Serial.print(nama_hari[i]);
    Serial.print(": ");
    Serial.println(kode_jadwal[i]);
  }
  
  // A -- RUTIN
  for ( int i = 0; i < 20; i++  ){
    jam_A[i] = EEPROM.read(addrJamA + 1 + i);
    menit_A[i] = EEPROM.read(addrMenitA + 1 + i);

    Serial.print(jam_A[i]);
    Serial.print(":");
    Serial.println(menit_A[i]);
  }

  // B -- SENIN
  for ( int i = 0; i < 20; i++  ){
    jam_B[i] = EEPROM.read(addrJamB + 1 + i);
    menit_B[i] = EEPROM.read(addrMenitB + 1 + i);

    Serial.print(jam_B[i]);
    Serial.print(":");
    Serial.println(menit_B[i]);
  }

  // C -- JUMAT
  for ( int i = 0; i < 20; i++ ){
    jam_C[i] = EEPROM.read(addrJamC + 1 + i);
    menit_C[i] = EEPROM.read(addrMenitC + 1 + i);

    Serial.print(jam_C[i]);
    Serial.print(":");
    Serial.println(menit_C[i]);
  }

  // D -- SABTU
  for ( int i = 0; i < 20; i++ ){
    jam_D[i] = EEPROM.read(addrJamD + 1 + i);
    menit_D[i] = EEPROM.read(addrMenitD + 1 + i);

    Serial.print(jam_D[i]);
    Serial.print(":");
    Serial.println(menit_D[i]);
  }

  Serial.println(jam_pelajaran);
  Serial.println(status_jadwal);
  Serial.println(tanda);



  mySerial.begin(9600);
  mp3_set_serial(mySerial);
  delay(10);
  mp3_reset();
  delay(1000);
  mp3_set_volume(volume);
  delay(10);

  mulai();
  // mp3_play(1);
  delay(2000);
}

void loop()
{
  char tombol = keypad.getKey();
  if ( tombol ) {
    hold_tombol = tombol;
    counter = 0;
    lcd.backlight();
  }

  if ( keypad.getState() == 2 ) {
    if ( hold_tombol == '1' ) mode_manual();
    else if ( hold_tombol == '2' ) pengaturan();
    else if ( hold_tombol == '3' ) tes_sound();
    else if ( hold_tombol == '8' ) atur_volume(hold_tombol);
    else if ( hold_tombol == '0' ) atur_volume(hold_tombol);
    
    hold_tombol = "";
  }

  currentMillis = millis();
  if ( currentMillis - previousMillis > jeda ) {
    
    baca_waktu();
    tampil_waktu();
    tampil_status();

    previousMillis = currentMillis;
    counter += 1;
    if (counter > 60 * 2) {
      counter = 0;
      lcd.noBacklight();
    }
  }
}







//============================================================
void tes_sound()
{
  masuk_menu("Tes Suara");

  lcd.print("1.prev  2.next");
  lcd.setCursor(0,1);
  lcd.print("track=");
  lcd.print(track);
  lcd.print("  ");
  lcd.setCursor(10,1);
  lcd.print("vol=");
  lcd.print(volume);
  lcd.print("  ");

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case '1':
          track -= 1;
          if ( track < 1 ) track = 30;
          mp3_play(track);
        break;
        case '2':
          track += 1;
          if ( track > 30 ) track = 1;
          mp3_play(track);
        break;
        case '4':
          mp3_play();
        break;
        case '5':
          mp3_pause();
        break;
        case '8':
          tambah_volume();
        break;
        case '0':
          kurang_volume();
        break;
        case '9':
          kembali();
        break;
        case '#':
          kembali();
        break;  
        default:

        break;
      }
      lcd.setCursor(0,1);
      lcd.print("track=");
      lcd.print(track);
      lcd.print("  ");
      lcd.setCursor(10,1);
      lcd.print("vol=");
      lcd.print(volume);
      lcd.print("  ");
    }
  } while (keluar == false);
  
  EEPROM.update(addrVolume, volume);
  EEPROM.update(addrTrack, track);
  keluar = false;
}

void tambah_volume()
{
  volume += 1;
  if ( volume > 30 ) volume = 30;
  mp3_set_volume(volume);
}

void kurang_volume()
{
  volume -= 1;
  if ( volume < 0 ) volume = 0;
  mp3_set_volume(volume);
}

void atur_volume(char tombol)
{
  if ( tombol == '8') {
    tambah_volume();
  } else {
    kurang_volume();
  }

  lcd.clear();
  lcd.print("set volume(0-30)");
  lcd.setCursor(6,1);
  lcd.print(volume);
  lcd.print("  ");

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case '8':
          tambah_volume();
        break;
        case '0':
          kurang_volume();
        break; 
        default:

        break;
      }
      lcd.setCursor(6,1);
      lcd.print(volume);
      lcd.print("  ");
    }

    if ( counter == 3 ) keluar = true;

  } while (keluar == false);

  EEPROM.update(addrVolume, volume);
  keluar = false;
}

void pengaturan()
{
  masuk_menu("PENGATURAN");

  lcd.print("1.waktu 2.hari");
  lcd.setCursor(0,1);
  lcd.print("3.kode  4.jadwal");
  lcd.print("   ");

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case '1':
          set_waktu();
          keluar = true;
        break;
        case '2':
          set_hari();
          keluar = true;
        break;
        case '3':
          menu_kode_jadwal();
          keluar = true;
        break;
        case '4':
          set_jadwal();
          keluar = true;
        break;
        case '9':
          kembali();
        break;
        case '#':
          kembali();
        break;  
        default:

        break;
      }
    }
  } while (keluar == false);
  keluar = false;
}

void mode_manual()
{
  masuk_menu("MODE MANUAL");

  lcd.print("Pilih Track 0-9");

  do {
    // start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      // counter = 0;
      lcd.setCursor(3,1);
      lcd.print("Play: ");
      switch (tombol) {
        case '0':
          mp3_play(10);
          lcd.setCursor(9,1);
          lcd.print("10 ");
        break;
        case '1':
          mp3_play(1);
          lcd.setCursor(9,1);
          lcd.print("1 ");
        break;
        case '2':
          mp3_play(2);
          lcd.setCursor(9,1);
          lcd.print("2 ");
        break;
        case '3':
          mp3_play(3);
          lcd.setCursor(9,1);
          lcd.print("3 ");
        break;
        case '4':
          mp3_play(4);
          lcd.setCursor(9,1);
          lcd.print("4 ");
        break;
        case '5':
          mp3_play(5);
          lcd.setCursor(9,1);
          lcd.print("5 ");
        break;
        case '6':
          mp3_play(6);
          lcd.setCursor(9,1);
          lcd.print("6 ");
        break;
        case '7':
          mp3_play(7);
          lcd.setCursor(9,1);
          lcd.print("7 ");
        break;
        case '8':
          mp3_play(8);
          lcd.setCursor(9,1);
          lcd.print("8 ");
        break;
        case '9':
          mp3_play(9);
          lcd.setCursor(9,1);
          lcd.print("9 ");
        break;
        case '#':
          kembali();
        break;
        case '*':
          mp3_pause();
          lcd.setCursor(3,1);
          lcd.print("STOP... ");
        break;
        default:

        break;
      }
    }
  } while (keluar == false);

  keluar = false;
}

void simpan()
{
  keluar = true;
  lcd.clear();
  lcd.print("disimpan..");
  delay(500);
}

void batal()
{
  keluar = true;
  lcd.clear();
  lcd.print("dibatalkan..");
  delay(500);
}

void kembali()
{
  keluar = true;
  lcd.clear();
  lcd.print("kembali..");
  delay(500);
}


void set_jadwal()
{
  menu_set_jadwal();

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case '1':
          set_jadwal_A();
          
          menu_set_jadwal();
        break;
        case '2':
          set_jadwal_B();

          menu_set_jadwal();
        break;
        case '3':
          set_jadwal_C();

          menu_set_jadwal();
        break;
        case '4':
          set_jadwal_D();

          menu_set_jadwal();
        break;
        case '9':
          kembali();
        break;
        case '#':
          kembali();
        break;  
        default:

        break;
      }
    }
  } while (keluar == false);
  keluar = false;
}

void set_jadwal_D()
{
  int jam_ke = 1;
  int jam_edit = EEPROM.read(addrJamD + jam_ke);
  int menit_edit = EEPROM.read(addrMenitD + jam_ke);
  int track_edit = EEPROM.read(addrTrackD + jam_ke);

  masuk_menu("Set Jadwal D");

  view_set(jam_ke, jam_edit, menit_edit, track_edit);

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case '6':
          jam_ke += 1;
          if ( jam_ke > 20 ) { jam_ke = 1; }

          jam_edit = EEPROM.read(addrJamD + jam_ke);
          menit_edit = EEPROM.read(addrMenitD + jam_ke);
          track_edit = EEPROM.read(addrTrackD + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case '3':
          jam_ke -= 1;
          if ( jam_ke < 1 ) { jam_ke = 20; }

          jam_edit = EEPROM.read(addrJamD + jam_ke);
          menit_edit = EEPROM.read(addrMenitD + jam_ke);
          track_edit = EEPROM.read(addrTrackD + jam_ke);
          
          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case '8':
         
        break;
        case '0':
          
        break;
        case '9':
          kembali();

        break;
        case '#':
          set_jam_ke(jam_ke, addrJamA, addrMenitD, addrTrackD);

          jam_edit = EEPROM.read(addrJamD + jam_ke);
          menit_edit = EEPROM.read(addrMenitD + jam_ke);
          track_edit = EEPROM.read(addrTrackD + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;  
        default:

        break;
      }
    }
  } while (keluar == false);
  keluar = false;
}

void set_jadwal_C()
{
  int jam_ke = 1;
  int jam_edit = EEPROM.read(addrJamC + jam_ke);
  int menit_edit = EEPROM.read(addrMenitC + jam_ke);
  int track_edit = EEPROM.read(addrTrackC + jam_ke);

  masuk_menu("Set Jadwal C");

  view_set(jam_ke, jam_edit, menit_edit, track_edit);

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case '6':
          jam_ke += 1;
          if ( jam_ke > 20 ) { jam_ke = 1; }

          jam_edit = EEPROM.read(addrJamC + jam_ke);
          menit_edit = EEPROM.read(addrMenitC + jam_ke);
          track_edit = EEPROM.read(addrTrackC + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case '3':
          jam_ke -= 1;
          if ( jam_ke < 1 ) { jam_ke = 20; }

          jam_edit = EEPROM.read(addrJamC + jam_ke);
          menit_edit = EEPROM.read(addrMenitC + jam_ke);
          track_edit = EEPROM.read(addrTrackC + jam_ke);
          
          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case '8':
         
        break;
        case '0':
          
        break;
        case '9':
          kembali();

        break;
        case '#':
          set_jam_ke(jam_ke, addrJamC, addrMenitC, addrTrackC);

          jam_edit = EEPROM.read(addrJamC + jam_ke);
          menit_edit = EEPROM.read(addrMenitC + jam_ke);
          track_edit = EEPROM.read(addrTrackC + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;  
        default:

        break;
      }
    }
  } while (keluar == false);
  keluar = false;
}

void set_jadwal_B()
{
  int jam_ke = 1;
  int jam_edit = EEPROM.read(addrJamB + jam_ke);
  int menit_edit = EEPROM.read(addrMenitB + jam_ke);
  int track_edit = EEPROM.read(addrTrackB + jam_ke);

  masuk_menu("Set Jadwal B");

  view_set(jam_ke, jam_edit, menit_edit, track_edit);

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case '6':
          jam_ke += 1;
          if ( jam_ke > 20 ) { jam_ke = 1; }

          jam_edit = EEPROM.read(addrJamB + jam_ke);
          menit_edit = EEPROM.read(addrMenitB + jam_ke);
          track_edit = EEPROM.read(addrTrackB + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case '3':
          jam_ke -= 1;
          if ( jam_ke < 1 ) { jam_ke = 20; }

          jam_edit = EEPROM.read(addrJamB + jam_ke);
          menit_edit = EEPROM.read(addrMenitB + jam_ke);
          track_edit = EEPROM.read(addrTrackB + jam_ke);
          
          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case '8':
         
        break;
        case '0':
          
        break;
        case '9':
          kembali();

        break;
        case '#':
          set_jam_ke(jam_ke, addrJamB, addrMenitB, addrTrackB);

          jam_edit = EEPROM.read(addrJamB + jam_ke);
          menit_edit = EEPROM.read(addrMenitB + jam_ke);
          track_edit = EEPROM.read(addrTrackB + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;  
        default:

        break;
      }
    }
  } while (keluar == false);
  keluar = false;
}

void set_jadwal_A()
{
  int jam_ke = 1;
  int jam_edit = EEPROM.read(addrJamA + jam_ke);
  int menit_edit = EEPROM.read(addrMenitA + jam_ke);
  int track_edit = EEPROM.read(addrTrackA + jam_ke);

  masuk_menu("Set Jadwal A");

  view_set(jam_ke, jam_edit, menit_edit, track_edit);

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case '6':
          jam_ke += 1;
          if ( jam_ke > 20 ) { jam_ke = 1; }

          jam_edit = EEPROM.read(addrJamA + jam_ke);
          menit_edit = EEPROM.read(addrMenitA + jam_ke);
          track_edit = EEPROM.read(addrTrackA + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case '3':
          jam_ke -= 1;
          if ( jam_ke < 1 ) { jam_ke = 20; }

          jam_edit = EEPROM.read(addrJamA + jam_ke);
          menit_edit = EEPROM.read(addrMenitA + jam_ke);
          track_edit = EEPROM.read(addrTrackA + jam_ke);
          
          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case '8':
         
        break;
        case '0':
          
        break;
        case '9':
          kembali();

        break;
        case '#':
          set_jam_ke(jam_ke, addrJamA, addrMenitA, addrTrackA);

          jam_edit = EEPROM.read(addrJamA + jam_ke);
          menit_edit = EEPROM.read(addrMenitA + jam_ke);
          track_edit = EEPROM.read(addrTrackA + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;  
        default:

        break;
      }
    }
  } while (keluar == false);
  keluar = false;
}


void set_jam_ke(int jam_ke, int addr_jam, int addr_menit, int addr_track)
{
  int jam_edit = EEPROM.read(addr_jam + jam_ke);
  int menit_edit = EEPROM.read(addr_menit + jam_ke);
  int track_edit = EEPROM.read(addr_track + jam_ke);

  lcd.clear();
  lcd.print("Waktu: ");
  if ( jam_edit < 10 ) { lcd.print("0"); }
  lcd.print(jam_edit);
  lcd.print(":");
  if ( menit_edit < 10 ) { lcd.print("0"); }
  lcd.print(menit_edit);
  lcd.print(" ");
  lcd.setCursor(0,1);
  lcd.print("nada : ");
  lcd.print(track_edit);
  lcd.print(" ");

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case '3':
          track_edit += 1;
          if ( track_edit > 20 ) { track_edit = 1; }
          lcd.setCursor(7,1);
          // if ( track_edit < 10 ) { lcd.print("0"); }
          lcd.print(track_edit);
          lcd.print(" ");
        break;
        case '6':
          track_edit -= 1;
          if ( track_edit < 1 ) { track_edit = 20; }
          lcd.setCursor(7,1);
          // if ( track_edit < 10 ) { lcd.print("0"); }
          lcd.print(track_edit);
          lcd.print(" ");
        break;
        case '7':
          jam_edit += 1;
          if ( jam_edit > 23 ) { jam_edit = 0; }
          lcd.setCursor(7,0);
          if ( jam_edit < 10 ) { lcd.print("0"); }
          lcd.print(jam_edit);
        break;
        case '*':
          jam_edit -= 1;
          if ( jam_edit < 0 ) { jam_edit = 23; }
          lcd.setCursor(7,0);
          if ( jam_edit < 10 ) { lcd.print("0"); }
          lcd.print(jam_edit);
        break;
        case '8':
          menit_edit += 1;
          if ( menit_edit > 59 ) { menit_edit = 0; }
          lcd.setCursor(10,0);
          if ( menit_edit < 10 ) { lcd.print("0"); }
          lcd.print(menit_edit);
        break;
        case '0':
          menit_edit -= 1;
          if ( menit_edit < 0 ) { menit_edit = 59; }
          lcd.setCursor(10,0);
          if ( menit_edit < 10 ) { lcd.print("0"); }
          lcd.print(menit_edit);
        break;
        case '9':
          kembali();
        break;
        case '#':
          EEPROM.update(addr_jam + jam_ke, jam_edit);
          EEPROM.update(addr_menit + jam_ke, menit_edit);
          EEPROM.update(addr_track + jam_ke, track_edit);
          simpan();
        break;  
        default:

        break;
      }
    }
  } while (keluar == false);
  keluar = false;
}

void baca_waktu()
{
  myRTC.updateTime();
  hari = myRTC.dayofweek;
  tanggal = myRTC.dayofmonth;
  bulan = myRTC.month;
  tahun= myRTC.year;
  jam = myRTC.hours;
  menit =  myRTC.minutes;
  detik = myRTC.seconds;

  // hari = 1;
  // tanggal = 16;
  // bulan = 7;
  // tahun= 2020;
  // jam = 12;
  // menit = 41;
  // detik = 20;
}

void tampil_waktu()

{
  lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("<");
  lcd.print(nama_hari[hari]);
  lcd.setCursor(4,0);
  lcd.print("> ");
  if( tanggal < 10 ) lcd.print("0");
  lcd.print(tanggal);
  lcd.print("/");
  if( bulan < 10 ) lcd.print("0");
  lcd.print(bulan);
  lcd.print("/");
  lcd.print(tahun);
  
  lcd.setCursor(8,1);
  if( jam < 10 ) lcd.print("0");
  lcd.print(jam);
  lcd.print(":");
  if( menit < 10 ) lcd.print("0");
  lcd.print(menit);
  lcd.print(":");
  if( detik < 10 ) lcd.print("0");
  lcd.print(detik);
  lcd.print("  ");
}

void tampil_status()
{
  String kode = label_kode_jadwal[kode_jadwal[hari]];
  if ( kode == "A" ) {
    for ( int i = 0; i < 20; i++ ){
      jam_jadwal[i] = jam_A[i];
      menit_jadwal[i] = menit_A[i];
      nada[i] = track_A[i];
    }
  } else if ( kode == "B" ) {
    for ( int i = 0; i < 20; i++ ){
      jam_jadwal[i] = jam_B[i];
      menit_jadwal[i] = menit_B[i];
      nada[i] = track_B[i];
    }
  } else if ( kode == "C" ) {
    for ( int i = 0; i < 20; i++ ){
      jam_jadwal[i] = jam_C[i];
      menit_jadwal[i] = menit_C[i];
      nada[i] = track_C[i];
    }
  } else if ( kode == "D" ) {
    for ( int i = 0; i < 20; i++ ){
      jam_jadwal[i] = jam_D[i];
      menit_jadwal[i] = menit_D[i];
      nada[i] = track_D[i];
    }
  } 
  else {
    for ( int i = 0; i < 20; i++ ){
      jam_jadwal[i] = jam_A[i];
      menit_jadwal[i] = menit_A[i];
      nada[i] = track_A[i];
    }
  }

  jam_pelajaran = "OFF";
  tanda = "->";
  status_jadwal = kode;
  lcd.setCursor(0,1);
  lcd.print(status_jadwal);
  lcd.print(tanda);
  lcd.print(jam_pelajaran);
  lcd.print(" ");

  for (int i = 0; i < arraySize; i++) {
    if ( jam < jam_jadwal[i] || jam <= jam_jadwal[i] && menit < menit_jadwal[i] ) {
      tanda = "->";
      jam_pelajaran = i + 1;

      view_status_jampel(i);

      break;
    } else if ( jam == jam_jadwal[i] && menit == menit_jadwal[i] ) {
      tanda = "=";
      jam_pelajaran = i + 1;
      if ( detik < 1 ) mp3_play(nada[i]);

      view_status_jampel(i);

      break;
    }
  }

}

void view_status_jampel(int i)
{
  if ( counter % 2 == 0 ) {
    lcd.setCursor(0,1);
    lcd.print(status_jadwal);
    lcd.print(tanda);
    lcd.print(jam_pelajaran);
    lcd.print("   ");
  } else {
    lcd.setCursor(0,1);
    if ( jam_jadwal[i] < 10 ) { lcd.print("0"); }
    lcd.print(jam_jadwal[i]);
    lcd.print(":");
    if ( menit_jadwal[i] < 10 ) { lcd.print("0"); }
    lcd.print(menit_jadwal[i]);
    lcd.print(" ");
  }
}

void mulai() 
{
  lcd.begin();
  lcd.setCursor(0,0);
  lcd.print("#  BEL SEKOLAH #");
  lcd.setCursor(0,1);
  lcd.print("#   OTOMATIS   #");
  delay(1000);
}

void start_counter()
{
  currentMillis = millis();
  if ( currentMillis - previousMillis > jeda ) {
    previousMillis = currentMillis;
    counter += 1;
    Serial.println(counter);
    
    if (counter > autoOut) {
      counter = 0;
      keluar = true;
    }
  }
}

void masuk_menu(char menu[])
{
  lcd.clear();
  lcd.print(menu);
  delay(1000);
  lcd.clear();
}

void set_hari()
{
  masuk_menu("Menu Set Hari");

  lcd.print("Hari: ");
  lcd.print(nama_hari[hari]);
  lcd.print("   ");

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter = 0;
      switch (tombol) {
        case '1':
          hari += 1;
          if (hari > 6) hari = 0;
          lcd.setCursor(6,0);
          lcd.print(nama_hari[hari]);
          lcd.print("   ");
        break;
        case '4':
          hari -= 1;
          if (hari < 0) hari = 6;
          lcd.setCursor(6,0);
          lcd.print(nama_hari[hari]);
          lcd.print("   ");
        break;
        case '9':
          batal();
        break;
        case '#':
          myRTC.setDS1302Time(00,menit,jam,hari,tanggal,bulan,tahun);
          
          simpan();
        break;
        default:

        break;
      }
    }
  } while (keluar == false);

  keluar = false;
}

void set_waktu()
{
  masuk_menu("Menu Set Waktu");
  
  lcd.print("Date: ");

  if( tanggal < 10 ) lcd.print("0");
  lcd.print(tanggal);
  lcd.print("/");
  if( bulan < 10 ) lcd.print("0");
  lcd.print(bulan);
  lcd.print("/");
  lcd.print(tahun);
  
  lcd.setCursor(0,1);
  lcd.print("Time: ");
  if( jam < 10 ) lcd.print("0");
  lcd.print(jam);
  lcd.print(":");
  if( menit < 10 ) lcd.print("0");
  lcd.print(menit);
  lcd.print(":");
  if( detik < 10 ) lcd.print("0");
  lcd.print(detik);
  lcd.print("  ");
  
  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter = 0;
      switch (tombol) {
        case '1':
          tanggal += 1;
          if (tanggal > 31) tanggal = 1;
          lcd.setCursor(6,0);
          if (tanggal < 10) lcd.print("0");
          lcd.print(tanggal);
        break;
        case '4':
          tanggal -= 1;
          if (tanggal < 1) tanggal = 31;
          lcd.setCursor(6,0);
          if (tanggal < 10) lcd.print("0");
          lcd.print(tanggal);
        break;
        case '2':
          bulan += 1;
          if (bulan > 12) bulan = 1;
          lcd.setCursor(9,0);
          if (bulan < 10) lcd.print("0");
          lcd.print(bulan);
        break;
        case '5':
          bulan -= 1;
          if (bulan < 1) bulan = 12;
          lcd.setCursor(9,0);
          if (bulan < 10) lcd.print("0");
          lcd.print(bulan);
        break;
        case '3':
          tahun += 1;
          if (tahun > 2100) tahun = 2010;
          lcd.setCursor(12,0);
          lcd.print(tahun);
        break;
        case '6':
          tahun -= 1;
          if (tahun < 2010) tahun = 2100;
          lcd.setCursor(12,0);
          lcd.print(tahun);
        break;
        case '7':
          jam += 1;
          if (jam > 23) jam = 0;
          lcd.setCursor(6,1);
          if (jam < 10) lcd.print("0");
          lcd.print(jam);
        break;
        case '*':
          jam -= 1;
          if (jam < 0) jam = 23;
          lcd.setCursor(6,1);
          if (jam < 10) lcd.print("0");
          lcd.print(jam);
        break;
        case '8':
          menit += 1;
          if (menit > 59) menit = 0;
          lcd.setCursor(9,1);
          if (menit < 10) lcd.print("0");
          lcd.print(menit);
        break;
        case '0':
          menit -= 1;
          if (menit < 0) menit = 59;
          lcd.setCursor(9,1);
          if (menit < 10) lcd.print("0");
          lcd.print(menit);
        break;
        case '9':
          batal();
        break;
        case '#':
          myRTC.setDS1302Time(00,menit,jam,hari,tanggal,bulan,tahun);
          
          simpan();
        break;
        default:

        break;
      }
    }
  } while (keluar == false);
  
  //myRTC.setDS1302Time(00,8,12,1,23,03,2020);
  //detik, menit, jam, hari dalam seminggu, tanggal, bulan, tahun
  //00:59:23 "Jumat" 10-Oktober-2017

  keluar = false;
}

void menu_set_jadwal()
{
  masuk_menu("SET JADWAL");

  lcd.print("1.A 2.B  ");
  lcd.setCursor(0,1);
  lcd.print("3.C 4.D  ");
  lcd.print("   ");
}

void view_set(int jam_ke, int jam_edit, int menit_edit, int track_edit)
{
  lcd.clear();
  lcd.print("Jam ke-   # nada");

  lcd.setCursor(7,0);
  lcd.print(jam_ke);
  lcd.print(" ");
  lcd.setCursor(0,1);
  if ( jam_edit < 10 ) { lcd.print("0"); }
  lcd.print(jam_edit);
  lcd.print(":");
  if ( menit_edit < 10 ) { lcd.print("0"); }
  lcd.print(menit_edit);
  lcd.print(" ");

  lcd.setCursor(10,1);
  lcd.print("# ");
  lcd.print(track_edit);
  lcd.print(" ");
}

void set_kode_jadwal(byte hari_ke)
{
  int kode_edit = EEPROM.read(addrKodeJadwal + hari_ke + 1);
  lcd.clear();
  lcd.print("Hari: ");
  lcd.print(nama_hari[hari_ke]);
  lcd.setCursor(0,1);
  lcd.print("Kode Jadwal: ");
  lcd.setCursor(13,1);
  lcd.print(label_kode_jadwal[kode_edit]);
  lcd.print("  ");

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case '8':
          kode_edit += 1;
          if ( kode_edit > jumlah_kode - 1 ) { kode_edit = 0; }
          lcd.setCursor(13,1);
          lcd.print(label_kode_jadwal[kode_edit]);
          lcd.print("  ");
        break;
        case '0':
          kode_edit -= 1;
          if ( kode_edit < 0) { kode_edit = jumlah_kode - 1; }
          lcd.setCursor(13,1);
          lcd.print(label_kode_jadwal[kode_edit]);
          lcd.print("  ");
        break;
        case '9':
          kembali();
        break;
        case '#':
          kode_jadwal[hari_ke] = kode_edit;
          EEPROM.update(addrKodeJadwal + hari_ke + 1, kode_edit);
          simpan();
        break;  
        default:

        break;
      }
    }
  } while (keluar == false);
  keluar = false;
}

void view_menu_kode_jadwal(int baris)
{
  lcd.setCursor(0,0);
  lcd.print(baris);
  lcd.print(".");
  lcd.setCursor(0,1);
  lcd.print(baris + 1);
  lcd.print(".");

  lcd.setCursor(2,0);
  lcd.print(nama_hari[baris]);
  lcd.print("  ");
  lcd.setCursor(2,1);
  lcd.print(nama_hari[baris + 1]);
  lcd.print("  ");

  lcd.setCursor(8,0);
  lcd.print(" = ");
  lcd.setCursor(8,1);
  lcd.print(" = ");

  lcd.setCursor(11,0);
  lcd.print(label_kode_jadwal[kode_jadwal[baris]]);
  lcd.print("  ");
  lcd.setCursor(11,1);
  lcd.print(label_kode_jadwal[kode_jadwal[baris + 1]]);
  lcd.print("  ");
}

void menu_kode_jadwal()
{
  int baris = 1;

  masuk_menu("Pilih Kode Jadwal");
  
  view_menu_kode_jadwal(baris);
    
  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case '1':
          set_kode_jadwal(1);
          view_menu_kode_jadwal(baris);
        break;
        case '2':
          set_kode_jadwal(2);
          view_menu_kode_jadwal(baris);
        break;
        case '3':
          set_kode_jadwal(3);
          view_menu_kode_jadwal(baris);
        break;
        case '4':
          set_kode_jadwal(4);
          view_menu_kode_jadwal(baris);
        break;
        case '5':
          set_kode_jadwal(5);
          view_menu_kode_jadwal(baris);
        break;
        case '6':
          set_kode_jadwal(6);
          view_menu_kode_jadwal(baris);
        break;
        case '0':
        baris += 2;
        if ( baris > 5 ) { baris = 5; }

        view_menu_kode_jadwal(baris);
        break;
        case '8':
          baris -= 2;
          if ( baris < 1 ) { baris = 1; }

          view_menu_kode_jadwal(baris);
        break;
        case '9':
          kembali();
        break;
        case '#':
          kembali();
        break;  
        default:

        break;
      }
    }
  } while (keluar == false);
  keluar = false;
}
