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
    cout << "������� ��� ����� � ���������: "
            "(���� ���������� .exe �� � ���� ������ ������ � ����� � ���, ���� ������ � ide �� � ����� cmake-build-debug)" << endl;
	string file_name;
	cin >> file_name;
	finite_state_automata automata(file_name);

    cout << "\n========================= " << "������� ���������" << " =========================\n" << endl;
    automata.transition_table();
    cout << "\n========================= " << "����� ��������������" << " =========================\n" << endl;
    automata.determinization();
    automata.transition_table();

    std::string str;
    while(str != "-1") {
        cout << "������� �����, �������������� �������� ���� ���������. \n������� eps ��� �������� ������� �����. ��� ���������� ������� -1:" << endl;
        cin >> str;
        if (automata.recognize(str) == 1) {
            cout << "�������������" << endl;
        } else {
            cout << "�� �������������" << endl;
        }
    }
}