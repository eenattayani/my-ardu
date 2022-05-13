// bel sekolah 17-06-2020

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <virtuabotixRTC.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

#define jml_jadwal 20
#define jumlah_kode 7

//address EEPROM 0 - 1023
const byte addrVolume = 0;
const byte addrTrack = 1;

const byte addrA = 80;
const byte addrB = 140;
const int addrC = 200;
const int addrD = 260;
const int addrE = 320;
const int addrF = 380;

const int addrKodeJadwal = 450; //[7]
const int spaceAddrMenit = 20;
const int spaceAddrTrack = 40;


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

// byte rowPins[ROWS] = {9,8,7,6};
// byte colPins[COLS] = {5,4,3,2};

Keypad keypad = Keypad(makeKeymap(keys),rowPins,colPins,ROWS,COLS);

String nama_hari[7] = {"MINGGU","SENIN","SELASA","RABU","KAMIS","JUMAT","SABTU"};
int hari,tanggal, bulan, tahun, jam, menit, detik;

unsigned long int previousMillis = millis(); 
unsigned long int currentMillis;
const int jeda = 1000;
const byte autoOut = 60;
bool keluar = false;
int jam_pelajaran = 0;
char status_jadwal = 'A';
char tanda = '>';
byte counter = 0;
char hold_tombol;

byte kode_jadwal[7];
char label_kode_jadwal[jumlah_kode] = {'O', 'A', 'B', 'C', 'D', 'E', 'F'};

byte jam_A[jml_jadwal], menit_A[jml_jadwal], track_A[jml_jadwal];
byte jam_B[jml_jadwal], menit_B[jml_jadwal], track_B[jml_jadwal];
byte jam_C[jml_jadwal], menit_C[jml_jadwal], track_C[jml_jadwal];
byte jam_D[jml_jadwal], menit_D[jml_jadwal], track_D[jml_jadwal];
byte jam_E[jml_jadwal], menit_E[jml_jadwal], track_E[jml_jadwal];
byte jam_F[jml_jadwal], menit_F[jml_jadwal], track_F[jml_jadwal];

byte jam_jadwal[jml_jadwal];
byte menit_jadwal[jml_jadwal];
                              //  1    2     3    4    5    6    7     8     9    10   11
byte nada[jml_jadwal];

byte volume = EEPROM.read(addrVolume);
byte track = EEPROM.read(addrTrack);


