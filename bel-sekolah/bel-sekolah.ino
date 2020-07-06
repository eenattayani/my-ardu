// bel sekolah 17-06-2020

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>
#include <virtuabotixRTC.h>
#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>

#define jumlah_jadwal 20

//address EEPROM 0 - 1023
const byte addrVolume = 0;
const byte addrTrack = 1;

const byte addrJamRutin = 100;
const byte addrMenitRutin = 120;
const byte addrTrackRutin = 10;

const byte addrJamRutinSenin = 140;
const byte addrMenitRutinSenin = 160;
const byte addrTrackRutinSenin = 30;

const byte addrJamRutinJumat = 180;
const byte addrMenitRutinJumat = 200;
const byte addrTrackRutinJumat = 50;

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
String nama_bulan[] = {"Januari","Februari","Maret","April","Mei","Juni","Juli","Agustus","September","Oktober","November","Desember"};
int hari,tanggal, bulan, tahun, jam, menit, detik;

unsigned long int previousMillis = millis(); 
unsigned long int currentMillis;
const int jeda = 1000;
const byte autoOut = 60;
bool keluar = false;
// String menu = "";
String mode = "";
String jam_pelajaran = "";
String status_jadwal = "B";
String tanda = "->";
String data_jam_pelajaran[] = {};
byte counter = 0;
char hold_tombol;



byte jam_rutin[jumlah_jadwal] =   { 7,   7,    8,   9,   9,  10,  11,   11,   12,   12,  13};
byte menit_rutin[jumlah_jadwal] = { 0,  45,   30,  15,  30,  15,   0,   45,    0,   45,  30};

byte jam_senin[jumlah_jadwal] =   { 7,   7,    8,   9,   9,  10,  11,   11,   12,   12,  13};
byte menit_senin[jumlah_jadwal] = { 0,  45,   30,  15,  30,  15,   0,   45,    0,   45,  30};

byte jam_jumat[jumlah_jadwal] =   { 7,   7,    8,   9,   9,  10,  11,    0,    0,    0,   0};
byte menit_jumat[jumlah_jadwal] = { 0,  45,   30,  15,  30,  15,   0,   45,    0,   45,  30};

byte jam_jadwal[jumlah_jadwal] =   { 7,   7,    8,   9,   9,  10,  11,   11,   12,   12,  13};
byte menit_jadwal[jumlah_jadwal] = { 0,  45,   30,  15,  30,  15,   0,   45,    0,   45,  30};
byte nada[] =         { 1,   5,    5,   4,   1,   5,   5,    4,    1,    5,   8};
String jam_ke[] =    {"I","II","III","IV","V","VI","VII","VIII","IX", "X", "XI"};
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
  
  for ( int i = 0; i < 20; i++  ){
    jam_rutin[i] = EEPROM.read(addrJamRutin + 1 + i);
    menit_rutin[i] = EEPROM.read(addrMenitRutin + 1 + i);

    Serial.print(jam_rutin[i]);
    Serial.print(":");
    Serial.println(menit_rutin[i]);
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
    if ( hold_tombol == '1' ) pilihan();
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
          if ( track < 1 ) track = 22;
          mp3_play(track);
        break;
        case '2':
          track += 1;
          if ( track > 22 ) track = 1;
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

  lcd.print("1.waktu  2.hari");
  lcd.setCursor(0,1);
  lcd.print("3.manual 4.rutin");
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
          set_manual();
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

void pilihan()
{
  masuk_menu("MODE BEL");

  lcd.print("1.manual 2.rutin");
  lcd.setCursor(0,1);
  lcd.print("3.UTS    4.UAS");
  lcd.print("   ");

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter = 0;
      switch (tombol) {
        case '1':
          status_jadwal = "A";
          lcd.clear();
          mode = "Mode Manual";
          mode_dipilih();
        break;
        case '2':
          status_jadwal = "B";
          mode = "Mode Rutin";
          mode_dipilih();
        break;
        case '3':
          status_jadwal = "C";
          mode = "Mode UTS";
          mode_dipilih();
        break;
        case '4':
          status_jadwal = "D";
          mode = "Mode UAS";
          mode_dipilih();
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

void mode_dipilih()
{
  keluar = true;
  lcd.clear();
  lcd.print(mode);
  lcd.setCursor(0,1);
  lcd.print("Dipilih");
  delay(1000);
}

void simpan()
{
  keluar = true;
  lcd.clear();
  lcd.print("disimpan..");
  delay(1000);
}

void batal()
{
  keluar = true;
  lcd.clear();
  lcd.print("dibatalkan..");
  delay(1000);
}

void kembali()
{
  keluar = true;
  lcd.clear();
  lcd.print("kembali..");
  delay(1000);
}

void set_manual()
{
  masuk_menu("Set Mode Manual");
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
          set_jadwal_rutin();
          
         menu_set_jadwal();
        break;
        case '2':
          set_jadwal_senin();

          menu_set_jadwal();
        break;
        case '3':
        
        break;
        case '4':
          
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

void set_jadwal_senin()
{
  int jam_ke = 1;
  int jam_edit = EEPROM.read(addrJamRutinSenin + jam_ke);
  int menit_edit = EEPROM.read(addrMenitRutinSenin + jam_ke);

  masuk_menu("Set Jadwal Senin");

  view_set_rutin(jam_ke, jam_edit, menit_edit);

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case '3':
          jam_ke += 1;
          if ( jam_ke > 20 ) { jam_ke = 1; }

          jam_edit = EEPROM.read(addrJamRutinSenin + jam_ke);
          menit_edit = EEPROM.read(addrMenitRutinSenin + jam_ke);

          view_set_rutin(jam_ke, jam_edit, menit_edit);

        break;
        case '6':
          jam_ke -= 1;
          if ( jam_ke < 1 ) { jam_ke = 20; }

          jam_edit = EEPROM.read(addrJamRutinSenin + jam_ke);
          menit_edit = EEPROM.read(addrMenitRutinSenin + jam_ke);
          
          view_set_rutin(jam_ke, jam_edit, menit_edit);

        break;
        case '8':
         
        break;
        case '0':
          
        break;
        case '9':
          kembali();

        break;
        case '#':
          set_jam_ke(jam_ke, addrJamRutinSenin, addrMenitRutinSenin);

          jam_edit = EEPROM.read(addrJamRutinSenin + jam_ke);
          menit_edit = EEPROM.read(addrMenitRutinSenin + jam_ke);

          view_set_rutin(jam_ke, jam_edit, menit_edit);

        break;  
        default:

        break;
      }
    }
  } while (keluar == false);
  keluar = false;
}

void set_jadwal_rutin()
{
  int jam_ke = 1;
  int jam_edit = EEPROM.read(addrJamRutin + jam_ke);
  int menit_edit = EEPROM.read(addrMenitRutin + jam_ke);

  masuk_menu("Set Jadwal Rutin");

  view_set_rutin(jam_ke, jam_edit, menit_edit);

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case '3':
          jam_ke += 1;
          if ( jam_ke > 20 ) { jam_ke = 1; }

          jam_edit = EEPROM.read(addrJamRutin + jam_ke);
          menit_edit = EEPROM.read(addrMenitRutin + jam_ke);

          view_set_rutin(jam_ke, jam_edit, menit_edit);

        break;
        case '6':
          jam_ke -= 1;
          if ( jam_ke < 1 ) { jam_ke = 20; }

          jam_edit = EEPROM.read(addrJamRutin + jam_ke);
          menit_edit = EEPROM.read(addrMenitRutin + jam_ke);
          
          view_set_rutin(jam_ke, jam_edit, menit_edit);

        break;
        case '8':
         
        break;
        case '0':
          
        break;
        case '9':
          kembali();

        break;
        case '#':
          set_jam_ke(jam_ke, addrJamRutin, addrMenitRutin);

          jam_edit = EEPROM.read(addrJamRutin + jam_ke);
          menit_edit = EEPROM.read(addrMenitRutin + jam_ke);

          view_set_rutin(jam_ke, jam_edit, menit_edit);

        break;  
        default:

        break;
      }
    }
  } while (keluar == false);
  keluar = false;
}


