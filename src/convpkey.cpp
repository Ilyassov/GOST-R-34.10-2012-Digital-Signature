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

        // Нужно, чтобы file.pub был создан
        // FILE * output;
        // output = file_open(argc, argv);

        u8 buff[paramSet->mode];
        memset(buff, 0, paramSet->mode);
        size_t data_read = 0;
        while (data_read != paramSet->mode) {
            data_read += fread(buff, sizeof(u8), paramSet->mode, privkey);
        }

        // printf("buff:\n");
        // print(buff, paramSet->mode);

        uint1024_t k;
        uint1024_t p;
        k = init_1024(buff, paramSet->mode);
        p = paramSet->p;
        
        Point uv = mul(paramSet, k);

        // std::cout << "uv\n";
        // std::cout << std::hex << uv << std::endl;

        // Point xy = convert_xy_to_uv(paramSet, uv);
        // std::cout << "xy\n";
        // std::cout << std::hex << xy << std::endl;

    } catch (const char * S) {
        std::cerr << S;
    }

    return 0;
}

/*
*
*ANSWER:
*private key d:
*3065E25BDC52EB2821E122D4206E42B9F8C9CBD0DF169A8AB2DC3103242845C3B67DE17EEBC41AE
*489ACC6DAEC6A478D659FCCED00D2638E05FD8167B327F30F
*public key Q:
*2CCB8B63F2C4E28426BFD68F60FAED5F663415AE9DCA0D63E7B2BC55A54E0F1CA590528D3EEF104
*811CAACCBA1E7327D41ABFD2BF1B6AA6003A2017BD838515F
*8D03F3C8E78826B624A04FE44AE4C1A3070AC51C12BA6926B0741F6AD1B7BDD7C05FD337029074E
*C9DA1062337FA424D5D216230D25DE284C32E831CEE3A6BBB
*digest for M1:
*2FDBDEC3E20060DE92881A852551A7C7178052E70A4A6B4FC432E80C457E48FCB1DD71CF0C14F61
*FD697DCA62A6747D93AB2C8C0A3318FD6C29B97B0823D2211
*1E9C9220BAC3286093C0AFC8E800EF175885C6505150E6CC8804DDB5836301EC20AAE9F6399551B
*A380A03B347BE25456FAB600B96A01BF063BE1158D24C720D
*digest for M2:
*2FDBDEC3E20060DE92881A852551A7C7178052E70A4A6B4FC432E80C457E48FCB1DD71CF0C14F61
*FD697DCA62A6747D93AB2C8C0A3318FD6C29B97B0823D2211
*2E4A19878938C9B8B6C6801AE2FF366E660292BCD4737CF5DDF2D1A1BCC25256174AB7AB3248EB0
*F299334CFB47D6CC833B55E5AC087178CCB36A08595E3E528
*
*MY ANSWER:
*uv
*778FF5D67F7B905F0B10DE61A5CD838BA1D6EB5CDFDAA8EAA7164E593CC3EB981F3AB5B9566A214
*7F43B262F030CF683FC1C8B79135C4C21241C15CD882C7C1B
*6911043A44D650190DCC2878149FB7831408A8D16E03D8835EF9D91317A897C489EEB941DDC5003
*265CFB798A9C264D2F3EBB4FB2F6C62155B18FBD19B00DB05
*xy
*B049F93D6312FBB788F0B4941AF95E75EAF07E54AB02C255780AB5AA00DC96BBB5FEDC849122E3D
*D415331BEB2031292E4D7A96E3337C3666A4B1319810F0D4C
*36BDC5AA927FDCA6F1B865CE587B41848D81AB9FB17C19CEE7003E4CA63CB818E4CEFF212A386D1
*AE774B9B38EA60FA35423692343C116F7081698EC2104E9
*
*/