#include "FileIO.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

bool FileIO::loadFromFile(std::string_view filename, Warehouse& warehouse) {
    std::ifstream file(std::string{filename});
    if (!file.is_open()) {
        return false;
    }

    std::vector<Product> loadedProducts;
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.empty() || line == "\r") {
            continue;
        }

        std::stringstream ss(line);
        std::string idStr;
        std::string name;
        std::string category;
        std::string priceStr;
        std::string qtyStr;

        if (!std::getline(ss, idStr, ',') ||
            !std::getline(ss, name, ',') ||
            !std::getline(ss, category, ',') ||
            !std::getline(ss, priceStr, ',') ||
            !std::getline(ss, qtyStr, ',')) {
            continue;
        }

        if (!qtyStr.empty() && qtyStr.back() == '\r') {
            qtyStr.pop_back();
        }

        try {
            const int id = std::stoi(idStr);
            const double price = std::stod(priceStr);
            const int qty = std::stoi(qtyStr);

            if (Product::isValidId(id) &&
                Product::isValidName(name) &&
                Product::isValidCategory(category) &&
                Product::isValidPrice(price) &&
                Product::isValidQuantity(qty)) {
                
                bool isDuplicate = false;
                for (const auto& p : loadedProducts) {
                    if (p.getId() == id) {
                        isDuplicate = true;
                        break;
                    }
                }

                if (!isDuplicate) {
                    loadedProducts.emplace_back(id, std::move(name), std::move(category), price, qty);
                }
            }
        } catch (...) {
            continue;
        }
    }

    file.close();
    warehouse.setProducts(loadedProducts);
    return true;
}

bool FileIO::saveToFile(std::string_view filename, const Warehouse& warehouse) {
    std::ofstream file(std::string{filename}, std::ios::out | std::ios::trunc);
    if (!file.is_open()) {
        return false;
    }

    for (const auto& p : warehouse.getProducts()) {
        file << p.getId() << ","
             << p.getName() << ","
             << p.getCategory() << ","
             << p.getPrice() << ","
             << p.getQuantity() << "\n";
    }

    file.close();
    return true;
}
