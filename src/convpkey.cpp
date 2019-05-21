#include "../inc/structure.hpp"
#include "../inc/parameters.hpp"
#include "../inc/functions.hpp"

//Проверка количества входных аргументов
void check_args(int argc) {
    if (argc < 2 || argc > 4) {
        throw "Wrong number of arguments\n";
    }
}

//Формирование параметров работы ЭЦП
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

int main(int argc, const char * argv[]) {

    try {

        //Обработка входных аргументов
        check_args(argc);
        SEQUENCE * paramSet = param_form(argv[1]);

        //Обработка файла
        FILE * privkey;
        privkey = (paramSet->mode == PARAM_SIZE) ?
            open_file_rb(argv[1]) : open_file_rb(argv[2]);

        //Чтение приватного ключа из файла
        u8 buff[paramSet->mode];
        read_data(buff, privkey, paramSet->mode);

        //Инициализация k
        uint1024_t k = init_1024(buff, paramSet->mode);

        //Произведение в скрученных кривых Эдвардса
        Point uv = mul(paramSet, k);
        u8 u[PARAM_SIZE];
        u8 v[PARAM_SIZE];
        init_u8(u, uv.x, paramSet->mode);
        init_u8(v, uv.y, paramSet->mode);

        //Печать открытого ключа в файл
        FILE * output;
        output = file_open(argc, argv);
        write_data(u, v, output, paramSet->mode, argc, argv);

    } catch (const char * S) {
        std::cerr << S;
    }

    return 0;
}