#include <stdio.h>
#include <stdlib.h>
#include "rsa.h"

int main() {
    long long e, d, n;
    int pil, i, panjang_pesan;
    char str[100];
    char input_filename[20];
    char output_filename[20];
    char filename[20];
    char key_filename[20];
    FILE *file, *file_key, *file_in, *file_out;

    do {
        system("cls");
        printf("=================================\n");
        printf("        Kriptografi RSA\n");
        printf("=================================\n");
        printf("Menu Utama:\n");
        printf("1. Generate Kunci\n");
        printf("2. Deskripsi\n");
        printf("3. Keluar\n");
        printf("Masukkan pilihan (1, 2 atau 3): ");
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
                } else {
                    printf("Kunci gagal di-generate\n");
                    fclose(file_key);
                    break;
                }
                fclose(file_key);
                do {
                    system("cls");
                    printf("Kunci berhasil di-generate dan disimpan di '%s':\n\n", key_filename);
                    printf("Public Key (e, n): (%lld, %lld)\n", e, n);
                    printf("Private Key (d, n): (%lld, %lld)\n", d, n);
                    printf("\nMenu Enkripsi/Deskripsi:\n");
                    printf("1. Enkripsi\n");
                    printf("2. Kembali ke Menu Utama\n");
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
                            printf("Kembali ke Menu Utama...\n");
                            break;
                        default:
                            printf("Pilihan tidak tepat.\n");
                            break;
                    }
                    getchar(); // Membersihkan buffer stdin
			        printf("Tekan enter untuk melanjutkan...\n");
			        getchar(); // Menunggu user menekan enter
                } while (pil != 2);
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

                dekripsi(d, n, file_in, file_out);

                // Menutup file setelah selesai proses dekripsi
                fclose(file_in);
                fclose(file_out);
                printf("Hasil dekripsi berhasil disimpan ke dalam file '%s'\n", output_filename);
                break;
            case 3:
            	printf("Keluar ...\n");
            	break;
            default:
                printf("Pilihan tidak tepat.\n");
                break;
        }
        getchar(); // Membersihkan buffer stdin
		printf("Tekan enter untuk melanjutkan...\n");
		getchar(); // Menunggu user menekan enter
    } while (pil != 3);

    return 0;
}
