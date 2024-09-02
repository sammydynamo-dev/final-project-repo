/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
** 
** Class: 	CSC 242
** Assignment: 	Project #2
** File: 	country_population_data_processor.cpp
** Description: This program reads a file containing country names and their populations,
** processes the data, and calculates the total population excluding the European Union.
**
** Author: 	Temitope S. Olugbemi
** Date: 	Aug 31, 2024
** -------------------------------------------------------------------------*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

/**
 * @brief The main function of the program.
 * 
 * This function opens a file named "worldpop.txt", reads each line to extract the country name
 * and its population, and calculates the total population excluding the European Union.
 * 
 * @return int Returns 0 on successful execution, 1 if there is an error opening the input file.
 */
int main() {
    ifstream inputFile("how_to_1/worldpop.txt");
    if (!inputFile)
    {
        cerr << "Error opening input file." << endl;
        return 1;
    }

    string line;
    long long totalPopulation = 0;

    // Process each line of the file
    while (getline(inputFile, line))
    {
        istringstream iss(line);
        string countryName;
        char ch;

        // Read characters until a whitespace is encountered
        while (iss.get(ch) && !isspace(ch))
        {
            countryName += ch;
        }
        
        // Process the rest of the line
        while (iss)
        {
            // Skip white spaces
            while (iss.get(ch) && isspace(ch));

            if (!iss) break;

            // Check if the next character is a digit
            if (isdigit(ch))
            {
                iss.unget();
                break;
            }

            // Add a space and the character to the country name
            countryName += ' ';
            countryName += ch;

            // Add characters until the next white space
            while (iss.get(ch) && !isspace(ch))
            {
                countryName += ch;
            }
        }

        // Read the population number
        long long population;
        iss >> population;

        // Add to total population if the country is not "European Union"
        if (countryName != "European Union")
        {
            totalPopulation += population;
        }
    }
    
    inputFile.close();

    cout << "Total population (excluding European Union): " << totalPopulation << endl;

    return 0;
}