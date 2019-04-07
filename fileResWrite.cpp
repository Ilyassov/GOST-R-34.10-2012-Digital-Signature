#include "structure.hpp"
#include "parameters.hpp"
#include "functions.hpp"

void check_args(int argc, const char * argv[]) {
    if (argc != 3) {
        throw "Too few arguments\n";
    }
}

FILE * open_file(const char * str) {
    FILE * f;
    if ((f = fopen(str, "rb")) == NULL) {
        throw "File not opened!\n";
    }
    return f;
}

int param_form(const char * param_set) {
    if (strncmp(param_set, "id-tc26-gost-3410-2012-256-paramSetA", 36) == 0) {
        return PARAM_SIZE/2;
    } else if (strncmp(param_set,
            "id-tc26-gost-3410-2012-512-paramSetC", 36) == 0) {
        return PARAM_SIZE;
    } else {
        throw "Wrong paramSet\n";
    }
}

int main(int argc, const char * argv[]) {

    try {
        check_args(argc, argv);
        //Обработка файла
        FILE *f;
        f = open_file(argv[2]);

        //Массив для ответа
        int size = param_form(argv[1]);
        u8 d[size];
        memset(d, 0, size);
        fread(d, sizeof(u8), size, f);
        print(d, size);

        fclose(f);

    } catch(const char * S) {
        std::cerr << S;
        return(1);
    }
    return 0;
}
