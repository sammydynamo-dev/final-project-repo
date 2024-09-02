/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
** 
** Class: 	CSC 242
** Assignment: 	Project #4
** File: 	playfair_cipher.cpp
** Description: This program encrypts text by applying a simple set of rules to 
** digraphs of letters, using a keyword-based grid to obscure patterns. 
** This method ensures that no letter is encrypted the same way twice and offers 
** security through its digraph-based encryption process.
**
** Author: 	Temitope S. Olugbemi
** Date: 	Aug 31, 2024
** -------------------------------------------------------------------------*/
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class PlayfairCipher
{
    vector<vector<char>> grid;
    unordered_map<char, pair<int, int>> positions;


private:
/**
 * @brief Preprocesses a keyword by removing duplicate characters and excluding the character 'J'.
 * 
 * This function takes a keyword as input and processes it by converting all characters to uppercase,
 * removing any duplicate characters, and excluding the character 'J'. The resulting string is returned.
 * 
 * @param keyword The input keyword to be processed.
 * @return A string that contains the processed keyword with unique characters in uppercase, excluding 'J'.
 */
string preprocessKeyword(const string& keyword)
{
    string result = "";
    unordered_set<char> seen;
    for (char ch : keyword)
    {
        if (!seen.count(toupper(ch)) && ch != 'J')
        {
            result += toupper(ch);
            seen.insert(toupper(ch));
        }
    }

    return result;
}

/**
 * @brief Adds a character to the grid and updates its position.
 * 
 * This function places a character into a 5x5 grid based on the given index.
 * It also updates the character's position in a separate tracking structure.
 * 
 * @param index Reference to the current index in the grid. This will be incremented after placing the character.
 * @param ch The character to be placed in the grid.
 */
void add_to_grid(int &index, char ch)
{
    grid[index / 5][index % 5] = ch;
    positions[ch] = {index / 5, index % 5};
    index++;
}

/**
 * @brief Creates a 5x5 grid using the provided keyword and the alphabet.
 * 
 * This function initializes a 5x5 grid with characters from the alphabet,
 * excluding the letter 'J'. The characters from the keyword are used to 
 * populate the grid first, followed by the remaining letters of the alphabet.
 * 
 * @param keyword The keyword used to populate the grid initially.
 */
void createGrid(const string& processedKeyword)
{
    string alphabet = "ABCDEFGHIKLMNOPQRSTUVWXYZ"; // 'I' == 'J'
    unordered_set<char> used(processedKeyword.begin(), processedKeyword.end());

    /**
     * Resizes the 2D grid to 5x5 and initializes each element with the default value of 'char'.
     * 
     * @param grid The 2D vector to be resized.
     */
    grid.resize(5, vector<char>(5));

    // Add the processedKeyword to the grid first.
    int index = 0;
    for (char ch: processedKeyword)
    {
        add_to_grid(index, ch);
    }

    // Add the other alphabets to the grid while ensuring only one of 'I' or 'J' gets on the grid.
    for (char ch : alphabet)
    {
        if (ch != 'J' && !used.count(ch))
        {
            add_to_grid(index, ch);
        }else if (ch == 'J' && !used.count('I'))
        {
            add_to_grid(index, 'I');
        }
    }
    
}

/**
 * @brief Processes the input text by either encrypting or decrypting it using a Playfair cipher.
 *
 * This function processes the input text in digraphs (pairs of two letters) and applies the Playfair cipher
 * rules to either encrypt or decrypt the text based on the value of the `encrypt` parameter.
 *
 * @param text The input text to be processed.
 * @param encrypt A boolean flag indicating whether to encrypt (true) or decrypt (false) the text.
 * @return The processed text after applying the Playfair cipher.
 */
string processText(const string& text, bool encrypt)
{
    string result = "";
    string trimmedText = "";
    vector<pair<size_t, char>> non_char_positions; // To store positions and their corresponding non-character values

    // Trim text to remove non-characters
    for (size_t i = 0; i < text.size(); i++)
    {
        if (!isalpha(text[i])) 
        {  // Check if the character is not a letter
            non_char_positions.push_back(make_pair(i, text[i]));
        } else if(text[i] == 'J') // Ensure 'J' == 'I'
        {
            trimmedText.push_back('I');
        }
        else 
        {
            trimmedText.push_back(text[i]);
        }

    }
    
    // Iterate through each pair of letters(diagraph)
    for (size_t i = 0; i < trimmedText.size(); i += 2)
    {
        char a = toupper(trimmedText[i]);
        char b = (i + 1 < trimmedText.size()) ? toupper(trimmedText[i + 1]) : 'X';

        if (a == b) b = 'X'; // If both letters are the same, replace the second with 'X'

        pair<int, int> posA = positions[a];
        pair<int, int> posB = positions[b];
        
        // if on the same row
        if (posA.first == posB.first)
        {
            // Same row
            result += grid[posA.first][(posA.second + (encrypt ? 1 : 4)) % 5];
            result += grid[posB.first][(posB.second + (encrypt ? 1 : 4)) % 5];
        }
        else if (posA.second == posB.second) // if on the same column
        {
            // Same column
            result += grid[(posA.first + (encrypt ? 1 : 4)) % 5][posA.second];
            result += grid[(posB.first + (encrypt ? 1 : 4)) % 5][posB.second];
        }
        else
        {
            // Rectangle swap
            result += grid[posA.first][posB.second];
            result += grid[posB.first][posA.second];
        }
    }

    // Re-add the previously-removed non-character values
    for (auto& pos : non_char_positions)
    {
        result.insert(result.begin() + pos.first, pos.second);
    }

    // Remove any trailing 'X' character added if decrypting
    if (!encrypt && result.back() == 'X')
    {
        result.pop_back();
    }
    

    return result;
}

public:
/**
 * @brief Constructs a PlayfairCipher object with the given keyword.
 * 
 * This constructor initializes the Playfair cipher by preprocessing the keyword
 * and creating the cipher grid based on the processed keyword.
 * 
 * @param keyword The keyword used to generate the Playfair cipher grid.
 */
PlayfairCipher(const string& keyword)
{
    string processedKey = preprocessKeyword(keyword);
    createGrid(processedKey);
}

/**
 * Encrypts the given plaintext.
 *
 * @param plaintext The text to be encrypted.
 * @return The encrypted text.
 */
string encrypt(const string& plaintext)
{
    return processText(plaintext, true);
}

/**
 * Decrypts the given ciphertext.
 *
 * This function takes a ciphertext string and decrypts it by calling the
 * processText function with the decryption flag set to false.
 *
 * @param ciphertext The encrypted text to be decrypted.
 * @return The decrypted text.
 */
string decrypt(const string& ciphertext)
{
    return processText(ciphertext, false);
}
};

/**
 * @brief Entry point of the program.
 * 
 * This function initializes a PlayfairCipher object with the keyword "PLAYFAIR",
 * encrypts a plaintext message, and then decrypts the resulting ciphertext.
 * The encrypted and decrypted texts are printed to the standard output.
 * 
 * @return int Returns 0 upon successful execution.
 */
int main() {
    PlayfairCipher cipher("UNIVERSITY");
    string plaintext = "Welcome to my universe!";
    string ciphertext = cipher.encrypt(plaintext);

    cout << "Encrypted text: " << ciphertext << endl;
    cout << "Decrypted text: " << cipher.decrypt(ciphertext) << endl;
}