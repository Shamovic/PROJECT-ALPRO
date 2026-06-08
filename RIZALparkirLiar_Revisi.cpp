#include <cstring>
#include <iostream>
#include <fstream>
#include <cctype>
using namespace std;

//  KONSTANTA & STRUCT GLOBAL

const int MAX_LANTAI = 5;
const int MAX_SLOT   = 8;

struct Kendaraan {
    char plat[20];
    char jenis[10];   
    bool terisi;
};

struct DataParkir {
    char plat[20];
    char jenis[10];
    int  lantai;
    int  slot;
};

Kendaraan gedungMobil[MAX_LANTAI][MAX_SLOT];
Kendaraan gedungMotor[MAX_LANTAI][MAX_SLOT];


//  INISIALISASI SLOT


void inisialisasiGedung(Kendaraan gedung[][MAX_SLOT]) {
    for (int i = 0; i < MAX_LANTAI; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            gedung[i][j].plat[0]  = '-';
            gedung[i][j].plat[1]  = '\0';
            gedung[i][j].jenis[0] = '-';
            gedung[i][j].jenis[1] = '\0';
            gedung[i][j].terisi   = false;
        }
    }
}

void inisialisasi() {
    inisialisasiGedung(gedungMobil);
    inisialisasiGedung(gedungMotor);
}



//  RECURSIVE 

bool cariSlotOtomatis(Kendaraan gedung[][MAX_SLOT],
                      int lantai, int slot,
                      int &lFound, int &sFound) {
    if (lantai >= MAX_LANTAI) return false;

    if (!gedung[lantai][slot].terisi) {
        lFound = lantai;
        sFound = slot;
        return true;
    }

    if (slot < MAX_SLOT - 1)
        return cariSlotOtomatis(gedung, lantai, slot + 1, lFound, sFound);
    else
        return cariSlotOtomatis(gedung, lantai + 1, 0, lFound, sFound);
}


void toUpper(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper((unsigned char)str[i]);
    }
}

// Validasi Plat Nomor
bool validasiDanNormalisasiPlat(const char *input, char *platBersih) {
    char raw[30];
    int indexRaw = 0;

    
    for (int i = 0; input[i] != '\0' && indexRaw < 29; i++) {
        if (input[i] != ' ') {
            raw[indexRaw++] = toupper((unsigned char)input[i]);
        }
    }
    raw[indexRaw] = '\0';

    int panjang = indexRaw;
    int pos = 0;

    
    char hurufDepan[5] = "";
    int jumlahHurufDepan = 0;

    while (pos < panjang &&
           isalpha((unsigned char)raw[pos]) &&
           jumlahHurufDepan < 2) {
        hurufDepan[jumlahHurufDepan++] = raw[pos++];
    }

    hurufDepan[jumlahHurufDepan] = '\0';

    if (jumlahHurufDepan < 1)
        return false;

    
    char angka[6] = "";
    int jumlahAngka = 0;

    while (pos < panjang &&
           isdigit((unsigned char)raw[pos]) &&
           jumlahAngka < 4) {
        angka[jumlahAngka++] = raw[pos++];
    }

    angka[jumlahAngka] = '\0';

    if (jumlahAngka < 1)
        return false;

    
    char hurufBelakang[5] = "";
    int jumlahHurufBelakang = 0;

    while (pos < panjang &&
           isalpha((unsigned char)raw[pos]) &&
           jumlahHurufBelakang < 3) {
        hurufBelakang[jumlahHurufBelakang++] = raw[pos++];
    }

    hurufBelakang[jumlahHurufBelakang] = '\0';

    if (jumlahHurufBelakang < 1)
        return false;

    
    if (pos != panjang)
        return false;

    sprintf(platBersih, "%s %s %s",
            hurufDepan, angka, hurufBelakang);

    return true;
}


// Cek Duplikat


bool platSudahParkir(Kendaraan gedung[][MAX_SLOT], const char *plat) {
    for (int i = 0; i < MAX_LANTAI; i++)
        for (int j = 0; j < MAX_SLOT; j++)
            if (gedung[i][j].terisi && strcmp(gedung[i][j].plat, plat) == 0)
                return true;
    return false;
}

