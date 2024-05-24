#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encodeLSB.h"
#include "decodeLSB.h"
#include "rsa.h"

int main() {
    int choice;
    char input_filename[100];
    char output_filename[100];
    char message[1000];
    int exit = 0; 
    long long e, d, n;
    int pil, i, panjang_pesan, validasi, kolom, baris = 2;
    char str[100];
    char filename[20];
    char key_filename[20];
    char **matriks;
    FILE *file, *file_key, *file_in, *file_out;

    while (!exit) { 
        system("cls");
        printf("-------------------------------------------\n");
	    printf("|                                         |\n");
	    printf("|             Selamat Datang!             |\n");
	    printf("|        di HidingData Application        |\n");
	    printf("|                                         |\n");
	    printf("|                                         |\n");
	    printf("-------------------------------------------\n");
	    printf("-------------------------------------------\n");
	    printf("|                                         |\n");
	    printf("|         Mau Sembunyikan Pesan?          |\n");
	    printf("|         Tempat ini Solusinya!           |\n");
	    printf("|                                         |\n");
	    printf("|                                         |\n");
	    printf("-------------------------------------------\n");
	    printf("Menu:\n");
		printf("1. Enkripsi\n");
        printf("2. Dekripsi \n");
        printf("3. Keluar\n");
        printf("Pilih menu: ");
        scanf("%d", &choice);

        switch(choice) {
        	case 1:
        		system("cls");
        		
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
                
                system("cls");
            
            	printf("Kunci berhasil di-generate dan disimpan di '%s':\n\n", key_filename);
                printf("Public Key (e, n): (%lld, %lld)\n", e, n);
                printf("Private Key (d, n): (%lld, %lld)\n", d, n);
                
		printf("Masukkan pesan: ");
		fgets(str, sizeof(str), stdin);
							
		// Membersihkan buffer
		int c;
		while ((c = getchar()) != '\n' && c != EOF);
							
		panjang_pesan = strlen(str);
		kolom = (panjang_pesan + 1) / baris;
							
		matriks = alokasiMatriks(baris, kolom);
		isiMatriks(matriks, baris, kolom, str);
							
		do {
			printf("Masukkan nama file untuk menyimpan hasil enkripsi (.txt): ");
			scanf("%s", filename);
			validasi = (!is_txt_file(filename));
				if (validasi == 1) {
					printf("File harus memiliki ekstensi .txt\n");
				}
			} while (validasi == 1);
				
			file = createFile(filename);
				
				file = createFile(filename);
				 
                printf("Masukkan public key : ");
                scanf("%lld", &e);
                printf("Masukkan n: ");
                scanf("%lld", &n);

                enkripsi(matriks, e, n, kolom, file);

                fclose(file);
                printf("Hasil enkripsi berhasil disimpan ke dalam file '%s'\n", filename);
                getchar();
                
                system("pause");
                system("cls");
                
                printf("\nMasukkan nama file gambar BMP(.bmp): ");
                scanf("%s", input_filename);
                getchar();
				
                printf("Masukkan nama file untuk salinan(.bmp): ");
                scanf("%s", output_filename);
                getchar();
                
				printf("Masukkan nama file yang akan dibaca : ");
			    fgets(filename, sizeof(filename), stdin);
			
			    filename[strcspn(filename, "\n")] = 0;
			
			    char* message = (char*)malloc(1000 * sizeof(char));
			
			    if (message == NULL) {
			        perror("Memory allocation failed");
			        return 1;
			    }
			
			    readAndDeleteFile(filename, message, 1000);

                encodeLSB(input_filename, output_filename, message);
                system("pause");
                break;
			case 2:
				system("cls");
				
            	printf("\nMasukkan nama file gambar BMP(.bmp): ");
                scanf("%s", input_filename);

				char* pesan;
                decodeLSB(input_filename, &pesan);
                
                system("pause");
                system("cls");
                
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
                system("pause");
                break;
            case 3:
                exit = 1; 
                printf("\nTerima kasih telah menggunakan program ini.\n");
                break;
            default:
                printf("\nPilihan tidak valid.\n");
        }
    }

    return 0;
}
