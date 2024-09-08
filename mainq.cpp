#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace std;

// Константы для настройки склада
const int MAX_CELL_CAPACITY = 10; // Максимальная вместимость ячейки
const int ZONES = 2; // Количество зон хранения
const int RACKS_PER_ZONE = 4; // Количество стеллажей в зоне
const int VERTICAL_SECTIONS = 18; // Количество вертикальных секций стеллажа
const int SHELVES_PER_VERTICAL_SECTION = 5; // Количество полок в вертикальной секции
const int TOTAL_CAPACITY = ZONES * RACKS_PER_ZONE * VERTICAL_SECTIONS * SHELVES_PER_VERTICAL_SECTION * MAX_CELL_CAPACITY; // Общая вместимость склада

// Структура для хранения информации о товаре в ячейке
struct Cell {
    string address;
    std::string productName;
    int quantity;
};

// Функция для парсинга адреса ячейки
bool parseCellAddress(const std::string& address, int& zone, int& rack, int& verticalSection, int& shelf) {
    if (address.length() != 5 || !isalpha(address[0]) || !isdigit(address[1]) || !isdigit(address[2]) || !isdigit(address[3]) || !isdigit(address[4]))
    {
        return false;
    }
    zone = toupper(address[0]) - 'A';
    rack = (address[1] - '0');
    verticalSection = ((address[2] - '0') * 10 + (address[3] - '0'));
    shelf = (address[4] - '0');
    if (zone > ZONES || rack > RACKS_PER_ZONE || rack < 1 || verticalSection > VERTICAL_SECTIONS || verticalSection < 1 || shelf > SHELVES_PER_VERTICAL_SECTION || shelf < 1) {
        return false;
    }
    return true;
}

// Функция для добавления товара на склад
void addProduct(vector<Cell>& warehouse, const std::string& productName, int quantity, const std::string& address) {
    int zone, rack, verticalSection, shelf;
    if (!parseCellAddress(address, zone, rack, verticalSection, shelf)) {
        std::cout << "Нет такого адреса." << std::endl;
        return;
    }

    if (quantity > 10 || quantity < 1)
    {
        std::cout << "Некорректное количество товаров или избыток товаров" << std::endl;
        return;
    }

    std::string cellKey = address;

    for (int i = 0; i < warehouse.size(); i++)
    {
        if (warehouse[i].address == address)
        {
            if (warehouse[i].productName == productName)
            {
                if (warehouse[i].quantity + quantity > 10)
                {
                    std::cout << "Нельзя добавить продукты ячейка переполниться" << std::endl;
                    return;
                }
                else {
                    warehouse[i].quantity = warehouse[i].quantity + quantity;
                    std::cout << "Продукты успешно добавлены." << std::endl;
                    return;
                }
            }
            else {
                std::cout << "Нужно добавлять только одинаковые продукты" << std::endl;
                return;
            }
        }
    }
    Cell in;
    in.address = address;
    in.productName = productName;
    in.quantity = quantity;
    warehouse.push_back(in);
    std::cout << "Продукты успешно добавлены" << std::endl;

}



// Функция для удаления товара со склада
void removeProduct(vector<Cell>& warehouse, const string& productName, int quantity, const string& address) {
    int zone, rack, verticalSection, shelf;
    if (!parseCellAddress(address, zone, rack, verticalSection, shelf)) {
        cout << "Нет такого адреса'." << endl;
        return;
    }


    for (auto it = warehouse.begin(); it != warehouse.end(); it++) {
        if (it->address == address) {
            if (it->productName == productName) {
                if (it->quantity >= quantity && quantity >= 1) {
                    it->quantity -= quantity;
                    if (it->quantity == 0) {
                        warehouse.erase(it);
                    }
                    cout << "Продукты успешно удалены" << endl;
                    return;
                }
                else {
                    cout << "Не корректное количество товаров или недостаток товаров для удаления" << endl;
                    return;
                }
            }
            else {
                cout << "В ячейке нет такого продукта" << endl;
                return;
            }
        }
    }
}

// Функция для вывода информации о состоянии склада
void printWarehouseInfo(const vector<Cell>& warehouse) {
    int totalUsedCapacity = 0;
    vector<int> zoneUsage(ZONES, 0);

    for (const auto& cell : warehouse) {
        totalUsedCapacity += cell.quantity;
        int zoneIndex = toupper(cell.address[0]) - 'A';
        zoneUsage[zoneIndex] += cell.quantity;
    }

    double percentageUsed = static_cast<double>(totalUsedCapacity) / TOTAL_CAPACITY * 100.0;
    cout << "Склад заполнен на " << fixed << setprecision(2) << percentageUsed << "%" << endl;

    for (int zone = 0; zone < ZONES; ++zone) {
        double zonePercentageUsed = static_cast<double>(zoneUsage[zone]) / (RACKS_PER_ZONE * VERTICAL_SECTIONS * SHELVES_PER_VERTICAL_SECTION * MAX_CELL_CAPACITY) * 100.0;
        cout << "Зона " << static_cast<char>('A' + zone) << " заполнена на  " << fixed << setprecision(2) << zonePercentageUsed << "%" << endl;
    }

    cout << "Ячейки с продуктами:" << endl;
    for (const auto& cell : warehouse) {
        cout << "Ячейка " << cell.address << ": " << cell.productName << " - " << cell.quantity << endl;
    }

    cout << "Пустые ячейки:" << endl;
    for (int zone = 0; zone < ZONES; ++zone) {
        for (int rack = 0; rack < RACKS_PER_ZONE; ++rack) {
            for (int verticalSection = 0; verticalSection < VERTICAL_SECTIONS; ++verticalSection) {
                for (int shelf = 0; shelf < SHELVES_PER_VERTICAL_SECTION; ++shelf) {
                    stringstream ss;
                    ss << static_cast<char>('A' + zone) << (rack + 1) << setw(2) << setfill('0') << (verticalSection + 1) << (shelf + 1);
                    string cellAddress = ss.str();
                    bool cellOccupied = false;
                    for (const auto& cell : warehouse) {
                        if (cell.address == cellAddress) {
                            cellOccupied = true;
                            break;
                        }
                    }
                    if (!cellOccupied) {
                        cout << "Ячейка " << cellAddress << endl;
                    }
                }
            }
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    vector<Cell> warehouse;
    string command;

    for (; cin >> command;) {
        if (command == "ADD") {
            string productName;
            int quantity;
            string address;
            cin >> productName >> quantity >> address;
            addProduct(warehouse, productName, quantity, address);
        }
        else if (command == "REMOVE") {
            string productName;
            int quantity;
            string address;
            cin >> productName >> quantity >> address;
            removeProduct(warehouse, productName, quantity, address);
        }
        else if (command == "INFO") {
            printWarehouseInfo(warehouse);
        }
        else {
            cout << "Не известная команда используйте ADD, REMOVE, или INFO." << endl;
        }
    }
    return 0;
}
