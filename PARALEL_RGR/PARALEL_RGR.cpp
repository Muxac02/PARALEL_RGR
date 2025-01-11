﻿#include <iostream>
#include <typeinfo>
#include <vector>
#include <random>
#include <chrono>
#include <thread>
#include <mutex>
//#include <ostream>
std::mutex mutex;
//using namespace std::chrono_literals;

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
        if (std::abs(this->value - val) <= precision) {
            return true;
        }
        else {
            return false;
        }
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

void fillRandom(std::vector<std::unique_ptr<Base>>& vec, int n, int max = 10000)
{
    vec.clear();
    for (int i = 0; i < n; ++i)
    {
        int t = rand() % max;
        if (t % 2 == 0)
        {
            vec.push_back(std::make_unique<IntElement>(t));
        }
        else
        {
            double t2 = rand() % max / 10000.0;
            vec.push_back(std::make_unique<DoubleElement>(t + t2));
        }
       
    }
}

template<typename T>
std::vector<int> singleThreadSearch(std::vector<std::unique_ptr<Base>>& vec, T value, int start, int end, double precision = 0.000) {
    std::vector<int> res;
    for (int i = start; i < end; i++) 
    {
        if (vec[i]->equalWithPrecision(value, precision))
        {
            res.push_back(i);
        }
    }
    return res;
}

int pow(int a, int b) {
    if (b == 1)
        return a;
    else
        return (a * pow(a, b - 1));

}

template<typename T>
std::vector<int> multiThreadSearch(std::vector<std::unique_ptr<Base>>& vec, T value, int threadCount = 1, double precision = 0.000) {
    std::vector<int> res;
    std::vector<std::thread> threads;
    for (int i = 0; i < threadCount; i++)
    {
        int start = vec.size() / threadCount * i;
        int end = vec.size() / threadCount * (i + 1);
        threads.push_back(std::thread([&res, &vec, &value, &precision, &start, &end]() {
            std::vector <int> tmp;
            if (end == vec.size())
            {
                tmp = singleThreadSearch(vec, value, start, end - 1, precision);
            }
            else
            {
                tmp = singleThreadSearch(vec, value, start, end, precision);
            }
            mutex.lock();
            res.insert(std::end(res), std::begin(tmp), std::end(tmp));
            mutex.unlock();
        }));
    }
    for (auto &t : threads) {
        t.join();
    }
    threads.clear();
    return res;
}

int main() {
    srand(time(0));
    std::vector<std::unique_ptr<Base>> elements;
    int testNum = 50;
    for (int threadCount = 1; threadCount < 17; threadCount++)
    {
        std::cout << "-------------------------------------------------------------------\nTests for " << threadCount << " threads in search"<<std::endl;
        for (int i = 3; i < 8; i++)
        {
            //std::cout << "Vector length: " << pow(10, i) << std::endl;
            fillRandom(elements, pow(10, i), 10000);
            double averageTime = 0.0;
            for (int j = 0; j < testNum; j++)
            {
                const auto start = std::chrono::high_resolution_clock::now();
                std::vector<int> res = multiThreadSearch(elements, 500, threadCount, 0.001);
                const auto end = std::chrono::high_resolution_clock::now();
                const std::chrono::duration<double, std::milli> elapsed = end - start;
                //std::cout << "Attempt " << j + 1 << " | found " << res.size() << " elements in  " << elapsed.count() << " ms" << std::endl;
                averageTime += elapsed.count();
            }
            std::cout << "Average time for " << pow(10, i) << " elements in vector: " << averageTime / testNum << " ms" << std::endl;
        }
    }
    //for (const auto& element : elements) {
    //    element->print();
    //    std::cout << " ";
    //}
    //std::cout << "\nSearch result:\n";
    //std::vector<int> res = singleThreadSearch(elements, 500, 0.001);
    /*for (const auto el : res)
    {
        std::cout << "elements[" << el << "] = ";
        elements[el]->print();
        std::cout << std::endl;
    }
    std::cout << std::endl;*/
    //res.clear();
    elements.clear();
    return 0;
}