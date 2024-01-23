/* Задание 3
 *
 * Продолжите исследовать стек и то, как его можно реализовать.
 * В этот раз напишите класс SortedSack, который бы всегда поддерживал свои элементы в
 * отсортированном виде. При вставке сортировка должна сохраняться. На вершине стека всегда должен
 * быть минимум. Это класс чем-то похож на стек, но имеет важное отличие — принцип LIFO в нём не
 * соблюдается. Чтобы подчеркнуть различие, мы используем в названии класса английское слово sack —
 * «мешок». Не применяйте в этой задаче алгоритмы сортировки — её можно решить без них. Методы
 * класса перестанут быть такими простыми, эффективными и быстрыми, как раньше. В них могут
 * появиться условия и циклы. Это цена, которую вы платите за то, что элементы будут постоянно
 * находиться в сортированном состоянии. Используйте заготовку и допишите необходимую реализацию
 * методов Push и Pop:
 */

#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

using namespace std;

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}

template <typename Type>
class Stack {
public:
    void Push(const Type& element) {
        elements_.push_back(element);
    }
    void Pop() {
        elements_.pop_back();
    }
    const Type& Peek() const {
        return elements_.back();
    }
    Type& Peek() {
        return elements_.back();
    }
    void Print() const {
        PrintRange(elements_.begin(), elements_.end());
    }
    uint64_t Size() const {
        return elements_.size();
    }
    bool IsEmpty() const {
        return elements_.empty();
    }

private:
    vector<Type> elements_;
};

template <typename Type>
class SortedSack {
public:
    void Push(const Type& element) {
        Stack<Type> buffer;
        while(!elements_.IsEmpty() && elements_.Peek() < element) {
            buffer.Push(elements_.Peek());
            elements_.Pop();
        }
        elements_.Push(element);
        while(!buffer.IsEmpty()) {
            elements_.Push(buffer.Peek());
            buffer.Pop();
        }
    }
    void Pop() {
        elements_.Pop();
    }
    const Type& Peek() const {
        return elements_.Peek();
    }
    Type& Peek() {
        return elements_.Peek();
    }
    void Print() const {
        elements_.Print();
    }
    uint64_t Size() const {
        return elements_.Size();
    }
    bool IsEmpty() const {
        return elements_.IsEmpty();
    }
private:
    Stack<Type> elements_;
};

int main() {
    SortedSack<int> sack;
    vector<int> values(5);
    // заполняем вектор для тестирования нашего класса
    iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    random_device rd;
    mt19937 g(rd());
    shuffle(values.begin(), values.end(), g);

            // заполняем класс и проверяем, что сортировка сохраняется после каждой вставки
    for (int i = 0; i < 5; ++i) {
        cout << "Вставляемый элемент = "s << values[i] << endl;
        sack.Push(values[i]);
        sack.Print();
    }
}
