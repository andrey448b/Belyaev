#include "Menu.h"
#include "FileIO.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <utility>

#ifdef _WIN32
#include <windows.h>
#endif

static size_t utf8_length(std::string_view str) noexcept {
    size_t len = 0;
    for (size_t i = 0; i < str.length(); ++i) {
        const unsigned char c = str[i];
        if ((c & 0xC0) != 0x80) {
            len++;
        }
    }
    return len;
}

static std::string padString(std::string_view str, size_t width) {
    const size_t len = utf8_length(str);
    if (len >= width) {
        if (width <= 2) {
            return "..";
        }
        std::string truncated = "";
        size_t charCount = 0;
        for (size_t i = 0; i < str.length(); ) {
            const unsigned char c = str[i];
            size_t charBytes = 1;
            if ((c & 0xE0) == 0xC0) charBytes = 2;
            else if ((c & 0xF0) == 0xE0) charBytes = 3;
            else if ((c & 0xF8) == 0xF0) charBytes = 4;

            if (charCount + 1 > width - 2) {
                truncated += "..";
                break;
            }
            truncated += str.substr(i, charBytes);
            i += charBytes;
            charCount++;
        }
        return truncated;
    }
    return std::string{str} + std::string(width - len, ' ');
}

void Menu::setupConsoleEncoding() noexcept {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif
}

Menu::Menu(std::string_view dbPath) : databasePath(dbPath) {
    setupConsoleEncoding();
}

int Menu::readInt(std::string_view prompt, int minVal, int maxVal) {
    int val = 0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            if (val >= minVal && val <= maxVal) {
                std::cin.ignore(10000, '\n');
                return val;
            }
            std::cout << " [Ошибка] Число должно быть в диапазоне от " << minVal << " до " << maxVal << "\n";
        } else {
            std::cout << " [Ошибка] Некорректный ввод, ожидается целое число\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    }
}

