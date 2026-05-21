#include "Product.h"
#include <utility>

Product::Product() = default;

Product::Product(int idVal, std::string nameVal, std::string categoryVal, double priceVal, int quantityVal)
    : id(idVal),
      name(std::move(nameVal)),
      category(std::move(categoryVal)),
      price(priceVal),
      quantity(quantityVal) {}

int Product::getId() const noexcept {
    return id;
}

std::string Product::getName() const {
    return name;
}

std::string Product::getCategory() const {
    return category;
}

double Product::getPrice() const noexcept {
    return price;
}

int Product::getQuantity() const noexcept {
    return quantity;
}

bool Product::setId(int newId) noexcept {
    if (isValidId(newId)) {
        id = newId;
        return true;
    }
    return false;
}

bool Product::setName(std::string newName) {
    if (isValidName(newName)) {
        name = std::move(newName);
        return true;
    }
    return false;
}

bool Product::setCategory(std::string newCategory) {
    if (isValidCategory(newCategory)) {
        category = std::move(newCategory);
        return true;
    }
    return false;
}

bool Product::setPrice(double newPrice) noexcept {
    if (isValidPrice(newPrice)) {
        price = newPrice;
        return true;
    }
    return false;
}

bool Product::setQuantity(int newQuantity) noexcept {
    if (isValidQuantity(newQuantity)) {
        quantity = newQuantity;
        return true;
    }
    return false;
}

bool Product::isValidName(std::string_view nameVal) noexcept {
    if (nameVal.empty()) {
        return false;
    }
    
    bool allSpaces = true;
    for (const char c : nameVal) {
        if (c != ' ' && c != '\t') {
            allSpaces = false;
        }
        if (c == ',' || c == '\n' || c == '\r') {
            return false;
        }
    }
    return !allSpaces;
}

bool Product::isValidCategory(std::string_view catVal) noexcept {
    if (catVal.empty()) {
        return false;
    }
    
    bool allSpaces = true;
    for (const char c : catVal) {
        if (c != ' ' && c != '\t') {
            allSpaces = false;
        }
        if (c == ',' || c == '\n' || c == '\r') {
            return false;
        }
    }
    return !allSpaces;
}
