#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

// Function to perform SCF verification
void scf_scan_now() {
    system("cls");
    cout << "Running SCF verification...\n";
    system("sfc /scannow");
    cout << "Verification completed and logged.\n";
}

// Function to perform a virus scan
void virus_scan() {
    system("cls");
    string file_path;
    cout << "Enter the file path to scan: ";
    cin >> file_path;

    ifstream sig_file("content\\signature.txt");
    if (!sig_file) {
        cout << "Error: Signature file not found!\n";
        return;
    }

    vector<string> signatures;
    string signature;
    while (getline(sig_file, signature)) {
        signatures.push_back(signature);
    }
    sig_file.close();

    ifstream scan_file(file_path);
    if (!scan_file) {
        cout << "Error: Unable to open file!\n";
        return;
    }

    string file_content;
    while (getline(scan_file, file_content)) {
        for (const auto& sig : signatures) {
            if (file_content.find(sig) != string::npos) {
                cout << "Virus detected in file! You should remove the file.\n";
                scan_file.close();
                return;
            }
        }
    }
    cout << "No virus found in the file.\n";
    scan_file.close();
}

// Function to block a website
void block_website() {
    system("cls");
    string domain;
    cout << "Enter the domain of the website to block (e.g., example.com): ";
    cin >> domain;

    ofstream host_file("C:\\Windows\\System32\\drivers\\etc\\hosts", ios::app);
    if (!host_file) {
        cout << "Error: Unable to access the hosts file. Run as Administrator.\n";
        return;
    }
    
    host_file << "127.0.0.1 www." << domain << "\n127.0.0.1 " << domain << "\n";
    host_file.close();

    ofstream blocked_list("content\\blocked_websites.txt", ios::app);
    blocked_list << domain << endl;
    blocked_list.close();

    cout << "Website successfully blocked.\n";
}

// Function to list or remove blocked websites
void list_remove_blocked_websites() {
    system("cls");
    vector<string> websites;
    ifstream blocked_list("content\\blocked_websites.txt");
    if (!blocked_list) {
        cout << "No blocked websites found.\n";
        return;
    }

    string site;
    int index = 1;
    while (getline(blocked_list, site)) {
        websites.push_back(site);
        cout << index++ << "- " << site << endl;
    }
    blocked_list.close();

    cout << "Enter the number to remove (0 to exit): ";
    int choice;
    cin >> choice;

    if (choice > 0 && choice <= websites.size()) {
        string to_remove = websites[choice - 1];

        ofstream new_list("content\\blocked_websites_removal.txt");
        for (size_t i = 0; i < websites.size(); i++) {
            if (i != choice - 1) {
                new_list << websites[i] << endl;
            }
        }
        new_list.close();
        remove("content\\blocked_websites.txt");
        rename("content\\blocked_websites_removal.txt", "content\\blocked_websites.txt");

        ifstream host_file("C:\\Windows\\System32\\drivers\\etc\\hosts");
        ofstream temp_host_file("content\\hosts_temp.txt");
        if (!host_file || !temp_host_file) {
            cout << "Error accessing the hosts file. Run as Administrator.\n";
            return;
        }

        string line;
        while (getline(host_file, line)) {
            if (line.find(to_remove) == string::npos) {
                temp_host_file << line << endl;
            }
        }
        host_file.close();
        temp_host_file.close();

        remove("C:\\Windows\\System32\\drivers\\etc\\hosts");
        rename("content\\hosts_temp.txt", "C:\\Windows\\System32\\drivers\\etc\\hosts");

        cout << "Website '" << to_remove << "' successfully removed.\n";
    }
}

int main() {
    int choice;
    do {
        system("cls");
        cout << "1. Full PC Scan\n";
        cout << "2. Virus Scan\n";
        cout << "3. Block Website\n";
        cout << "4. List/Remove Blocked Websites\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: scf_scan_now(); break;
            case 2: virus_scan(); break;
            case 3: block_website(); break;
            case 4: list_remove_blocked_websites(); break;
            case 5: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice, try again.\n";
        }
        system("pause");
    } while (choice != 5);

    return 0;
}
