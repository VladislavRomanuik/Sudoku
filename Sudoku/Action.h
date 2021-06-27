#pragma once
#include <vector>
#include <functional>

using namespace std;

// класс событие, на которое мы моем указать спискок подписчиков-функций и они будут вызыватьс€ в определенный момент
class Action
{
private:
    void Empty() {} // конструктор по умолчанию
    vector<function<void()>> funcPtrs; // список указателей на фукнции

public:
    void operator()() // перегружаем оператор скобочек, как бы имитиру€ вызов функции (пишем им€ объекта класса и скобочки и вызываетс€ этот оператор)
    {
        for (auto f : funcPtrs) // проходим по всему списку фукнций 
            f(); // и вызваем эти функции
    }

    void operator+=(function<void()> f) // перегруженный оператор += дл€ добавлени€ указател€ на функцию в список
    {
        funcPtrs.push_back(f);
    }
};

