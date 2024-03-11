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
	y = pangkat;
    while (pangkat > 0)
    {
        if (pangkat % 2 == 1)
        {
        	x = (x * y) % mod;
		}  
        y = (y * y) % mod;
        pangkat = pangkat / 2;
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

int KeyGeneration(long long *e, long long *d, long long *n)
{
    long long p, q, phi_n;
    srand(time(0));

    do
    {
        do
            p = ((rand() << 15) ^ rand());
        while (p % 2 == 0);

    } while (!isPrime(p, 5));

    do
    {
        do
            q = ((rand() << 15) ^ rand());
        while (q % 2 == 0);
    } while (!isPrime(q, 5));

    *n = p * q;
    phi_n = (p - 1) * (q - 1);

    do
        *e = rand() % (phi_n - 2) + 2; // 1 < e < phi_n
    while (fpb(*e, phi_n) != 1);

    *d = privateKey(phi_n, *e);

    return 1; 
}

// convert plain teks to ascii
void ASCIISentence(char *str) {
    int l = strlen(str);
    int i;
    int convert;
    for (i = 0; i < l; i++) {
        convert = str[i];
        printf("%d ", convert);
    }
    printf("\n");
}

// enkripsi
int *enkripsi(long long e, long long n, char *pesan)
{
    int *enkrip;
    int panjang_pesan, i;
    long long cipher;

    panjang_pesan = strlen(pesan); // Menghitung panjang pesan

    // Alokasi memori untuk array enkrip
    enkrip = (int *)malloc(panjang_pesan * sizeof(int));

    // Melakukan enkripsi pada setiap karakter pesan
    for(i = 0; i < panjang_pesan; i++)
    {
        cipher = modular(pesan[i], e, n);
        enkrip[i] = cipher;
    }

    return enkrip; // Mengembalikan array enkripsi
}



// Fungsi untuk dekripsi 
char *dekripsi(long long d, long long n, int *pesan_enkripsi, int panjang_pesan) {
	int i;
    char *dekrip;
	
	dekrip = (char *)malloc(panjang_pesan * sizeof(char));

    // Melakukan dekripsi pada setiap karakter pesan yang telah dienkripsi
    for(i = 0; i < panjang_pesan; i++) {
        long long plain = modular(pesan_enkripsi[i], d, n);
        dekrip[i] = (char)plain;
    }

    return dekrip; // Mengembalikan pesan yang telah didekripsi
}

// Main
int main() {
    long long e, d, n;
    int *enkrip;
    char *dekrip, *token;
    int pil, i, len;
    int pesan_enkripsi[len];
    char str[100];
    
	do
	{
	    printf("Pilih Operasi:\n");
	    printf("1. Generate Kunci\n");
	    printf("2. Enkripsi\n");
	    printf("3. Deskripsi\n");
	    printf("4. Keluar\n");
	    printf("Masukkan pilihan (1, 2, or 3): ");
	    scanf("%d", &pil);
	    
	    switch (pil) {
	        case 1:
	            if (KeyGeneration(&e, &d, &n)) {
	                printf("Public Key (e, n): (%lld, %lld)\n", e, n);
	                printf("Private Key (d, n): (%lld, %lld)\n", d, n);
	            } else {
	                printf("Kunci gagal di-generate\n");
	            }
	            break;
	        case 2:
	            printf("Masukkan pesan: ");
	            getchar(); // Untuk membersihkan stdin
	            fgets(str, sizeof(str), stdin);
	            if (str[strlen(str) - 1] == '\n') {
	                str[strlen(str) - 1] = '\0';
	            }
	            
	            printf("Masukkan public key : ");
	            scanf("%lld", &e);
	            printf("Masukkan n: ");
	            scanf("%lld", &n);
	            
	            enkrip = enkripsi(e, n, str);
	            printf("Hasil enkripsi:\n");
	            for (i = 0; i < strlen(str); i++) {
	                printf("%d ", enkrip[i]);
	            }
	            printf("\n");
	            free(enkrip);
	            break;
	        case 3:
	            printf("Masukkan private key : ");
	            scanf("%lld", &d);
	            printf("Masukkan n: ");
	            scanf("%lld", &n);
	            
	            printf("Masukkan pesan terenkripsi: ");
	            getchar(); // Untuk membersihkan stdin
	            fgets(str, sizeof(str), stdin);
	            if (str[strlen(str) - 1] == '\n') {
	                str[strlen(str) - 1] = '\0';
	            }
	            len = strlen(str);
	            token = strtok(str, " ");
	            
	            i = 0;
	            while (token != NULL) {
	                pesan_enkripsi[i++] = atoi(token);
	                token = strtok(NULL, " ");
	            }
	            
	            dekrip = dekripsi(d, n, pesan_enkripsi, len);
	            printf("Hasil dekripsi: %s\n", dekrip);
	            free(dekrip);
	            break;
	        case 4:
	        	printf("Keluar...\n");
	        	break;
	        default:
	            printf("Pilihan tidak tepat.\n");
	            break;
	    	} 
  	} while (pil != 4);
return 0;
}
