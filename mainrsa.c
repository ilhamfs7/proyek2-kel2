#include <stdio.h>
#include <stdlib.h>
#include "rsa.h"

int main() {
    long long e, d, n;
    int pil, i, panjang_pesan, validasi;
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
        printf("2. Dekripsi\n");
        printf("3. Keluar\n");
        printf("Masukkan pilihan (1, 2 atau 3): ");
        scanf("%d", &pil);

        switch (pil) {
            case 1:
            	do {
					printf("Masukkan nama file untuk menyimpan hasil generate key (.txt): ");
                	scanf("%s", key_filename);
					validasi = (!is_txt_file(key_filename));	
				} while (validasi == 1);
				    
                file_key = createFile(key_filename);
        
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
                            
                            do {
                            	printf("Masukkan nama file untuk menyimpan hasil enkripsi (.txt): ");
                            	scanf("%s", filename);
                            	validasi = (!is_txt_file(filename));
							} while (validasi == 1);
							
							file = createFile(filename);
							 
                            printf("Masukkan public key : ");
                            scanf("%lld", &e);
                            printf("Masukkan n: ");
                            scanf("%lld", &n);

                            enkripsi(e, n, str, panjang_pesan, file);

                            fclose(file);
                            printf("Hasil enkripsi berhasil disimpan ke dalam file '%s'\n", filename);
                            getchar();
                            getchar();
                            break;
                        case 2:
                            printf("Kembali ke Menu Utama...\n");
                            break;
                        default:
                            printf("Pilihan tidak tepat.\n");
                            break;
                    }
                    //getchar(); // Membersihkan buffer stdin
                } while (pil != 2);
                break;
            case 2:
            	do {
            		printf("Masukkan nama file input yang akan didekripsi (.txt): ");
                	scanf("%s", input_filename);
                	validasi = (!is_txt_file(input_filename));
				} while (validasi == 1);
            	file_in = openFile(input_filename);
            	
            	do {
            		printf("Masukkan nama file output untuk menyimpan hasil dekripsi (.txt): ");
                	scanf("%s", output_filename);
                	validasi = (!is_txt_file(output_filename));
				} while (validasi == 1);
				file_out = createFile(output_filename);
				
                do {
                	printf("Masukkan nama file kunci (.txt) : ");
                	scanf("%s", key_filename);
                	validasi = (!is_txt_file(key_filename));
				} while (validasi == 1);
				file_key = openFile(key_filename);
                
                fscanf(file_key, "Public Key (e, n): (%*d, %*lld)\nPrivate Key (d, n): (%lld, %lld)", &d, &n);

                dekripsi(d, n, file_in, file_out);

                // Menutup file setelah selesai proses dekripsi
                fclose(file_in);
                fclose(file_out);
                fclose(file_key);
                printf("Hasil dekripsi berhasil disimpan ke dalam file '%s'\n", output_filename);
                getchar();
                break;
            case 3:
            	printf("Keluar ...\n");
            	break;
            default:
                printf("Pilihan tidak tepat.\n");
                break;
        }
        //getchar(); // Membersihkan buffer stdin
		getchar(); // Menunggu user menekan enter
    } while (pil != 3);
    return 0;
}
