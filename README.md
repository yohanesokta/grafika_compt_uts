
# Maze Generator (DFS + Backtracking)

## Deskripsi

Program ini merupakan implementasi pembentukan maze menggunakan algoritma Depth-First Search (DFS) dengan pendekatan stack (backtracking). Maze direpresentasikan dalam bentuk grid 2 dimensi di mana setiap cell memiliki status sebagai tembok atau jalan.

---

## Struktur Data

Maze terdiri dari grid:


m->grid[y][x]


Setiap cell memiliki atribut:
- `x1, x2, y1, y2` : koordinat (digunakan untuk rendering)
- `wall` : status cell (1 = tembok, 0 = jalan)

---

## Alur Program

Urutan eksekusi utama:

1. `initMaze()` → inisialisasi semua cell
2. `divide()` → generate maze menggunakan DFS
3. `printMaze()` → menampilkan data (opsional untuk debug)

---

## Fungsi `initMaze(Maze *m)`

### Tujuan
Menginisialisasi seluruh maze.

### Langkah-langkah

### 1. Loop seluruh grid
```c
for(int y=0; y<HEIGHT; y++){
    for(int x=0; x<WIDTH; x++){
````

### 2. Set koordinat setiap cell

```c
m->grid[y][x].x1 = x * CELL_SIZE;
m->grid[y][x].x2 = m->grid[y][x].x1 + CELL_SIZE;

m->grid[y][x].y1 = y * CELL_SIZE;
m->grid[y][x].y2 = m->grid[y][x].y1 + CELL_SIZE;
```

### 3. Set semua cell sebagai tembok

```c
m->grid[y][x].wall = 1;
```

Kondisi awal maze:

```
████████
████████
████████
```

### 4. Membuka titik awal dan pintu

```c
m->grid[1][1].wall = 0;
m->grid[0][WIDTH/2].wall = 0;
m->grid[HEIGHT-1][WIDTH/2].wall = 0;
```

---

## Fungsi `printMaze(Maze *m)`

### Tujuan

Menampilkan koordinat tiap cell.

### Implementasi

```c
printf("(%d,%d,%d,%d)", x1, x2, y1, y2);
```

Fungsi ini hanya untuk debugging dan tidak mempengaruhi pembentukan maze.

---

## Fungsi `divide(Maze *m, int x, int y, int w, int h)`

### Tujuan

Menghasilkan jalur maze menggunakan algoritma DFS dengan stack.

---

## Inisialisasi

### 1. Stack

```c
int stack[WIDTH*HEIGHT][2];
int top = 0;
```

Digunakan untuk menyimpan posisi cell selama eksplorasi.

---

### 2. Array visited

```c
int visited[HEIGHT][WIDTH] = {0};
```

Menandai cell yang sudah dikunjungi.

---

### 3. Arah gerak

```c
int dx[4] = {0,0,2,-2};
int dy[4] = {2,-2,0,0};
```

Mapping arah:

| dir | dx | dy | arah  |
| --- | -- | -- | ----- |
| 0   | 0  | +2 | bawah |
| 1   | 0  | -2 | atas  |
| 2   | +2 | 0  | kanan |
| 3   | -2 | 0  | kiri  |

Pergerakan dilakukan dengan melompat 2 cell.

---

### 4. Titik awal

```c
stack[top][0] = 1;
stack[top][1] = 1;
top++;

visited[1][1] = 1;
```

---

## Loop Utama

```c
while(top > 0)
```

Selama stack tidak kosong, algoritma terus berjalan.

---

### 1. Ambil posisi saat ini

```c
int cx = stack[top-1][0];
int cy = stack[top-1][1];
```

---

### 2. Inisialisasi arah

```c
int dirs[4] = {0,1,2,3};
```

---

### 3. Shuffle arah

```c
for(int i=0;i<4;i++){
    int r = rand()%4;
    int tmp = dirs[i];
    dirs[i] = dirs[r];
    dirs[r] = tmp;
}
```

Mengacak urutan arah agar maze tidak selalu sama.

---

### 4. Coba setiap arah

```c
for(int i=0;i<4;i++){
```

Ambil arah:

```c
int nx = cx + dx[dirs[i]];
int ny = cy + dy[dirs[i]];
```

---

### 5. Validasi posisi

```c
if(nx > 0 && nx < WIDTH-1 &&
   ny > 0 && ny < HEIGHT-1 &&
   !visited[ny][nx])
```

Syarat:

* Tidak keluar batas
* Belum dikunjungi

---

## Proses Membuka Jalan

### 1. Tandai dikunjungi

```c
visited[ny][nx] = 1;
```

---

### 2. Hapus tembok di tengah

```c
m->grid[cy + dy[dirs[i]]/2][cx + dx[dirs[i]]/2].wall = 0;
```

### Penjelasan detail

Jika bergerak dari:

```
(cx, cy) → (nx, ny)
```

Contoh:

```
(5,5) → (5,7)
```

Struktur:

```
(5,5)  cell awal
(5,6)  tembok
(5,7)  cell tujuan
```

Perhitungan:

```
dy = 2 → dy/2 = 1
```

Maka:

```
cy + dy/2 = 5 + 1 = 6
```

Sehingga:

```
m->grid[5][6].wall = 0;
```

Artinya tembok di tengah dihapus.

---

### 3. Buka cell tujuan

```c
m->grid[ny][nx].wall = 0;
```

---

### 4. Masukkan ke stack

```c
stack[top][0] = nx;
stack[top][1] = ny;
top++;
```

---

### 5. Tandai berhasil bergerak

```c
moved = 1;
break;
```

---

## Backtracking

Jika tidak ada arah yang bisa dilanjutkan:

```c
if(!moved)
    top--;
```

Artinya kembali ke cell sebelumnya.

---

## Alur Lengkap

1. Mulai dari titik awal
2. Pilih arah secara acak
3. Jika valid:

   * hancurkan tembok tengah
   * buka cell tujuan
   * lanjut eksplorasi
4. Jika tidak ada jalan:

   * mundur (backtrack)
5. Ulang sampai semua cell dikunjungi

---

## Karakteristik Maze

* Semua cell terhubung
* Tidak ada jalur loop
* Hanya satu jalur antara dua titik
* Struktur acak

---

## Kesimpulan

Maze dibentuk dengan cara:

* Menjelajahi grid menggunakan DFS
* Menghapus tembok antar cell
* Menggunakan stack untuk menyimpan jalur
* Melakukan backtracking saat mentok

Baris paling penting dalam pembentukan jalur adalah:

```c
m->grid[cy + dy[dirs[i]]/2][cx + dx[dirs[i]]/2].wall = 0;
```

Karena baris ini yang menghubungkan dua cell dengan menghapus tembok di antaranya.

```
```
