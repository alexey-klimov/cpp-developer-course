/* Задание 1
 *
 * Чтобы работать со стеком по всем правилам LIFO, напишите класс, который поможет это сделать.
 * Храните элементы стека в векторе. Напишите свою реализацию методов Push, Pop, Peek, Size и
 * IsEmpty. Для удобства добавьте ещё метод Print. Элементы могут быть разных типов, поэтому нужен
 * класс-шаблон. Методов Peek два, потому что один из них будет вызываться в случае константного
 * объекта, а другой — в остальных случаях. При этом возвращаемое значение будет разным. Если объект
 * константный, нужно гарантировать, что пользователь не изменит тот объект, на который ему
 * передаётся ссылка. Поэтому в константном методе возвращается const Type&. В другом случае можно
 * спокойно вернуть ссылку без const, так как стек не будет неизменяемым объектом — пользователь
 * может свободно его менять. Этот же принцип будет выполняться всегда, когда предоставляете
 * пользователю доступ к элементам контейнера.
 */

#include <iostream>
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

int main() {
    Stack<int> stack;
    for (uint32_t i = 0; i < 10; ++i) {
        stack.Push(i);
        stack.Print();
    }
    while (!stack.IsEmpty()) {
        stack.Pop();
        stack.Print();
    }
}
