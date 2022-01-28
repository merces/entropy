#include <iostream>
#include <cstdint>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <vector>

using namespace std;

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
    cout << "Usage:\n\tentropy FILE\n";
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        usage();
        return 1;
    }

    cout << fixed << std::setprecision(2);

    for (int i = 1; i < argc; i++) {
        // Skip directories, symlinks, etc
        if (!filesystem::is_regular_file(argv[i])) {
            continue;
        }

        // Open the file
        ifstream f(argv[i], ios::binary);
        if (f.fail()) {
            cerr << "Could not open \"" << argv[i] << "\" for reading.\n";
            continue;
        }

        // Get file size
        f.seekg(0, ios::end);
        streampos len = f.tellg();
        f.seekg(0, ios::beg);
        
        // Read file contents
        vector<char> buff(len);
        f.read(&buff[0], len);
        f.close();

        // Count occurrence of each possible byte, from zero to 255.
        unsigned int counted_bytes[256] = { 0 };
        for (int j = 0; j < len; j++) {
            unsigned char c = buff[j];
            counted_bytes[c]++;
        }

        cout << calculate_entropy(counted_bytes, len) << " " << argv[i] << "\n";
    }
	return 0;
}