double Menu::readDouble(std::string_view prompt, double minVal, double maxVal) {
    double val = 0.0;
    while (true) {
        std::cout << prompt;
        if (std::cin >> val) {
            if (val >= minVal && val <= maxVal) {
                std::cin.ignore(10000, '\n');
                return val;
            }
            std::cout << " [Ошибка] Число должно быть в диапазоне от " << minVal << " до " << maxVal << "\n";
        } else {
            std::cout << " [Ошибка] Некорректный ввод, ожидается число с плавающей точкой\n";
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    }
}

std::string Menu::readString(std::string_view prompt, bool allowEmpty) {
    std::string val = "";
    while (true) {
        std::cout << prompt;
        std::getline(std::cin, val);
        
        if (!val.empty() && val.back() == '\r') {
            val.pop_back();
        }

        if (!allowEmpty && val.empty()) {
            std::cout << " [Ошибка] Поле не может быть пустым\n";
            continue;
        }

        if (val.find(',') != std::string::npos) {
            std::cout << " [Ошибка] Запятые (,) запрещены в названиях или категориях во избежание сбоя базы данных\n";
            continue;
        }
        return val;
    }
}

void Menu::waitForEnter() {
    std::cout << "\n Нажмите Enter для продолжения...";
    std::string dummy;
    std::getline(std::cin, dummy);
}

void Menu::viewAllProducts(const std::vector<Product>& productList, std::string_view title) const {
    std::cout << "\n═════════════════════════════════════════════════════════════════════════════\n";
    std::cout << "  " << title << " (Всего: " << productList.size() << ")\n";
    std::cout << "═════════════════════════════════════════════════════════════════════════════\n";

    if (productList.empty()) {
        std::cout << "   Склад пуст или нет товаров, удовлетворяющих условиям\n";
        std::cout << "═════════════════════════════════════════════════════════════════════════════\n";
        return;
    }

    std::cout << "┌──────┬──────────────────────┬──────────────────────┬──────────┬──────────┐\n";
    std::cout << "│ " << padString("ID", 4) 
              << " │ " << padString("Название", 20) 
              << " │ " << padString("Категория", 20) 
              << " │ " << padString("Цена", 8) 
              << " │ " << padString("Кол-во", 8) << " │\n";
    std::cout << "├──────┼──────────────────────┼──────────────────────┼──────────┼──────────┤\n";

    for (const auto& p : productList) {
        std::stringstream priceSS;
        priceSS << std::fixed << std::setprecision(2) << p.getPrice();
        
        std::stringstream qtySS;
        qtySS << p.getQuantity();

        std::stringstream idSS;
        idSS << p.getId();

        std::cout << "│ " << padString(idSS.str(), 4) 
                  << " │ " << padString(p.getName(), 20) 
                  << " │ " << padString(p.getCategory(), 20) 
                  << " │ " << padString(priceSS.str(), 8) 
                  << " │ " << padString(qtySS.str(), 8) << " │\n";
    }
    std::cout << "└──────┴──────────────────────┴──────────────────────┴──────────┴──────────┘\n";
}

void Menu::showMainMenu() const {
    std::cout << "\n╔══════════════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║                  СИСТЕМА УЧЕТА ТОВАРОВ НА СКЛАДЕ                         ║\n";
    std::cout << "╚══════════════════════════════════════════════════════════════════════════╝\n";
    std::cout << " [1] Просмотреть все товары\n";
    std::cout << " [2] Добавить новый товар\n";
    std::cout << " [3] Редактировать товар\n";
    std::cout << " [4] Удалить товар\n";
    std::cout << " [5] Поиск и фильтрация\n";
    std::cout << " [6] Сортировка товаров\n";
    std::cout << " [7] Аналитика склада\n";
    std::cout << " [8] Загрузить из файла (ручной импорт)\n";
    std::cout << " [9] Сохранить изменения в файл (ручной экспорт)\n";
    std::cout << " [0] Выйти из программы\n";
    std::cout << "────────────────────────────────────────────────────────────────────────────\n";
}

void Menu::run() {
    std::cout << " [Система] Автоматическая загрузка базы данных...\n";
    if (FileIO::loadFromFile(databasePath, warehouse)) {
        std::cout << " [Успех] Загружено товаров: " << warehouse.getProducts().size() << "\n";
    } else {
        std::cout << " [Предупреждение] Файл базы данных не найден. Будет создан новый при сохранении\n";
    }
    waitForEnter();

    int choice = -1;
    while (choice != 0) {
        showMainMenu();
        choice = readInt(" Выберите пункт меню: ", 0, 9);
        
        switch (choice) {
            case 1:
                viewAllProducts(warehouse.getProducts());
                waitForEnter();
                break;
            case 2:
                addProduct();
                waitForEnter();
                break;
            case 3:
                editProduct();
                waitForEnter();
                break;
            case 4:
                deleteProduct();
                waitForEnter();
                break;
            case 5:
                searchAndFilterMenu();
                break;
            case 6:
                sortingMenu();
                break;
            case 7:
                analyticsMenu();
                break;
            case 8:
                loadDatabase();
                waitForEnter();
                break;
            case 9:
                saveDatabase();
                waitForEnter();
                break;
            case 0:
                std::cout << "\n [Система] Программа завершает работу, не забудьте сохранить данные\n";
                break;
        }
    }
}

void Menu::addProduct() {
    std::cout << "\n--- ДОБАВЛЕНИЕ НОВОГО ТОВАРА ---\n";
    
    const int id = readInt(" Введите ID товара (уникальный, > 0): ", 1);
    if (warehouse.hasId(id)) {
        std::cout << " [Ошибка] Товар с таким ID уже существует на складе\n";
        return;
    }

    std::string name = readString(" Введите название товара: ", false);
    std::string category = readString(" Введите категорию товара: ", false);
    const double price = readDouble(" Введите цену товара (>= 0): ", 0.0);
    const int quantity = readInt(" Введите количество на складе (>= 0): ", 0);

    Product p(id, std::move(name), std::move(category), price, quantity);
    if (warehouse.addProduct(p)) {
        std::cout << " [Успех] Товар успешно добавлен\n";
    } else {
        std::cout << " [Ошибка] Не удалось добавить товар\n";
    }
}

void Menu::editProduct() {
    std::cout << "\n--- РЕДАКТИРОВАНИЕ ТОВАРА ---\n";
    const int id = readInt(" Введите ID товара для редактирования: ", 1);
    
    const Product* const p = warehouse.getProductById(id);
    if (p == nullptr) {
        std::cout << " [Ошибка] Товар с ID " << id << " не найден на складе!\n";
        return;
    }

    std::cout << "\n Товар найден:\n";
    std::cout << " Текущее название: " << p->getName() << "\n";
    std::cout << " Текущая категория: " << p->getCategory() << "\n";
    std::cout << " Текущая цена: " << p->getPrice() << "\n";
    std::cout << " Текущее количество: " << p->getQuantity() << "\n";
    std::cout << "\n (Нажмите Enter без ввода текста, чтобы сохранить старое значение)\n\n";

    std::string newName = readString(" Новое название: ", true);
    if (newName.empty()) {
        newName = p->getName();
    }

    std::string newCategory = readString(" Новая категория: ", true);
    if (newCategory.empty()) {
        newCategory = p->getCategory();
    }

    double newPrice = p->getPrice();
    const std::string priceInput = readString(" Новая цена (>= 0): ", true);
    if (!priceInput.empty()) {
        try {
            newPrice = std::stod(priceInput);
            if (newPrice < 0.0) {
                std::cout << " [Ошибка] Неверная цена, сохранено старое значение\n";
                newPrice = p->getPrice();
            }
        } catch (...) {
            std::cout << " [Ошибка] Неверный формат цены, сохранено старое значение\n";
        }
    }

    int newQuantity = p->getQuantity();
    const std::string qtyInput = readString(" Новое количество (>= 0): ", true);
    if (!qtyInput.empty()) {
        try {
            newQuantity = std::stoi(qtyInput);
            if (newQuantity < 0) {
                std::cout << " [Ошибка] Неверное количество, сохранено старое значение\n";
                newQuantity = p->getQuantity();
            }
        } catch (...) {
            std::cout << " [Ошибка] Неверный формат количества, сохранено старое значение\n";
        }
    }

    if (warehouse.editProduct(id, std::move(newName), std::move(newCategory), newPrice, newQuantity)) {
        std::cout << "\n [Успех] Сведения о товаре ID " << id << " успешно обновлены!\n";
    } else {
        std::cout << " [Ошибка] Не удалось обновить товар.\n";
    }
}

void Menu::deleteProduct() {
    std::cout << "\n--- УДАЛЕНИЕ ТОВАРА ---\n";
    std::cout << " [1] Удалить по уникальному ID\n";
    std::cout << " [2] Удалить по названию (все совпадения)\n";
    std::cout << " [0] Отмена\n";
    
    const int choice = readInt(" Выберите метод удаления: ", 0, 2);
    
    if (choice == 1) {
        const int id = readInt(" Введите ID товара для удаления: ", 1);
        if (warehouse.deleteProductById(id)) {
            std::cout << " [Успех] Товар с ID " << id << " удален\n";
        } else {
            std::cout << " [Ошибка] Товар с ID " << id << " не найден\n";
        }
    } else if (choice == 2) {
        const std::string name = readString(" Введите название товара(ов) для удаления: ", false);
        const int deletedCount = warehouse.deleteProductByName(name);
        if (deletedCount > 0) {
            std::cout << " [Успех] Найдено и удалено товаров: " << deletedCount << "\n";
        } else {
            std::cout << " [Ошибка] Товары с таким названием не найдены\n";
        }
    }
}

void Menu::searchAndFilterMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- ПОИСК И ФИЛЬТРАЦИЯ ---\n";
        std::cout << " [1] Поиск по названию (регистронезависимо, частичное совпадение)\n";
        std::cout << " [2] Фильтрация по категории\n";
        std::cout << " [3] Фильтрация по диапазону цен\n";
        std::cout << " [0] Вернуться в главное меню\n";
        
        choice = readInt(" Выберите пункт: ", 0, 3);
        
        if (choice == 1) {
            const std::string query = readString(" Введите поисковый запрос (название): ", false);
            const std::vector<Product> results = warehouse.findByName(query);
            viewAllProducts(results, "РЕЗУЛЬТАТЫ ПОИСКА ПО НАЗВАНИЮ: \"" + query + "\"");
            waitForEnter();
        } else if (choice == 2) {
            const std::string query = readString(" Введите категорию для фильтрации: ", false);
            const std::vector<Product> results = warehouse.findByCategory(query);
            viewAllProducts(results, "ТОВАРЫ В КАТЕГОРИИ: \"" + query + "\"");
            waitForEnter();
        } else if (choice == 3) {
            const double minPrice = readDouble(" Введите минимальную цену: ", 0.0);
            const double maxPrice = readDouble(" Введите максимальную цену: ", minPrice);
            const std::vector<Product> results = warehouse.findByPriceRange(minPrice, maxPrice);
            
            std::stringstream titleSS;
            titleSS << "ФИЛЬТР ПО ЦЕНЕ: от " << std::fixed << std::setprecision(2) << minPrice << " до " << maxPrice;
            viewAllProducts(results, titleSS.str());
            waitForEnter();
        }
    }
}

