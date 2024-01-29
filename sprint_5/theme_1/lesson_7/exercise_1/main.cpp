/* Задание
 *
 * Даже теперь код оптимизирован не полностью. В прошлом уроке мы рассматривали простую оптимизацию,
 * но применили её не везде. Попробуйте угадать какую. А если не догадаетесь — загляните в
 * подсказку. Сделайте оптимизацию, которую упустили в прошлом уроке. Самостоятельно внесите в код
 * все сделанные правки, кроме неудачной, с оптимизацией функции CountPops: реверсирование вектора
 * одной строкой кода; уменьшение количества вызовов функции rand в 15 раз.
 */

#include <iostream>
#include <vector>

#include "log_duration.h"

using namespace std;

vector<int> ReverseVector(const vector<int>& source_vector) {
    return {source_vector.rbegin(), source_vector.rend()};
}

int CountPops(const vector<int>& source_vector, int begin, int end) {
    int res = 0;

    for (int i = begin; i < end; ++i) {
        if (source_vector[i]) {
            ++res;
        }
    }

    return res;
}

void AppendRandom(vector<int>& v, int n) {
    v.reserve(n);
    int step = 15;
    for (int i = 0; i < n; i += 15) {
        int number = rand();
        int count = min(step, n - i);
        for (int j = 0; j < count; ++j) {
            v.push_back((number >> 1) % 2);
        }
    }
}

void Operate() {
    LOG_DURATION("Total"s);

    vector<int> random_bits;

    // операции << для целых чисел это сдвиг всех бит в двоичной
    // записи числа. Запишем с её помощью число 2 в степени 17 (131072)
    static const int N = 1 << 17;

    // заполним вектор случайными числами 0 и 1
    
    {
        LOG_DURATION("Append random"s);
        AppendRandom(random_bits, N);
    }

    // перевернём вектор задом наперёд
    vector<int> reversed_bits;
    {
        LOG_DURATION("Reverse"s);

        reversed_bits = ReverseVector(random_bits);
    }

    {
        LOG_DURATION("Counting"s);
        // посчитаем процент единиц на начальных отрезках вектора
        for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
            double rate = CountPops(reversed_bits, 0, i) * 100. / i;
            cout << "After "s << i << " bits we found "s << rate << "% pops"s << endl;
        }
    }
}

int main() {
    Operate();
}
