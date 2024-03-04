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

long long modulo(long long base, long long exponent, long long mod)
{
    long long x = 1;
    long long y = base;
    while (exponent > 0)
    {
        if (exponent % 2 == 1)
            x = (x * y) % mod;
        y = (y * y) % mod;
        exponent = exponent / 2;
    }
    return x % mod;
}

/* tes prima miller rabin*/

int Miller(long long p, int iteration)
{
    int i;
    long long s;
    if (p < 2)
    {
        return 0;
    }
    if (p != 2 && p % 2 == 0)
    {
        return 0;
    }
    s = p - 1;
    while (s % 2 == 0)
    {
        s /= 2;
    }
    for (i = 0; i < iteration; i++)
    {
        long long a = rand() % (p - 1) + 1, temp = s;
        long long mod = modulo(a, temp, p);
        while (temp != p - 1 && mod != 1 && mod != p - 1)
        {
            mod = mulmod(mod, mod, p);
            temp *= 2;
        }
        if (mod != p - 1 && temp % 2 == 0)
        {
            return 0;
        }
    }
    return 1;
}

/* mencari fpb dari dua bilangan */

int gcd(int a, int b)
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

int inverse(int a, int b)
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

    } while (!Miller(p, 5));

    do
    {
        do
            q = ((rand() << 15) ^ rand());
        while (q % 2 == 0);
    } while (!Miller(q, 5));

    *n = p * q;
    phi_n = (p - 1) * (q - 1);

    do
        *e = rand() % (phi_n - 2) + 2; // 1 < e < phi_n
    while (gcd(*e, phi_n) != 1);

    *d = inverse(phi_n, *e);

    return 1; 
}

// Main
int main()
{
    long long e, d, n;
    int pil;
    char ch;

    printf("Pilih Operasi:\n");
    printf("1. Generate Kunci\n");
    printf("2. Enkripsi\n");
    printf("3. Deskripsi\n");
    printf("Masukkan pilihan (1, 2, or 3): ");
    scanf("%d", &pil);
    
    switch (pil) {
        case 1:
        	if (KeyGeneration(&e, &d, &n))
		    {
		        printf("Public Key (e, n): (%lld, %lld)\n", e, n);
		        printf("Private Key (d, n): (%lld, %lld)\n", d, n);
		    }
		    else
		    {
		        printf("Kunci gagal di-generate\n");
		    }
		    break;
		case 2:
			printf("\nMasukkan teks yang akan di-enkripsi : ");
			scanf("%c", &ch);
			
		default:
            printf("Pilihan tidak tepat.\n");
	}
    return 0;
}
