/* Задание 1
 *
 * Внутри контейнера queue в стандартной библиотеке находится deque. Класс queue даёт доступ только
 * к определённым возможностям класса deque. Вам реализовывать класс queue с применением deque не
 * нужно. Сделайте свою реализацию, используя два стека. Вы спросите, зачем заниматься такой
 * ерундой. Вспомним задачу о нахождении минимума среди элементов стека без перебора. Структура
 * стека позволяет сделать это достаточно легко. Но структура deque так не сможет. Поэтому, если для
 * очереди понадобится поддержка поиска минимума или максимума без перебора элементов, реализация с
 * применением стеков — самая подходящая. Напишите реализацию методов класса Queue, отвечающих
 * принципу FIFO, если внутри контейнера два стека. Внимание, ваша очередь имеет лишь неконстантную
 * версию метода Front. В этом задании для доступа к первому элементу понадобится переместить
 * элементы из одного стека в другой, а в константном методе это сделать проблематично. Язык C++ в
 * некоторых случаях разрешает константным методам изменять поля класса. Эту возможность рассмотрим
 * в будущих уроках.
 */

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <stack>
#include <random>
#include <vector>

using namespace std;

template <typename Type>
class Queue {
public:
    void Push(const Type& element) {
        MakeItRightible();
        stack1_.push(element);
    }
    void Pop() {
        MakeItLeftible();
        stack2_.pop();
    }
    Type& Front() {
        MakeItLeftible();
        return stack2_.top();
    }
    uint64_t Size() const {
        return !stack1_.empty() ? stack1_.size() : stack2_.size();
    }
    bool IsEmpty() const {
        return stack1_.empty() && stack2_.empty();
    }

private:
    stack<Type> stack1_;
    stack<Type> stack2_;

    void ReverseSwap(stack<Type>& from, stack<Type>& to) {
        while (!from.empty()) {
            to.push(from.top());
            from.pop();
        }
    }
    void MakeItRightible() {
        ReverseSwap(stack2_, stack1_);
    }
    void MakeItLeftible() {
        ReverseSwap(stack1_, stack2_);
    }
};

template <typename It>
void PrintRange(It range_begin, It range_end) {
    for (auto it = range_begin; it != range_end; ++it) {
        cout << *it << " "s;
    }
    cout << endl;
}


int main() {
    Queue<int> queue;
    vector<int> values(5);
    // заполняем вектор для тестирования очереди
    iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    random_device rd;
    mt19937 g(rd());
    shuffle(values.begin(), values.end(), g);
    PrintRange(values.begin(), values.end());
    cout << "Заполняем очередь"s << endl;
    // заполняем очередь и выводим элемент в начале очереди
    for (int i = 0; i < 5; ++i) {
        queue.Push(values[i]);
        cout << "Вставленный элемент "s << values[i] << endl;
        cout << "Первый элемент очереди "s << queue.Front() << endl;
    }
    cout << "Вынимаем элементы из очереди"s << endl;
    // выводим элемент в начале очереди и вытаскиваем элементы по одному
    while (!queue.IsEmpty()) {
        // сначала будем проверять начальный элемент, а потом вытаскивать,
        // так как операция Front на пустой очереди не определена
        cout << "Будем вынимать элемент "s << queue.Front() << endl;
        queue.Pop();
    }
    return 0;
}
