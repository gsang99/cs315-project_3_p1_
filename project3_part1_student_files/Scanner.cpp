//
// Created by Ali Kooshesh on 9/27/25.
//

#include "Scanner.hpp"

#include <utility>
#include <iostream>
#include <fstream>
#include <cctype>
#include "utils.hpp"

Scanner::Scanner(std::filesystem::path inputPath) {
    // You complete this...
    //we are just grabbing the path from main and passing it to the constructor

    inputPath_ = (std::move(inputPath));




}

error_type Scanner::tokenize(std::vector<std::string> &words) {

    // Tokenize into memory (according to the Rules in this section).

    //does the file path exist from the input, check if the directory exists first though
    //then if it even has a path

    if (inputPath_.has_parent_path() && !std::filesystem::exists(inputPath_.parent_path()))
    {
        return DIR_NOT_FOUND;
    }
    //does it exist

    if (!std::filesystem::exists(inputPath_))
    {
        return FILE_NOT_FOUND;
    }
    //is it open
    std::ifstream in(inputPath_);

    //if its not open
    if (!in.is_open()) {
        return UNABLE_TO_OPEN_FILE;
    }

    words.clear();

    //
    for (std::string temp; !(temp = readWord(in)).empty();)
    {
        words.push_back(std::move(temp));
    }

    return NO_ERROR;
}

error_type Scanner::tokenize(std::vector<std::string>& words, const std::filesystem::path& outputFile) {
    // Tokenize and also write one token per line to 'outputFile' (e.g., <base>.tokens).
    // This overload should internally call the in‑memory tokenize() to avoid duplicate logic.


    //filling in from the words into memory
    error_type fillMem = tokenize(words);

    //if memory actually has something we will just return it
    if (fillMem != NO_ERROR)
    {
        return fillMem;
    }
    //cool, lets open it to make an output file
    std::ofstream out(outputFile);

    //if its not open
    if (!out.is_open()) {
        return UNABLE_TO_OPEN_FILE_FOR_WRITING;
    }

    //now we are taking input and putting it into the output file
    for (size_t i = 0; i < words.size(); i++)
    {
        out << words.at(i) << std::endl;
        //if its not working then we fail it and call the enum
        if (!out)
        {
            return FAILED_TO_WRITE_FILE;
        }
    }

    return NO_ERROR;
}

std::string Scanner::readWord(std::istream &in) {
    //create a character to iterate over and string

    //making a vector to hold token and a character to compare each letter
    std:: string token;
    char character;


    //pre check for "seperators"
    while (in.get(character))
    {
        if (isalpha(character))
        {
            in.unget();
            break;
        }
    }
    //checking the letters

    while (in.get(character))
    {
        //is it a lower case, then store
        if (character >= 'a' && character <= 'z')
        {
            token +=character;
        }
        //is it an upper case, make it lower and store it
        else if(character >= 'A' && character <='Z')
        {
            token += tolower(character);
        }
        //is it an apostrophe and is it a letter then store it
        else if ( character == '\'' && !token.empty() && isalpha(in.peek()) && isalpha(token.back()) )
        {
            token += character;
        }
        else if (!token.empty())
        {
            in.unget();
            break;
        }


    }

    return token;

}

