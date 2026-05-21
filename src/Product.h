#ifndef PRODUCT_H
#define PRODUCT_H

#include <string>
#include <string_view>

class Product {
private:
    int id = 0;
    std::string name = "";
    std::string category = "";
    double price = 0.0;
    int quantity = 0;

public:
    Product();
    Product(int idVal, std::string nameVal, std::string categoryVal, double priceVal, int quantityVal);

    [[nodiscard]] int getId() const noexcept;
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getCategory() const;
    [[nodiscard]] double getPrice() const noexcept;
    [[nodiscard]] int getQuantity() const noexcept;

    [[nodiscard]] bool setId(int newId) noexcept;
    [[nodiscard]] bool setName(std::string newName);
    [[nodiscard]] bool setCategory(std::string newCategory);
    [[nodiscard]] bool setPrice(double newPrice) noexcept;
    [[nodiscard]] bool setQuantity(int newQuantity) noexcept;

    [[nodiscard]] constexpr static bool isValidId(int idVal) noexcept {
        return idVal > 0;
    }
    
    [[nodiscard]] constexpr static bool isValidPrice(double priceVal) noexcept {
        return priceVal >= 0.0;
    }
    
    [[nodiscard]] constexpr static bool isValidQuantity(int qtyVal) noexcept {
        return qtyVal >= 0;
    }
    
    [[nodiscard]] static bool isValidName(std::string_view nameVal) noexcept;
    [[nodiscard]] static bool isValidCategory(std::string_view catVal) noexcept;
};

#endif // PRODUCT_H
