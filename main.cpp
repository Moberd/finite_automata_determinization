#include<iostream>
#include <string>
#include <vector>
#include "finite_state_automata.h"
#include <ctime>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;
int main() {
	setlocale(LC_ALL, "RUS");
    cout << "Введите имя файла с автоматом: "
            "(если запускаете .exe то в файл должен лежать в папке с ним, если просто в ide то в папке cmake-build-debug)" << endl;
	string file_name;
	cin >> file_name;
	finite_state_automata automata(file_name);

    cout << "\n========================= " << "Таблица переходов" << " =========================\n" << endl;
    automata.transition_table();
    cout << "\n========================= " << "После детерминизации" << " =========================\n" << endl;
    automata.determinization();
    automata.transition_table();

    std::string str;
    while(str != "-1") {
        cout << "Введите слово, принадлежность которого надо проверить. \nВведите eps для проверки пустого слова. Для завершения введите -1:" << endl;
        cin >> str;
        if (automata.recognize(str) == 1) {
            cout << "Удовлетворяет" << endl;
        } else {
            cout << "Не удовлетворяет" << endl;
        }
    }
}