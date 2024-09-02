/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
** 
** Class: 	CSC 242
** Assignment: 	Project #1
** File: 	spell_checker.cpp
** Description: This program reads a dictionary file and stores all the words in a vector.
** It then reads another file and checks each word to see if it is present in the dictionary.
** If a word is not found in the dictionary, it is printed to the standard output.
**
** Author: 	Temitope S. Olugbemi
** Date: 	Aug 31, 2024
** -------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

/**
 * @brief Main function that checks for words in a file against a dictionary.
 * 
 * @return int Returns 0 on success, 1 on failure to open files.
 */
int main() 
{
    // Open the dictionary file
    ifstream dictFile("/usr/share/dict/words");
    if (!dictFile)
    {
        cerr << "Error opening dictionary file." << endl;
        return 1;
    }

    // Define a vector of strings called words
    vector<string> words;
    string word;

    // For each word in the dictionary file, append it to the words vector
    while (dictFile >> word) 
    {
        words.push_back(word);
    }
    dictFile.close();

    // Open the file to be checked
    ifstream fileToCheck("words_file.txt");
    if (!fileToCheck)
    {
        cerr << "Error opening file to be checked." << endl;
        return 1;
    }

    // For each word in that file
    cout << "The following words are not present:" << endl; // If the word is not contained in the words vector, print the word
    while (fileToCheck >> word) 
    {
        if (find(words.begin(), words.end(), word) == words.end())
        {
            cout << word << endl;
        }
    }

    fileToCheck.close();
    return 0;
}