void set_jam_ke(int jam_ke, int addr_jam, int addr_menit)
{
  int jam_edit = EEPROM.read(addr_jam + jam_ke);
  int menit_edit = EEPROM.read(addr_menit + jam_ke);

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
  lcd.print(track);
  lcd.print(" ");

  do {
    start_counter();

    char tombol = keypad.getKey();
    if ( tombol != NO_KEY ) {
      counter  = 0;
      switch (tombol) {
        case '3':
          
        break;
        case '6':
          
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
}

void tampil_waktu()

{
  // Serial.print(nama_hari[hari]);
  // Serial.print(", ");
  // Serial.print(tanggal);
  // Serial.print("/");
  // Serial.print(bulan);
  // Serial.print("/");
  // Serial.print(tahun);
  // Serial.print(" ");
  // Serial.print(jam);
  // Serial.print(":");
  // Serial.print(menit);
  // Serial.print(":");
  // Serial.println(detik);

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
  if ( nama_hari[hari] == "SENIN" ) {
    for ( int i = 0; i < 20; i++  ){
      jam_jadwal[i] = jam_senin[i];
      menit_jadwal[i] = menit_senin[i];

      Serial.print(jam_jadwal[i]);
      Serial.print(":");
      Serial.println(menit_jadwal[i]);
    }
  }

  jam_pelajaran = "off";

  for (int i = 0; i < arraySize; i++) {
    if ( jam < jam_jadwal[i] || jam <= jam_jadwal[i] && menit < menit_jadwal[i] ) {
      tanda = "->";
      jam_pelajaran = jam_ke[i];
      break;
    } else if ( jam == jam_jadwal[i] && menit == menit_jadwal[i] ) {
      tanda = "=";
      jam_pelajaran = jam_ke[i];
      if ( detik < 1 ) mp3_play(nada[i]);
      break;
    }
  }

  lcd.setCursor(0,1);
  lcd.print(status_jadwal);
  lcd.print(tanda);
  lcd.print(jam_pelajaran);
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
  delay(2000);
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

  lcd.print("1.RUTIN 2.SENIN");
  lcd.setCursor(0,1);
  lcd.print("3.JUMAT 4.EKSTRA");
  lcd.print("   ");
}

void view_set_rutin(int jam_ke, int jam_edit, int menit_edit)
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
  lcd.print(track);
}


