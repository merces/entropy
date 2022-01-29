#include <iostream>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <vector>

double calculate_entropy(const unsigned int counted_bytes[256], const size_t total_length) {
    double entropy = 0.;

    for (int i = 0; i < 256; i++) {
        double temp = (double)counted_bytes[i] / total_length;

        if (temp > 0.) {
            entropy += temp * fabs(log2(temp));
        }
    }

    return entropy;
}

void usage() {
    std::cout << "Usage:\n\tentropy FILE\n";
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage();
        return 1;
    }

    std::cout << std::fixed << std::setprecision(2);

    for (int i = 1; i < argc; i++) {
        // Skip directories, symlinks, etc
        if (!std::filesystem::is_regular_file(argv[i])) {
            continue;
        }

        // Open the file
        std::ifstream f(argv[i], std::ios::binary);
        if (f.fail()) {
            std::cerr << "Could not open \"" << argv[i] << "\" for reading.\n";
            continue;
        }

        // 16KB chunks
        std::vector<char> buff(1024*16, 0);
        std::streamsize total = 0;

        // Count occurrence of each possible byte, from zero to 255.
        unsigned int counted_bytes[256] = { 0 };

        // Read file in chunks and count the occurrences of each possible byte (0-255)
        while (!f.eof()) {
            f.read(buff.data(), buff.size());
            auto s = f.gcount();
            total += s;

            for (int j = 0; j < s; j++) {
                unsigned char c = buff[j];
                counted_bytes[c]++;
            }
        }

        f.close();
        std::cout << calculate_entropy(counted_bytes, total) << " " << argv[i] << "\n";
    }
	return 0;
}