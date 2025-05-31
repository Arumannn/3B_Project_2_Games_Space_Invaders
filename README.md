# Proyek 2 Alien Invaders
Pengembangan Aplikasi Berbasis Library dengan beranggotakan : 
| Nama                        | NIM        |
|-----------------------------|------------|
| Abdurahman Nur Fadillah     | 241511033  |
| Arman Yusuf Rifandi         | 241511038  |
| Fiandra Putera Mardani      | 241511043  |
| Nazriel Ramdhani            | 241511055  |
| Rina Permata Dewi           | 241511061  |



## Deskripsi Game
Alien Invaders adalah game arcade 2D bertipe move and shoot, di mana pemain mengendalikan pesawat luar angkasa untuk menghindari tembakan alien dan menghancurkan sebanyak mungkin musuh. Game ini tidak memiliki akhir (endless gameplay), sehingga tujuan utama pemain adalah meraih skor setinggi mungkin dan bertahan selama mungkin dari serangan gelombang demi gelombang alien.
Game dibuat dengan bahasa C dan menggunakan library Graphics.h

## Instalisasi

Game ini diharuskan menggunakan library graphics.h dan juga mingw32 bit. Berikut cara penginstalannya :
1. Instalisasi Mingw 32bit di https://sourceforge.net/projects/mingw/
2. Instalisasi library graphics.h sesuai dengan intruksi https://github.com/BJMinhNhut/graphics.h
3. Clone https://github.com/Arumannn/3B_Project_2_Games_Space_Invaders 
4. Lakukan compile dengan menekan **Win + Shift + B** pada mastermain.c
5. Jika tidak bisa menggunakan tombol compile tersebut maka buka terminal dan masukan perintah `g++ mastermain.c src/mainmenu.c src/mainsprite.c src/alien.c src/ufo.c src/barrier.c src/gameplay.c -o output.exe -Iinclude -lbgi -lgdi32 -lcomdlg32 -luuid -loleaut32 -lole32 -lwinmm`
6. Maka akan muncul file output.exe, Execute file tersebut dengan klik kanan lalu pilih `Run executable`
7. Jika tidak bisa maka buka terminal. lalu ketikan perintah `./output.exe`

## Guide to Play
Tujuan permainan ini adalah mengumpulkan skor sebanyak mungkin dengan menghancurkan alien dan UFO yang datang secara bergelombang. Player akan diberikan 3 nyawa setiap kali main, game akan berakhir jika player telah menghabiskan seluruh nyawa yang dimiliki.
### Control Space Invaders

| Tombol               | Fungsi                    |
|----------------------|---------------------------|
| `←` / `A`            | Gerak ke kiri             |
| `→` / `D`            | Gerak ke kanan            |
| `Space`              | Menembak peluru           |
| `Esc` (opsional)     | Keluar dari permainan     |


