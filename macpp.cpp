#include <iostream>
#include <cstdlib>
#include <string>
#include <chrono>
#include <thread>
#include <random>
#include <sstream>
#include <iomanip>

std::string GenerateRandomMacAddress() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, 255);

    std::stringstream macStream;
    macStream << std::hex << std::setfill('0');
    
    for (int i = 0; i < 6; ++i) {
        macStream << std::setw(2) << dis(gen);
        if (i < 5) {
            macStream << ":";
        }
    }

    std::string macAddress = macStream.str();
    return macAddress;
}

void ChangeMacAddress(std::string newMac) {
    std::string command = "netsh interface set interface name=\"Ethernet\" admin=disable";
    std::system(command.c_str());

    command = "netsh interface set interface name=\"Ethernet\" admin=enable";
    std::system(command.c_str());

    command = "netsh interface set interface name=\"Wi-Fi\" admin=disable";
    std::system(command.c_str());

    command = "netsh interface set interface name=\"Wi-Fi\" admin=enable";
    std::system(command.c_str());

    command = "reg add HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002BE10318}\\0001 /v NetworkAddress /d " + newMac + " /f";
    std::system(command.c_str());

    std::this_thread::sleep_for(std::chrono::seconds(1));

    command = "ipconfig /flushdns";
    std::system(command.c_str());

    std::cout << "MAC adresi değiştirildi: " << newMac << std::endl;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "Kullanım: macchanger.exe -r (rastgele MAC) -h (yardım) -t (saniyede bir MAC değiştirme) <yeni MAC adresi>" << std::endl;
        return 0;
    }

    std::string option = argv[1];
    if (option == "-r") {
        std::string randomMac = GenerateRandomMacAddress();
        ChangeMacAddress(randomMac);
    }
    else if (option == "-h") {
        std::cout << "Yardım: macchanger.exe -r (rastgele MAC) -h (yardım) -t (saniyede bir MAC değiştirme) <yeni MAC adresi>" << std::endl;
    }
    else if (option == "-t") {
        int seconds = std::stoi(argv[2]);
        std::string newMac = argv[3];

        while (true) {
            ChangeMacAddress(newMac);
            std::this_thread::sleep_for(std::chrono::seconds(seconds));
        }
    }
    else {
        std::string newMac = argv[1];
        ChangeMacAddress(newMac);
    }

    return 0;
}