void setup()
{
  // Serial.begin(9600);
  // while (!Serial)
  // {
  //   /* code */
  // }

  // KODE JADWAL
  for ( int i = 0; i < 7; i++  ){
    kode_jadwal[i] = EEPROM.read(addrKodeJadwal + 1 + i);
    if ( kode_jadwal[i] == 255 ) { kode_jadwal[i] = 0; }
    
  }
  
  // A -- RUTIN
  for ( int i = 0; i < 20; i++  ){
    int t_addrMenit = addrA + spaceAddrMenit;
    int t_addrTrack = addrA + spaceAddrTrack;

    jam_A[i] = EEPROM.read(addrA + 1 + i);
    menit_A[i] = EEPROM.read(t_addrMenit + 1 + i);
    track_A[i] = EEPROM.read(t_addrTrack + 1 + i);

  }

  // B -- SENIN
  for ( int i = 0; i < 20; i++  ){
    jam_B[i] = EEPROM.read(addrB + 1 + i);
    menit_B[i] = EEPROM.read(addrB + spaceAddrMenit + 1 + i);
    track_B[i] = EEPROM.read(addrB + spaceAddrTrack + 1 + i);

  }

  // C -- JUMAT
  for ( int i = 0; i < 20; i++ ){
    jam_C[i] = EEPROM.read(addrC + 1 + i);
    menit_C[i] = EEPROM.read(addrC + spaceAddrMenit + 1 + i);
    track_C[i] = EEPROM.read(addrC + spaceAddrTrack + 1 + i);

  }

  // D -- SABTU
  for ( int i = 0; i < 20; i++ ){
    jam_D[i] = EEPROM.read(addrD + 1 + i);
    menit_D[i] = EEPROM.read(addrD + spaceAddrMenit + 1 + i);
    track_D[i] = EEPROM.read(addrD + spaceAddrTrack + 1 + i);

  }

  // E -- 
  for ( int i = 0; i < 20; i++ ){
    jam_E[i] = EEPROM.read(addrE + 1 + i);
    menit_E[i] = EEPROM.read(addrE + spaceAddrMenit + 1 + i);
    track_E[i] = EEPROM.read(addrE + spaceAddrTrack + 1 + i);

  }

  // F -- 
  for ( int i = 0; i < 20; i++ ){
    jam_F[i] = EEPROM.read(addrF + 1 + i);
    menit_F[i] = EEPROM.read(addrF + spaceAddrMenit + 1 + i);
    track_F[i] = EEPROM.read(addrF + spaceAddrTrack + 1 + i);

  }

  mySerial.begin(9600);
  mp3_set_serial(mySerial);
  delay(10);
  mp3_reset();
  delay(1000);
  mp3_set_volume(volume);
  delay(10);

  mulai();
  mp3_play(1);
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
    else if ( hold_tombol == 'C' ) atur_volume(hold_tombol);
    else if ( hold_tombol == 'D' ) atur_volume(hold_tombol);
    
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
  masuk_menu("   TES SUARA");

  lcd.print("*:PREV    #:NEXT");
  lcd.setCursor(0,1);
  lcd.print("track:");
  lcd.print(track);
  lcd.print("  ");
  lcd.setCursor(10,1);
  lcd.print("vol:");
  lcd.print(volume);
  lcd.print("  ");

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case '*':
          track -= 1;
          if ( track < 1 ) track = 30;
          mp3_play(track);
        break;
        case '#':
          track += 1;
          if ( track > 30 ) track = 1;
          mp3_play(track);
        break;
        case '7':
          mp3_play();
        break;
        case '8':
          mp3_pause();
        break;
        case '9':
          mp3_stop();
        break;
        case 'C':
          tambah_volume();
        break;
        case 'D':
          kurang_volume();
        break;
        case 'A':
          kembali();
        break;
        case 'B':
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
  if ( tombol == 'C') {
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
        case 'C':
          tambah_volume();
        break;
        case 'D':
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
  masuk_menu("   PENGATURAN");

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
        case 'A':
          kembali();
        break;
        case 'B':
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
  masuk_menu("  MODE MANUAL");

  lcd.print("Pilih Track 0-9");

  do {
    // start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      // counter = 0;
      lcd.setCursor(3,1);
      lcd.print("PLAY: ");
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
          mp3_play();
          lcd.setCursor(3,1);
          lcd.print("PLAY... ");
        break;
        case '*':
          mp3_pause();
          lcd.setCursor(3,1);
          lcd.print("STOP... ");
        break;
        case 'A':
          kembali();
        break;
        case 'B':
          kembali();
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
  lcd.print("disimpan...");
  delay(500);
}

void batal()
{
  keluar = true;
  lcd.clear();
  lcd.print("dibatalkan...");
  delay(500);
}

void kembali()
{
  keluar = true;
  lcd.clear();
  lcd.print("kembali...");
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
        case '5':
          set_jadwal_E();

          menu_set_jadwal();
        break;
        case '6':
          set_jadwal_F();

          menu_set_jadwal();
        break;
        case 'A':
          kembali();
        break;
        case 'B':
          kembali();
        break;  
        default:

        break;
      }
    }
  } while (keluar == false);
  keluar = false;
}

