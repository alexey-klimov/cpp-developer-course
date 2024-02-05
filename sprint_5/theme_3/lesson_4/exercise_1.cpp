#include <cassert>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// реализуйте эту функцию:
size_t GetFileSize(string file) {
    ifstream file_stream(file, ios::in);
    size_t file_size = 0;
    return (file_size = ifstream(file, ios::in).seekg(0, ios::end).tellg()) ? file_size : string::npos;
}

int main() {
    ofstream("test.txt") << "123456789"s;
    assert(GetFileSize("test.txt"s) == 9);
    cout << GetFileSize("test.txt"s) << endl;

    ofstream test2("test2.txt"s);
    test2.seekp(1000);
    test2 << "abc"s;
    test2.flush();

    // assert(GetFileSize("test2.txt"s) == 1003);
    cout << GetFileSize("test2.txt"s) << endl;

    assert(GetFileSize("missing file name"s) == string::npos);

    cout << GetFileSize("missing file name"s) << endl;
}
