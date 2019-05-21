#include "../inc/functions.hpp"

//Проверка количества аргументов командной строки
void check_args(int argc, const char * argv[]) {
    if (argc < 2 || argc > 3) {
        throw "Wrong number of arguments\n";
    }
}

//Формирование набора параметров
SEQUENCE * param_form(const char * param_set) {
    if (strncmp(param_set, "-s", 2) == 0) {
        return new SEQUENCE(
            pA256, aA256, bA256, eA256, dA256, mA256,
            qA256, xA256, yA256, uA256, vA256, PARAM_SIZE/2);
    } else {
        return new SEQUENCE(
            pA512, aA512, bA512, eA512, dA512, mA512,
            qA512, xA512, yA512, uA512, vA512, PARAM_SIZE);
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
