#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;


// KONSTANTA & STRUCT GLOBAL

const int MAX_LANTAI = 3;
const int MAX_SLOT   = 4;

struct Kendaraan {
    char plat[20];
    char jenis[10];
    bool terisi;
};

Kendaraan slotParkir[MAX_LANTAI][MAX_SLOT];


// INISIALISASI SLOT

void inisialisasi() {
    for (int i = 0; i < MAX_LANTAI; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            slotParkir[i][j].plat[0]  = '-';
            slotParkir[i][j].plat[1]  = '\0';
            slotParkir[i][j].jenis[0] = '-';
            slotParkir[i][j].jenis[1] = '\0';
            slotParkir[i][j].terisi   = false;
        }
    }
}


// RECURSIVE: Cari slot kosong pertama

bool cariSlotOtomatis(int lantai, int slot, int &lFound, int &sFound) {
    if (lantai >= MAX_LANTAI) return false;

    if (!slotParkir[lantai][slot].terisi) {
        lFound = lantai;
        sFound = slot;
        return true;
    }

    if (slot < MAX_SLOT - 1)
        return cariSlotOtomatis(lantai, slot + 1, lFound, sFound);
    else
        return cariSlotOtomatis(lantai + 1, 0, lFound, sFound);
}


// INPUT: Parkir masuk (pakai rekursi)

void parkirMasuk() {
    int l, s;
    if (cariSlotOtomatis(0, 0, l, s)) {
        cout << "Slot Kosong Ditemukan di [Lantai " << l << "][Slot " << s << "]\n";
        cout << "Input Plat Nomor : "; cin >> slotParkir[l][s].plat;
        cout << "Input Jenis (Mobil/Motor): "; cin >> slotParkir[l][s].jenis;
        slotParkir[l][s].terisi = true;
        ofstream file("riwayat_parkir.csv", ios::app);
        file << slotParkir[l][s].jenis << ","
             << slotParkir[l][s].plat << ","
             << l << ","
             << s << "\n";
        file.close();
        cout << "Berhasil diparkir!\n";
    } else {
        cout << "Maaf, Gedung Parkir Penuh!\n";
    }
}


// Parkir Keluar (hapus data)
void parkirKeluar() {
    char target[20];

    cout << "Masukkan Plat Nomor kendaraan: "; 
    cin >> target;
    
    for (int i = 0; i < MAX_LANTAI; i++) {
        for (int j = 0; j < MAX_SLOT; j++) { 
            if (slotParkir[i][j].terisi && strcmp(slotParkir[i][j].plat, target)==0) {
                slotParkir[i][j].terisi   = false;
                slotParkir[i][j].plat[0]  = '-';
                slotParkir[i][j].plat[1]  = '\0';
                slotParkir[i][j].jenis[0] = '-';
                slotParkir[i][j].jenis[1] = '\0';
                cout << ">> Kendaraan keluar dari Lantai " << i << " Slot " << j << "\n";
                return;
            }
        }
    }            
    cout << "Kendaraan tidak ditemukan.\n";
}



// SEARCHING: Linear Search cari kendaraan

void cariLokasiKendaraan() {
    char target[20];
    cout << "Masukkan Plat Nomor yang dicari: "; cin >> target;
    bool ditemukan = false;

    for (int i = 0; i < MAX_LANTAI; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            // Bandingkan char array manual
            bool sama = true;
            for (int k = 0; target[k] != '\0' || slotParkir[i][j].plat[k] != '\0'; k++) {
                if (target[k] != slotParkir[i][j].plat[k]) { sama = false; break; }
            }
            if (slotParkir[i][j].terisi && sama) {
                cout << ">> Ditemukan! Lantai: " << i << ", Slot: " << j << "\n";
                ditemukan = true;
            }
        }
    }
    if (!ditemukan) cout << ">> Kendaraan tidak ada di gedung ini.\n";
}


// SORTING: Bubble Sort plat nomor (A-Z)

