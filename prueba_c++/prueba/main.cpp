#include <iostream>
#include "header.h"

using namespace std;

int main()
{
    double rate = 44100.9;
    int length = 256*12;
    double threshold_ratio = 0.9;
    std::string in_file("prueba.txt");
    std::string out_corr("corr.txt");
    std::string out_raw("raw.txt");

    vector<double> data(length, 0);

    std::ifstream input(in_file);

    for (int iter = 0; iter < 1; iter++) {

        uint32_t i = 0;
        for (i; i < length; i++) {
            input >> data[i];
        };

        //std::vector<double> data = load_data(in_file, length);
        std::vector<double> corr = compute_autocorrelation(data);

        save_data(out_corr, corr);
        save_data(out_raw, data);

        std::cout << detect_frequency(data, threshold_ratio) << std::endl;

    }

    input.close();

}



