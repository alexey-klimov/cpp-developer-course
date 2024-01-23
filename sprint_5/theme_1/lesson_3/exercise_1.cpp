/* Задание
 *
 * В прошлом уроке вы работали с программой, в которой три операции. Измерьте время каждой из них.
 * Вывод в cerr должен выглядеть так: Append random: ??? ms Reverse: ??? ms Counting: ??? ms Если всё
 * сделаете правильно, время самой долгой операции будет существенно больше двух других. Попробуйте
 * угадать заранее, во сколько раз. Эту, как и все другие задачи на производительность, имеет смысл
 * тестировать только с оптимизацией в конфигурации Release. Она настраивается в среде разработки.
 * Отсутствие оптимизации и отладочные проверки могут серьёзно исказить реальную картину
 * быстродействия. Формат выходных данных Вывод в cout должен совпадать с выводом программы из
 * заготовки кода. В cerr нужно вывести три строки, содержащие изменение времени:
 *
 * Append random: ??? ms
 * Reverse: ??? ms
 * Counting: ??? ms
 */

#include <chrono>
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
    for (int i = 0; i < n; ++i) {
        // получаем случайное число с помощью функции rand.
        // с помощью (rand() % 2) получим целое число в диапазоне 0..1.
        // в C++ имеются более современные генераторы случайных чисел,
        // но в данном уроке не будем их касаться
        v.push_back(rand() % 2);
    }
}

auto Tic() {
    return chrono::steady_clock::now();
}

void Toc(const chrono::steady_clock::time_point& start_time, const string& message) {
    auto stop_time = chrono::steady_clock::now();
    auto dur = stop_time - start_time;
    cerr << message << ": "s << chrono::duration_cast<chrono::milliseconds>(dur).count() << " ms"s << endl;
}

void Operate() {
    vector<int> random_bits;

    // операция << для целых чисел это сдвиг всех бит в двоичной
    // записи числа. Запишем с её помощью число 2 в степени 17 (131072)
    static const int N = 1 << 17;

    // заполним вектор случайными числами 0 и 1
    auto start_time = Tic();
    AppendRandom(random_bits, N);
    Toc(start_time, "Append random"s);

    // перевернём вектор задом наперёд
    start_time = Tic();
    vector<int> reversed_bits = ReverseVector(random_bits);
    Toc(start_time, "Reverse"s);

    // посчитаем процент единиц на начальных отрезках вектора
    start_time = Tic();
    for (int i = 1, step = 1; i <= N; i += step, step *= 2) {
        // чтобы вычислить проценты, мы умножаем на литерал 100. типа double;
        // целочисленное значение функции CountPops при этом автоматически
        // преобразуется к double, как и i
        double rate = CountPops(reversed_bits, 0, i) * 100. / i;
        cout << "After "s << i << " bits we found "s << rate << "% pops"s << endl;
    }
    Toc(start_time, "Counting"s);
}

int main() {
    Operate();
    return 0;
}
