// Каркас агента: читает журнал событий построчно и считает строки.
//
// Это заготовка занятия 1.1, а не решение. Детектов она не ищет — их вы
// добавите здесь же, в отмеченном месте ниже. Формат строки детекта, список
// признаков и правило про их порядок заданы в постановке занятия: по ним
// сравниваются эталоны.
//
// Весь код лежит в main, и на этом занятии так и надо: функции появятся
// на занятии 1.2, ссылки — на 1.3. Разбор аргументов, коды возврата и флаг
// --quiet — часть задания.
//
// Запуск:
//   nano-edr <журнал.log>
#include <cstdio>
#include <fstream>
#include <print>
#include <string>
#include <map>
#include <vector>

int main(int argc, char** argv) {
    // Аргументы разбираются грубо: путь к журналу и ничего больше. Остальное,
    // включая --quiet, добавляется по заданию.
    if (argc < 2) {
        std::print(stderr, "использование: nano-edr <журнал.log>\n");
        return 2;
    }
    bool flag = true;
    if(argc == 3){
        if(std::string(argv[2]) == "--quiet")
        {
            flag = false;
        }
    }
    std::ifstream log(argv[1]);
    if (!log) {
        std::print(stderr, "не удалось открыть журнал: {}\n", argv[1]);
        return 2;
    }

    long long lines = 0;
    long long comments = 0;
    std::string line;
    std::map <std::string,int> types;
    std::vector <std::string> warnings{"wscript.exe",".locked","certutil.exe","\\Startup\\"};
    int ind_t;
    std::string type;
    while (std::getline(log, line)) {
        // Счётчик увеличивается до всех проверок: он считает строки файла,
        // а не события. Номер, посчитанный по событиям, бесполезен — по нему
        // нельзя открыть файл и посмотреть.
        ++lines;

        // Строки-комментарии в журнале начинаются с '#'. Они не события,
        // и детекта по ним быть не должно.
        if (!line.empty() && line[0] == '#') {
            ++comments;
            continue;
        }
        type="";
        ind_t = line.find("type=");
        while(line[ind_t]!=' ')
            {
                type+=line[ind_t];
                ind_t+=1;
            }
        //std::print("type {} str === {}\n",lines,type);
        types[type]++;
        for(int i=0;i<4;++i){
            if(line.find(warnings[i])!=std::string::npos){
                std::print("[DETECT] строка {}, признак {}: {}\n",lines,warnings[i],line);
            }
    }
}
    if(flag){
        std::print("всего событий {}\n", lines - comments);
        for(const auto& [type,amount] : types)
        {
            std::print("type = {} || count events with this type - {}\n",type,amount);
        }
    }
    return 0;
}