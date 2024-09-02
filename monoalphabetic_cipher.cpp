/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
** 
** Class: 	CSC 242
** Assignment: 	Project #3
** File: 	monoalphabetic_cipher.cpp
** Description: This program encrypts or decrypts a given input file using a keyword-based cipher.
**
** Author: 	Temitope S. Olugbemi
** Date: 	Aug 31, 2024
** -------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <unordered_map>

using namespace std;

string generateCipherAlphabet(const string& keyword);
void processFile(const string& inputFile, const string& outputFile, const string& cipherAlphabet, bool decrypt);

/**
 * @brief Entry point of the crypt program.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return int Returns 0 on success, 1 on failure.
 */
int main(int argc, char* argv[])
{
    // Check for the minimum required number of arguments
    if (argc < 4 || argc > 5)
    {
        cerr << "Usage: crypt [-d] -kKEY input.txt output.txt" << endl;
        return 1;
    }

    bool decrypt = false;
    string keyword;
    string inputFile;
    string outputFile;

    // Parse command-line arguments
    int argIndex = 1;
    if (argc == 5) {
        // Check if the first argument is '-d' for decryption
        if (string(argv[argIndex]) == "-d") {
            decrypt = true;
            argIndex++;
        } else {
            cerr << "Usage: crypt [-d] -kKEY input.txt output.txt" << endl;
            return 1;
        }
    }

    // Expect the next argument to be '-kKEY'
    string keyArg = string(argv[argIndex]);
    if (keyArg.substr(0, 2) == "-k" && keyArg.length() > 2) {
        keyword = keyArg.substr(2); // Extract the KEY part
        argIndex++;
    } else {
        cerr << "Usage: crypt [-d] -kKEY input.txt output.txt" << endl;
        return 1;
    }

    // The next two arguments should be the input and output file names
    inputFile = string(argv[argIndex]);
    outputFile = string(argv[argIndex + 1]);

    // Generate the cipher alphabet based on the keyword
    string cipherAlphabet = generateCipherAlphabet(keyword);
    
    // Process the input file and produce the output file
    processFile(inputFile, outputFile, cipherAlphabet, decrypt);

    return 0;
}

/**
 * @brief Generates a cipher alphabet based on a given keyword.
 * 
 * The cipher alphabet starts with the unique letters from the keyword (in uppercase),
 * followed by the remaining letters of the alphabet in reverse order.
 *
 * @param keyword The keyword used to generate the cipher alphabet.
 * @return A string representing the generated cipher alphabet.
 */
string generateCipherAlphabet(const string& keyword)
{
    string cipherAlphabet = "";
    unordered_map<char, bool> seen;

    // Add letters from the keyword to the cipher alphabet, removing duplicates
    for (char ch : keyword)
    {
        ch = toupper(ch);
        if (!seen[ch] && isalpha(ch))
        {
            cipherAlphabet += ch;
            seen[ch] = true;
        }
    }

    // Add the remaining letters of the alphabet in reverse order
    for (char ch = 'Z'; ch >= 'A'; ch--)
    {
        if (!seen[ch])
        {
            cipherAlphabet += ch;
        }
    }
    
    return cipherAlphabet;
}

/**
 * @brief Processes a file by either encrypting or decrypting its contents using a substitution cipher.
 *
 * This function reads the contents of an input file, processes each character according to the provided
 * cipher alphabet, and writes the result to an output file. The processing can be either encryption or
 * decryption based on the `decrypt` flag.
 *
 * @param inputFile The path to the input file to be processed.
 * @param outputFile The path to the output file where the processed content will be written.
 * @param cipherAlphabet A string representing the substitution cipher alphabet. It must be 26 characters long.
 * @param decrypt A boolean flag indicating whether to decrypt (true) or encrypt (false) the input file.
 */
void processFile(const string& inputFile, const string& outputFile, const string& cipherAlphabet, bool decrypt)
{
    ifstream inFile(inputFile);
    ofstream outFile(outputFile);

    if (!inFile || !outFile)
    {
        cerr << "Error opening files." << endl;
        return;
    }

    string alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unordered_map<char, char> cipherMap;

    if (decrypt)
    {
        for (size_t i = 0; i < 26; i++)
        {
            cipherMap[cipherAlphabet[i]] = alphabet[i];
        }
    }
    else
    {
        for (size_t i = 0; i < 26; i++)
        {
            cipherMap[alphabet[i]] = cipherAlphabet[i];
        }
    }

    char ch;
    while (inFile.get(ch))
    {
        ch = toupper(ch);
        if (isalpha(ch))
        {
            outFile.put(cipherMap[ch]);
        }
        else
        {
            outFile.put(ch);
        }
    }
    
    inFile.close();
    outFile.close();
}
