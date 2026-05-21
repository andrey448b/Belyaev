#include "Warehouse.h"
#include <algorithm>
#include <iostream>
#include <utility>

std::string Warehouse::toLowerUTF8(std::string_view str) {
    std::string result = "";
    for (size_t i = 0; i < str.length(); ++i) {
        unsigned char c1 = str[i];
        if (c1 >= 'A' && c1 <= 'Z') {
            result += (char)(c1 + 32);
        } else if (c1 == 0xD0 && i + 1 < str.length()) {
            unsigned char c2 = str[i+1];
            if (c2 >= 0x90 && c2 <= 0x9F) {
                result += (char)0xD0;
                result += (char)(c2 + 0x20);
                i++;
            } else if (c2 >= 0xA0 && c2 <= 0xAF) {
                result += (char)0xD1;
                result += (char)(c2 - 0x20);
                i++;
            } else if (c2 == 0x81) {
                result += (char)0xD1;
                result += (char)0x91;
                i++;
            } else {
                result += (char)c1;
            }
        } else if (c1 == 0xD1 && i + 1 < str.length()) {
            result += (char)c1;
            result += str[i+1];
            i++;
        } else {
            result += (char)c1;
        }
    }
    return result;
}

Warehouse::Warehouse() = default;

bool Warehouse::addProduct(const Product& product) {
    if (hasId(product.getId())) {
        return false;
    }
    products.push_back(product);
    return true;
}

bool Warehouse::editProduct(int id, std::string name, std::string category, double price, int quantity) {
    for (auto& p : products) {
        if (p.getId() == id) {
            static_cast<void>(p.setName(std::move(name)));
            static_cast<void>(p.setCategory(std::move(category)));
            static_cast<void>(p.setPrice(price));
            static_cast<void>(p.setQuantity(quantity));
            return true;
        }
    }
    return false;
}

bool Warehouse::deleteProductById(int id) noexcept {
    const auto initialSize = products.size();
    products.erase(
        std::remove_if(products.begin(), products.end(), [id](const Product& p) noexcept {
            return p.getId() == id;
        }),
        products.end()
    );
    return products.size() < initialSize;
}

int Warehouse::deleteProductByName(std::string_view name) {
    const std::string searchName = toLowerUTF8(name);
    const auto initialSize = products.size();
    
    products.erase(
        std::remove_if(products.begin(), products.end(), [&searchName](const Product& p) {
            return toLowerUTF8(p.getName()) == searchName;
        }),
        products.end()
    );
    
    return static_cast<int>(initialSize - products.size());
}

bool Warehouse::hasId(int id) const noexcept {
    return std::any_of(products.begin(), products.end(), [id](const Product& p) noexcept {
        return p.getId() == id;
    });
}

const Product* Warehouse::getProductById(int id) const noexcept {
    for (const auto& p : products) {
        if (p.getId() == id) {
            return &p;
        }
    }
    return nullptr;
}

const std::vector<Product>& Warehouse::getProducts() const noexcept {
    return products;
}

void Warehouse::setProducts(const std::vector<Product>& newProducts) {
    products = newProducts;
}

void Warehouse::clear() noexcept {
    products.clear();
}

std::vector<Product> Warehouse::findByName(std::string_view partialName) const {
    std::vector<Product> result;
    const std::string query = toLowerUTF8(partialName);
    
    for (const auto& p : products) {
        const std::string nameLower = toLowerUTF8(p.getName());
        if (nameLower.find(query) != std::string::npos) {
            result.push_back(p);
        }
    }
    return result;
}

std::vector<Product> Warehouse::findByCategory(std::string_view category) const {
    std::vector<Product> result;
    const std::string query = toLowerUTF8(category);
    
    for (const auto& p : products) {
        const std::string catLower = toLowerUTF8(p.getCategory());
        if (catLower.find(query) != std::string::npos) {
            result.push_back(p);
        }
    }
    return result;
}

std::vector<Product> Warehouse::findByPriceRange(double minPrice, double maxPrice) const {
    std::vector<Product> result;
    for (const auto& p : products) {
        if (p.getPrice() >= minPrice && p.getPrice() <= maxPrice) {
            result.push_back(p);
        }
    }
    return result;
}

void Warehouse::sortByPrice(bool ascending) {
    std::sort(products.begin(), products.end(), [ascending](const Product& a, const Product& b) noexcept {
        if (a.getPrice() == b.getPrice()) {
            return a.getId() < b.getId();
        }
        return ascending ? (a.getPrice() < b.getPrice()) : (a.getPrice() > b.getPrice());
    });
}

void Warehouse::sortByQuantity(bool ascending) {
    std::sort(products.begin(), products.end(), [ascending](const Product& a, const Product& b) noexcept {
        if (a.getQuantity() == b.getQuantity()) {
            return a.getId() < b.getId();
        }
        return ascending ? (a.getQuantity() < b.getQuantity()) : (a.getQuantity() > b.getQuantity());
    });
}

void Warehouse::sortByName(bool ascending) {
    std::sort(products.begin(), products.end(), [ascending, this](const Product& a, const Product& b) {
        const std::string la = toLowerUTF8(a.getName());
        const std::string lb = toLowerUTF8(b.getName());
        if (la == lb) {
            return a.getId() < b.getId();
        }
        return ascending ? (la < lb) : (la > lb);
    });
}

double Warehouse::getTotalStockValue() const noexcept {
    double total = 0.0;
    for (const auto& p : products) {
        total += p.getPrice() * p.getQuantity();
    }
    return total;
}

std::vector<Product> Warehouse::getLowStockProducts(int threshold) const {
    std::vector<Product> result;
    for (const auto& p : products) {
        if (p.getQuantity() <= threshold) {
            result.push_back(p);
        }
    }
    return result;
}

std::vector<Product> Warehouse::getTop3ExpensiveProducts() const {
    std::vector<Product> sortedProducts = products;
    
    std::sort(sortedProducts.begin(), sortedProducts.end(), [](const Product& a, const Product& b) noexcept {
        if (a.getPrice() == b.getPrice()) {
            return a.getQuantity() > b.getQuantity();
        }
        return a.getPrice() > b.getPrice();
    });
    
    std::vector<Product> top3;
    const size_t count = std::min(static_cast<size_t>(3), sortedProducts.size());
    for (size_t i = 0; i < count; ++i) {
        top3.push_back(sortedProducts[i]);
    }
    
    return top3;
}
