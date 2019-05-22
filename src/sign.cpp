#include "../inc/structure.hpp"
#include "../inc/parameters.hpp"
#include "../inc/functions.hpp"
#include "../inc/gsum.h"

void check_args(int argc) {
    if (argc < 3 || argc > 5) {
        throw "Wrong number of arguments\n";
    }
}

FILE * fileOpen(int argc, const char ** argv) {
    if (argc == 3) {
        if (strncmp(argv[1], "-s", 2) == 0) {
            throw "Wrong parameters!\n";
        } else {
            return open_file_rb(argv[2]);
        }

    } else if (argc == 4) {
        if (strncmp(argv[1], "-s", 2) == 0) {
            return open_file_rb(argv[3]);
        } else {
            return open_file_rb(argv[2]);
        }
    } else if (argc == 5) {
        return open_file_rb(argv[3]);
    } else {
        throw "File open error!\n";
    }
}

void calc_hash(FILE * file, struct ctx * hash_ctx, int size,
          u8 * data, u8* digest, size_t mode){

    size_t Mode = (mode == PARAM_SIZE) ? HASH512 : HASH256;
    init(hash_ctx, Mode);

    while ((size = data_read(file, data))) {
        update(hash_ctx, data, size);
    }

    finish(hash_ctx, digest);
}

void make_calc(SEQUENCE* paramSet, FILE * file, uint1024_t d, uint1024_t* Q) {
    u8 data[paramSet->mode];
    u8 digest[paramSet->mode];

    memset(data, 0x00, paramSet->mode);
    memset(digest, 0x00, paramSet->mode);

    int size = 0;
    struct ctx  hash_ctx;
    // uint1024_t q = paramSet->q;

    calc_hash(file, &hash_ctx, size, data, digest, paramSet->mode);
    uint1024_t hash = init_1024(digest, paramSet->mode);


    //Произведение в скрученных кривых Эдвардса
    u8 k_u8[PARAM_SIZE];

    r_s(paramSet, k_u8, d, hash, Q[0], Q[1]);
}

FILE* output_file(int argc, const char* argv[]) {
    if (argc == 3) {
        return open_file_wb("keys/file.crt");
    } else if (argc == 4) {
        if (strncmp(argv[1], "-s", 2) == 0) {
            return open_file_wb("keys/file.crt");
        } else {
            return open_file_wb(argv[3]);        
        }
    } else if (argc == 5) {
        return open_file_wb(argv[4]);
    } else {
        throw "File open error!\n";
    }
}


int main(int argc, const char* argv[]) {

    try {
        //Обработка входных аргументов
        check_args(argc);
        SEQUENCE * paramSet = param_form(argv[1]);

        // Обработка файлов
        FILE* priv_key;
        priv_key = (paramSet->mode == PARAM_SIZE) ?
            open_file_rb(argv[1]) : open_file_rb(argv[2]);

        //Чтение приватного ключа из файла
        u8 buff[paramSet->mode];
        read_data(buff, priv_key, paramSet->mode);
        uint1024_t d = init_1024(buff, paramSet->mode);

        FILE *file;
        file = fileOpen(argc, argv);

        uint1024_t Q[2];
        //Вычисление ЭЦП
        make_calc(paramSet, file, d, Q);
        u8 DS[2][paramSet->mode];
        init_u8(DS[0], Q[0], paramSet->mode);
        init_u8(DS[1], Q[1], paramSet->mode);
        fclose(file);

        //Печать ЭЦП в файл
        FILE * output;
        output = output_file(argc, argv);
        write_data(DS[0], DS[1], output, paramSet->mode, argc, argv);
    } catch (const char * S) {
        std::cerr << S;
    }

    return 0;
}