void tampilkanLaporanTerurut() {
    // Kumpulkan plat yang terisi ke array sementara
    char listPlat[MAX_LANTAI * MAX_SLOT][20];
    int total = 0;

    for (int i = 0; i < MAX_LANTAI; i++)
        for (int j = 0; j < MAX_SLOT; j++)
            if (slotParkir[i][j].terisi) {
                // Salin manual
                int k = 0;
                while ((listPlat[total][k] = slotParkir[i][j].plat[k]) != '\0') k++;
                total++;
            }

    if (total == 0) { cout << "Belum ada kendaraan parkir.\n"; return; }

    // Bubble Sort (bandingkan char per char)
    for (int i = 0; i < total - 1; i++) {
        for (int j = 0; j < total - i - 1; j++) {
            // Cek apakah listPlat[j] > listPlat[j+1]
            bool lebihBesar = false;
            for (int k = 0; ; k++) {
                if (listPlat[j][k] == '\0' && listPlat[j+1][k] == '\0') break;
                if (listPlat[j][k] > listPlat[j+1][k]) { lebihBesar = true;  break; }
                if (listPlat[j][k] < listPlat[j+1][k]) { lebihBesar = false; break; }
            }
            if (lebihBesar) {
                char tmp[20];
                int k = 0;
                while ((tmp[k] = listPlat[j][k]) != '\0') k++;
                tmp[k] = '\0';
                k = 0;
                while ((listPlat[j][k] = listPlat[j+1][k]) != '\0') k++;
                listPlat[j][k] = '\0';
                k = 0;
                while ((listPlat[j+1][k] = tmp[k]) != '\0') k++;
                listPlat[j+1][k] = '\0';
            }
        }
    }

    cout << "\n--- LAPORAN KENDARAAN (A-Z) ---\n";
    for (int i = 0; i < total; i++) cout << "| " << listPlat[i] << " |\n";
}


// VISUALISASI: Denah gedung array 2D

void cekGedung() {
    cout << "\n=== DENAH GEDUNG (X = Terisi, O = Kosong) ===\n";
    for (int i = 0; i < MAX_LANTAI; i++) {
        cout << "Lantai " << i << ": ";
        for (int j = 0; j < MAX_SLOT; j++)
            cout << "[" << (slotParkir[i][j].terisi ? "X" : "O") << "] ";
        cout << "\n";
    }
}

// Riwayat
void riwayat() {
    system("cls");
    ifstream file("riwayat_parkir.csv");

     file.seekg(0, ios::end);
    if (file.tellg() == 0) {
      cout << "Belum ada riwayat parkir.\n";
      system("pause");
      file.close();
    return;
    }

file.seekg(0, ios::beg);

    string jenis, plat, lantai, slot;

    cout << "\n=== RIWAYAT PARKIR ===\n\n";

    while (getline(file, jenis, ',') &&
           getline(file, plat, ',') &&
           getline(file, lantai, ',') &&
           getline(file, slot)) {

        cout << "Jenis  : " << jenis   << endl;
        cout << "Plat   : " << plat    << endl;
        cout << "Lantai : " << lantai  << endl;
        cout << "Slot   : " << slot    << endl;
        cout << "----------------------\n";
    }
    system("pause");
    system("cls");
}

// MAIN

int main() {
    inisialisasi();
    int menu;

    do {
        cout << "\n--- SMART PARKING SYSTEM 2026 ---\n";
        cout << "1. Parkir Masuk (Auto Scan)\n";
        cout << "2. Parkir Keluar\n";
        cout << "3. Cari Lokasi Kendaraan\n";
        cout << "4. Laporan Plat Terurut (Sorting)\n";
        cout << "5. Lihat Denah Gedung\n";
        cout << "6. Riwayat Parkir\n";
        cout << "0. Keluar\n";
        cout << "Pilihan: "; cin >> menu;

        switch (menu) {
            case 1: parkirMasuk();             break;
            case 2: parkirKeluar();            break;
            case 3: cariLokasiKendaraan();     break;
            case 4: tampilkanLaporanTerurut(); break;
            case 5: cekGedung();               break;
            case 6: riwayat();                 break;
            case 0: cout << "Sistem Shutdown.\n"; break;
            default: cout << 
            "Menu salah!\n";
        }
    } while (menu != 0);

    return 0;
}