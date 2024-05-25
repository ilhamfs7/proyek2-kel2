#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> // Untuk time()
#include "rsa.h"

int is_txt_file(const char *filename) {
    const char *extension;
	extension = strrchr(filename, '.');
    if (extension != NULL && strcmp(extension, ".txt") == 0) {
        return 1;
    } else {
    	printf("File harus memiliki ekstensi .txt\n");
        return 0; 
    }
}

FILE *createFile(const char *filename) {
	FILE *file;
	file = fopen(filename, "w");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
    }
    return file;
}

FILE *openFile(const char *filename) {
	FILE *file;
	file = fopen(filename, "r");
    if (file == NULL) {
        printf("Gagal membuka file.\n");
    }
    return file;
}


long long mulmod(long long a, long long b, long long mod)
{
    long long x = 0, y = a % mod;
    while (b > 0)
    {
        if (b % 2 == 1)
        {
            x = (x + y) % mod;
        }
        y = (y * 2) % mod;
        b /= 2;
    }
    return x % mod;
}

/* operasi modular */

long long modular(long long bil, long long pangkat, long long mod)
{
    long long x, y;
    
    x = 1;
    y = bil;
    while (pangkat > 0)
    {
        if (pangkat % 2 == 1)
        {
            x = (x * y) % mod;
        }  
        y = (y * y) % mod;
        pangkat /= 2;
    }
    return x % mod;
}

/* tes prima miller rabin*/

int isPrime(long long bil, int perulangan)
{
    int i;
    long long s, a, temp, mod;
    
    if (bil < 2)
    {
        return 0;
    }
    if (bil != 2 && bil % 2 == 0)
    {
        return 0;
    }
    
    s = bil - 1;
    while (s % 2 == 0)
    {
        s = s / 2;
    }
    for (i = 0; i < perulangan; i++)
    {
        a = rand() % (bil - 1) + 1;
        temp = s;
        mod = modular(a, temp, bil);
        
        while (temp != bil - 1 && mod != 1 && mod != bil - 1)
       {
            mod = mulmod(mod, mod, bil);
            temp *= 2;
        }
        if (mod != bil - 1 && temp % 2 == 0)
        {
            return 0;
        }
    }
    return 1;
}

/* mencari fpb dari dua bilangan */

int fpb(int a, int b)
{
    int q, r1, r2, r;

    if (a > b)
    {
        r1 = a;
        r2 = b;
    }
    else
    {
        r1 = b;
        r2 = a;
    }

    while (r2 > 0)
    {
        q = r1 / r2;
        r = r1 - q * r2;
        r1 = r2;
        r2 = r;
    }

    return r1;
}

int privateKey(int a, int b)
{
    int inv;
    int q, r, r1 = a, r2 = b, t, t1 = 0, t2 = 1;

    while (r2 > 0)
    {
        q = r1 / r2;
        r = r1 - q * r2;
        r1 = r2;
        r2 = r;

        t = t1 - q * t2;
        t1 = t2;
        t2 = t;
    }

    if (r1 == 1)
    {
        inv = t1;
    }

    if (inv < 0)
    {
        inv = inv + a;
    }

    return inv;
}

int KeyGeneration(long long *e, long long *d, long long *n, FILE *file_key)
{
    long long p, q, phi_n;
    srand(time(0));
    do
    {
        do
            p = rand();
        while (p % 2 == 0);
    } while (!isPrime(p, 5));

    do
    {
        do
           q = rand();
        while (q % 2 == 0);
    } while (!isPrime(q, 5));

    *n = p * q;
    phi_n = (p - 1) * (q - 1);

    do
        *e = rand() % (phi_n - 2) + 2; // 1 < e < phi_n
    while (fpb(*e, phi_n) != 1);

    *d = privateKey(phi_n, *e);

    fprintf(file_key, "Public Key (e, n): (%lld, %lld)\n", *e, *n);
    fprintf(file_key, "Private Key (d, n): (%lld, %lld)\n", *d, *n);

    return 1; 
}

char** alokasiMatriks(int baris, int kolom) {
	int i;
    char **matriks = (char **)malloc(baris * sizeof(char *));
    for (i = 0; i < baris; i++) {
        matriks[i] = (char *)malloc(kolom * sizeof(char));
    }
    return matriks;
}

void isiMatriks(char **matriks, int baris, int kolom, const char *input) {
    int i, j, index = 0;
    int length = strlen(input);
    for (i = 0; i < baris; i++) {
        for (j = 0; j < kolom; j++) {
            if (index < length) {
                matriks[i][j] = input[index++];
            } else {
                matriks[i][j] = ' ';  // Isi dengan spasi jika input habis
            }
        }
    }
}

void dealokasiMatriks(char **matriks, int baris) {
	int i;
    for (i = 0; i < baris; i++) {
        free(matriks[i]);
    }
    free(matriks);
}

void shiftRow(char *state, int columns, unsigned char nbr) {
	unsigned char i;
	int j;
    for (i = 0; i < nbr; i++) {
        char tmp = state[columns - 1];
        for (j = columns - 1; j > 0; j--) {
            state[j] = state[j - 1];
        }
        state[0] = tmp;
    }
}

void shiftRows(char **state, int columns) {
	int i;
    for (i = 0; i < 2; i++) {
        shiftRow(state[i], columns, i + 1);
    }
}

void reverseShiftRow(char *state, int columns, unsigned char nbr) {
	unsigned char i;
	int j;
    for (i = 0; i < nbr; i++) {
        char tmp = state[0];
        for (j = 0; j < columns - 1; j++) {
            state[j] = state[j + 1];
        }
        state[columns - 1] = tmp;
    }
}

void reverseShiftRows(char **state, int columns) {
	int i;
    for (i = 0; i < 2; i++) {
        reverseShiftRow(state[i], columns, i + 1);
    }
}

// enkripsi
void enkripsi(char **matriks, long long e, long long n, int kolom, FILE *file) {
	long long cipher;
	int i, j;
    shiftRows(matriks, kolom);
    fprintf(file, "%d ", kolom);
    for (i = 0; i < 2; i++) {
        for (j = 0; j < kolom; j++) {
            cipher = modular(matriks[i][j], e, n);
            fprintf(file, "%lld ", cipher); // Menulis hasil enkripsi ke dalam file
        }
    }
    fprintf(file, "\n");
}

// deskripsi
void dekripsi(long long d, long long n, FILE *file_in, FILE *file_out) {
    long long cipher;
    int i, j, kolom;
    char **dekrip, c[1000];

    // Membaca jumlah kolom dari file input
    fscanf(file_in, "%d ", &kolom);

    // Dekripsi pesan
    i = 0;
    while (fscanf(file_in, "%lld", &cipher) != EOF) {
        c[i] = (char)modular(cipher, d, n);
        i++;
    }
    c[i] = '\0';

    // Alokasi memori untuk matriks
    dekrip = alokasiMatriks(2, kolom);

    // Mengisi matriks dengan hasil dekripsi
    isiMatriks(dekrip, 2, kolom, c);
    
    for (i = 0; i < 2; i++) {
        for (j = 0; j < kolom; j++) {
            printf("%c ", dekrip[i][j]);
        }
        printf("\n");
    }
    
    reverseShiftRows(dekrip, kolom);

    // Menulis hasil dekripsi dari matriks ke file output
    for (j = 0; j < 2; j++) {
        for (i = 0; i < kolom; i++) {
            fprintf(file_out, "%c", dekrip[j][i]);
        }
    }
    fprintf(file_out, "\n");

    // Dealokasi memori
    dealokasiMatriks(dekrip, 2);
}
