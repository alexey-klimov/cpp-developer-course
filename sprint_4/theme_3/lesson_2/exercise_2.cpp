/* Задание 2
 *
 * Удобно, когда IsEmpty не пересчитывает все элементы, а другим способом отвечает, пуст стек или
 * нет. В этом задании вы сделаете подобный трюк. Напишите реализацию класса StackMin, у которого
 * кроме стандартных методов класса Stack будет дополнительный метод PeekMin. Он должен уметь
 * вернуть минимальный по значению на данный момент элемент в стеке. Важно организовать стек так,
 * чтобы для поиска минимума не пришлось перебирать все элементы в стеке. Перебора элементов не
 * должно быть ни в методе PeekMin, ни в каком другом. Возьмите эту заготовку класса и дополните её
 * своей реализацией:
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
struct StackMinElement {
    Type value;
    Type minimum;
};

template <typename Type>
ostream& operator<<(ostream& os, const StackMinElement<Type>& element) {
    os << element.value;
    return os;
}

template <typename Type>
class StackMin {
public:
    void Push(const Type& element) {
        StackMinElement<Type> new_element = {element,
                                             elements_.IsEmpty() ? element : std::min(element, PeekMin())};
        elements_.Push(new_element);
    }
    void Pop() {
        elements_.Pop();
    }
    const Type& Peek() const {
        return elements_.Peek().value;
    }
    Type& Peek() {
        return elements_.Peek().value;
    }
    void Print() const {
        elements_.Print();
    }
    uint64_t Size() const {
        return elements_.size();
    }
    bool IsEmpty() const {
        return elements_.IsEmpty();
    }
    const Type& PeekMin() const {
        return elements_.Peek().minimum;
    }
    Type& PeekMin() {
        return elements_.Peek().minimum;
    }

private:
    Stack<StackMinElement<Type>> elements_;
};

int main() {
    StackMin<int> stack;
    vector<int> values(5);
    // заполняем вектор для тестирования нашего стека
    iota(values.begin(), values.end(), 1);
    // перемешиваем значения
    random_device rd;
    mt19937 g(rd());
    shuffle(values.begin(), values.end(), g);
    // заполняем стек
    for (int i = 0; i < 5; ++i) {
        stack.Push(values[i]);
    }
    // печатаем стек и его минимум, постепенно убирая из стека элементы
    while (!stack.IsEmpty()) {
        stack.Print();
        cout << "Минимум = "s << stack.PeekMin() << endl;
        stack.Pop();
    }
}
