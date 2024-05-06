#include "jpeg.h"
#include <stdlib.h>
#include <string.h>

// Fungsi untuk membuka dan membaca file gambar
void open_image(const char *filename, ImageData *image) {
    // Implementasi fungsi open_image
     FILE *file = fopen(filename, "rb");    					// Membuka file dalam mode baca binary
    if (!file) {
        printf("ERROR: Cannot open source image\n");  		// Menampilkan pesan error jika file tidak dapat dibuka
        exit(EXIT_FAILURE); 								// Menghentikan program dengan status error
    }

    fseek(file, 0, SEEK_END);   							// Pindahkan pointer file ke akhir file
    image->buffer_size = ftell(file);  						// Dapatkan ukuran file
    fseek(file, 0, SEEK_SET);   							// Kembalikan pointer file ke awal file

    image->buffer = (uint8_t *)malloc(image->buffer_size); 	// Alokasi memori untuk buffer gambar
    if (!image->buffer) {
        printf("ERROR: Memory allocation failed\n");   		// Menampilkan pesan error jika alokasi memori gagal
        fclose(file);   									// Menutup file
        exit(EXIT_FAILURE); 								// Menghentikan program dengan status error
    }

    fread(image->buffer, 1, image->buffer_size, file);   	// Membaca isi file ke dalam buffer
    fclose(file);   										// Menutup file
}



// Fungsi untuk menutup buffer gambar
void close_image(ImageData *image) {
    // Implementasi fungsi close_image
 	free(image->buffer);    								// Membebaskan memori yang dialokasikan untuk buffer gambar
}

// Fungsi untuk menyisipkan pesan ke dalam gambar
void encode(const char *source_image, const char *dest_image, const char *message) {
    // Implementasi fungsi encode
    
   ImageData image;
    open_image(source_image, &image);  						// Membuka file gambar sumber

    FILE *outfile = fopen(dest_image, "wb"); 				// Membuka file tujuan dalam mode tulis binary
    if (!outfile) {
        printf("ERROR: Cannot open destination file\n");  	// Menampilkan pesan error jika file tujuan tidak dapat dibuka
        close_image(&image);    							// Menutup buffer gambar
        exit(EXIT_FAILURE); 								// Menghentikan program dengan status error
    }

    uint8_t *buffer = image.buffer;    						// Mendapatkan pointer ke buffer gambar
    size_t buffer_size = image.buffer_size;   				// Mendapatkan ukuran buffer gambar

    // Menambahkan terminator pada pesan
    size_t message_length = strlen(message); 				// Mendapatkan panjang pesan
    size_t total_message_length = message_length ; 			// Total panjang pesan setelah ditambah terminator

    // Memeriksa apakah pesan dapat disisipkan dalam gambar
    if (total_message_length * 8 > buffer_size) {
        printf("ERROR: Need larger file size\n"); 			// Menampilkan pesan error jika gambar tidak cukup besar
        close_image(&image);    							// Menutup buffer gambar
        fclose(outfile);    								// Menutup file tujuan
        exit(EXIT_FAILURE); 								// Menghentikan program dengan status error
    }

    // Menyalin semua data dari gambar ke gambar yang akan disimpan
    fwrite(buffer, 1, buffer_size, outfile); 				// Menyalin data gambar ke file tujuan

    // Menambahkan marker COM (Comment) untuk menyimpan panjang pesan
    uint16_t message_length_be = total_message_length;    	// Panjang pesan dalam big-endian
    fwrite("\xFF\xFE", 1, 2, outfile); 						// Menulis marker COM
    fwrite(&message_length_be, sizeof(uint16_t), 1, outfile); // Menulis panjang pesan

    // Menambahkan pesan
    fwrite(message, 1, message_length, outfile); 			// Menulis pesan
    fwrite("$t3g0", 1, strlen("$t3g0"), outfile);		    // Mengganti MESSAGE_TERMINATOR

    fclose(outfile);    									// Menutup file tujuan
    close_image(&image);    								// Menutup buffer gambar

    printf("Berhasil enkripsi pesan \n"); 					// Menampilkan pesan keberhasilan
}

// Fungsi untuk mengekstrak pesan dari gambar
void decode(const char *image_path) {
    // Implementasi fungsi decode
     ImageData image;
    open_image(image_path, &image); 						 // Membuka file gambar

    uint8_t *buffer = image.buffer;    						// Mendapatkan pointer ke buffer gambar
    size_t buffer_size = image.buffer_size;   				// Mendapatkan ukuran buffer gambar

    // Mencari panjang pesan dalam komentar
    size_t pos = 0; // Posisi awal pencarian
    while (pos < buffer_size - 4) {   						 // Loop sampai ke 4 byte terakhir dari buffer
        if (buffer[pos] == 0xFF && buffer[pos + 1] == 0xFE) { // Jika ditemukan marker COM
            uint16_t message_length_be;    						// Panjang pesan dalam big-endian
            memcpy(&message_length_be, &buffer[pos + 2], sizeof(uint16_t));    // Mendapatkan panjang pesan

            size_t message_length = message_length_be;		 // Mengkonversi panjang pesan ke ukuran yang sesuai

            char *message = (char *)malloc(message_length + 1); // Alokasi memori untuk pesan
            if (!message) {
                printf("ERROR: Memory allocation failed\n");   // Menampilkan pesan error jika alokasi memori gagal
                close_image(&image);    						// Menutup buffer gambar
                exit(EXIT_FAILURE); 							// Menghentikan program dengan status error
            }

            memcpy(message, &buffer[pos + 4], message_length); 	// Mendapatkan pesan dari buffer gambar
            message[message_length] = '\0'; 					// Menambahkan terminator pada pesan

            printf("Hidden Message: %s\n", message);   			// Menampilkan pesan tersembunyi

            free(message);  									// Membebaskan memori pesan
            break;  											// Keluar dari loop
        }
        pos++;  												// Pindahkan ke posisi berikutnya
    }

    if (pos >= buffer_size - 4) { 								 // Jika tidak ada pesan yang ditemukan
        printf("No Hidden Message Found\n");   					 // Menampilkan pesan bahwa tidak ada pesan yang ditemukan
    }

    close_image(&image);    // Menutup buffer gambar

}
