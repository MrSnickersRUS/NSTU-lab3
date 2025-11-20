#include <iostream>
#include <string>
#include "commands.h"

using namespace std;

int main() {                                                                                                                                                                                                                                                                                                                                                                        
    cout << "Введите 'HELP' для справки или 'EXIT' для выхода\n" << endl;
    
    string line;
    
    while (true) {
        cout << "> ";
        getline(cin, line);
        
        if (line.empty()) {
            continue;
        }
        
        // Check exit command
        if (line == "EXIT" || line == "QUIT") {
            // Предложить сохранить данные
            cout << "\nСохранить данные перед выходом? (y/n): ";
            string response;
            getline(cin, response);
            
            if (response == "y" || response == "Y" || response == "да" || response == "ДА") {
                try {
                    saveAllContainersBinary("containers.bin");
                    cout << "✓ Данные сохранены в containers.bin" << endl;
                } catch (const exception& e) {
                    cerr << "⚠ Ошибка при сохранении: " << e.what() << endl;
                }
            }
            
            cout << "\nДо свидания!" << endl;
            break;
        }
        
        // Process command
        try {
            processCommand(line);
        } catch (const exception& e) {
            cerr << "❌ Ошибка: " << e.what() << endl;
        }
    }
    
    return 0;
}
