#include <iostream>
#include "jpeg.h"
#include <stdio.h>

int main() {
    printf("--LSB Enkripsi jpeg--\n");    // Menampilkan judul program
    printf("1: Enkripsi Pesan \n");
    printf("2: Dekripsi Pesan \n");
    printf("Masukkan pilihan: ");
    
    char choice;
    scanf("%c", &choice);   
    if (choice == '1') {   										 
        char src_image[100], dest_image[100], message[1000]; 	
        printf("Masukkan gambar: ");   					
        scanf("%s", src_image);									
        printf("Masukkan pesan yang ingin disembunyikan: ");
        getchar(); 												
        fgets(message, sizeof(message), stdin);   				
        message[strcspn(message, "\n")] = '\0'; 				
        printf("Masukkan nama file yang telah disisipkan pesan: ");   			
        scanf("%s", dest_image);    							
        printf("Encoding...\n");    							
        encode_jpeg(src_image, dest_image, message);    				
    } else if (choice == '2') {    								
        char src_image[100];    								
        printf("Masukkan gambar: ");    			
        scanf("%s", src_image); 								
        printf("Decoding...\n");    							
        decode_jpeg(src_image);  									
    } else {
        printf("ERROR: Pilihan Invalid \n"); 				
    }

    return 0;   
}
