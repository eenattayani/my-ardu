1# pada saat koneksi terputus, LED tombol pada SLAVE tetap menyala

Solusi: Ketika MASTER baru nyala, langsung mengirimkan perintah(1000) semua padam, untuk mengembalikan SLAVE ke mode standby(flashing)
Solusi: ketika tidak menerima sinyal, counter aktif hingga 2 menit, dan secara otomatis mematikan LED tombol dan lampu kendaraan kembali standby(flashing)
Resiko: ketika waktu tunggu kendaraan lebih dari 2 menit, maka LED tombol akan padam lebih dulu. 


2# ada kemungkinan koneksi lost saat perintah pedestrian jadi merah(dari hijau), berpotensi hijau kendaraan menyala bersamaan hijau pedestrian.
Solusi: Hijau Pedestrian harus OFF ketika Hijau kendaraan ON(demikian sebaliknya);

3# ketika tombol ditahan, SLAVE akan terus mengirim perintah dan tetap pada mode TRANSMIT sehingga tidak menerima perintah dari MASTER
Solusi: perbaiki algoritma program utama SLAVE, tombol harus disable ketika ditekan.

4# jika salah satu(MASTER/SLAVE) ada yang OFF, SLAVE akan kembali ke mode standby setelah 60 detik atau setelah MASTER menyala kembali(karena mengirimkan perintah PADAM pada saat booting), 

