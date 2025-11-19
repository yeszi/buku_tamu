#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char *NAMA_FILE = "daftar.csv";

// --- Deklarasi Fungsi ---
void tampilkanMenu();
void tambahTamu();
void cariBerdasarkanNama();
void cariBerdasarkanTanggal();
void cariBerdasarkanBulan();
void bersihkanInputBuffer();
void toLowerCase(char *str);

// --- Mengubah string ke lowercase (untuk pencarian nama) ---
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z')
            str[i] = str[i] + 32;
    }
}

// --- Fungsi Utama ---
int main() {
    int pilihan;

    do {
        tampilkanMenu();

        if (scanf("%d", &pilihan) != 1) {
            printf("\nInput tidak valid! Harap masukkan angka.\n");
            bersihkanInputBuffer();
            pilihan = 0;
        } else {
            bersihkanInputBuffer();
        }

        printf("\n");

        switch (pilihan) {
            case 1: tambahTamu(); break;
            case 2: cariBerdasarkanNama(); break;
            case 3: cariBerdasarkanTanggal(); break;
            case 4: cariBerdasarkanBulan(); break;
            case 5: printf("Terima kasih, program ditutup.\n"); break;
            default:
                if (pilihan != 0)
                    printf("Pilihan tidak valid! Silakan coba lagi.\n");
        }

        if (pilihan != 5) {
            printf("\nTekan Enter untuk kembali ke menu...");
            getchar();
        }

    } while (pilihan != 5);

    return 0;
}

// --- Implementasi Fungsi ---

void tampilkanMenu() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    printf("=======================================\n");
    printf("  SISTEM BUKU TAMU DIGITAL (CSV READY)\n");
    printf("         KANTOR KELURAHAN X\n");
    printf("=======================================\n");
    printf("1. Tambah Data Pengunjung\n");
    printf("2. Cari Pengunjung (Nama)\n");
    printf("3. Laporan (Tanggal)\n");
    printf("4. Laporan (Bulan)\n");
    printf("5. Keluar\n");
    printf("---------------------------------------\n");
    printf("Masukkan pilihan Anda (1-5): ");
}

void bersihkanInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// ----------------------
// 1. Tambah Data Tamu (SUDAH DITAMBAH WAKTU EKSEKUSI)
// ----------------------
void tambahTamu() {

    clock_t start, end;
    start = clock();   // mulai hitung waktu

    char nik[30], nama[100], tujuan[255];
    char tanggal_str[20], jam_str[10];

    printf("--- Tambah Data Pengunjung ---\n");

    printf("Masukkan NIK        : ");
    fgets(nik, sizeof(nik), stdin);
    nik[strcspn(nik, "\n")] = 0;

    printf("Masukkan Nama       : ");
    fgets(nama, sizeof(nama), stdin);
    nama[strcspn(nama, "\n")] = 0;

    printf("Masukkan Tujuan     : ");
    fgets(tujuan, sizeof(tujuan), stdin);
    tujuan[strcspn(tujuan, "\n")] = 0;

    // Ambil waktu sistem
    time_t raw;
    struct tm *waktu;
    time(&raw);
    waktu = localtime(&raw);

    strftime(tanggal_str, 20, "%Y-%m-%d", waktu);
    strftime(jam_str, 10, "%H:%M:%S", waktu);

    // Simpan ke CSV
    FILE *fp = fopen(NAMA_FILE, "a");
    if (!fp) {
        printf("Error membuka file CSV!\n");
        return;
    }

    fprintf(fp, "%s;%s;%s;%s;%s\n",
            tanggal_str, jam_str, nik, nama, tujuan);

    fclose(fp);

    // Output real-time ke terminal
    printf("\nDATA TERSIMPAN (REAL-TIME CSV):\n");
    printf("%s;%s;%s;%s;%s\n",
           tanggal_str, jam_str, nik, nama, tujuan);

    printf("\nSUKSES: Data pengunjung '%s' berhasil disimpan ke CSV.\n", nama);

    // HITUNG WAKTU EKSEKUSI
    end = clock();
    double waktu_eksekusi = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Waktu eksekusi proses: %.6f detik\n", waktu_eksekusi);
}

