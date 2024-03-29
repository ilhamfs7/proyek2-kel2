#ifndef RSA
#define RSA

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
void enkripsi(long long e, long long n, char *pesan, int panjang_pesan, FILE *file);

// Prosedur untuk dekripsi
void dekripsi(long long d, long long n, FILE *file_in, FILE *file_out);

#endif
