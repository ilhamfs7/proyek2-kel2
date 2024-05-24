#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encodeLSB.h"
#include "decodeLSB.h"
#include "rsa.h"
#include "JPG.h"

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
                do {
                    printf("-------------------------------------------------------------\n");
                    printf("                       Enkripsi Pesan                        \n");
                    printf("-------------------------------------------------------------\n\n");
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
                
                printf("-------------------------------------------------------------\n");
                printf("Kunci berhasil di-generate dan disimpan di '%s':\n", key_filename);
                printf("-------------------------------------------------------------\n");
                printf("Public Key (e, n): (%lld, %lld)\n", e, n);
                printf("Private Key (d, n): (%lld, %lld)\n", d, n);
                printf("-------------------------------------------------------------\n");
                
                printf("Masukkan pesan: ");
                getchar(); // Untuk membersihkan stdin
                fgets(str, sizeof(str), stdin);
                if (str[strlen(str) - 1] == '\n') {
                    str[strlen(str) - 1] = '\0';
                }
                panjang_pesan = strlen(str);
                kolom = (panjang_pesan + 1) / baris;
                
                matriks = alokasiMatriks(baris, kolom);
                isiMatriks(matriks, baris, kolom, str);
                
                do {
                    printf("-------------------------------------------------------------\n");
                    printf("Masukkan nama file untuk menyimpan hasil enkripsi (.txt): ");
                    scanf("%s", filename);
                    validasi = (!is_txt_file(filename));
                } while (validasi == 1);
                
                file = createFile(filename);
                
                printf("------------------------------------\n"); 
                printf("Masukkan public key : ");
                scanf("%lld", &e);
                printf("------------------------------------\n");
                printf("Masukkan n: ");
                scanf("%lld", &n);

                enkripsi(matriks, e, n, kolom, file);

                fclose(file);
                printf("-------------------------------------------------------------\n");
                printf("Hasil enkripsi berhasil disimpan ke dalam file '%s'\n", filename);
                printf("-------------------------------------------------------------\n");
                getchar();
                
                system("pause");
                system("cls");
                
                printf("-------------------------------------------------------------\n");
                printf("\nMasukkan nama file gambar(.bmp / .jpg): ");
                scanf("%s", input_filename);
                getchar();
                
                // Get the file extension
                const char *ext = strrchr(input_filename, '.');
            
                if (ext != NULL) {
                    if (strcmp(ext, ".bmp") == 0) {
                        printf("----------------------------------------------------------\n");
                        printf("Masukkan nama file untuk salinan(.bmp): ");
                        scanf("%s", output_filename);
                        getchar();
                        
                        printf("----------------------------------------------------------\n");
                        printf("Masukkan nama file yang akan dibaca : ");
                        fgets(filename, sizeof(filename), stdin);
                    
                        filename[strcspn(filename, "\n")] = 0;
                    
                        char* message = (char*)malloc(1000 * sizeof(char));
                    
                        if (message == NULL) {
                            printf("-----------------------------------\n");
                            perror("|     Memory allocation failed    |\n");
                            printf("-----------------------------------\n");
                            return 1;
                        }
                    
                        readAndDeleteFile(filename, message, 1000);
        
                        encodeLSB(input_filename, output_filename, message);
                        system("pause");
                    } else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
                        char src_image[100], dest_image[100], message_file[100];
                        printf("----------------------------------------------------------\n");
                        printf("Masukkan file yang berisi pesan(.txt): ");
                        scanf("%s", message_file);
                        printf("----------------------------------------------------------\n");
                        printf("Masukkan nama salinan file yang telah disisipkan pesan(.jpg): ");
                        scanf("%s", dest_image);
                        printf("-------------------------------------\n");
                        printf("|             Encoding...           |\n");
                        printf("-------------------------------------\n");
                        encode_jpeg(input_filename, dest_image, message_file); 
                    } else {
                        printf("Format file tidak dikenali. Harap masukkan file dengan format .bmp atau .jpg\n");
                    }
                } else {
                    printf("Tidak ada ekstensi file. Harap masukkan file dengan format .bmp atau .jpg\n");
                }
                system("pause");
                break;
            case 2:
                system("cls");

                printf("-------------------------------------------------------------\n");
                printf("                       Dekripsi Pesan                        \n");
                printf("-------------------------------------------------------------\n\n");
                printf("Masukkan nama file gambar(.bmp / .jpg): ");
                scanf("%s", input_filename);

                // Get the file extension
                ext = strrchr(input_filename, '.');
            
                if (ext != NULL) {
                    if (strcmp(ext, ".bmp") == 0) {
                        char* pesan;
                        decodeLSB(input_filename, &pesan);
                    } else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
                        char src_image[100];
                        printf("------------------------------------------\n");
                        printf("|               Decoding...              |\n");  
                        printf("------------------------------------------\n");                                
                        decode_jpeg(input_filename);
                    } else {
                        printf("Format file tidak dikenali. Harap masukkan file dengan format .bmp atau .jpg\n");
                    }
                } else {
                    printf("Tidak ada ekstensi file. Harap masukkan file dengan format .bmp atau .jpg\n");
                }
                
                system("pause");
                system("cls");
                
                do {
                    printf("---------------------------------------------------------------\n");
                    printf("Masukkan nama file input yang akan didekripsi (.txt): ");
                    scanf("%s", input_filename);
                    validasi = (!is_txt_file(input_filename));
                } while (validasi == 1);
                file_in = openFile(input_filename);
                
                do {
                    printf("---------------------------------------------------------------\n");
                    printf("Masukkan nama file output untuk menyimpan hasil dekripsi (.txt): ");
                    scanf("%s", output_filename);
                    validasi = (!is_txt_file(output_filename));
                } while (validasi == 1);
                file_out = createFile(output_filename);
                
                do {
                    printf("---------------------------------------------------------------\n");
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
                printf("---------------------------------------------------------------\n");
                printf("Hasil dekripsi berhasil disimpan ke dalam file '%s'\n", output_filename);
                printf("---------------------------------------------------------------\n");
                getchar();
                system("pause");
                break;
            case 3:
                exit = 1; 
                printf("----------------------------------------------------------------------\n");
                printf("|             Terima kasih telah menggunakan program ini.            |\n");
                printf("----------------------------------------------------------------------\n");
                break;
            default:
                printf("\nPilihan tidak valid.\n");
        }
    }

    return 0;
}