// ----------------------
// 2. Cari Berdasarkan Nama
// ----------------------
void cariBerdasarkanNama() {
    char kata[100];
    char buffer[1024];

    printf("--- Cari Pengunjung Berdasarkan Nama ---\n");
    printf("Masukkan nama / sebagian nama: ");
    fgets(kata, sizeof(kata), stdin);
    kata[strcspn(kata, "\n")] = 0;

    toLowerCase(kata);

    FILE *fp = fopen(NAMA_FILE, "r");
    if (!fp) {
        printf("File CSV belum ada.\n");
        return;
    }

    int ditemukan = 0;
    printf("\nHasil Pencarian:\n");
    printf("Tanggal | Jam | NIK | Nama | Tujuan\n");

    while (fgets(buffer, sizeof(buffer), fp)) {
        char temp[1024];
        strcpy(temp, buffer);

        char *tgl = strtok(temp, ";");
        char *jam = strtok(NULL, ";");
        char *nik = strtok(NULL, ";");
        char *nama = strtok(NULL, ";");
        char *tujuan = strtok(NULL, ";");

        if (!nama) continue;

        nama[strcspn(nama, "\n")] = 0;

        char namaLower[200];
        strcpy(namaLower, nama);
        toLowerCase(namaLower);

        if (strstr(namaLower, kata)) {
            printf("%s | %s | %s | %s | %s\n",
                   tgl, jam, nik, nama, tujuan);
            ditemukan++;
        }
    }

    if (ditemukan == 0)
        printf("\nTidak ditemukan.\n");
    else
        printf("\nTotal ditemukan: %d data.\n", ditemukan);

    fclose(fp);
}

// ----------------------
// 3. Cari Berdasarkan Tanggal
// ----------------------
void cariBerdasarkanTanggal() {
    char tglCari[20], buffer[1024];

    printf("--- Laporan Tanggal ---\n");
    printf("Masukkan tanggal (YYYY-MM-DD): ");
    fgets(tglCari, sizeof(tglCari), stdin);
    tglCari[strcspn(tglCari, "\n")] = 0;

    FILE *fp = fopen(NAMA_FILE, "r");
    if (!fp) {
        printf("File CSV belum ada.\n");
        return;
    }

    int ditemukan = 0;
    printf("\nHasil Laporan:\n");

    while (fgets(buffer, sizeof(buffer), fp)) {
        char temp[1024];
        strcpy(temp, buffer);

        char *tgl = strtok(temp, ";");

        if (tgl && strcmp(tgl, tglCari) == 0) {
            char *jam = strtok(NULL, ";");
            char *nik = strtok(NULL, ";");
            char *nama = strtok(NULL, ";");
            char *tujuan = strtok(NULL, ";");

            printf("%s | %s | %s | %s | %s\n", tgl, jam, nik, nama, tujuan);
            ditemukan++;
        }
    }

    if (!ditemukan)
        printf("\nTidak ada data.\n");
    else
        printf("\nTotal ditemukan: %d data.\n", ditemukan);

    fclose(fp);
}

// ----------------------
// 4. Cari Berdasarkan Bulan
// ----------------------
void cariBerdasarkanBulan() {
    char bulan[10];
    char buffer[1024];

    printf("--- Laporan Bulanan ---\n");
    printf("Masukkan bulan (YYYY-MM): ");
    fgets(bulan, sizeof(bulan), stdin);
    bulan[strcspn(bulan, "\n")] = 0;

    if (strlen(bulan) != 7 || bulan[4] != '-') {
        printf("Format salah! Gunakan YYYY-MM (contoh: 2025-11)\n");
        return;
    }

    FILE *fp = fopen(NAMA_FILE, "r");
    if (!fp) {
        printf("File CSV belum ada.\n");
        return;
    }

    int ditemukan = 0;

    printf("\nHasil Laporan Bulanan:\n");

    while (fgets(buffer, sizeof(buffer), fp)) {
        char temp[1024];
        strcpy(temp, buffer);

        char *tgl = strtok(temp, ";");

        if (tgl && strncmp(tgl, bulan, 7) == 0) {
            char *jam = strtok(NULL, ";");
            char *nik = strtok(NULL, ";");
            char *nama = strtok(NULL, ";");
            char *tujuan = strtok(NULL, ";");

            printf("%s | %s | %s | %s | %s\n", tgl, jam, nik, nama, tujuan);
            ditemukan++;
        }
    }

    if (!ditemukan)
        printf("\nTidak ada data.\n");
    else
        printf("\nTotal ditemukan: %d data.\n", ditemukan);

    fclose(fp);
}
