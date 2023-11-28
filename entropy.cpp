#include <iostream>
#include <cmath>
#include <filesystem>
#include <fstream>
#include <vector>
#include <iomanip>

void print_detailed_report(const std::vector<int>& counted_bytes) {
    std::cout << "Detailed Report:\n";
    std::cout << std::setw(35) << std::setfill('-') << "\n";

    for (int i = 0; i < 256; ++i) {
        if (counted_bytes[i] > 0) {
            std::cout << "Byte 0x" << 
            std::setw(2) << std::setfill('0') << 
            std::hex << i << ": " << std::setw(5) << 
            std::setfill(' ') << std::dec << counted_bytes[i] << " occurrences\n";
        }
    }

    std::cout << std::setw(35) << std::setfill('-') << "\n";
}

double calculate_entropy(const std::vector<int>& counted_bytes, std::streamsize total_length) {
    double entropy = 0.;
    double temp;

    for (int i = 0; i < 256; i++) {
        temp = static_cast<double>(counted_bytes[i]) / total_length;

        if (temp > 0.)
            entropy += temp * fabs(log2(temp));
    }

    return entropy; 
}

void process_file(const std::string& filename, bool detailed_report) {
    // 16KB chunks
    std::vector<char> buff(1024 * 16, 0);
    std::streamsize total_bytes_read;
    std::streamsize bytes_read;

    // This vector will hold how many times every possible byte value occurs
    std::vector<int> counted_bytes(256);

    // Open the file
    std::ifstream input_file(filename, std::ios::binary);
    if (input_file.fail()) {
        std::cerr << "Couldn't open \"" << filename << "\" for reading." << std::endl;
        return;
    }

    // Perform a fresh calculation
    total_bytes_read = 0;
    std::fill(counted_bytes.begin(), counted_bytes.end(), 0);

    // Read file in chunks and count the occurrences for each possible byte value (0-255)
    while (!input_file.eof()) {
        input_file.read(buff.data(), buff.size());
        bytes_read = input_file.gcount();
        total_bytes_read += bytes_read;

        for (int j = 0; j < bytes_read; j++) {
            const unsigned char count = static_cast<unsigned char>(buff[j]);
            counted_bytes[count]++;
        }
    }

    input_file.close();
    std::cout << calculate_entropy(counted_bytes, total_bytes_read) << " <= " << filename << "\n";
    if (detailed_report) {
        print_detailed_report(counted_bytes);
    }
}

void usage() {
    std::cout << "entropy v1.2 calculates the entropy of files and provides a detailed report.\n\n"
        "Usage:\n\tentropy FILE [-d]\n\n"
        "Options:\n\t-d : Print detailed report (byte-wise occurrences)\n\n"    
        "Examples:\n\tentropy image.png || entropy image.png -d\n\tentropy music.mp3 document.xls || entropy music.mp3 -d document.xls\n\tentropy *.exe\n\n"
        "For more information and bug reports, refer to https://github.com/merces/entropy\n";
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        usage();
        return 1;
    }

    bool detailed_report = false;
    if (argc > 2 && std::string(argv[2]) == "-d") {
        detailed_report = true;
    }

    // Entropy value will use two decimal places
    std::cout << std::fixed << std::setprecision(2);

    double total_entropy = 0.0;
    int num_files = 0;

    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "-d") {
            continue;
        }

        // Skip directories, symlinks, etc
        if (!std::filesystem::is_regular_file(argv[i])) {
            std::cerr << "\"" << argv[i] << "\"" << " isn't a regular file, skipping." << std::endl;
            continue;
        }

        process_file(argv[i], detailed_report);
        num_files++;
    }
    
    // Display average entropy if valid files were processed
    if (num_files == 0) {
        std::cout << "No valid files were processed." << std::endl;
    } else {
        std::cout << "\nAverage Entropy for " << num_files << " files: " << total_entropy / num_files << "\n";
    }

    return 0;
}