void Menu::sortingMenu() {
    int choice = -1;
    while (choice != 0) {
        std::cout << "\n--- СОРТИРОВКА ТОВАРОВ ---\n";
        std::cout << " [1] По цене (возрастание)\n";
        std::cout << " [2] По цене (убывание)\n";
        std::cout << " [3] По количеству на складе (возрастание)\n";
        std::cout << " [4] По количеству на складе (убывание)\n";
        std::cout << " [5] По алфавиту (название A-Я)\n";
        std::cout << " [6] По алфавиту (название Я-A)\n";
        std::cout << " [0] Вернуться в главное меню\n";
        
        choice = readInt(" Выберите метод сортировки: ", 0, 6);
        
        if (choice == 1) {
            warehouse.sortByPrice(true);
            std::cout << " [Система] Товары отсортированы по цене (возрастание)\n";
            viewAllProducts(warehouse.getProducts());
            waitForEnter();
        } else if (choice == 2) {
            warehouse.sortByPrice(false);
            std::cout << " [Система] Товары отсортированы по цене (убывание)\n";
            viewAllProducts(warehouse.getProducts());
            waitForEnter();
        } else if (choice == 3) {
            warehouse.sortByQuantity(true);
            std::cout << " [Система] Товары отсортированы по количеству (возрастание)\n";
            viewAllProducts(warehouse.getProducts());
            waitForEnter();
        } else if (choice == 4) {
            warehouse.sortByQuantity(false);
            std::cout << " [Система] Товары отсортированы по количеству (убывание)\n";
            viewAllProducts(warehouse.getProducts());
            waitForEnter();
        } else if (choice == 5) {
            warehouse.sortByName(true);
            std::cout << " [Система] Товары отсортированы по алфавиту (А-Я)\n";
            viewAllProducts(warehouse.getProducts());
            waitForEnter();
        } else if (choice == 6) {
            warehouse.sortByName(false);
            std::cout << " [Система] Товары отсортированы по алфавиту (Я-А)\n";
            viewAllProducts(warehouse.getProducts());
            waitForEnter();
        }
    }
}

