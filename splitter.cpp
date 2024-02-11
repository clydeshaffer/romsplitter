#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <iomanip>
#include <algorithm>

void splitAndSaveChunks(const std::string& filePath) {
    // Open the input file
    std::ifstream inputFile(filePath, std::ios::binary);

    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl;
        return;
    }

    // Get the file size
    inputFile.seekg(0, std::ios::end);
    std::streampos fileSize = inputFile.tellg();
    inputFile.seekg(0, std::ios::beg);

    const std::size_t chunkSize = 16384;
    char buffer[chunkSize];

    // Ensure the file size is a multiple of the chunk size
    if (fileSize % chunkSize != 0) {
        std::cerr << "Error: File size is not a multiple of the chunk size." << std::endl;
        inputFile.close();
        return;
    }

    // Calculate the number of chunks
    std::size_t numChunks = fileSize / chunkSize;

    std::size_t chunkNumOffset = 256 - numChunks;

    // Read and save each chunk
    for (std::size_t i = 0; i < numChunks; ++i) {
        // Read a chunk from the input file
        inputFile.read(buffer, chunkSize);

        // Check if the chunk contains all zeros
        if (std::all_of(buffer, buffer + inputFile.gcount(), [](char c) { return c == 0; })) {
            std::cout << "Skipping chunk " << std::hex << std::uppercase << (i + chunkNumOffset) << " (all zeros)" << std::endl;
        } else {
            // Save the chunk to a new file
            std::stringstream ss;
            ss << std::hex << std::uppercase << (i + chunkNumOffset);
            std::string outputFileName = filePath + ".bank" + ss.str();
            std::ofstream outputFile(outputFileName, std::ios::binary);
            outputFile.write(buffer, inputFile.gcount());
            std::cout << "Saved chunk " << std::hex << std::uppercase << (i + chunkNumOffset) << " to " << outputFileName << std::endl;
        }
    }

    // Close the input file
    inputFile.close();
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    splitAndSaveChunks(filePath);

    return 0;
}
