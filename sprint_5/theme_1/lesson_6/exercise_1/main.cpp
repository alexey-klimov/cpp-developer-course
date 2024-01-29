/* Задание
 * Напишите программу, которая будет сравнивать четыре реализации реверсирования вектора:
 * наивную (Naive), со вставками в начало;
 * хорошую (Good), со вставками в конец;
 * отличную (Best), с инициализацией двумя итераторами;
 * вашу (Your): она изначально создаёт вектор из нужного числа элементов и заполняет его с
 * последнего элемента. Прочитайте из cin одно число — размер вектора. Создайте вектор такого
 * размера, заполните его случайными числами и произведите сравнение нескольких версий
 * реверсирования: Если размер вектора меньше либо равен 100'000, сравните наивную и хорошую
 * реализацию. Если размер вектора больше 100'000, сравните хорошую с отличной и вашей. Сравнение
 * заключается в измерении и выводе времени работы операций.
 */

#include "log_duration.h"

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

vector<int> ReverseVector(const vector<int>& source_vector) {
    vector<int> res;

    for (int i : source_vector) {
        res.insert(res.begin(), i);
    }

    return res;
}

vector<int> ReverseVector2(const vector<int>& source_vector) {
    vector<int> res;

    // будем проходить source_vector задом наперёд
    // с помощью обратного итератора
    for (auto iterator = source_vector.rbegin(); iterator != source_vector.rend(); ++iterator) {
        res.push_back(*iterator);
    }

    return res;
}

vector<int> ReverseVector3(const vector<int>& source_vector) {
    return {source_vector.rbegin(), source_vector.rend()};
}

vector<int> ReverseVector4(const vector<int>& source_vector) {
    vector<int> res(source_vector.size());

    for (int i = res.size(); i >= 1; --i) {
        res[i - 1] = source_vector[res.size() - i];
    }

    return res;
}

void Operate() {
    vector<int> rand_vector;
    int n;

    cin >> n;
    rand_vector.reserve(n);

    for (int i = 0; i < n; ++i) {
        rand_vector.push_back(rand());
    }

    if (n <= 100'000)
    {
        {
            LOG_DURATION("Naive"s);
            vector<int> reverse_vector = ReverseVector(rand_vector);
        }
        {
            LOG_DURATION("Good"s);
            vector<int> reverse_vector = ReverseVector2(rand_vector);
        }
    } else {
        {
            LOG_DURATION("Good"s);
            vector<int> reverse_vector = ReverseVector2(rand_vector);
        }
        {
            LOG_DURATION("Best"s);
            vector<int> reverse_vector = ReverseVector3(rand_vector);
        }
        {
            LOG_DURATION("Your"s);
            vector<int> reverse_vector = ReverseVector4(rand_vector);
        }
    }
}

int main() {
    Operate();
    return 0;
}
