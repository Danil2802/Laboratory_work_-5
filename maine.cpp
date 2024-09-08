#include <iostream>
#include <queue>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

struct Patient {
    int duration;
    std::string ticketNumber;
};

int main() {
    setlocale(LC_ALL, "Russian");
    int Count = 0;
    std::string windowCount;

    for (;;) {
        std::cout << ">>> Введите кол-во окон:\n>>> ";
        std::cin >> windowCount;

        Count = 0;
        for (int index = 0; index < windowCount.length(); ++index) {
            char c = windowCount[index];
            if (isdigit(c)) {
                Count = Count * 10 + (c - '0');
                if (Count == 0) {
                    std::cout << ">>> Не балуетесь" << std::endl;
                    break;
                }
            }
            else {
                std::cout << ">>> Не балуетесь" << std::endl;
                break;
            }
        }
        if (Count > 1 && windowCount.length() == windowCount.length()) {
            break;
        }
    }

    std::queue<Patient> patientQueue;
    std::vector<int> windowTimes(Count, 0); // Время обслуживания каждого окна
    int ticketNumber = 1; // Начальный номер талона

    std::string command;
    for (;;) {
        std::cout << "<<< ";
        std::cin >> command;

        if (command == "ENQUEUE") {
            std::string duration;
            int durationCount = 0;

            for (;;) {
                std::cin >> duration;

                durationCount = 0;
                for (int index = 0; index < duration.length(); ++index) {
                    char c = duration[index];
                    if (isdigit(c)) {
                        durationCount = durationCount * 10 + (c - '0');
                        if (durationCount == 0) {
                            std::cout << ">>> Не балуетесь" << std::endl;
                            break;
                        }
                    }
                    else {
                        std::cout << ">>> Не балуетесь" << std::endl;
                        break;
                    }
                }
                if (durationCount > 0 && duration.length() == duration.length()) {
                    // Генерируем номер талона
                    std::stringstream ticketStream;
                    ticketStream << "T" << std::setw(3) << std::setfill('0') << ticketNumber++;
                    std::string ticket = ticketStream.str();

                    // Добавляем посетителя в очередь
                    patientQueue.push({ durationCount, ticket });
                    std::cout << ">>> " << ticket << std::endl;
                    break;
                }
                else {
                    break;
                }
            }
        }

        else if (command == "DISTRIBUTE") {
            // Очищаем текущие данные о времени обслуживания окон
            std::fill(windowTimes.begin(), windowTimes.end(), 0);

            // Распределяем посетителей по окнам
            std::vector<std::vector<std::string>> windowQueues(Count);
            for (; !patientQueue.empty();) {
                // Находим окно с минимальным временем обслуживания
                int minTimeIndex = 0;
                for (int i = 1; i < Count; ++i) {
                    if (windowTimes[i] < windowTimes[minTimeIndex]) {
                        minTimeIndex = i;
                    }
                }

                // Добавляем посетителя в очередь окна
                Patient patient = patientQueue.front();
                patientQueue.pop();
                windowQueues[minTimeIndex].push_back(patient.ticketNumber);
                windowTimes[minTimeIndex] += patient.duration;
            }

            // Выводим распределение посетителей по окнам
            for (int i = 0; i < Count; ++i) {
                std::cout << ">>> Окно " << (i + 1) << " (" << windowTimes[i] << " минут): ";
                for (size_t j = 0; j < windowQueues[i].size(); ++j) {
                    std::cout << windowQueues[i][j];
                    if (j < windowQueues[i].size() - 1) {
                        std::cout << ", ";
                    }
                }
                std::cout << std::endl;
            }
        }
        else if (command == "EXIT") {
            break;
        }
        else {
            std::cout << "Неизвестная команда." << std::endl;
        }
    }

    return 0;
}
