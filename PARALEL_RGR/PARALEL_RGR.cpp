#include <iostream>
#include <typeinfo>
#include <vector>
#include <random>
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
private:
    int value;
public:
    IntElement(int value) : value(value) {}
    void print() const override {
        std::cout << value;
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


};
// Класс для хранения числа с плавающей точкой
class DoubleElement : public Base {
private:
    double value;
public:
    DoubleElement(double value) : value(value) {}
    void print() const override {
        std::cout << value;
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


};

void fillRandom(std::vector<std::unique_ptr<Base>>& vec, int n)
{
    
    for (int i = 0; i < n; ++i)
    {
        int t = rand() % 10000;
        if (t % 2 == 0)
        {
            vec.push_back(std::make_unique<IntElement>(t));
        }
        else
        {
            double t2 = rand() % 10000 / 10000.0;
            vec.push_back(std::make_unique<DoubleElement>(t + t2));
        }
       
    }
}

std::vector<int> singleThreadSearch(std::vector<std::unique_ptr<Base>>& vec, int value, double precision=0.000) {
    std::vector<int> res;
    for (int i = 0; i < vec.size(); i++) 
    {
        if (vec[i]->equalWithPrecision(value, precision))
        {
            res.push_back(i);
        }
    }
    return res;
}
std::vector<int> singleThreadSearch(std::vector<std::unique_ptr<Base>>& vec, double value, double precision = 0.000) {
    std::vector<int> res;
    for (int i = 0; i < vec.size(); i++)
    {
        if (vec[i]->equalWithPrecision(value, precision))
        {
            res.push_back(i);
        }
    }
    return res;
}

int main() {
    srand(time(0));
    // Вектор уникальных указателей на объекты базового класса
    std::vector<std::unique_ptr<Base>> elements;
    fillRandom(elements, 10);
    for (auto& e : elements)
    {
       e->print();
    }
    // Добавление элементов разных типов в массив
    elements.push_back(std::make_unique<IntElement>(5));
    elements.push_back(std::make_unique<DoubleElement>(5.0001));
    // Вывод элементов
    for (const auto& element : elements) {
        element->print();
        std::cout << " ";
    }
    std::cout << std::endl;
    for (const auto el : singleThreadSearch(elements, 5))
    {
        std::cout << el << " ";
    }
    std::cout << std::endl;
    elements.clear();
    return 0;
}