#ifndef RSA
#define RSA

// Operasi File
int is_txt_file(const char *filename);
FILE *createFile(const char *filename);
FILE *openFile(const char *filename);

// Melakukan operasi perkalian modular
long long mulmod(long long a, long long b, long long mod);

// Operasi perpangkatan modular
long long modular(long long bil, long long pangkat, long long mod);

// Fungsi untuk mengecek bilangan prima
int isPrime(long long bil, int perulangan);

// Fungsi untuk mencari FPB dari dua bilangan
int fpb(int a, int b);

// Fungsi untuk menghasilkan private key (e)
int privateKey(int a, int b);

// Fungsi untuk menghasilkan public dan privatekey
int KeyGeneration(long long *e, long long *d, long long *n, FILE *file_key);

// Prosedur untuk Enkripsi
void enkripsi(char **matriks, long long e, long long n, int kolom, FILE *file);

// Prosedur untuk dekripsi
void dekripsi(long long d, long long n, FILE *file_in, FILE *file_out);

// Fungsi untuk mengalokasi matriks
char** alokasiMatriks(int baris, int kolom);

// Prosedur untuk mengisi matriks
void isiMatriks(char **matriks, int baris, int kolom, const char *input);

// Prosedur untuk mendealokasi matriks
void dealokasiMatriks(char **matriks, int baris);

// Prosedur untuk menggeser baris-baris dalam array state
void shiftRows(char **state, int columns);

// Prosedur untuk menggeser satu baris dalam array state ke kanan
void shiftRow(char *state, int columns, unsigned char nbr);

// Prosedur untuk mengembalikan baris-baris dalam array state ke posisi semula
void reverseShiftRows(char **state, int columns);

// Prosedur untuk menggeser balik satu baris dalam array state ke kiri
void reverseShiftRow(char *state, int columns, unsigned char nbr);

#endif