void Menu::analyticsMenu() {
    std::cout << "\n--- АНАЛИТИКА И ПОКАЗАТЕЛИ СКЛАДА ---\n";
    
    const double totalVal = warehouse.getTotalStockValue();
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "  » Общая стоимость товаров на складе: " << totalVal << " руб\n";
    
    const std::vector<Product> lowStock = warehouse.getLowStockProducts(5);
    std::cout << "  » Количество позиций с остатком <= 5 шт: " << lowStock.size() << "\n";
    if (!lowStock.empty()) {
        viewAllProducts(lowStock, "ТОВАРЫ С ОСТАТКОМ <= 5 ШТ");
    }
    
    const std::vector<Product> top3 = warehouse.getTop3ExpensiveProducts();
    viewAllProducts(top3, "ТОП-3 САМЫХ ДОРОГИХ ПОЗИЦИЙ НА СКЛАДЕ");
    
    waitForEnter();
}

void Menu::saveDatabase() {
    std::cout << "\n [Система] Сохранение изменений в файл базы данных: \"" << databasePath << "\"...\n";
    if (FileIO::saveToFile(databasePath, warehouse)) {
        std::cout << " [Успех] Изменения успешно сохранены\n";
    } else {
        std::cout << " [Ошибка] Не удалось записать данные в файл, проверьте права доступа к директории\n";
    }
}

void Menu::loadDatabase() {
    std::cout << "\n [Система] Считывание товаров из файла базы данных: \"" << databasePath << "\"...\n";
    if (FileIO::loadFromFile(databasePath, warehouse)) {
        std::cout << " [Успех] Загружено товаров: " << warehouse.getProducts().size() << "\n";
    } else {
        std::cout << " [Ошибка] Не удалось считать файл, убедитесь, что файл существует\n";
    }
}
