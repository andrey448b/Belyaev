#include "Menu.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    try {
        // C++17 filesystem: Create the database folder if it doesn't exist yet
        if (!fs::exists("data")) {
            fs::create_directory("data");
        }

        // Initialize and run the interactive menu using the standard relative path
        Menu menu("data/products.txt");
        menu.run();

    } catch (const std::exception& e) {
        std::cerr << " [Критическая ошибка] Непредвиденное исключение: " << e.what() << "\n";
        return 1;
    } catch (...) {
        std::cerr << " [Критическая ошибка] Непредвиденная системная ошибка\n";
        return 1;
    }

    return 0;
}
