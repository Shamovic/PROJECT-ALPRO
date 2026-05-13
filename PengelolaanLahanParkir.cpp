#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// ==========================================
// 1. CLASS KENDARAAN 
// ==========================================
class Kendaraan {
public:
    string plat;
    string jenis;
    bool terisi;

    // Constructor Default
    Kendaraan() : plat("-"), jenis("-"), terisi(false) {}
};

// ==========================================
// 2. CLASS GEDUNG PARKIR
// ==========================================
class GedungParkir {
private:
    // ARRAY MULTI-DIMENSI: 3 Lantai, 4 Slot per lantai
    Kendaraan slotParkir[3][4];
    const int MAX_LANTAI = 3;
    const int MAX_SLOT = 4;

public:
    // RECURSIVE: Mencari slot kosong pertama secara otomatis
    bool cariSlotOtomatis(int lantai, int slot, int &lFound, int &sFound) {
        // Case 1: Kalo sudah melewati lantai terakhir
        if (lantai >= MAX_LANTAI) return false;

        // Base Case 2: Kalo ditemukan slot kosong
        if (!slotParkir[lantai][slot].terisi) {
            lFound = lantai;
            sFound = slot;
            return true;
        }

        // Rekursi: Pindah ke slot berikutnya atau lantai berikutnya
        if (slot < MAX_SLOT - 1) 
            return cariSlotOtomatis(lantai, slot + 1, lFound, sFound);
        else 
            return cariSlotOtomatis(lantai + 1, 0, lFound, sFound);
    }

    // SEARCHING: Linear Search untuk mencari koordinat kendaraan
    void cariLokasiKendaraan() {
        string target;
        cout << "Masukkan Plat Nomor yang dicari: "; cin >> target;
        bool ditemukan = false;

        for (int i = 0; i < MAX_LANTAI; i++) {
            for (int j = 0; j < MAX_SLOT; j++) {
                if (slotParkir[i][j].terisi && slotParkir[i][j].plat == target) {
                    cout << ">> Ditemukan! Lantai: " << i << ", Slot: " << j << endl;
                    ditemukan = true;
                }
            }
        }
        if (!ditemukan) cout << ">> Kendaraan tidak ada di gedung ini.\n";
    }

    // INPUT: Menambahkan kendaraan ke slot hasil rekursi
    void parkirMasuk() {
        int l, s;
        if (cariSlotOtomatis(0, 0, l, s)) {
            cout << "Slot Kosong Ditemukan di [Lantai " << l << "][Slot " << s << "]\n";
            cout << "Input Plat Nomor: "; cin >> slotParkir[l][s].plat;
            cout << "Input Jenis (Mobil/Motor): "; cin >> slotParkir[l][s].jenis;
            slotParkir[l][s].terisi = true;
            cout << "Berhasil diparkir!\n";
        } else {
            cout << "Maaf, Gedung Parkir Penuh!\n";
        }
    }

    // SORTING: Bubble Sort untuk menampilkan laporan plat terurut
    void tampilkanLaporanTerurut() {
        vector<string> listPlat;
        for (int i = 0; i < MAX_LANTAI; i++) {
            for (int j = 0; j < MAX_SLOT; j++) {
                if (slotParkir[i][j].terisi) listPlat.push_back(slotParkir[i][j].plat);
            }
        }

        if (listPlat.empty()) {
            cout << "Belum ada kendaraan parkir.\n";
            return;
        }

        // Algoritmanya
        for (int i = 0; i < (int)listPlat.size() - 1; i++) {
            for (int j = 0; j < (int)listPlat.size() - i - 1; j++) {
                if (listPlat[j] > listPlat[j+1]) swap(listPlat[j], listPlat[j+1]);
            }
        }

        cout << "\n--- LAPORAN KENDARAAN (A-Z) ---\n";
        for (const string& p : listPlat) cout << "| " << p << " |\n";
    }

    // VISUALISASI GEDUNG (Melihat kondisi Array 2D)
    void cekGedung() {
        cout << "\n=== DENAH GEDUNG (X = Terisi, O = Kosong) ===\n";
        for (int i = 0; i < MAX_LANTAI; i++) {
            cout << "Lantai " << i << ": ";
            for (int j = 0; j < MAX_SLOT; j++) {
                cout << "[" << (slotParkir[i][j].terisi ? "X" : "O") << "] ";
            }
            cout << endl;
        }
    }
};

// ==========================================
// 3. MAIN FUNCTION
// ==========================================
int main() {
    GedungParkir mall;
    int menu;

    do {
        cout << "\n--- SMART PARKING SYSTEM 2026 ---\n";
        cout << "1. Parkir Masuk (Auto Scan)\n";
        cout << "2. Cari Lokasi Kendaraan\n";
        cout << "3. Laporan Plat Terurut (Sorting)\n";
        cout << "4. Lihat Denah Gedung\n";
        cout << "0. Keluar\n";
        cout << "Pilihan: "; cin >> menu;

        switch (menu) {
            case 1: mall.parkirMasuk(); break;
            case 2: mall.cariLokasiKendaraan(); break;
            case 3: mall.tampilkanLaporanTerurut(); break;
            case 4: mall.cekGedung(); break;
            case 0: cout << "Sistem Shutdown.\n"; break;
            default: cout << "Menu salah!\n";
        }
    } while (menu != 0);

    return 0;
}