//  INPUT: Parkir Masuk

void parkirMasuk() {
    int pilihanJenis;
    cout << "\nPilih Jenis Kendaraan:\n";
    cout << "1. Mobil\n";
    cout << "2. Motor\n";
    cout << "Pilihan (1/2): ";

    while (!(cin >> pilihanJenis) || (pilihanJenis != 1 && pilihanJenis != 2)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Input tidak valid! Masukkan 1 (Mobil) atau 2 (Motor): ";
    }

    Kendaraan (*gedungDipilih)[MAX_SLOT];
    char labelJenis[10];
    if (pilihanJenis == 1) {
        gedungDipilih = gedungMobil;
        strcpy(labelJenis, "Mobil");
    } else {
        gedungDipilih = gedungMotor;
        strcpy(labelJenis, "Motor");
    }

    int l, s;
    if (!cariSlotOtomatis(gedungDipilih, 0, 0, l, s)) {
        cout << "Maaf, Gedung " << labelJenis << " Penuh!\n";
        return;
    }
    cout << "Slot Kosong Ditemukan di Gedung " << labelJenis
         << " [Lantai " << l << "][Slot " << s << "]\n";

    char inputPlat[30];
    char platBersih[20];
    bool platValid = false;

    do {
        cout << "Input Plat Nomor (contoh: B1234ABC) : ";
        cin >> inputPlat;

        if (!validasiDanNormalisasiPlat(inputPlat, platBersih)) {
            cout << "Format plat tidak valid! Gunakan format plat nomor Indonesia:\n";
            continue;
        }

        if (platSudahParkir(gedungDipilih, platBersih)) {
            cout << "Plat " << platBersih << " sudah terparkir di gedung " << labelJenis << "!\n";
            continue;
        }

        platValid = true;
    } while (!platValid);

   
    strcpy(gedungDipilih[l][s].plat, platBersih);
    strcpy(gedungDipilih[l][s].jenis, labelJenis);
    gedungDipilih[l][s].terisi = true;

    
    ofstream file("riwayat_parkir.csv", ios::app);
    if (!file.is_open()) {
        cout << "Peringatan: Gagal menyimpan ke file riwayat!\n";
    } else {
        file << gedungDipilih[l][s].jenis << ","
             << gedungDipilih[l][s].plat  << ","
             << l << ","
             << s << "\n";
        file.close();
    }

    cout << ">> Berhasil! " << platBersih << " diparkir di Gedung "
         << labelJenis << " - Lantai " << l << " Slot " << s << "\n";
}


//  Parkir Keluar 

void parkirKeluar() {
    // Pilih gedung dulu
    int pilihanJenis;
    cout << "\nCari kendaraan di gedung:\n";
    cout << "1. Mobil\n";
    cout << "2. Motor\n";
    cout << "Pilihan (1/2): ";

    while (!(cin >> pilihanJenis) || (pilihanJenis != 1 && pilihanJenis != 2)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Input tidak valid! Masukkan 1 atau 2: ";
    }

    Kendaraan (*gedungDipilih)[MAX_SLOT];
    char labelJenis[10];
    if (pilihanJenis == 1) {
        gedungDipilih = gedungMobil;
        strcpy(labelJenis, "Mobil");
    } else {
        gedungDipilih = gedungMotor;
        strcpy(labelJenis, "Motor");
    }

    char inputPlat[30];
    char platBersih[20];
    bool platValid = false;

    do {
        cout << "Masukkan Plat Nomor kendaraan: ";
        cin >> inputPlat;

        if (!validasiDanNormalisasiPlat(inputPlat, platBersih)) {
            cout << "Format plat tidak valid! Coba lagi.\n";
        } else {
            platValid = true;
        }
    } while (!platValid);

    for (int i = 0; i < MAX_LANTAI; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            if (gedungDipilih[i][j].terisi &&
                strcmp(gedungDipilih[i][j].plat, platBersih) == 0) {

                gedungDipilih[i][j].terisi   = false;
                gedungDipilih[i][j].plat[0]  = '-';
                gedungDipilih[i][j].plat[1]  = '\0';
                gedungDipilih[i][j].jenis[0] = '-';
                gedungDipilih[i][j].jenis[1] = '\0';
                cout << ">> Kendaraan " << platBersih
                     << " keluar dari Gedung " << labelJenis
                     << " - Lantai " << i << " Slot " << j << "\n";
                return;
            }
        }
    }
    cout << ">> Kendaraan " << platBersih
         << " tidak ditemukan di Gedung " << labelJenis << ".\n";
}


