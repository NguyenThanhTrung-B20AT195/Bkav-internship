#include <windows.h>
#include <iostream>
#include <string>

void ReadRegistryValue(HKEY hKeyRoot, LPCSTR subKey, LPCSTR valueName) {
    HKEY hKey;
    BYTE value[255];
    DWORD value_length = sizeof(value);
    DWORD type;

    if (RegOpenKeyExA(hKeyRoot, subKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegGetValueA(hKey, NULL, valueName, RRF_RT_ANY, &type, &value, &value_length) == ERROR_SUCCESS) {
            if (type == REG_DWORD) {
                DWORD dwordValue;
                memcpy(&dwordValue, value, sizeof(DWORD));
                std::cout << "Value: " << dwordValue << std::endl;
            }
            else if (type == REG_BINARY) {
                for (DWORD i = 0; i < value_length; ++i) {
                    for (int bit = 7; bit >= 0; --bit) {
                        std::cout << ((value[i] >> bit) & 1);
                    }
                    std::cout << " ";
                }
            }
            else if (type == REG_SZ) std::cout << "Value: " << (char*)value << std::endl;
        }
        else {
            std::cout << "Failed to read value.\n";
        }
        RegCloseKey(hKey);
    }
    else {
        std::cout << "Failed to open registry key.\n";
    }
}

void CreateRegistryKey(HKEY hKeyRoot, LPCSTR subKey, LPCSTR valueName, LPCSTR data) {
    HKEY hKey;
    if (RegCreateKeyExA(hKeyRoot, subKey, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        if (RegSetValueExA(hKey, valueName, 0, REG_SZ, (const BYTE*)data, strlen(data) + 1) == ERROR_SUCCESS) {
            std::cout << "Key and value created successfully.\n";
        }
        else {
            std::cout << "Failed to set value.\n";
        }
        RegCloseKey(hKey);
    }
    else {
        std::cout << "Failed to create key.\n";
    }
}

void DeleteRegistryKey(HKEY hKeyRoot, LPCSTR subKey) {
    HKEY hKey;
    if (RegCreateKeyExA(hKeyRoot, subKey, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {

    }
    else {
        std::cout << "Failed to open\n";
    }
    if (RegDeleteKeyA(hKeyRoot, subKey) == ERROR_SUCCESS) {
        std::cout << "Key deleted successfully.\n";
    }
    else {
        std::cout << "Failed to delete key.\n";
    }
}

void TurnOnAutoStart(HKEY hKeyRoot, LPCSTR subKey, LPCSTR valueName, LPCSTR data) {
    HKEY hKey;
    if (RegCreateKeyExA(hKeyRoot, subKey, 0, NULL, 0, KEY_WRITE, NULL, &hKey, NULL) == ERROR_SUCCESS) {
        if (RegSetValueExA(hKey, valueName, 0, REG_SZ, (const BYTE*)data, strlen(data) + 1) == ERROR_SUCCESS) {
            std::cout << "Turn on auto start with windows.\n";
        }
        else {
            std::cout << "Failed to set key.\n";
        }
        RegCloseKey(hKey);
    }
    else {
        std::cout << "Failed to create auto start.\n";
    }
}

void TurnOffAutoStart(HKEY hKeyRoot, LPCSTR subKey, LPCSTR valueName) {
    HKEY hKey;
    if (RegOpenKeyExA(hKeyRoot, subKey, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        if (RegDeleteValueA(hKey, valueName) == ERROR_SUCCESS) {
            std::cout << "Turn off auto start with windows.\n";
        }
        else {
            std::cout << "Failed to Turn off auto start.\n";
        }
    }
    else {
        std::cout << "Failed to delete auto start.\n";
    }
}

bool AutoStartState(HKEY hKeyRoot, LPCSTR subKey, LPCSTR valueName, LPCSTR data) {
    HKEY hKey;
    BYTE value[255];
    DWORD value_length = sizeof(value);
    DWORD type;
    bool c = true;
    std::string a = data;
    if (RegOpenKeyExA(hKeyRoot, subKey, 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        if (RegGetValueA(hKey, NULL, valueName, RRF_RT_ANY, &type, &value, &value_length) == ERROR_SUCCESS) {
            if ((char*)value != a) c = false;
        }
        else {
            c = false;
        }
        RegCloseKey(hKey);
    }
    else {
        c = false;
    }
    if(c==true) std::cout << "ON\n";
    else std::cout << "OFF\n";
    return c;
}

int main() {
    std::string subKey = "";
    std::string valueName = "";
    std::string data = "";
    HKEY AhKey = HKEY_CURRENT_USER;
    std::string AsubKey = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
    std::string AvalueName = "Auto Start Edit REG_KEY";
    std::string Adata = "D:\\VS\\Project\\Project1\\Debug\\Project1.exe";
    while (true) {
        std::cout << "\n------***------\n";
        std::cout << "1. Create key\n2. Read key value\n3. Delete key\n4. Turn on/off auto start\n0. Exit\nState of automatic start: ";
        bool c = AutoStartState(AhKey, AsubKey.c_str(), AvalueName.c_str(), Adata.c_str());
        std::cout << "---------------\nChon: ";
        int chon;
        std::cin >> chon;
        if (chon == 0) return 0;
        if (chon == 4) {
            if (c!=true) {
                TurnOnAutoStart(AhKey, AsubKey.c_str(), AvalueName.c_str(), Adata.c_str());
            }
            else {
                
                TurnOffAutoStart(AhKey, AsubKey.c_str(), AvalueName.c_str());
            }
        }
        if (chon > 0 && chon < 4) {
            std::cout << "Press any opion in here:\n";
            std::cout << "1.HKEY_CLASSES_ROOT   2.HKEY_CURRENT_USER   3.HKEY_LOCAL_MACHINE   4.HKEY_USERS   5.HKEY_CURRENT_CONFIG\nChon: ";
            int choice;
            std::cin >> choice;
            HKEY hKey = (choice == 1) ? HKEY_CLASSES_ROOT :
                (choice == 2) ? HKEY_CURRENT_USER :
                (choice == 3) ? HKEY_LOCAL_MACHINE :
                (choice == 4) ? HKEY_USERS :
                (choice == 5) ? HKEY_CURRENT_CONFIG :
                NULL;
            std::cin.ignore();
            std::cout << "Enter subKey (defaut NULL): ";
            std::getline(std::cin, subKey);
            if (chon != 3) std::cout << "Enter valueName (defaut NULL): ";
            if (chon != 3) std::getline(std::cin, valueName);
            if (chon == 1) std::cout << "Enter data (defaut NULL): ";
            if (chon == 1) std::getline(std::cin, data);
            
            char check;
            // Tạo khóa và giá trị
            if (chon == 1) CreateRegistryKey(hKey, subKey.c_str(), valueName.c_str(), data.c_str());
            // Đọc giá trị
            if (chon == 2) ReadRegistryValue(hKey, subKey.c_str(), valueName.c_str());
            // Xóa khóa
            if (chon == 3) {
                char check;
                std::string rootKey = (choice == 1) ? "HKEY_CLASSES_ROOT" :
                    (choice == 2) ? "HKEY_CURRENT_USER" :
                    (choice == 3) ? "HKEY_LOCAL_MACHINE" :
                    (choice == 4) ? "HKEY_USERS" :
                    (choice == 5) ? "HKEY_CURRENT_CONFIG" :
                    NULL;
                std::cout << "Press y to delete key *" << rootKey << '\\' << subKey << "*: ";
                std::cin >> check;
                if (check == 'y') DeleteRegistryKey(hKey, subKey.c_str());
            }
        }
    }
    return 0;
}
