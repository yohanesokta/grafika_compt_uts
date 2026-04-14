Berikut versi README.md yang sudah aku rapikan dan **ditambahkan pemahaman yang tadi kamu “klik” (stack, top-1, random arah, bukan random posisi)** secara runtut dan nyambung dari awal sampai akhir.

Kamu bisa langsung copy-replace isi file kamu 👇

---


# Maze Generator (DFS + Backtracking)

## Deskripsi

Program ini merupakan implementasi pembentukan maze menggunakan algoritma Depth-First Search (DFS) dengan pendekatan stack (backtracking).

Maze direpresentasikan dalam bentuk grid 2 dimensi:
- `1` = tembok
- `0` = jalan

---

## Struktur Data

Maze terdiri dari grid:


m->grid[y][x]

````

Setiap cell memiliki atribut:
- `x1, x2, y1, y2` : koordinat (untuk rendering)
- `wall` : status cell (1 = tembok, 0 = jalan)

---

## Alur Program

Urutan eksekusi:

1. `initMaze()` → semua cell jadi tembok
2. `divide()` → generate maze (DFS)
3. `printMaze()` → debug (opsional)

---

# 1. Fungsi `initMaze()`

### Tujuan
Mengisi seluruh grid dengan tembok, lalu membuka titik awal dan pintu.

---

### Semua cell jadi tembok

```c
m->grid[y][x].wall = 1;
````

Kondisi awal:

```
████████
████████
████████
```

---

### Membuka titik penting

```c
m->grid[1][1].wall = 0;
m->grid[0][WIDTH/2].wall = 0;
m->grid[HEIGHT-1][WIDTH/2].wall = 0;
```

Catatan:

* `WIDTH/2` hanya untuk posisi pintu
* tidak berhubungan dengan random

---

# 2. Fungsi `divide()` (INTI)

Maze dibuat menggunakan:

* DFS (Depth First Search)
* Stack manual
* Backtracking

---

# 3. Konsep Penting Sebelum Masuk Loop

## 3.1 Stack

```c
int stack[WIDTH*HEIGHT][2];
int top = 0;
```

Makna:

* `stack` = menyimpan jalur
* `top` = jumlah isi stack
* posisi terakhir = `top-1`

---

## Kenapa pakai `top-1`?

```c
int cx = stack[top-1][0];
int cy = stack[top-1][1];
```

Karena:

* `top` menunjuk ke slot kosong
* elemen terakhir ada di `top-1`

Contoh:

```
index 0 → (1,1)
index 1 → (3,1)
top = 2
```

Posisi aktif:

```
stack[top-1] = stack[1]
```

---

## 3.2 Titik Awal

```c
stack[top] = (1,1);
top++;
visited[1][1] = 1;
```

Maze selalu mulai dari satu titik.

---

# 4. Pergerakan (Bukan Random Posisi)

## Penting:

Algoritma ini **tidak pernah memilih posisi secara random**

Tidak ada:

```c
cx = rand()%WIDTH;
```

Yang ada:

```
posisi baru = posisi sekarang + arah
```

---

## Arah gerak

```c
int dx[4] = {0,0,2,-2};
int dy[4] = {2,-2,0,0};
```

Mapping:

| dir | arah  |
| --- | ----- |
| 0   | bawah |
| 1   | atas  |
| 2   | kanan |
| 3   | kiri  |

---

# 5. Random Terjadi di Arah, Bukan Posisi

```c
int dirs[4] = {0,1,2,3};
```

Lalu diacak:

```c
for(int i=0;i<4;i++){
    int r = rand()%4;
    swap(dirs[i], dirs[r]);
}
```

---

## Artinya:

Bukan:

```
pilih arah random langsung
```

Tapi:

```
acak urutan arah → coba satu per satu
```

---

# 6. Cara Memilih Arah

```c
for(int i=0;i<4;i++){
    int nx = cx + dx[dirs[i]];
    int ny = cy + dy[dirs[i]];

    if(valid){
        // pakai arah ini
        break;
    }
}
```

---

## Contoh

Misal hasil shuffle:

```
[kanan, bawah, kiri, atas]
```

Maka:

1. coba kanan → kalau valid → pakai
2. kalau tidak → coba bawah
3. dst

---

## Kesimpulan bagian ini

Random terjadi karena:

* urutan arah selalu berubah
* bukan karena posisi diacak

---

# 7. Kenapa Lompat 2 Cell?

Pergerakan:

```
(5,5) → (5,7)
```

Struktur:

```
CELL - WALL - CELL
```

---

# 8. Membuka Jalan (Bagian Paling Penting)

```c
m->grid[cy + dy/2][cx + dx/2].wall = 0;
```

---

## Penjelasan

Contoh:

```
(5,5) → (5,7)
```

Grid:

```
(5,5)  cell
(5,6)  tembok
(5,7)  cell
```

Perhitungan:

```
dy = 2 → dy/2 = 1
```

Maka:

```
(5,5) + (0,1) = (5,6)
```

Yang dihapus:

```
m->grid[5][6].wall = 0
```

---

## Lalu buka tujuan

```c
m->grid[ny][nx].wall = 0;
```

---

# 9. Push ke Stack

```c
stack[top] = (nx, ny);
top++;
```

Artinya:

* lanjut eksplorasi dari posisi baru

---

# 10. Backtracking (Kalau Mentok)

```c
if(!moved)
    top--;
```

Artinya:

* mundur ke posisi sebelumnya

---

## Visual Stack

```
(1,1) → (3,1) → (3,3)
top = 3
```

Mentok:

```
top-- → 2
```

Posisi sekarang:

```
(3,1)
```

---

# 11. Kenapa Maze Tidak Ketutup?

Karena:

1. Setiap langkah selalu membuka jalan
2. Tidak ada kode yang menutup kembali (`wall = 1`)
3. Semua cell terhubung melalui DFS
4. Backtracking memastikan semua area dikunjungi

---

# 12. Intuisi Sederhana

Algoritma ini bekerja seperti:

1. Mulai dari satu titik
2. Pilih arah secara acak
3. Jalan ke sana sambil menghancurkan tembok
4. Simpan jalur di stack
5. Kalau mentok → mundur
6. Ulangi sampai selesai

---

# 13. Kesimpulan Utama

* Random terjadi di **arah**, bukan posisi
* Posisi selalu berasal dari stack (`top-1`)
* Maze terbentuk karena tembok dihancurkan antar cell
* DFS + backtracking memastikan semua terhubung

---

## Baris paling penting

```c
m->grid[cy + dy[dirs[i]]/2][cx + dx[dirs[i]]/2].wall = 0;
```

Karena:

* ini yang menghubungkan dua cell
* tanpa ini maze tidak terbentuk

