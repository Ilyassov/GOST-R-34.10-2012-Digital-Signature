#include "../inc/functions.hpp"

//Проверка количества аргументов командной строки
void check_args(int argc, const char * argv[]) {
    if (argc < 2 || argc > 3) {
        throw "Wrong number of arguments\n";
    }
}

//Открытие файла куда будем записывать
FILE * open_file(int argc, const char * argv[]) {
    FILE * f;
    if ((strncmp(argv[1], "-s", 2) == 0) && (argc == 2)) {
        throw "No file name found!\n";
    }
    const char * str = argc == 2 ? argv[1] : argv[2];
    if ((f = fopen(str, "wb")) == NULL) {
        throw "File not opened!\n";
    }
    return f;
}


int main(int argc, const char * argv[]) {

    try {

        //Обработка входных аргументов
        check_args(argc, argv);
        SEQUENCE * paramSet = param_form(argv[1]);

        //Обработка файла
        FILE *f;
        f = open_file(argc, argv);

        //Массив для ответа
        u8 d[PARAM_SIZE];
        memset(d, 0, PARAM_SIZE);

        //Генерация приватного ключа
        gen_priv_key(d, paramSet);

        //Печать ключа в файл
        for (size_t i = 0; i < paramSet->mode; i++) {
            fwrite(&d[i], sizeof (u8), 1, f);
        }
        fclose(f);

    } catch(const char * S) {
        std::cerr << S;
        return(1);
    }

    return 0;
}
