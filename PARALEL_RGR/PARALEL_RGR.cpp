#include <iostream>
#include <typeinfo>
#include <vector>
//#include <ostream>

// Базовый класс
class Base {
public:
    virtual void print() const = 0; // Чистая виртуальная функция для вывода
    virtual bool equalWithPrecision(double val, double precision = 0.001) const = 0;
    virtual bool equalWithPrecision(int val, double precision = 0.001) const = 0;
    virtual ~Base() {} // Виртуальный деструктор
};
// Класс для хранения целого числа
class IntElement : public Base {
public:
    IntElement(int value) : value(value) {}
    void print() const override {
        std::cout << "Int: " << value << std::endl;
    }
    bool equalWithPrecision(double val, double precision) const {
        if (std::abs((double)this->value - val) <= precision) {
            return true;
        }
        else {
            return false;
        }
    }
    bool equalWithPrecision(int val, double precision) const {
        return this->value == val;
    }
private:
    int value;
};
// Класс для хранения числа с плавающей точкой
class DoubleElement : public Base {
public:
    DoubleElement(double value) : value(value) {}
    void print() const override {
        std::cout << "Double: " << value << std::endl;
    }
    bool equalWithPrecision(double val, double precision) const {
        if (std::abs(this->value - val) <= precision) {
            return true;
        }
        else {
            return false;
        }
    }
    bool equalWithPrecision(int val, double precision) const {
        if (std::abs(this->value - (double)(val)) <= precision) {
            return true;
        }
        else {
            return false;
        }
    }
private:
    double value;
};

int main() {
    // Вектор уникальных указателей на объекты базового класса
    std::vector<std::unique_ptr<Base>> elements;
    // Добавление элементов разных типов в массив
    elements.push_back(std::make_unique<IntElement>(5));
    elements.push_back(std::make_unique<DoubleElement>(5.0001));
    // Вывод элементов
    for (const auto& element : elements) {
        std::cout << element->equalWithPrecision(5)<<"|" <<element->equalWithPrecision(5.001)<< std::endl;
    }
    elements.clear();
    return 0;
}