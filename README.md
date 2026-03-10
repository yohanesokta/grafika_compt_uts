# Recursive Division Maze Generator (C)

Program ini adalah implementasi sederhana dari algoritma **Recursive Division** untuk menghasilkan **maze (labirin)** menggunakan bahasa **C**.
Setiap sel maze memiliki **koordinat pixel** berupa `(x1, x2, y1, y2)` sehingga dapat digunakan untuk visualisasi grafis seperti **SDL, OpenGL, atau HTML Canvas**.

---

## Konsep Dasar

Maze dibuat menggunakan algoritma **Recursive Division**, yaitu metode pembangkitan labirin dengan cara:

1. Memulai dari area grid kosong.
2. Membuat dinding secara **horizontal** atau **vertikal**.
3. Menyisakan **satu celah (passage)** agar maze tetap bisa dilewati.
4. Membagi area menjadi dua bagian.
5. Mengulangi proses tersebut secara **rekursif** sampai area terlalu kecil untuk dibagi.

---

## Struktur Data

Program menggunakan struktur `Cell` untuk menyimpan informasi setiap sel.

```c
typedef struct {
    int x1, x2;
    int y1, y2;
    int wall;
} Cell;
```

Penjelasan field:

| Field  | Deskripsi                               |
| ------ | --------------------------------------- |
| `x1`   | koordinat x kiri                        |
| `x2`   | koordinat x kanan                       |
| `y1`   | koordinat y atas                        |
| `y2`   | koordinat y bawah                       |
| `wall` | status dinding (1 = dinding, 0 = jalan) |

---

## Konstanta Program

```c
#define WIDTH 10
#define HEIGHT 10
#define CELL_SIZE 10
```

| Konstanta   | Fungsi                        |
| ----------- | ----------------------------- |
| `WIDTH`     | jumlah kolom maze             |
| `HEIGHT`    | jumlah baris maze             |
| `CELL_SIZE` | ukuran setiap sel dalam pixel |

Contoh:

Jika `CELL_SIZE = 10`, maka setiap sel memiliki ukuran **10px × 10px**.

---

## Perhitungan Koordinat Pixel

Koordinat sel dihitung berdasarkan indeks grid:

```
x1 = column * CELL_SIZE
x2 = x1 + CELL_SIZE

y1 = row * CELL_SIZE
y2 = y1 + CELL_SIZE
```

Contoh:

| Cell  | Koordinat    |
| ----- | ------------ |
| (0,0) | (0,10,0,10)  |
| (1,0) | (10,20,0,10) |
| (2,0) | (20,30,0,10) |

---

## Fungsi Program

### 1. `initMaze()`

Menginisialisasi maze dengan:

* menghitung koordinat pixel setiap sel
* mengatur semua sel sebagai **jalan (wall = 0)**

```c
void initMaze()
```

---

### 2. `printMaze()`

Menampilkan koordinat setiap sel di terminal.

Contoh output:

```
(0,10,0,10) (10,20,0,10) (20,30,0,10)
(0,10,10,20) (10,20,10,20) (20,30,10,20)
```

---

### 3. `divide(x, y, w, h)`

Fungsi utama algoritma **Recursive Division**.

Parameter:

| Parameter | Fungsi                 |
| --------- | ---------------------- |
| `x`       | posisi kolom awal area |
| `y`       | posisi baris awal area |
| `w`       | lebar area             |
| `h`       | tinggi area            |

Langkah kerja:

1. Jika area terlalu kecil → berhenti
2. Pilih orientasi dinding (horizontal / vertical)
3. Buat dinding
4. Buat satu celah
5. Bagi area menjadi dua
6. Panggil fungsi secara rekursif


## Contoh Output

Program akan mencetak koordinat setiap sel:

```
(0,10,0,10) (10,20,0,10) (20,30,0,10)
(0,10,10,20) (10,20,10,20) (20,30,10,20)
...
```

Maze yang dihasilkan akan berbeda setiap kali dijalankan karena menggunakan:

```
rand()
```

---
