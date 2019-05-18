#include "structure.hpp"
#include "parameters.hpp"
#include "functions.hpp"

void check_args(int argc, const char * argv[]) {
    if (argc != 3 && argc != 4 && argc != 5) {
        throw "Wrong number of arguments\n";
    }
}

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

FILE * open_file_rb(const char * str) {
    FILE * f;
    if ((f = fopen(str, "rb")) == NULL) {
        throw "File not opened!\n";
    }
    return f;
}

FILE * file_open(int argc, const char * argv[]) {
    if ((argc == 3) || (argc == 4 && (strncmp(argv[1], "-s", 2) == 0))) {
        return open_file_rb("file.crt");
    } else if (argc == 4 && (strncmp(argv[1], "-s", 2) != 0)) {
        return open_file_rb(argv[3]);
    } else if (argc == 5) {
        return open_file_rb(argv[4]);
    } else {
        throw "File open error!\n";
    }
}

int main(int argc, char * argv[]) {

    check_args(argc, argv);

    //Определение набора параметров
    SEQUENCE * paramSet = param_form(argv[1])

    // Обработка файлов
    FILE *priv_key;
    priv_key = (paramSet->mode == PARAM_SIZE) ?
        open_file(argc, argv[1]) : open_file(argc, argv[2]);
    FILE *file;
    file = file_open(argc, argv);

    u8 buff[PARAM_SIZE];
    memset(buff, 0, PARAM_SIZE);
    u8 Q[2][PARAM_SIZE];
    memset(Q, 0, PARAM_SIZE * 2);


    // system("./hash/g256sum hash/M1 > hash/M1ans");

    // 1 Этап
    int size = fread(buff, sizeof(u8), PARAM_SIZE, priv_key);

    // 2 Этап
    Verylong alpha = binaryToDecimal(buff, PARAM_SIZE);
    Verylong q = binaryToDecimal(paramSetA256->q, PARAM_SIZE);
    Verylong e = alpha%q;
    e = e ? one : e;

    std::cout << "alpha\t" << alpha << std::endl;
    std::cout << "q\t" << q << std::endl;
    std::cout << "e\t" << e << std::endl;

    fclose(f);

    return 0;
}

// #include "gsum.h"
//
// void make_calc(struct ctx *hash_ctx, uint8_t *data, int size,
//                uint8_t *digest, FILE * f) {
//     init(hash_ctx, HASH256);
//
//     while ((size = data_read(f, data))) {
//         update(hash_ctx, data, size);
//     }
//
//     finish(hash_ctx, digest);
//     print_h(digest, BLOCK_SIZE / 2);
// }
//
// int main(int argc, char *argv[]) {
//     try {
//
//         uint8_t data[BLOCK_SIZE];
//         int size = 0;
//         uint8_t digest[BLOCK_SIZE / 2];
//         memset(digest, 0x00, BLOCK_SIZE / 2);
//         struct ctx hash_ctx;
//
//         FILE *f;
//         if ((f = fopen(argv[1], "rb")) == NULL) {
//             std::cerr << "File not opened!\n";
//         }
//
//         make_calc(&hash_ctx, data, size, digest, f);
//
//         fclose(f);
//     }
//     catch(...) {
//         std::cerr << "Error!\n";
//     }
//     return 0;
// }
