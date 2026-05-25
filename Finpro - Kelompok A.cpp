#include <iostream>
#include <string>

using namespace std;

class Barang {
private:
    string idBarang;
    string namaBarang;
    int stok;

public:
    Barang(string id, string nama, int s)
        : idBarang(id), namaBarang(nama), stok(s) {}

    string getId() const {
        return idBarang;
    }

    string getNama() const {
        return namaBarang;
    }

    int getStok() const {
        return stok;
    }

    void setStok(int s) {
        stok = s;
    }
};

struct Node {
    Barang* data;
    Node* next;

    Node(Barang* b) : data(b), next(nullptr) {}
};

class GudangLinkedList {
private:
    Node* head;

public:
    GudangLinkedList() : head(nullptr) {}

    void tambahBarangAwal(string id, string nama, int stok) {
        Barang* barangBaru = new Barang(id, nama, stok);
        Node* nodeBaru = new Node(barangBaru);

        if (!head) {
            head = nodeBaru;
            return;
        }
        Node* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = nodeBaru;
    }

    bool tambahStok(string id, int jumlah) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data->getId() == id) {
                int stokBaru = temp->data->getStok() + jumlah;
                temp->data->setStok(stokBaru);
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    bool kurangiStok(string id, int jumlah) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data->getId() == id) {
                int stokSekarang = temp->data->getStok();
                if (jumlah > stokSekarang) {
                    return false;
                }
                temp->data->setStok(stokSekarang - jumlah);
                return true;
            }
            temp = temp->next;
        }
        return false;
    }

    Barang* cariBarangManual(string id) {
        Node* temp = head;
        while (temp != nullptr) {
            if (temp->data->getId() == id) {
                return temp->data;
            }
            temp = temp->next;
        }
        return nullptr;
    }

    void urutkanBarangManual() {
        if (!head || !head->next) {
            return;
        }

        bool swapped;
        Node* ptr1;
        Node* lptr = nullptr;

        do {
            swapped = false;
            ptr1 = head;

            while (ptr1->next != lptr) {
                if (ptr1->data->getNama() > ptr1->next->data->getNama()) {
                    Barang* tempData = ptr1->data;
                    ptr1->data = ptr1->next->data;
                    ptr1->next->data = tempData;
                    swapped = true;
                }
                ptr1 = ptr1->next;
            }
            lptr = ptr1;
        } while (swapped);
    }

    void cetakDaftar() {
        Node* temp = head;
        if (!temp) {
            cout << "Data barang kosong." << endl;
            return;
        }

        cout << "--------------------------------------" << endl;
        cout << "ID\tNama Barang\t\tStok" << endl;
        cout << "--------------------------------------" << endl;

        while (temp) {
            cout << temp->data->getId() << "\t"
                 << temp->data->getNama() << "\t\t"
                 << temp->data->getStok() << endl;
            temp = temp->next;
        }
        cout << "--------------------------------------" << endl;
    }
};

class Pengguna {
protected:
    string username;

public:
    Pengguna(string uname) : username(uname) {}
    virtual void tampilkanMenu() = 0;
    virtual ~Pengguna() {}
};

class Admin : public Pengguna {
public:
    Admin(string uname) : Pengguna(uname) {}

    void tampilkanMenu() override {
        cout << endl;
        cout << "=== MENU UTAMA ADMIN (" << username << ") ===" << endl;
        cout << "1. Cek Seluruh Stok Barang (Sorted)" << endl;
        cout << "2. Tambah Stok Barang" << endl;
        cout << "3. Kurangi Stok Barang" << endl;
        cout << "4. Cari Barang" << endl;
        cout << "5. Keluar Sistem" << endl;
        cout << "Pilih opsi: ";
    }
};

class UserBiasa : public Pengguna {
public:
    UserBiasa(string uname) : Pengguna(uname) {}

    void tampilkanMenu() override {
        cout << endl;
        cout << "=== MENU UTAMA USER (" << username << ") ===" << endl;
        cout << "1. Cek Seluruh Stok Barang (Sorted)" << endl;
        cout << "2. Cari Barang" << endl;
        cout << "3. Keluar Sistem" << endl;
        cout << "Pilih opsi: ";
    }
};

string serializeToJSON(string status, string pesan, string dataPayload) {
    return "{\n  \"status\": \"" + status + "\",\n  \"message\": \"" + pesan + "\",\n  \"data\": \"" + dataPayload + "\"\n}";
}

