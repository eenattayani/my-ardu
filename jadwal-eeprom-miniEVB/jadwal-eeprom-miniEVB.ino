#include <EEPROM.h>

void setup(){
  int addr = 0;
  float f = 0.00f;
    
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.print("Read float from EEPROM: ");


  for (int i = 0 ; i < 512 ; i++) {
    if (EEPROM.read(i) == 255) {
      EEPROM.write(i, 0);      
    }
  }

  jadwal_default();
//  jadwal_sekolah_sambas();

    
   for(int i=0; i< 512; i++){ 
    Serial.print("alamat : ");
    Serial.print(i);
    Serial.print(" || value : ");
    Serial.println(EEPROM.read(i));
    delay(10);
   }

}

void loop(){

  Serial.print("EEPROM lenght: ");
  Serial.println(512);
  delay(1000);
}

void jadwal_default()
{
   EEPROM.write(0, 14);  
   EEPROM.write(1, 24);  

   EEPROM.write(81, 7); 
   EEPROM.write(82, 7); 
   EEPROM.write(83, 8); 
   EEPROM.write(84, 9); 
   EEPROM.write(85, 10); 
   EEPROM.write(86, 10); 
   EEPROM.write(87, 11); 
   EEPROM.write(88, 11); 
   EEPROM.write(89, 12); 
   EEPROM.write(90, 13); 
   EEPROM.write(91, 14); 
   EEPROM.write(92, 14); 

   EEPROM.write(102, 45); 
   EEPROM.write(103, 30); 
   EEPROM.write(104, 15); 
   EEPROM.write(105, 0); 
   EEPROM.write(106, 15); 
   EEPROM.write(107, 0); 
   EEPROM.write(108, 45); 
   EEPROM.write(109, 30); 
   EEPROM.write(110, 10); 
   EEPROM.write(111, 0); 
   EEPROM.write(112, 45);    

   EEPROM.write(121, 11); 
   EEPROM.write(122, 6); 
   EEPROM.write(123, 7); 
   EEPROM.write(124, 7); 
   EEPROM.write(125, 8); 
   EEPROM.write(126, 6); 
   EEPROM.write(127, 7); 
   EEPROM.write(128, 7); 
   EEPROM.write(129, 8); 
   EEPROM.write(130, 6); 
   EEPROM.write(131, 7); 
   EEPROM.write(132, 9); 

   EEPROM.write(141, 7); 
   EEPROM.write(142, 7); 
   EEPROM.write(143, 8); 
   EEPROM.write(144, 9); 
   EEPROM.write(145, 10); 
   EEPROM.write(146, 10); 
   EEPROM.write(147, 11); 
   EEPROM.write(148, 11); 
   EEPROM.write(149, 12); 
   EEPROM.write(150, 13); 
   EEPROM.write(151, 14); 
   EEPROM.write(152, 14); 

   EEPROM.write(162, 45); 
   EEPROM.write(163, 30); 
   EEPROM.write(164, 15); 
   EEPROM.write(165, 0); 
   EEPROM.write(166, 15); 
   EEPROM.write(167, 0); 
   EEPROM.write(168, 45); 
   EEPROM.write(169, 30); 
   EEPROM.write(170, 15); 
   EEPROM.write(171, 0); 
   EEPROM.write(172, 45); 

   EEPROM.write(181, 6); 
   EEPROM.write(182, 7); 
   EEPROM.write(183, 7); 
   EEPROM.write(184, 7); 
   EEPROM.write(185, 8); 
   EEPROM.write(186, 6); 
   EEPROM.write(187, 7); 
   EEPROM.write(188, 7); 
   EEPROM.write(189, 8); 
   EEPROM.write(190, 6); 
   EEPROM.write(191, 7); 
   EEPROM.write(192, 9);

   EEPROM.write(201, 7); 
   EEPROM.write(202, 7); 
   EEPROM.write(203, 8); 
   EEPROM.write(204, 9); 
   EEPROM.write(205, 10); 
   EEPROM.write(206, 10); 
   EEPROM.write(207, 11); 
   EEPROM.write(208, 11); 
   EEPROM.write(209, 13); 
   EEPROM.write(210, 14); 
   EEPROM.write(211, 14); 

   EEPROM.write(222, 45); 
   EEPROM.write(223, 30); 
   EEPROM.write(224, 15); 
   EEPROM.write(225, 0); 
   EEPROM.write(226, 15); 
   EEPROM.write(227, 0); 
   EEPROM.write(228, 45); 
   EEPROM.write(229, 15); 
   EEPROM.write(230, 0); 
   EEPROM.write(231, 45); 

   EEPROM.write(241, 6); 
   EEPROM.write(242, 7); 
   EEPROM.write(243, 7); 
   EEPROM.write(244, 7); 
   EEPROM.write(245, 8); 
   EEPROM.write(246, 6); 
   EEPROM.write(247, 7); 
   EEPROM.write(248, 8); 
   EEPROM.write(249, 6); 
   EEPROM.write(250, 7); 
   EEPROM.write(251, 9); 

   EEPROM.write(452, 1); 
   EEPROM.write(453, 2); 
   EEPROM.write(454, 2); 
   EEPROM.write(455, 2); 
   EEPROM.write(456, 3); 
   EEPROM.write(457, 2);
}


