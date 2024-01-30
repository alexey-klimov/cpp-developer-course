/* Задание
 *
 * Разработайте класс StreamUntier, который выполняет следующие действия:
 * В конструкторе — «отвязывание» потока, переданного в конструктор как аргумент.
 * В деструкторе — «привязывание» того потока, который был отвязан.
 * В этом уроке вы столкнулись с указателями — объектами C++, которые изучите позже в курсе. Тип
 * указателя содержит звёздочку. Вы видели такой тип в параметре main в уроке. Метод потока tie тоже
 * оперирует с указателем — ostream*. Именно им был тип переменной tied_before, выведенный
 * автоматически в выражении auto tied_before = cin.tie(nullptr);. Интерфейс класса StreamUntier дан
 * в заготовке кода. Добавьте конструктор, деструктор и, при необходимости, дополнительные поля.
 */

#include <iostream>

#include "log_duration.h"

using namespace std;

class StreamUntier {
public:
    StreamUntier(istream& is) : is_(is), tied_before_(is.tie(nullptr)) {}
    ~StreamUntier() {
        is_.tie(tied_before_);
    }

private:
    istream& is_;
    ostream* tied_before_;
};

int main() {
    LOG_DURATION("\\n with tie"s);

    StreamUntier guard(cin);
    int i;
    while (cin >> i) {
        cout << i * i << "\n"s;
    }

    return 0;
}