int main() {
    GudangLinkedList databaseGudang;
    databaseGudang.tambahBarangAwal("B01", "Laptop ASUS", 15);
    databaseGudang.tambahBarangAwal("B02", "Keyboard Mechanical", 22);
    databaseGudang.tambahBarangAwal("B03", "Mouse Logitech", 50);

    cout << "=== SISTEM MANAJEMEN GUDANG ===" << endl;

    const string PASSWORD_ADMIN_BENAR = "admin123";
    string username;
    int peran;
    Pengguna* userAktif = nullptr;

    while (userAktif == nullptr) {
        cout << "Masukkan Username: ";
        cin >> username;
        cout << "Pilih Role (1. Admin, 2. User): ";
        cin >> peran;

        if (peran == 1) {
            string inputPassword;
            cout << "Masukkan Password Admin: ";
            cin >> inputPassword;
            if (inputPassword == PASSWORD_ADMIN_BENAR) {
                cout << "Login Admin Berhasil." << endl;
                userAktif = new Admin(username);
            } else {
                cout << "Password Salah." << endl;
            }
        } else if (peran == 2) {
            cout << "Login User Berhasil." << endl;
            userAktif = new UserBiasa(username);
        } else {
            cout << "Role Tidak Valid." << endl;
        }
    }

    int pilihan = 0;

    while (pilihan != 5) {
        userAktif->tampilkanMenu();
        cin >> pilihan;

        if (peran == 2 && pilihan == 3) {
            break;
        }

        if (pilihan == 1) {
            databaseGudang.urutkanBarangManual();
            
            string jsonResponse = serializeToJSON("200 OK", "Sukses mengambil data", "DaftarBarangTersortir");
            cout << "\n[Network Response Data]:\n" << jsonResponse << "\n\n";
            
            cout << "Daftar barang:" << endl;
            databaseGudang.cetakDaftar();
        }
        else if (pilihan == 2 && peran == 1) {
            string id;
            int jumlah;
            cout << "Masukkan ID Barang: "; cin >> id;
            cout << "Masukkan Jumlah Tambahan Stok: "; cin >> jumlah;

            bool sukses = databaseGudang.tambahStok(id, jumlah);
            
            if (sukses) {
                string jsonResponse = serializeToJSON("200 OK", "Stok berhasil ditambahkan", id);
                cout << "\n[Network Response Data]:\n" << jsonResponse << "\n";
            } else {
                string jsonResponse = serializeToJSON("404 Not Found", "Barang tidak ditemukan", "Null");
                cout << "\n[Network Response Data]:\n" << jsonResponse << "\n";
            }
        }
        else if (pilihan == 3 && peran == 1) {
            string id;
            int jumlah;
            cout << "Masukkan ID Barang: "; cin >> id;
            cout << "Masukkan Jumlah Pengurangan Stok: "; cin >> jumlah;

            bool sukses = databaseGudang.kurangiStok(id, jumlah);
            
            if (sukses) {
                string jsonResponse = serializeToJSON("200 OK", "Stok berhasil dikurangi", id);
                cout << "\n[Network Response Data]:\n" << jsonResponse << "\n";
            } else {
                string jsonResponse = serializeToJSON("400 Bad Request", "Gagal: Barang tidak ada atau stok tidak cukup", "Null");
                cout << "\n[Network Response Data]:\n" << jsonResponse << "\n";
            }
        }
        else if ((pilihan == 4 && peran == 1) || (pilihan == 2 && peran == 2)) {
            string idCari;
            cout << "Masukkan ID Barang: "; cin >> idCari;

            Barang* hasil = databaseGudang.cariBarangManual(idCari);

            if (hasil != nullptr) {
                string payload = "ID:" + hasil->getId() + ",Nama:" + hasil->getNama() + ",Stok:" + to_string(hasil->getStok());
                string jsonResponse = serializeToJSON("200 OK", "Barang Ditemukan", payload);
                
                cout << "\n[Network Response Data]:\n" << jsonResponse << "\n\n";
                cout << "Hasil pencarian:" << endl;
                cout << "Nama Barang : " << hasil->getNama() << endl;
                cout << "Sisa Stok   : " << hasil->getStok() << endl;
            } else {
                string jsonResponse = serializeToJSON("404 Not Found", "Barang Tidak Tersedia", "Null");
                cout << "\n[Network Response Data]:\n" << jsonResponse << "\n";
            }
        }
        else {
            if (pilihan != 5) {
                cout << "Pilihan tidak valid." << endl;
            }
        }
    }

    cout << "Keluar dari sistem." << endl;
    delete userAktif;

    return 0;
}