void jadwal_sekolah_sambas()
{
  EEPROM.write(0, 14);  
   EEPROM.write(1, 24);  

   EEPROM.write(81, 7); 
   EEPROM.write(82, 7); 
   EEPROM.write(83, 8); 
   EEPROM.write(84, 9); 
   EEPROM.write(85, 10); 
   EEPROM.write(86, 10); 
   EEPROM.write(87, 11); 
   EEPROM.write(88, 11); 
   EEPROM.write(89, 12); 
   EEPROM.write(90, 13); 
   EEPROM.write(91, 14); 
   EEPROM.write(92, 14); 

   EEPROM.write(102, 45); 
   EEPROM.write(103, 30); 
   EEPROM.write(104, 15); 
   EEPROM.write(105, 0); 
   EEPROM.write(106, 15); 
   EEPROM.write(107, 0); 
   EEPROM.write(108, 45); 
   EEPROM.write(109, 30); 
   EEPROM.write(110, 10); 
   EEPROM.write(111, 0); 
   EEPROM.write(112, 45);    

   EEPROM.write(121, 11); 
   EEPROM.write(122, 6); 
   EEPROM.write(123, 7); 
   EEPROM.write(124, 7); 
   EEPROM.write(125, 8); 
   EEPROM.write(126, 6); 
   EEPROM.write(127, 7); 
   EEPROM.write(128, 7); 
   EEPROM.write(129, 8); 
   EEPROM.write(130, 6); 
   EEPROM.write(131, 7); 
   EEPROM.write(132, 9); 

   EEPROM.write(141, 7); 
   EEPROM.write(142, 7); 
   EEPROM.write(143, 8); 
   EEPROM.write(144, 9); 
   EEPROM.write(145, 10); 
   EEPROM.write(146, 10); 
   EEPROM.write(147, 11); 
   EEPROM.write(148, 11); 
   EEPROM.write(149, 12); 
   EEPROM.write(150, 13); 
   EEPROM.write(151, 14); 
   EEPROM.write(152, 14); 

   EEPROM.write(162, 45); 
   EEPROM.write(163, 30); 
   EEPROM.write(164, 15); 
   EEPROM.write(165, 0); 
   EEPROM.write(166, 15); 
   EEPROM.write(167, 0); 
   EEPROM.write(168, 45); 
   EEPROM.write(169, 30); 
   EEPROM.write(170, 15); 
   EEPROM.write(171, 0); 
   EEPROM.write(172, 45); 

   EEPROM.write(181, 6); 
   EEPROM.write(182, 7); 
   EEPROM.write(183, 7); 
   EEPROM.write(184, 7); 
   EEPROM.write(185, 8); 
   EEPROM.write(186, 6); 
   EEPROM.write(187, 7); 
   EEPROM.write(188, 7); 
   EEPROM.write(189, 8); 
   EEPROM.write(190, 6); 
   EEPROM.write(191, 7); 
   EEPROM.write(192, 9);

   EEPROM.write(201, 7); 
   EEPROM.write(202, 7); 
   EEPROM.write(203, 8); 
   EEPROM.write(204, 9); 
   EEPROM.write(205, 10); 
   EEPROM.write(206, 10); 
   EEPROM.write(207, 11); 
   EEPROM.write(208, 11); 
   EEPROM.write(209, 13); 
   EEPROM.write(210, 14); 
   EEPROM.write(211, 14); 

   EEPROM.write(222, 45); 
   EEPROM.write(223, 30); 
   EEPROM.write(224, 15); 
   EEPROM.write(225, 0); 
   EEPROM.write(226, 15); 
   EEPROM.write(227, 0); 
   EEPROM.write(228, 45); 
   EEPROM.write(229, 15); 
   EEPROM.write(230, 0); 
   EEPROM.write(231, 45); 

   EEPROM.write(241, 6); 
   EEPROM.write(242, 7); 
   EEPROM.write(243, 7); 
   EEPROM.write(244, 7); 
   EEPROM.write(245, 8); 
   EEPROM.write(246, 6); 
   EEPROM.write(247, 7); 
   EEPROM.write(248, 8); 
   EEPROM.write(249, 6); 
   EEPROM.write(250, 7); 
   EEPROM.write(251, 9); 

   EEPROM.write(452, 1); 
   EEPROM.write(453, 2); 
   EEPROM.write(454, 2); 
   EEPROM.write(455, 2); 
   EEPROM.write(456, 3); 
   EEPROM.write(457, 2);
}