//  SEARCHING

void cariLokasiKendaraan() {
    char inputPlat[30];
    char platBersih[20];
    bool platValid = false;

    do {
        cout << "Masukkan Plat Nomor yang dicari: ";
        cin >> inputPlat;

        if (!validasiDanNormalisasiPlat(inputPlat, platBersih)) {
            cout << "Format plat tidak valid! Coba lagi.\n";
        } else {
            platValid = true;
        }
    } while (!platValid);

    bool ditemukan = false;

    // Cari di Gedung Mobil
    for (int i = 0; i < MAX_LANTAI; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            if (gedungMobil[i][j].terisi &&
                strcmp(gedungMobil[i][j].plat, platBersih) == 0) {
                cout << ">> Ditemukan! Gedung Mobil - Lantai: " << i
                     << ", Slot: " << j << "\n";
                ditemukan = true;
            }
        }
    }

    // Cari di Gedung Motor
    for (int i = 0; i < MAX_LANTAI; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            if (gedungMotor[i][j].terisi &&
                strcmp(gedungMotor[i][j].plat, platBersih) == 0) {
                cout << ">> Ditemukan! Gedung Motor - Lantai: " << i
                     << ", Slot: " << j << "\n";
                ditemukan = true;
            }
        }
    }

    if (!ditemukan)
        cout << ">> Kendaraan " << platBersih << " tidak ada di gedung manapun.\n";
}


//  Bubble Sort

void bubbleSortDataParkir(DataParkir arr[], int total) {
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            bool perluTukar = false;

            if (arr[j].lantai > arr[j+1].lantai) {
                perluTukar = true;
            } else if (arr[j].lantai == arr[j+1].lantai) {
                if (arr[j].slot > arr[j+1].slot) {
                    perluTukar = true;
                } else if (arr[j].slot == arr[j+1].slot) {
                    if (strcmp(arr[j].plat, arr[j+1].plat) > 0) {
                        perluTukar = true;
                    }
                }
            }

            if (perluTukar) {
                DataParkir tmp = arr[j];
                arr[j]         = arr[j+1];
                arr[j+1]       = tmp;
            }
        }
    }
}

void tampilkanLaporanTerurut() {
    DataParkir listMobil[MAX_LANTAI * MAX_SLOT];
    DataParkir listMotor[MAX_LANTAI * MAX_SLOT];
    int totalMobil = 0, totalMotor = 0;

    // Kumpulkan data Mobil
    for (int i = 0; i < MAX_LANTAI; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            if (gedungMobil[i][j].terisi) {
                strcpy(listMobil[totalMobil].plat,  gedungMobil[i][j].plat);
                strcpy(listMobil[totalMobil].jenis, gedungMobil[i][j].jenis);
                listMobil[totalMobil].lantai = i;
                listMobil[totalMobil].slot   = j;
                totalMobil++;
            }
        }
    }

    // Kumpulkan data Motor
    for (int i = 0; i < MAX_LANTAI; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            if (gedungMotor[i][j].terisi) {
                strcpy(listMotor[totalMotor].plat,  gedungMotor[i][j].plat);
                strcpy(listMotor[totalMotor].jenis, gedungMotor[i][j].jenis);
                listMotor[totalMotor].lantai = i;
                listMotor[totalMotor].slot   = j;
                totalMotor++;
            }
        }
    }

    if (totalMobil == 0 && totalMotor == 0) {
        cout << "Belum ada kendaraan yang parkir.\n";
        return;
    }

    // Sort kedua daftar
    if (totalMobil > 0) bubbleSortDataParkir(listMobil, totalMobil);
    if (totalMotor > 0) bubbleSortDataParkir(listMotor, totalMotor);

   cout << "\n--- LAPORAN GEDUNG MOBIL ---\n";

