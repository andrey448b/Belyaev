#ifndef MENU_H
#define MENU_H

#include "Warehouse.h"
#include <string>
#include <string_view>
#include <vector>

class Menu {
private:
    Warehouse warehouse;
    std::string databasePath = "";

    void showMainMenu() const;
    void viewAllProducts(const std::vector<Product>& productList, std::string_view title = "СПИСОК ТОВАРОВ") const;
    
    void addProduct();
    void editProduct();
    void deleteProduct();
    
    void searchAndFilterMenu();
    void sortingMenu();
    void analyticsMenu();
    
    void saveDatabase();
    void loadDatabase();

    [[nodiscard]] static int readInt(std::string_view prompt, int minVal = 0, int maxVal = 2147483647);
    [[nodiscard]] static double readDouble(std::string_view prompt, double minVal = 0.0, double maxVal = 1e9);
    [[nodiscard]] static std::string readString(std::string_view prompt, bool allowEmpty = false);
    
    static void waitForEnter();
    static void setupConsoleEncoding() noexcept;

public:
    explicit Menu(std::string_view dbPath);
    void run();
};

#endif // MENU_H
