#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>
using namespace std;
#define DEBUG_MODE
const int MAX_ANIMALS = 50;

struct Animal
{
    string name;
    string type;
    int age;
    string description;
    string health;
    string status;
};

int parseAge(string& str)
{
    for (char const &c : str)
    {
        if (!isdigit(c))
        {
            throw invalid_argument("Некорректный формат числа");
        }
    }
    int age = stoi(str);
    if (age < 0)
    {
        throw out_of_range("Возраст не может быть отрицательным");
    }
    return age;
}
int LFF(string& filename, Animal Shelter[])
{
    ifstream file(filename);

    if (!file.is_open())
    {
        throw runtime_error("Файл не существует или не открывается");
    }
    string line;
    int count = 0;

    while (getline(file, line))
    {
        if (line.empty())
        {
            continue;
        }
        if (count >= MAX_ANIMALS)
        {
            cout << "Превышен лимит массива. Загружено только " << MAX_ANIMALS << " животных." << endl;
            break;
        }

        
        string fields[6];
        int fieldcount = 0;
        size_t start = 0;
        size_t end = line.find(';');
        while (end != string::npos && fieldcount < 5)
        {
            fields[fieldcount] = line.substr(start, end - start);
            fieldcount++;
            start = end + 1;
            end = line.find(';', start);
        }
        if (fieldcount == 5)
        {
            fields[fieldcount] = line.substr(start);
            fieldcount++;
        }
        if (fieldcount < 6)
        {
            throw runtime_error("Строка файла содержит недостаточное кол-во данных");
        }
        Shelter[count].name = fields[0];
        Shelter[count].type = fields[1];
        Shelter[count].age = parseAge(fields[2]);
        Shelter[count].description = fields[3];
        Shelter[count].health = fields[4];
        Shelter[count].status = fields[5];

        count++;
    }
    file.close();

#ifdef DEBUG_MODE
    cout << "[ОТЛАДКА] Успешно загружено животных из файла: " << count << endl;
#endif // DEBUG_MODE
    return count;
}
void STF(string& filename, Animal Shelter[], int count)
{
    ofstream file(filename);

    if (!file.is_open())
    {
        throw runtime_error("Не удалось открыть файл для записи");
    }

    for (int i = 0; i < count; i++)
    {
        file << Shelter[i].name << ";"
            << Shelter[i].type << ";"
            << Shelter[i].age << ";"
            << Shelter[i].description << ";"
            << Shelter[i].health << ";"
            << Shelter[i].status << "\n";
    }
    file.close();

#ifdef DEBUG_MODE
    cout << "[ОТЛАДКА] Успешно сохранено строк в файл: " << count << endl;
#endif // DEBUG_MODE

}
void printAnimal(Animal& animal)
{
    cout << "Кличка: " << animal.name << endl;
    cout << "Вид животного: " << animal.type << endl;
    cout << "Возраст: " << animal.age << endl;
    cout << "Описание: " << animal.description << endl;
    cout << "Статус здоровья: " << animal.health << endl;
    cout << "Статус пристройства: " << animal.status << endl;
}
int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    
    string filename = "animals.txt";
    Animal Shelter[MAX_ANIMALS];
    int animalCount = 0;

    try
    {
        animalCount = LFF(filename, Shelter);
    }
    catch (exception& e)
    {
        cout << "Ошибка при загрузке: " << e.what() << endl;
    }

    int choice = 0;
    while (true)
    {
        cout << "---МЕНЮ ПРИЮТА---" << endl;
        cout << "1. Поиск жиотного по кличке" << endl;
        cout << "2. Вывод животных определенного вида" << endl;
        cout << "3. Вывод животных, доступных для пристройства" << endl;
        cout << "4. Поиск животных младше заданного возраста" << endl;
        cout << "5. Изменение статуса пристройства" << endl;
        cout << "6. Сохранение данных" << endl;
        cout << "0. Выход" << endl;
        cout << "Выберите действие: ";
        cin >> choice;

        if (choice == 0)
        {
            break;
        }
        try
        {
            if (choice == 1)
            {
                string searchName;
                cout << "Введите кличку для поиска: ";
                cin >> searchName;
                bool found = false;
                for (int i = 0; i < animalCount; i ++)
                {
                    if (Shelter[i].name == searchName)
                    {
                        printAnimal(Shelter[i]);
                        found = true;
                    }
                    cout << endl;
                }
                if (!found)
                {
                    cout << "Животное по кличке " << searchName << " не найдено" << endl;
                    cout << endl;
                }
            }
            else if (choice == 2)
            {
                string searchType;
                cout << "Введите вид животного для поиска: ";
                cin >> searchType;
                bool found = false;
                for (int i = 0; i < animalCount; i++)
                {
                    if (Shelter[i].type == searchType)
                    {
                        printAnimal(Shelter[i]);
                        found = true;
                    }
                    cout << endl;
                }
                if (!found)
                {
                    cout << "Животное по с видом " << searchType << " не найдено" << endl;
                    cout << endl;
                }
            }
            else if (choice == 3)
            {
                cout << "Доступные для пристройства: " << endl;
                for (int i = 0; i < animalCount; i++) 
                {
                    if (Shelter[i].status == "В приюте" || Shelter[i].status == "Ищет дом")
                    {
                        printAnimal(Shelter[i]);
                    }
                    cout << endl;
                }
            }
            else if (choice == 4) {
                string ageStr;
                cout << "Введите максимальный возраст: ";
                cin >> ageStr;
                int maxAge = parseAge(ageStr);

                for (int i = 0; i < animalCount; i++) {
                    if (Shelter[i].age < maxAge) printAnimal(Shelter[i]);
                    cout << endl;
                }
            }
            else if (choice == 5) {
                string searchName, newStatus;
                cout << "Введите кличку животного: ";
                cin >> searchName;

                bool found = false;
                for (int i = 0; i < animalCount; i++) {
                    if (Shelter[i].name == searchName) {
                        cout << "Текущий статус: " << Shelter[i].status << endl;
                        cout << "Введите новый статус: ";
                        cin.ignore();
                        getline(cin, newStatus);
                        Shelter[i].status = newStatus;
                        found = true;
                        cout << "Статус успешно изменен!" << endl;
                        break;
                    }
                    cout << endl;
                }
                if (!found) {
                    throw logic_error("Операция невозможна: животное не зарегистрировано в системе");
                }
            }
            else if (choice == 6) {
                STF(filename, Shelter, animalCount);
           cout << "Данные успешно сохранены!" << endl;
            }
            else {
           cout << "Неверный пункт меню." << endl;
            }
        }
        catch (exception& e)
        {
            cout << "[НАЙДЕНА ОШИБКА]:" << e.what() << endl;
        }
    }
}
