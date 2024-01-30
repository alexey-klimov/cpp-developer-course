/* Задание
 *
 * Рассмотрим специальный формат для задания набора текстовых файлов. В этом формате строки будут
 * двух видов: строка без знака > задаёт название файла; последующие строки, начинающиеся с >,
 * задают его содержимое. Сам знак > в содержимое файла не входит. Разработайте функцию void
 * CreateFiles(const std::string& config_file), единственный параметр которой — путь к
 * конфигурационному файлу указанного формата. Цель этой функции — создать все файлы с нужным
 * содержимым. При чтении конфигурационного файла функцией getline учтите, что она опускает перенос
 * строки. Вам следует самостоятельно добавлять его после каждой строчки, даже если это последняя
 * строка конфигурационного файла и переноса в ней не было.
 */

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

using namespace std;

void CreateFiles(const string& config_file) {
    ifstream config_stream(config_file);

    string op;
    ofstream ofs;
    while (getline(config_stream, op)) {
        if (op[0] != '>') {
            flush(ofs);
            ofs.close();
            ofs.open(op);
        } else {
            ofs << op.substr(1, op.size() - 1) << '\n';
        }
    }
    ofs.close();
}

string GetLine(istream& in) {
    string s;
    getline(in, s);
    return s;
}

int main() {
    ofstream("test_config.txt"s) << "a.txt\n"
                                    ">10\n"
                                    ">abc\n"
                                    "b.txt\n"
                                    ">123"s;

    CreateFiles("test_config.txt"s);

    ifstream in_a("a.txt"s);
    assert(GetLine(in_a) == "10"s && GetLine(in_a) == "abc"s);

    ifstream in_b("b.txt"s);
    assert(GetLine(in_b) == "123"s);
}
