#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> // Untuk time()

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

// enkripsi
void enkripsi(long long e, long long n, char *pesan, int panjang_pesan, FILE *file) {
    int i;
    long long cipher;

    for (i = 0; i < panjang_pesan; i++) {
        cipher = modular(pesan[i], e, n);
        fprintf(file, "%lld ", cipher); // Menulis hasil enkripsi ke dalam file
    }
    fprintf(file, "\n");
}

// deskripsi
void dekripsi_file(long long d, long long n, FILE *file_in, FILE *file_out) {
    long long cipher;
    char c;

    while (fscanf(file_in, "%lld", &cipher) != EOF) {
        c = (char)modular(cipher, d, n);
        fprintf(file_out, "%c", c);
    }
}

// Main
int main() {
    long long e, d, n;
    int pil, i, len, panjang_pesan;
    char str[100];
    char input_filename[100];
    char output_filename[100];
    char filename[100];
    char key_filename[100];
    FILE *file, *file_key, *file_in, *file_out;

    do {
        system("cls");
        printf("=======================\n");
        printf("   Kriptografi RSA\n");
        printf("=======================\n");
        printf("Menu Utama:\n");
        printf("1. Generate Kunci\n");
        printf("2. Keluar\n");
        printf("Masukkan pilihan (1 atau 2): ");
        scanf("%d", &pil);

        switch (pil) {
            case 1:
                printf("Masukkan nama file untuk menyimpan hasil generate key: ");
                scanf("%s", key_filename);

                file_key = fopen(key_filename, "w");
                if (file_key == NULL) {
                    printf("Gagal membuka file untuk hasil generate key.\n");
                    break;
                }

                if (KeyGeneration(&e, &d, &n, file_key)) {
                    system("cls");
                    printf("Kunci berhasil di-generate dan disimpan di '%s':\n\n", key_filename);
                    printf("Public Key (e, n): (%lld, %lld)\n", e, n);
                    printf("Private Key (d, n): (%lld, %lld)\n", d, n);
                } else {
                    printf("Kunci gagal di-generate\n");
                    fclose(file_key);
                    break;
                }
                fclose(file_key);
                do {
                    printf("\nMenu Enkripsi/Deskripsi:\n");
                    printf("1. Enkripsi\n");
                    printf("2. Deskripsi\n");
                    printf("3. Kembali ke Menu Utama\n");
                    printf("Masukkan pilihan (1, 2, atau 3): ");
                    scanf("%d", &pil);
                    switch (pil) {
                        case 1:
                            printf("Masukkan pesan: ");
                            getchar(); // Untuk membersihkan stdin
                            fgets(str, sizeof(str), stdin);
                            if (str[strlen(str) - 1] == '\n') {
                                str[strlen(str) - 1] = '\0';
                            }

                            panjang_pesan = strlen(str);
                            printf("Masukkan nama file untuk menyimpan hasil enkripsi: ");
                            scanf("%s", filename);

                            file = fopen(filename, "w");
                            if (file == NULL) {
                                printf("Gagal membuka file.\n");
                                break;
                            }
                            printf("Masukkan public key : ");
                            scanf("%lld", &e);
                            printf("Masukkan n: ");
                            scanf("%lld", &n);

                            enkripsi(e, n, str, panjang_pesan, file);

                            fclose(file);
                            printf("Hasil enkripsi berhasil disimpan ke dalam file '%s'\n", filename);
                            break;
                        case 2:
                            printf("Masukkan nama file input yang akan didekripsi: ");
                            scanf("%s", input_filename);
                            printf("Masukkan nama file output untuk menyimpan hasil dekripsi: ");
                            scanf("%s", output_filename);

                            // Membuka file input untuk dibaca
                            file_in = fopen(input_filename, "r");
                            if (file_in == NULL) {
                                printf("Gagal membuka file input.\n");
                                return 1;
                            }

                            // Membuka file output untuk ditulis
                            file_out = fopen(output_filename, "w");
                            if (file_out == NULL) {
                                printf("Gagal membuka file output.\n");
                                fclose(file_in);
                                return 1;
                            }
                            printf("Masukkan private key : ");
                            scanf("%lld", &d);
                            printf("Masukkan n: ");
                            scanf("%lld", &n);

                            dekripsi_file(d, n, file_in, file_out);

                            // Menutup file setelah selesai proses dekripsi
                            fclose(file_in);
                            fclose(file_out);
                            printf("Hasil dekripsi berhasil disimpan ke dalam file '%s'\n", output_filename);
                            break;
                        case 3:
                            printf("Kembali ke Menu Utama...\n");
                            break;
                        default:
                            printf("Pilihan tidak tepat.\n");
                            break;
                    }
                } while (pil != 3);
                break;
            case 2:
                printf("Keluar...\n");
                break;
            default:
                printf("Pilihan tidak tepat.\n");
                break;
        }
    } while (pil != 2);

    return 0;
}
