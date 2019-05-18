#include "../inc/structure.hpp"
#include "../inc/parameters.hpp"
#include "../inc/functions.hpp"
#include <time.h>

//Проверка количества входных аргументов
void check_args(int argc) {
    if (argc < 2 || argc > 4) {
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
        throw str;
    }
    return f;
}

FILE * file_open(int argc, const char * argv[]) {
    if ((argc == 2) || (argc == 3 && (strncmp(argv[1], "-s", 2) == 0))) {
        return open_file_rb("file.pub");
    } else if (argc == 3 && (strncmp(argv[1], "-s", 2) != 0)) {
        return open_file_rb(argv[2]);
    } else if (argc == 4) {
        return open_file_rb(argv[4]);
    } else {
        throw "File open error!\n";
    }
}

int main(int argc, const char * argv[]) {

    try {
        check_args(argc);

        SEQUENCE * paramSet = param_form(argv[1]);
        FILE * privkey;
        privkey = (paramSet->mode == PARAM_SIZE) ?
            open_file_rb(argv[1]) : open_file_rb(argv[2]);

        // FILE * output;
        // output = file_open(argc, argv);

        u8 buff[paramSet->mode];
        memset(buff, 0, paramSet->mode);
        size_t data_read = 0;
        while (data_read != paramSet->mode) {
            data_read += fread(buff, sizeof(u8), paramSet->mode, privkey);
        }

        printf("buff:\n");
        print(buff, paramSet->mode);
        
        
    } catch (const char * S) {
        std::cerr << S;
    }

    return 0;
}