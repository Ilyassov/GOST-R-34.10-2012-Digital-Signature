#include "../inc/structure.hpp"
#include "../inc/parameters.hpp"
#include "../inc/functions.hpp"
#include "../inc/gsum.h"

void check_args(int argc) {
    if (argc < 3 || argc > 5) {
        throw "Wrong number of arguments\n";
    }
}

FILE * ds_open(int argc, const char ** argv) {
    if (argc == 3) {
        if (strncmp(argv[1], "-s", 2) == 0) {
            throw "Wrong parameters!\n";
        } else {
            return open_file_rb("keys/file.crt");
        }

    } else if (argc == 4) {
        if (strncmp(argv[1], "-s", 2) == 0) {
            return open_file_rb("keys/file.crt");
        } else {
            return open_file_rb(argv[3]);
        }
    } else if (argc == 5) {
        return open_file_rb(argv[4]);
    } else {
        throw "File open error!\n";
    }
}

FILE *  fileOpen(int argc, const char ** argv) {
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

void calc_hash(SEQUENCE* paramSet, FILE * file, u8 * digest){

    u8 data[PARAM_SIZE];

    memset(data, 0x00, PARAM_SIZE);
    int size = 0;

    memset(digest, 0x00, paramSet->mode);

    struct ctx hash_ctx;

    init(&hash_ctx, (paramSet->mode == PARAM_SIZE) ? HASH512 : HASH256);

    while ((size = data_read(file, data))) {
        update(&hash_ctx, data, size);
    }

    finish(&hash_ctx, digest);
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

bool check_ds(uint1024_t r, uint1024_t s,
                     uint1024_t alpha, SEQUENCE* paramSet, Point p) {
    uint1024_t q = paramSet->q;
    if (r > q || s > q) {
        return false;
    }
    uint1024_t e = alpha % q;
    e = e == 0 ? 1 : e;
    uint1024_t v = inverse(e, q);
    uint1024_t z1 = (s * v) % q;
    uint1024_t z2 = negative(((r * v) % q), q);
    uint1024_t z(0);
    SEQUENCE Q(paramSet->p, paramSet->a, paramSet->b, paramSet->e, paramSet->d,
               paramSet->m, paramSet->q, paramSet->x, paramSet->y, p.x, p.y,
               paramSet->mode);
    Point z1P = muL(Point(paramSet->u, paramSet->v), z1, paramSet->p,
                          paramSet->d, paramSet->e);
    Point z2Q = muL(p, z2, paramSet->p, paramSet->d, paramSet->e);
    Point C = sum_p(z1P, z2Q, paramSet->p, paramSet->d, paramSet->e);
    C = convert_uv_to_xy(paramSet, C);
    uint1024_t R = C.x % q;
    if (R == r) {
        return true;
    }
    return false;
}

int main(int argc, const char* argv[]) {

    try {
        //Обработка входных аргументов
        // check_args(argc);
        SEQUENCE * paramSet = param_form(argv[1]);

        // Обработка файлов
        FILE* pub_key;
        pub_key = (paramSet->mode == PARAM_SIZE) ?
            open_file_rb(argv[1]) : open_file_rb(argv[2]);

        //Чтение публичного ключа из файла
        u8 buff[2][paramSet->mode];
        read_bufs(buff[0], buff[1], pub_key, paramSet->mode);
        uint1024_t x = init_1024(buff[0], paramSet->mode);
        uint1024_t y = init_1024(buff[1], paramSet->mode);

        FILE* sign_file;
        sign_file = ds_open(argc, argv);
        //Чтение публичного ключа из файла
        u8 buf[2][paramSet->mode];
        read_bufs(buf[0], buf[1], sign_file, paramSet->mode);
        uint1024_t r = init_1024(buf[0], paramSet->mode);
        uint1024_t s = init_1024(buf[1], paramSet->mode);

        FILE *file;
        file = fileOpen(argc, argv);

        //Вычисление хэша
        u8 hash[paramSet->mode];
        calc_hash(paramSet, file, hash);
        uint1024_t alpha = init_1024(hash, paramSet->mode);
        fclose(file);

        //Проверка ЭЦП
        if (check_ds(r, s, alpha, paramSet, Point(x, y))) {
            std::cout << "CORRECT!\n";
        } else {
            std::cout << "WRONG!\n";
        }

    } catch (const char * S) {
        std::cerr << S;
    }

    return 0;
}