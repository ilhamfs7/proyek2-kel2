// Main
int main()
{
    long long e, d, n;
    int pil;
    char ch;
    char *pesan = NULL; // Pointer untuk menyimpan pesan dari pengguna
    int panjang_pesan = 0; // Panjang pesan yang dimasukkan pengguna
    char karakter;

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
			
			// Membaca setiap karakter hingga ditemukan newline
		    while ((karakter = getchar()) != '\n') {
		        // Mengalokasikan memori untuk setiap karakter yang dimasukkan pengguna
		        pesan = (char *)realloc(pesan, (panjang_pesan + 1) * sizeof(char));
		
		        if (pesan == NULL) {
		            printf("Alokasi memori gagal!");
		            return 1;
		        }
		
		        pesan[panjang_pesan++] = karakter; // Menyimpan karakter ke dalam array dinamis
		    }
		
		    pesan = (char *)realloc(pesan, (panjang_pesan + 1) * sizeof(char));
		    pesan[panjang_pesan] = '\0'; // Menambahkan null-terminator untuk menandai akhir dari string
		
		    printf("Pesan yang dimasukkan: %s\n", pesan);
		
		    // Mencetak array karakter
		    printf("Array karakter: ");
		    
		    int i;
		    for (i = 0; i < panjang_pesan; i++) {
		        printf("%c ", pesan[i]);
		    }
		    printf("\n");
		
			// Konversi ke ASCII dan simpan ke dalam array dinamis hasil_ascii
		    int *hasil_ascii = (int *)malloc(panjang_pesan * sizeof(int));
		    if (hasil_ascii == NULL) {
		        printf("Alokasi memori gagal!");
		        free(pesan);
		        return 1;
		    }
		
		    printf("Hasil ASCII: ");
		    for (i = 0; i < panjang_pesan; i++) {
		        hasil_ascii[i] = (int)pesan[i];
		        printf("%d ", hasil_ascii[i]);
		    }
		    printf("\n");
		
		    // Membebaskan memori yang dialokasikan untuk array pesan dan hasil_ascii
		    free(pesan);
		    free(hasil_ascii);
			
		default:
            printf("Pilihan tidak tepat.\n");
            
	}
    return 0;
}