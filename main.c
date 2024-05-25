#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "encodeLSB.h"
#include "decodeLSB.h"
#include "rsa.h"
#include "JPG.h"
#include "validasi.h"

int main() {
    int choice, c;
    char input_filename[100];
    char output_filename[100];
    char message[1000];
    int exit = 0; 
    bool benar = true;
    long long e, d, n, input_e, input_n;
    int pil, i, panjang_pesan, validasi, kolom, baris = 2;
    char str[1000];
    char filename[20];
    char src_image[100], dest_image[100], message_file[100];
    char key_filename[20];
    char **matriks;
    const char *ext;
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
        printf("-------------------------------------------\n");
        printf("|                  Menu                   |\n");
        printf("-------------------------------------------\n");
        printf("|1. Enkripsi                              |\n");
        printf("|2. Dekripsi                              |\n");
        printf("|3. Keluar                                |\n");
        printf("-------------------------------------------\n");
        printf("Pilih menu: ");
        
        choice = inputBener(1, 3);

        switch(choice) {
        	case 1:
        	system("cls");
        	printf("-------------------------------------------------------------\n");
                printf("                    Enkripsi Pesan (RSA)                     \n");
                printf("-------------------------------------------------------------\n\n");
                
                do {
			printf("Masukkan nama file untuk menyimpan hasil generate key (.txt): ");
                	scanf("%s", key_filename);
                	printf("-------------------------------------------------------------\n");
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
            
            	printf("-------------------------------------------------------------\n");
                printf("Kunci berhasil di-generate dan disimpan di '%s':\n", key_filename);
                printf("-------------------------------------------------------------\n");
                printf("Public Key (e, n): (%lld, %lld)\n", e, n);
                printf("Private Key (d, n): (%lld, %lld)\n", d, n);
                printf("-------------------------------------------------------------\n");
                
                printf("Masukkan pesan: ");
                getchar(); // Untuk membersihkan stdin
                fgets(str, sizeof(str), stdin);
                
                panjang_pesan = strlen(str);
    			kolom = (panjang_pesan + 1) / baris;
                
                matriks = alokasiMatriks(baris, kolom);
                isiMatriks(matriks, baris, kolom, str);
                
                do {
                	printf("-------------------------------------------------------------\n");
                	printf("Masukkan nama file untuk menyimpan hasil enkripsi (.txt): ");
                	scanf("%s", filename);
                	validasi = (!is_txt_file(filename));
                	while ((c = getchar()) != '\n' && c != EOF);
				} while (validasi == 1);
				
				file = createFile(filename);
				 
               do {
                	printf("------------------------------------\n");
                    printf("Masukkan public key : ");
                    scanf("%lld", &input_e);
                    if (input_e != e) {
                        printf("Public key yang dimasukkan salah. Silakan coba lagi.\n");
                	}
                    while ((c = getchar()) != '\n' && c != EOF);
                } while (input_e != e);
                do {
                	printf("------------------------------------\n");
                    printf("Masukkan n: ");
                    scanf("%lld", &input_n);
                    if (input_n != n) {
                        printf("Nilai n yang dimasukkan salah. Silakan coba lagi.\n");
                	}
                	while ((c = getchar()) != '\n' && c != EOF);
                } while (input_n != n);

                enkripsi(matriks, e, n, kolom, file);

                fclose(file);
                printf("-------------------------------------------------------------\n");
                printf("Hasil enkripsi berhasil disimpan ke dalam file '%s'\n", filename);
                printf("-------------------------------------------------------------\n");
                
                system("pause");
                system("cls");
                
                while (1) {
                	system("cls");
                	printf("-------------------------------------------------------------\n");
                	printf("                    Enkripsi Pesan (LSB)                     \n");
                    printf("-------------------------------------------------------------\n\n");
			        printf("Masukkan nama file gambar (.bmp / .jpg): ");
			        scanf("%s", input_filename);
			        getchar();
			        
			        // Get the file extension
			        ext = strrchr(input_filename, '.');
			        
			        if (ext != NULL) {
			            if (strcmp(ext, ".bmp") == 0) {
			            	do {
			            		printf("-------------------------------------------------------------\n");
							    printf("Masukkan nama file yang berisi pesan (.txt): ");
							    fgets(filename, sizeof(filename), stdin);
							
							    filename[strcspn(filename, "\n")] = 0;
							
							    char* extension = strrchr(filename, '.');
							    if (extension == NULL || strcmp(extension, ".txt") != 0) {
							    	printf("-------------------------------------------------------------\n");
							        printf("Nama file harus memiliki ekstensi .txt\n");
							    } else {
							        break;
							    }
							} while (1);
							
			                do {
							    printf("-------------------------------------------------------------\n");
							    printf("Masukkan nama file untuk menyimpan salinan hasil enkripsi (.bmp): ");
							    scanf("%s", output_filename);
							    getchar();
							
							    char* extension = strrchr(output_filename, '.');
							    if (extension == NULL || strcmp(extension, ".bmp") != 0) {
							        printf("Nama file harus memiliki ekstensi .bmp\n");
							    } else {
							        break;
							    }
							} while (1);
			                
			                char* message = (char*)malloc(10000 * sizeof(char));
			                
			                if (message == NULL) {
			                    printf("-----------------------------------\n");
			                    perror("|     Memory allocation failed    |\n");
			                    printf("-----------------------------------\n");
			                    return 1;
			                }
			                
			                readAndDeleteFile(filename, message, 10000);
			                encodeLSB(input_filename, output_filename, message);
			                break;
			            } else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
			                do {
							    printf("----------------------------------------------------------\n");
							    printf("Masukkan file yang berisi pesan (.txt): ");
							    scanf("%s", message_file);
							    getchar();
							
							    char* extension = strrchr(message_file, '.');
							    if (extension == NULL || strcmp(extension, ".txt") != 0) {
							        printf("Nama file harus memiliki ekstensi .txt\n");
							    } else {
							        break;
							    }
							} while (1);
							
							do {
							    printf("----------------------------------------------------------\n");
							    printf("Masukkan nama file untuk menyimpan salinan hasil enkripsi (.jpg): ");
							    scanf("%s", dest_image);
							    getchar();
							
							    char* extension = strrchr(dest_image, '.');
							    if (extension == NULL || strcmp(extension, ".jpg") != 0) {
							        printf("Nama file harus memiliki ekstensi .jpg\n");
							    } else {
							        break;
							    }
							} while (1);
							
			                printf("\n-------------------------------------\n");
			                printf("|             Encoding...           |\n");
			                printf("-------------------------------------\n");
			                encode_jpeg(input_filename, dest_image, message_file);
			                break;
			            } else {
			            	printf("-------------------------------------------------------------\n");
			                printf("Format file tidak dikenali. Harap masukkan file dengan format .bmp atau .jpg\n");
			            }
			        } else {
			        	printf("-------------------------------------------------------------\n");
			            printf("Tidak ada ekstensi file. Harap masukkan file dengan format .bmp atau .jpg\n");
			        }
			        system("pause");
			    }
                system("pause");
                break;
			case 2:
				do {
	                while (1) {
						system("cls");
						
		            	printf("-------------------------------------------------------------\n");
                		printf("                    Dekripsi Pesan (LSB)                     \n");
                    	printf("-------------------------------------------------------------\n\n");
				        printf("Masukkan nama file gambar (.bmp / .jpg): ");
				        scanf("%s", input_filename);
				
				        // Cek apakah file ada
				        file = fopen(input_filename, "r");
				        if (file) {
				            // File ditemukan, tutup file dan keluar dari loop
				            fclose(file);
				            break;
				        } else {
				            // File tidak ditemukan, berikan peringatan dan minta input ulang
				            printf("File tidak ditemukan. Pastikan file tersebut ada di direktori dan coba lagi.\n\n");
				        }
				        system("pause");
				    }
	
	
					// Get the file extension
				    ext = strrchr(input_filename, '.');
				
				    if (ext != NULL) {
				        if (strcmp(ext, ".bmp") == 0) {
				            char* pesan;
	                		decodeLSB(input_filename, &pesan, &benar);
				        } else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
				            char src_image[100];
					        printf("\n------------------------------------------\n");
	                        printf("|               Decoding...              |\n");  
	                        printf("------------------------------------------\n");    							
					        decode_jpeg(input_filename, &benar);
				        } else {
				            printf("Format file tidak dikenali. Harap masukkan file dengan format .bmp atau .jpg\n");
				        }
				    } else {
				        printf("Tidak ada ekstensi file. Harap masukkan file dengan format .bmp atau .jpg\n");
				    }
				}while (benar);
                
                system("pause");
                system("cls");
                
                do {
                	printf("-------------------------------------------------------------\n");
                	printf("                    Dekripsi Pesan (RSA)                     \n");
                    printf("-------------------------------------------------------------\n\n");
            		printf("Masukkan nama file input yang akan didekripsi (.txt): ");
                	scanf("%s", input_filename);
                	validasi = (!is_txt_file(input_filename));
                	while ((c = getchar()) != '\n' && c != EOF);
				} while (validasi == 1);
            	file_in = openFile(input_filename);
            	
            	do {
            		printf("---------------------------------------------------------------\n");
            		printf("Masukkan nama file output untuk menyimpan hasil dekripsi (.txt): ");
                	scanf("%s", output_filename);
                	validasi = (!is_txt_file(output_filename));
                	while ((c = getchar()) != '\n' && c != EOF);
				} while (validasi == 1);
				file_out = createFile(output_filename);
				
                do {
                	printf("---------------------------------------------------------------\n");
                	printf("Masukkan nama file kunci (.txt) : ");
                	scanf("%s", key_filename);
                	validasi = (!is_txt_file(key_filename));
                	while ((c = getchar()) != '\n' && c != EOF);
				} while (validasi == 1);
				file_key = openFile(key_filename);
                
                fscanf(file_key, "Public Key (e, n): (%*d, %*lld)\nPrivate Key (d, n): (%lld, %lld)", &d, &n);

                dekripsi(d, n, file_in, file_out);

                // Menutup file setelah selesai proses dekripsi
                fclose(file_in);
                fclose(file_out);
                fclose(file_key);
                printf("---------------------------------------------------------------\n");
                printf("Hasil dekripsi berhasil disimpan ke dalam file '%s'\n", output_filename);
                printf("---------------------------------------------------------------\n");
                system("pause");
                break;
            case 3:
                exit = 1; 
                printf("\n----------------------------------------------------------------------\n");
                printf("|             Terima kasih telah menggunakan program ini.            |\n");
                printf("----------------------------------------------------------------------\n");
                break;
            default:
                printf("\nPilihan tidak valid.\n");
        }
    }

    return 0;
}
