#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Функция для определения количества дней в месяце
int getDaysInMonth(int month) {
    if (month == 2) return 28; // Февраль всегда 28 дней
    if (month == 4 || month == 6 || month == 9 || month == 11) return 30;
    return 31;
}

bool isValidCommandCount(const string& input, int& count) {
    stringstream ss(input);
    string word;
    ss >> word;
    if (ss >> word) return false; // Если есть еще одно слово, то ввод некорректен
    for (char c : word) {
        if (!isdigit(c)) return false;
    }
    count = stoi(word);
    return count > 0;
}

int main() {
    setlocale(LC_ALL, "Russian");
    int count;
    string input;

    for (;;) {
        cout << "Введите количество команд: ";
        getline(cin, input); // Используем getline для считывания всей строки

        if (isValidCommandCount(input, count)) {
            break; // Выход из цикла, если ввод корректен
        }
        else {
            cout << "Количество команд задано неправильно" << endl;
        }
    }

    map<int, vector<string>> schedule;
    int currentMonth = 1;

    for (int i = 0; i < count; ++i) {
        string operation;
        cin >> operation;
        if (operation == "CLASS") {
            string line;
            getline(cin, line);
            stringstream ss(line);
            string day, subject;
            ss >> day >> subject;
            if (day.empty() || subject.empty() || !ss.eof()) {
                cout << "Неправильный формат команды CLASS" << endl;
                continue; // Переходим к следующей итерации цикла
            }
            int dayindex = 0;
            int daycount = 0;
            for (char c : day) {
                if (isdigit(c)) {
                    daycount = daycount * 10 + (c - '0');
                }
                else {
                    cout << "Количество дней задано неправильно" << endl;
                    daycount = 0;
                    break;
                }
            }
            if (daycount == 0) continue;

            int subjectindex = 0;
            bool isValidSubject = true;
            for (char c : subject) {
                if (!isalpha(c)) {
                    cout << "Предмет задан неправильно" << endl;
                    isValidSubject = false;
                    break;
                }
            }

            if (!isValidSubject) continue;

            // Проверяем, что день входит в текущий месяц
            if (daycount > getDaysInMonth(currentMonth) || daycount < 1) {
                cout << "В текущем месяце такой даты нет" << endl;
                continue;
            }
            // Проверяем, что предмет еще не записан на этот день
            if (find(schedule[daycount].begin(), schedule[daycount].end(), subject) != schedule[daycount].end()) {
                cout << "Этот предмет уже записан на этот день." << endl;
            }
            else {
                // Добавляем предмет на день
                schedule[daycount].push_back(subject);
            }
        }
        else if (operation == "NEXT") {
            string line;
            getline(cin, line);
            if (!line.empty()) {
                cout << "Неправильный формат команды NEXT" << endl;
                continue; // Переходим к следующей итерации цикла
            }
            // Переходим на следующий месяц
            currentMonth++;
            if (currentMonth > 12) {
                currentMonth = 1;
            }

            // Переносим пары, которые не помещаются в текущий месяц, на предпоследний день
            int lastDayOfCurrentMonth = getDaysInMonth(currentMonth);
            int penultimateDay = lastDayOfCurrentMonth - 1;
            for (auto it = schedule.begin(); it != schedule.end();) {
                if (it->first > lastDayOfCurrentMonth) {
                    // Переносим пары на предпоследний день текущего месяца, если там еще нет такого предмета
                    for (const auto& subject : it->second) {
                        if (find(schedule[penultimateDay].begin(), schedule[penultimateDay].end(), subject) == schedule[penultimateDay].end()) {
                            schedule[penultimateDay].push_back(subject);
                        }
                    }
                    it = schedule.erase(it);
                }
                else {
                    ++it;
                }
            }
        }
        else if (operation == "VIEW") {
            string line;
            getline(cin, line);
            stringstream ss(line);
            string day;
            ss >> day;
            if (day.empty() || !ss.eof()) {
                cout << "Неправильный формат команды VIEW" << endl;
                continue; // Переходим к следующей итерации цикла
            }
            int dayindex = 0;
            int daycount = 0;
            for (char c : day) {
                if (isdigit(c)) {
                    daycount = daycount * 10 + (c - '0');
                }
                else {
                    cout << "День задан неправильно" << endl;
                    daycount = 0;
                    break;
                }
            }
            if (daycount == 0) continue;
            // Проверяем, что день входит в текущий месяц

            if (daycount > getDaysInMonth(currentMonth) || daycount < 1) {
                cout << "В текущем месяце такой даты нет" << endl;
                continue;
            }

            cout << "В " << daycount << " день ";
            if (schedule.count(daycount) == 0 || schedule[daycount].empty()) {
                cout << "мы свободны" << endl;
            }
            else {
                cout << schedule[daycount].size() << " уроков в школе: ";
                for (size_t j = 0; j < schedule[daycount].size(); ++j) {
                    cout << schedule[daycount][j];
                    if (j < schedule[daycount].size() - 1) {
                        cout << ", ";
                    }
                }
                cout << endl;
            }
        }
        else {
            cout << "Неправильная команда" << endl;
            continue; // Переходим к следующей итерации цикла
        }
    }
    return 0;
}
