#ifndef FILEIO_H
#define FILEIO_H

#include "Warehouse.h"
#include <string_view>

class FileIO {
public:
    [[nodiscard]] static bool loadFromFile(std::string_view filename, Warehouse& warehouse);
    [[nodiscard]] static bool saveToFile(std::string_view filename, const Warehouse& warehouse);
};

#endif // FILEIO_H
