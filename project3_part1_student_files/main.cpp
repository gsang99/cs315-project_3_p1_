#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <vector>



#include "Scanner.hpp"
#include "utils.hpp"


int main(int argc, char *argv[]) {

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    //changed multiple things to make the input able to be more flexible and take in 
    const std::string inputFileName = std::string(argv[1]);

    std::filesystem::path inputPath = inputFileName;

    std::filesystem::path dirName = inputPath.parent_path();
    std::string inputFileBaseName = inputPath.stem().string();

    // build the path to the .tokens output file.
    std::filesystem::path wordTokensFileName = dirName / (inputFileBaseName + ".tokens");


    // The next several if-statement make sure that the input file, the directory exist
    // and that the output file is writeable.
     if( error_type status; (status = regularFileExistsAndIsAvailable(inputFileName)) != NO_ERROR )
        exitOnError(status, inputFileName);


    if (error_type status; (status = directoryExists(dirName.string())) != NO_ERROR )
        exitOnError(status, dirName.string());

    if (error_type status; (status = canOpenForWriting(wordTokensFileName.string())) != NO_ERROR)
        exitOnError(status, wordTokensFileName.string());


    //constructor for input file
    Scanner fileToWords(inputPath);
    std::vector<std::string> words;


    if( error_type status; (status = fileToWords.tokenize(words)) != NO_ERROR)
	    exitOnError(status, inputFileName);

    if (error_type status; (status = writeVectorToFile(wordTokensFileName.string(), words)) != NO_ERROR)
        exitOnError(status, wordTokensFileName.string());

    return 0;
}
