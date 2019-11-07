#include <iostream>
#include <fstream>
#include <exception>
#include "huffman.h"

int main(int argc, char** argv) {
    int action = -1;
    enum {
        COMPRESS,
        UNCOMPRESS
    };
    std::string filename_in, filename_out;

    try {
        for (int i = 1; i < argc; i++) {
            std::string option(argv[i]);
            if (option == "-c") {
                action = COMPRESS;
            } else if (option == "-u") {
                action = UNCOMPRESS;
            } else if (option == "-f" || option == "--file") {
                filename_in = std::string(argv[i + 1]);
                i++;
            } else if (option == "-o" || option == "--output") {
                if (i + 1 >= argc)
                    throw HuffException("Bad input");
                filename_out = std::string(argv[i + 1]);
                i++;
            } else {
                throw HuffException("Bad input");
            }
        }

        std::ifstream in(filename_in, std::ifstream::binary);
        if (!in)
            throw HuffException("Bad filename");
        std::ofstream out(filename_out, std::ofstream::binary);

        if (action == COMPRESS) {
            HuffEncode encode = HuffEncode(in);
            encode.write(out);
            encode.write_info();
        } else if (action == UNCOMPRESS) {
            HuffDecode decode = HuffDecode(in);
            decode.write(out);
            decode.write_info();
        } else {
            throw HuffException("Bad action");
        }

        return 0;
    } catch(HuffException &ex) {
        std::cout << ex.what() << std::endl;
        return 1;
    }
}
