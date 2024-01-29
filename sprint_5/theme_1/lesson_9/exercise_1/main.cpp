/* Задание
 *
 * Метеостанция измеряет температуру определённое количество раз в течение дня. Каждый день
 * измерения производятся в одно и то же время, и их количество одинаково. Данные, полученные в
 * течение одного дня, сохраняются в виде вектора чисел float. В результате наблюдений в течение
 * нескольких дней получается набор векторов типа vector<float>. Вычислите среднее значение
 * измерений, сделанных в одно и то же время, но в разные дни. При этом учитывайте только
 * положительные температуры. Например, если наблюдения велись 30 дней, каждый день делалось по 1440
 * измерений, то на вход поступит 30 векторов по 1440 значений. Программа должна построить новый
 * вектор из 1440 значений типа float, каждое из которых — среднее значение положительных температур
 * за соответствующую минуту. Реализуйте функцию ComputeAvgTemp. Она должна возвращать вектор,
 * который содержит среднюю положительную температуру для каждого измерения. Если для какого-либо
 * измерения все температуры неположительные, запишите для этого измерения ноль. Количество
 * измерений в день определите сами по размеру первого вектора. Для пустого набора верните пустой
 * ответ. Гарантируется, что все векторы измерений имеют одинаковый размер. Ваша реализация должна
 * быть эффективна: правильно расположите вложенные циклы, избегайте if. Учтите, что деление чисел —
 * тоже долгая операция, следует минимизировать количество делений.
 */

#include <cassert>
#include <iostream>
#include <numeric>
#include <random>
#include <string>
#include <vector>

#include "log_duration.h"

using namespace std;

vector<float> ComputeAvgTemp(const vector<vector<float>>& measures) {
    if (measures.empty()) {
        return {};
    }

    vector<float> avg_temps(measures[0].size(), 0);
    vector<float> counts(measures[0].size(), 0);

    size_t row_count = measures.size();
    size_t column_count = measures[0].size();
    for (size_t i = 0; i < row_count; ++i) {
        const auto& row = measures[i];
        for (size_t j = 0; j < column_count; ++j) {
            const float temp = row[j];
            avg_temps[j] += temp > 0 ? temp : 0;
            counts[j] += temp > 0 ? 1 : 0;
        }
    }

    for (size_t j = 0; j < column_count; ++j) {
        const float count = counts[j];
        avg_temps[j] /=  count > 0 ? count : 1;
    }

    return avg_temps;
}

vector<float> GetRandomVector(int size) {
    static mt19937 engine;
    uniform_real_distribution<float> d(-100, 100);

    vector<float> res(size);
    for (int i = 0; i < size; ++i) {
        res[i] = d(engine);
    }

    return res;
}

void Test() {
    // 4 дня по 3 измерения
    vector<vector<float>> v = {{0, -1, -1}, {1, -2, -2}, {2, 3, -3}, {3, 4, -4}};

    // среднее для 0-го измерения (1+2+3) / 3 = 2 (не учитывам 0)
    // среднее для 1-го измерения (3+4) / 2 = 3.5 (не учитывам -1, -2)
    // среднее для 2-го не определено (все температуры отрицательны), поэтому должен быть 0

    assert(ComputeAvgTemp(v) == vector<float>({2, 3.5f, 0}));
}

int main() {
    Test();

    vector<vector<float>> data;
    data.reserve(5000);

    for (int i = 0; i < 5000; ++i) {
        data.push_back(GetRandomVector(5000));
    }

    vector<float> avg;
    {
        LOG_DURATION("ComputeAvgTemp"s);
        avg = ComputeAvgTemp(data);
    }

    cout << "Total mean: "s << accumulate(avg.begin(), avg.end(), 0.f) / avg.size() << endl;
}
