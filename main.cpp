
#include <algorithm>
#include <fstream>
#include <future>
#include <iostream>
#include <iterator>
#include <mutex>
#include <sstream>
#include <unordered_set>
#include <vector>

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

constexpr uint64_t BYTES_CHUNK = 262144;

void print_set(const std::unordered_set<std::string> &data) {
    std::cout << "Size of the set: " << data.size() << std::endl;
    for (auto &x : data)
        std::cout << x << " ";
    std::cout << std::endl;
}

static std::mutex setMutex;
static std::unordered_set<std::string> result_set;

static void countUniqueWords(std::string words, unsigned bytesRead) {
    std::unordered_set<std::string> tempSet;
    std::istringstream iss(words);

    std::copy_n(std::istream_iterator<std::string>(iss),
                bytesRead,
                std::inserter(tempSet, std::end(tempSet)));

    // Critical section to insert filtered words into the common set
    // This is a bottle neck for real multithread performing
    {
        std::lock_guard<std::mutex> l(setMutex);
        result_set.insert(tempSet.begin(), tempSet.end());
    }
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

    std::vector<std::future<void>> futures;
    static size_t counter = 0;

    while (!fin.eof()) {
        // read chunk of data to the string
        std::string contents(BYTES_CHUNK, ' ');
        fin.read(&contents[0], BYTES_CHUNK);
        auto bytesRead = fin.gcount();

        // move string with workds to the separate task
        futures.emplace_back(std::async(std::launch::async,
                                        countUniqueWords,
                                        std::move(contents),
                                        bytesRead));

        if (++counter % 500 == 0)
            std::cout << counter << " of futures created\n";

        // seek to the nearest space on the left
        while (!fin.eof() && fin.peek() != ' ') {
            fin.seekg(-1, std::ios::cur);
        }
    }

    std::cout << "futures vector size: " << futures.size() << "\n";
    for (auto &fut : futures) {
        fut.get();
        if (--counter % 500 == 0)
            std::cout << counter << " futures left\n";
    }

    auto uniqueWords = result_set.size();
    std::cout << "Unique words found: " << uniqueWords << std::endl;

    return 0;
}