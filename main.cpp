#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <unordered_set>

/*
- Please submit the complete C++ source code for a program that counts the number 
of distinct unique words in a file whose name is passed as an argument to a program.
- For example, given the file content "a horse and a dog" the program must output "4"
(the word 'a' appears twice but only accounts for one distinct unique occurrence).
- The input text is guaranteed to contain only 'a'..'z' and space characters in ASCII encoding.
- The program should be able to handle large inputs (e.g. 32 GiB)
- You can assume that all unique words fit into memory when using the data structure of your choice.
- The solution must utilize all available CPU resources.
*/

constexpr uint64_t BYTES_CHUNK = 65536;

void print_set(const std::unordered_set<std::string> &data) {
    std::cout << "Size of the set: " << data.size() << std::endl;
    for (auto &x : data)
        std::cout << x << " ";
    std::cout << std::endl;
}

std::unordered_set<std::string> result_set;

unsigned countUniqueWords(std::string words, unsigned bytesRead) {
    std::istringstream iss(words);

    std::copy_n(std::istream_iterator<std::string>(iss),
                bytesRead,
                std::inserter(result_set, std::end(result_set)));

    // print_set(result_set);

    return result_set.size();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    std::ifstream fin;
    fin.open(fileName, std::ios::in | std::ios::binary | std::ios::ate);
    if (!fin) {
        std::cerr << "Error: open file for input failed!" << std::endl;
        return 1;
    }

    const uint64_t fileLength = fin.tellg();
    std::ifstream::pos_type pos = 0;
    fin.seekg(pos, std::ios::beg);  // Move to the begin of file
    std::cout << "File length: " << fileLength << '\n';

    while (!fin.eof()) {
        // read chunk
        std::string contents(BYTES_CHUNK, ' ');
        fin.read(&contents[0], BYTES_CHUNK);
        auto bytesRead = fin.gcount();

        // std::cout << contents.size() << std::endl;
        // std::cout << "[" << contents << "]" << std::endl;
        // std::cout << "bytesRead: " << bytesRead << '\n';

        // pass to processing function data and real size
        countUniqueWords(std::move(contents), bytesRead);
        // std::cout << "----------------------------------\n";

        // seek to the nearest space on the left
        while (!fin.eof() && fin.peek() != ' ') {
            fin.seekg(-1, std::ios::cur);
            // std::cout << "Char read in while: " << fin.peek() << std::endl;
        }
    }

    unsigned uniqueWords = result_set.size();
    std::cout << "uniqueWords: " << uniqueWords << std::endl;

    return 0;
}