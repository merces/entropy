#include <iostream>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <vector>

double calculate_entropy(const unsigned int counted_bytes[256], const std::streamsize total_length) {
    double entropy = 0.;
    double temp;

    for (int i = 0; i < 256; i++) {
        temp = static_cast<double>(counted_bytes[i]) / total_length;

        if (temp > 0.) {
            entropy += temp * fabs(log2(temp));
        }
    }

    return entropy;
}

void usage() {
    std::cout << "entropy calculates the entropy of files, but you need to provide it with a file. :)\n\n"
        "Usage:\n\tentropy FILE\n\n"
        "Examples:\n\tentropy image.png\n\tentropy music.mp3 document.xls\n\tentropy *.exe\n\n"
        "For more information and bug reporting, refer to https://github.com/merces/entropy\n";
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage();
        return 1;
    }

    // Entropy will have two decimal places
    std::cout << std::fixed << std::setprecision(2);

    // 16KB chunks
    std::vector<char> buff(1024*16, 0);
    std::streamsize total_bytes_read = 0;
    std::streamsize bytes_read;
    unsigned char count;

    // Count occurrence of each possible byte, from zero to 255.
    unsigned int counted_bytes[256] = {};

    for (int i = 1; i < argc; i++) {
        // Skip directories, symlinks, etc
        if (!std::filesystem::is_regular_file(argv[i])) {
            std::cerr << "\"" << argv[1] << "\"" << " isn't a regular file, skipping." << std::endl;
            continue;
        }

        // Open the file
        std::ifstream input_file(argv[i], std::ios::binary);
        if (input_file.fail()) {
            std::cerr << "Couldn't open \"" << argv[i] << "\" for reading." << std::endl;
            continue;
        }

        // Read file in chunks and count the occurrences of each possible byte (0-255)
        while (!input_file.eof()) {
            input_file.read(buff.data(), buff.size());
            bytes_read = input_file.gcount();
            total_bytes_read += bytes_read;

            for (int j = 0; j < bytes_read; j++) {
                count = static_cast<unsigned char> (buff[j]);
                counted_bytes[count]++;
            }
        }
        input_file.close();
        std::cout << calculate_entropy(counted_bytes, total_bytes_read) << " " << argv[i] << "\n";
    }
	return 0;
}

