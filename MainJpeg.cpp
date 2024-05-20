#include <iostream>

#include "jpeg.h"
#include <stdio.h>

int main() {
    printf("--LSB Enkripsi-\n");    
    printf("1: Enkrip code\n");
    printf("2: Dekrip code\n");
    printf("masukan pilihan : ");
    
    char choice;
    scanf("%c", &choice);   // Membaca pilihan dari pengguna

    if (choice == '1') {   										
        char src_image[100], dest_image[100], message[1000]; 	
        printf("Masukkan gambar: ");   					 
        scanf("%s", src_image);									 
        printf("Masukkan pesan yang ingin di sembunyikan : ");
        getchar(); 												
        fgets(message, sizeof(message), stdin);   				
        message[strcspn(message, "\n")] = '\0'; 				
        printf("Masukkan nama file yang telah di sisipkan pesan : ");   			
        scanf("%s", dest_image);    							
        printf("Encoding...\n");    							
        encode(src_image, dest_image, message);    				
    } else if (choice == '2') {    								
        char src_image[100];    								
        printf("Masukkan gambar : ");    				
        scanf("%s", src_image); 								
        printf("Decoding...\n");    							
        decode(src_image);  									
    } else {
        printf("ERROR: Invalid option chosen\n"); 				
    }

    return 0;   
}