if (totalMobil == 0) {
    cout << "Belum ada mobil yang terparkir.\n";
} else {
    for (int i = 0; i < totalMobil; i++) {
        cout << i + 1 << ". Plat   : " << listMobil[i].plat << endl;
        cout << "   Lantai : " << listMobil[i].lantai << endl;
        cout << "   Slot   : " << listMobil[i].slot << endl;
        cout << endl;
    }
}

cout << "\n--- LAPORAN GEDUNG MOTOR ---\n";

if (totalMotor == 0) {
    cout << "Belum ada motor yang terparkir.\n";
} else {
    for (int i = 0; i < totalMotor; i++) {
        cout << i + 1 << ". Plat   : " << listMotor[i].plat << endl;
        cout << "   Lantai : " << listMotor[i].lantai << endl;
        cout << "   Slot   : " << listMotor[i].slot << endl;
        cout << endl;
    }
}
}



//  VISUALISASI Denah

void cekGedung() {
    cout << "\n=== DENAH GEDUNG MOBIL (X = Terisi, O = Kosong) ===\n";
    for (int i = 0; i < MAX_LANTAI; i++) {
        cout << "Lantai " << i << ": ";
        for (int j = 0; j < MAX_SLOT; j++)
            cout << "[" << (gedungMobil[i][j].terisi ? "X" : "O") << "] ";
        cout << "\n";
    }

    cout << "\n=== DENAH GEDUNG MOTOR (X = Terisi, O = Kosong) ===\n";
    for (int i = 0; i < MAX_LANTAI; i++) {
        cout << "Lantai " << i << ": ";
        for (int j = 0; j < MAX_SLOT; j++)
            cout << "[" << (gedungMotor[i][j].terisi ? "X" : "O") << "] ";
        cout << "\n";
    }
}


//  Riwayat Parkir 

void riwayat() {
    ifstream file("riwayat_parkir.csv");

    if (!file.is_open()) {
        cout << "File riwayat tidak ditemukan.\n";
        return;
    }

    // Cek apakah file kosong
    file.seekg(0, ios::end);
    if (file.tellg() == 0) {
        cout << "Belum ada riwayat parkir.\n";
        file.close();
        return;
    }
    file.seekg(0, ios::beg);

    string jenis, plat, lantai, slot;
    int nomorUrut = 1;

    cout << "\n=== RIWAYAT PARKIR ===\n\n";

    while (getline(file, jenis, ',') &&
           getline(file, plat,  ',') &&
           getline(file, lantai,',') &&
           getline(file, slot)) {

        // Abaikan baris kosong
        if (jenis.empty() && plat.empty()) continue;

        cout << "No     : " << nomorUrut++ << "\n";
        cout << "Jenis  : " << jenis   << "\n";
        cout << "Plat   : " << plat    << "\n";
        cout << "Lantai : " << lantai  << "\n";
        cout << "Slot   : " << slot    << "\n";
        cout << "----------------------\n";
    }

    file.close();
}


int main() {
    inisialisasi();
    int menu;

    do {
        
        cout << "=== SMART PARKING SYSTEM 2026 ===\n";
        cout << "1. Parkir Masuk\n";
        cout << "2. Parkir Keluar\n";
        cout << "3. Cari Lokasi Kendaraan\n";
        cout << "4. Laporan Terurut\n";
        cout << "5. Lihat Denah Kedua Gedung\n";
        cout << "6. Riwayat Parkir\n";
        cout << "0. Keluar\n";
        cout << "Pilihan: ";

        if (!(cin >> menu)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Input tidak valid! Masukkan angka 0-6.\n";
            menu = -1; 
            continue;
        }

        switch (menu) {
            case 1: parkirMasuk();             break;
            case 2: parkirKeluar();            break;
            case 3: cariLokasiKendaraan();     break;
            case 4: tampilkanLaporanTerurut(); break;
            case 5: cekGedung();               break;
            case 6: riwayat();                 break;
            case 0: cout << "Sistem Shutdown. Sampai jumpa!\n"; break;
            default: cout << "Menu tidak ada! Pilih angka 0-6.\n"; break;
        }

    } while (menu != 0);

    return 0;
}