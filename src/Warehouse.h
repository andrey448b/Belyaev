#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#include "Product.h"
#include <vector>
#include <string>
#include <string_view>

class Warehouse {
private:
    std::vector<Product> products;

    [[nodiscard]] static std::string toLowerUTF8(std::string_view str);

public:
    Warehouse();

    [[nodiscard]] bool addProduct(const Product& product);
    [[nodiscard]] bool editProduct(int id, std::string name, std::string category, double price, int quantity);
    [[nodiscard]] bool deleteProductById(int id) noexcept;
    [[nodiscard]] int deleteProductByName(std::string_view name);
    [[nodiscard]] bool hasId(int id) const noexcept;
    [[nodiscard]] const Product* getProductById(int id) const noexcept;
    [[nodiscard]] const std::vector<Product>& getProducts() const noexcept;
    
    void setProducts(const std::vector<Product>& newProducts);
    void clear() noexcept;

    [[nodiscard]] std::vector<Product> findByName(std::string_view partialName) const;
    [[nodiscard]] std::vector<Product> findByCategory(std::string_view category) const;
    [[nodiscard]] std::vector<Product> findByPriceRange(double minPrice, double maxPrice) const;

    void sortByPrice(bool ascending);
    void sortByQuantity(bool ascending);
    void sortByName(bool ascending);

    [[nodiscard]] double getTotalStockValue() const noexcept;
    [[nodiscard]] std::vector<Product> getLowStockProducts(int threshold = 5) const;
    [[nodiscard]] std::vector<Product> getTop3ExpensiveProducts() const;
};

#endif // WAREHOUSE_H
