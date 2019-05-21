#include "../inc/structure.hpp"
#include "../inc/parameters.hpp"
#include "../inc/functions.hpp"

//Проверка количества входных аргументов
void check_args(int argc) {
    if (argc < 2 || argc > 4) {
        throw "Wrong number of arguments\n";
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
        u8 PK[2][paramSet->mode];
        init_u8(PK[0], uv.x, paramSet->mode);
        init_u8(PK[1], uv.y, paramSet->mode);

        //Печать открытого ключа в файл
        FILE * output;
        output = file_open(argc, argv);
        write_data(PK[0], PK[1], output, paramSet->mode, argc, argv);

    } catch (const char * S) {
        std::cerr << S;
    }

    return 0;
}