void set_jadwal_F()
{
  int t_addrMenit = addrF + spaceAddrMenit;
  int t_addrTrack = addrF + spaceAddrTrack;
  int jam_ke = 1;
  int jam_edit = EEPROM.read(addrF + jam_ke);
  int menit_edit = EEPROM.read(t_addrMenit + jam_ke);
  int track_edit = EEPROM.read(t_addrTrack + jam_ke);

  masuk_menu("Set Jadwal F");

  view_set(jam_ke, jam_edit, menit_edit, track_edit);

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case 'C':
          jam_ke += 1;
          if ( jam_ke > 20 ) { jam_ke = 1; }

          jam_edit = EEPROM.read(addrF + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case 'D':
          jam_ke -= 1;
          if ( jam_ke < 1 ) { jam_ke = 20; }

          jam_edit = EEPROM.read(addrF + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);
          
          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case 'A':
          kembali();

        break;
        case 'B':
          set_jam_ke(jam_ke, addrF, t_addrMenit, t_addrTrack);

          jam_edit = EEPROM.read(addrF + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);
        break;  
        default:

        break;
      }
    }
  } while (keluar == false);
  keluar = false;
}

void set_jadwal_E()
{
  int t_addrMenit = addrE + spaceAddrMenit;
  int t_addrTrack = addrE + spaceAddrTrack;
  int jam_ke = 1;
  int jam_edit = EEPROM.read(addrE + jam_ke);
  int menit_edit = EEPROM.read(t_addrMenit + jam_ke);
  int track_edit = EEPROM.read(t_addrTrack + jam_ke);

  masuk_menu("Set Jadwal E");

  view_set(jam_ke, jam_edit, menit_edit, track_edit);

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case 'C':
          jam_ke += 1;
          if ( jam_ke > 20 ) { jam_ke = 1; }

          jam_edit = EEPROM.read(addrE + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case 'D':
          jam_ke -= 1;
          if ( jam_ke < 1 ) { jam_ke = 20; }

          jam_edit = EEPROM.read(addrE + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);
          
          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case 'A':
          kembali();

        break;
        case 'B':
          set_jam_ke(jam_ke, addrE, t_addrMenit, t_addrTrack);

          jam_edit = EEPROM.read(addrE + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);

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
  int t_addrMenit = addrD + spaceAddrMenit;
  int t_addrTrack = addrD + spaceAddrTrack;
  int jam_ke = 1;
  int jam_edit = EEPROM.read(addrD + jam_ke);
  int menit_edit = EEPROM.read(t_addrMenit + jam_ke);
  int track_edit = EEPROM.read(t_addrTrack + jam_ke);

  masuk_menu("Set Jadwal D");

  view_set(jam_ke, jam_edit, menit_edit, track_edit);

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case 'C':
          jam_ke += 1;
          if ( jam_ke > 20 ) { jam_ke = 1; }

          jam_edit = EEPROM.read(addrD + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case 'D':
          jam_ke -= 1;
          if ( jam_ke < 1 ) { jam_ke = 20; }

          jam_edit = EEPROM.read(addrD + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);
          
          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case 'A':
          kembali();

        break;
        case 'B':
          set_jam_ke(jam_ke, addrD, t_addrMenit, t_addrTrack);

          jam_edit = EEPROM.read(addrD + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);

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
  int t_addrMenit = addrC + spaceAddrMenit;
  int t_addrTrack = addrC + spaceAddrTrack;
  int jam_ke = 1;
  int jam_edit = EEPROM.read(addrC + jam_ke);
  int menit_edit = EEPROM.read(t_addrMenit + jam_ke);
  int track_edit = EEPROM.read(t_addrTrack + jam_ke);

  masuk_menu("Set Jadwal C");

  view_set(jam_ke, jam_edit, menit_edit, track_edit);

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case 'C':
          jam_ke += 1;
          if ( jam_ke > 20 ) { jam_ke = 1; }

          jam_edit = EEPROM.read(addrC + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case 'D':
          jam_ke -= 1;
          if ( jam_ke < 1 ) { jam_ke = 20; }

          jam_edit = EEPROM.read(addrC + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);
          
          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case 'A':
          kembali();

        break;
        case 'B':
          set_jam_ke(jam_ke, addrC, t_addrMenit, t_addrTrack);

          jam_edit = EEPROM.read(addrC + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);

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
  int t_addrMenit = addrB + spaceAddrMenit;
  int t_addrTrack = addrB + spaceAddrTrack;
  int jam_ke = 1;
  int jam_edit = EEPROM.read(addrB + jam_ke);
  int menit_edit = EEPROM.read(t_addrMenit + jam_ke);
  int track_edit = EEPROM.read(t_addrTrack + jam_ke);

  masuk_menu("Set Jadwal B");

  view_set(jam_ke, jam_edit, menit_edit, track_edit);

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case 'C':
          jam_ke += 1;
          if ( jam_ke > 20 ) { jam_ke = 1; }

          jam_edit = EEPROM.read(addrB + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case 'D':
          jam_ke -= 1;
          if ( jam_ke < 1 ) { jam_ke = 20; }

          jam_edit = EEPROM.read(addrB + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);
          
          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case 'A':
          kembali();

        break;
        case 'B':
          set_jam_ke(jam_ke, addrB, t_addrMenit, t_addrTrack);

          jam_edit = EEPROM.read(addrB + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);

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
  int t_addrMenit = addrA + spaceAddrMenit;
  int t_addrTrack = addrA + spaceAddrTrack;
  int jam_ke = 1;
  int jam_edit = EEPROM.read(addrA + jam_ke);
  int menit_edit = EEPROM.read(t_addrMenit + jam_ke);
  int track_edit = EEPROM.read(t_addrTrack + jam_ke);

  masuk_menu("Set Jadwal A");

  view_set(jam_ke, jam_edit, menit_edit, track_edit);

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case 'C':
          jam_ke += 1;
          if ( jam_ke > 20 ) { jam_ke = 1; }

          jam_edit = EEPROM.read(addrA + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);

          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case 'D':
          jam_ke -= 1;
          if ( jam_ke < 1 ) { jam_ke = 20; }

          jam_edit = EEPROM.read(addrA + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);
          
          view_set(jam_ke, jam_edit, menit_edit, track_edit);

        break;
        case 'A':
          kembali();

        break;
        case 'B':
          set_jam_ke(jam_ke, addrA, t_addrMenit, t_addrTrack);

          jam_edit = EEPROM.read(addrA + jam_ke);
          menit_edit = EEPROM.read(t_addrMenit + jam_ke);
          track_edit = EEPROM.read(t_addrTrack + jam_ke);

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
        case 'C':
          track_edit += 1;
          if ( track_edit > 20 ) { track_edit = 1; }
          lcd.setCursor(7,1);
          // if ( track_edit < 10 ) { lcd.print("0"); }
          lcd.print(track_edit);
          lcd.print(" ");
        break;
        case 'D':
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
        case 'A':
          kembali();
        break;
        case 'B':
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
  char kode = label_kode_jadwal[kode_jadwal[hari]];
  if ( kode == 'A' ) {
    for ( int i = 0; i < 20; i++ ){
      jam_jadwal[i] = jam_A[i];
      menit_jadwal[i] = menit_A[i];
      nada[i] = track_A[i];
    }
  } else if ( kode == 'B' ) {
    for ( int i = 0; i < 20; i++ ){
      jam_jadwal[i] = jam_B[i];
      menit_jadwal[i] = menit_B[i];
      nada[i] = track_B[i];
    }
  } else if ( kode == 'C' ) {
    for ( int i = 0; i < 20; i++ ){
      jam_jadwal[i] = jam_C[i];
      menit_jadwal[i] = menit_C[i];
      nada[i] = track_C[i];
    }
  } else if ( kode == 'D' ) {
    for ( int i = 0; i < 20; i++ ){
      jam_jadwal[i] = jam_D[i];
      menit_jadwal[i] = menit_D[i];
      nada[i] = track_D[i];
    }
  } else if ( kode == 'E' ) {
    for ( int i = 0; i < 20; i++ ){
      jam_jadwal[i] = jam_E[i];
      menit_jadwal[i] = menit_E[i];
      nada[i] = track_E[i];
    }
  } else if ( kode == 'F' ) {
    for ( int i = 0; i < 20; i++ ){
      jam_jadwal[i] = jam_F[i];
      menit_jadwal[i] = menit_F[i];
      nada[i] = track_F[i];
    }
  } else {
    // for ( int i = 0; i < 20; i++ ){
    //   jam_jadwal[i] = 0;
    //   menit_jadwal[i] = 0;
    //   nada[i] = 0;
    // }
  }

  jam_pelajaran = 0;
  tanda = '>';
  status_jadwal = kode;

  lcd.setCursor(0,1);
  lcd.print(status_jadwal);
  lcd.print(tanda);

  if ( jam_pelajaran == 0 ) {
    lcd.print("OFF");  
  } else { 
    lcd.print(jam_pelajaran);
  }
  lcd.print("  ");

  if ( kode == 'O' ) {
    lcd.setCursor(0,1);
    lcd.print("OFF   ");
  }

  for (int i = 0; i < 20; i++) {
    if ( jam == 0 ) {

      break;

    } else if ( jam < jam_jadwal[i] || jam <= jam_jadwal[i] && menit < menit_jadwal[i] ) {
      tanda = '>';
      jam_pelajaran = i + 1;



      view_status_jampel(i);

      break;

    } else if ( jam == jam_jadwal[i] && menit == menit_jadwal[i] ) {
      tanda = '=';
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
    if ( jam_pelajaran == 0 ) {
      lcd.print("OFF");  
    } else { 
      lcd.print(jam_pelajaran);
    }
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
    // Serial.println(counter);
    
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
  delay(800);
  lcd.clear();
}

void set_hari()
{
  masuk_menu("PENGATURAN HARI");

  lcd.print("Hari: ");
  lcd.print(nama_hari[hari]);
  lcd.print("   ");

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter = 0;
      switch (tombol) {
        case 'C':
          hari += 1;
          if (hari > 6) hari = 0;
          lcd.setCursor(6,0);
          lcd.print(nama_hari[hari]);
          lcd.print("   ");
        break;
        case 'D':
          hari -= 1;
          if (hari < 0) hari = 6;
          lcd.setCursor(6,0);
          lcd.print(nama_hari[hari]);
          lcd.print("   ");
        break;
        case 'A':
          batal();
        break;
        case 'B':
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
  masuk_menu("PENGATURAN WAKTU");
  
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
        case 'A':
          batal();
        break;
        case 'B':
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
  masuk_menu("MENU SET JADWAL");

  lcd.print("1.A   2.B   3.C ");
  lcd.setCursor(0,1);
  lcd.print("4.D   5.E   6.F ");
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
        case 'C':
          kode_edit += 1;
          if ( kode_edit > jumlah_kode - 1 ) { kode_edit = 0; }
          lcd.setCursor(13,1);
          lcd.print(label_kode_jadwal[kode_edit]);
          lcd.print("  ");
        break;
        case 'D':
          kode_edit -= 1;
          if ( kode_edit < 0) { kode_edit = jumlah_kode - 1; }
          lcd.setCursor(13,1);
          lcd.print(label_kode_jadwal[kode_edit]);
          lcd.print("  ");
        break;
        case 'A':
          kembali();
        break;
        case 'B':
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

  masuk_menu("SET KODE JADWAL");
  
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
        case 'D':
        baris += 2;
        if ( baris > 5 ) { baris = 5; }

        view_menu_kode_jadwal(baris);
        break;
        case 'C':
          baris -= 2;
          if ( baris < 1 ) { baris = 1; }

          view_menu_kode_jadwal(baris);
        break;
        case 'A':
          kembali();
        break;
        case 'B':
          kembali();
        break;  
        default:

        break;
      }
    }
  } while (keluar == false);
  keluar = false;
}
