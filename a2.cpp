////////////////////////////////////////////////////////////////////////////////
// File Name:      a2.cpp
//
// Author:         Gerald
// CS email:       gerald@cs.wisc.edu
//
// Description:    The source file for a2.
//
// IMPORTANT NOTE: THIS IS THE ONLY FILE THAT YOU SHOULD MODIFY FOR A2.
//                 You SHOULD NOT MODIFY any of the following:
//                   1. Name of the functions/methods.
//                   2. The number and type of parameters of the functions.
//                   3. Return type of the functions.
////////////////////////////////////////////////////////////////////////////////

#include "a2.hpp"
#include "trim.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

void cleanData(std::ifstream &inFile, std::ofstream &outFile,
               std::unordered_set<std::string> &stopwords) {
    std::string line;
    std::getline(inFile, line);
    // TODO: Implement this method.
    // # of lines of code in Gerald's implementation: 13
    // Do the following operations on each review before
    // storing it to the output file:
    //   1. Replace hyphens with spaces.

    //   2. Split the line of text into individual words.
    std::vector<std::string> words;
    std::vector<std::string> out;
    splitLine(line, words);

    //   3. Remove the punctuation marks from the words.
    removePunctuation(words, out);
    //   4. Remove the trailing and the leading whitespaces in each word.
    removeWhiteSpaces(words);
    //   5. Remove the empty words
    removeEmptyWords(words);
    //   6. Remove words with just one character in them. You should NOT remove
    //      numbers in this step because if you do so, you'll lose the ratings.
    removeSingleLetterWords(words);
    //   7. Remove stopwords.
    removeStopWords(words, stopwords);


}

void fillDictionary(std::ifstream &newInFile,
                    std::unordered_map<std::string, std::pair<long, long>> &dict) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 20
    // Vector to contain all the reviews
    std::vector<std::string> reviews;

    // Read the inFile and save each line to vector reviewLines
    std::string line;
    while (true) {
        getline(newInFile, line);
        if (newInFile.fail()) break;
        reviews.push_back(line);
    }

    // For each review
    for (std::string review : reviews) {

        // Create a vector of all the words in the review
        std::vector<std::string> words;
        splitLine(review, words);

        // Convert the score to an int
        int score = std::stoi(words.at(0));
        words.erase(words.begin());

        // For each word in the review
        for (const std::string& word : words) {

            // Add or update the word's entry in the dictionary
            if (dict.count(word)) {
                dict[word].first = dict[word].first + score;
                ++dict[word].second;
            } else {
                std::pair <long, long> wordScores;
                wordScores.first = score;
                wordScores.second = 1;
                dict[word] = wordScores;
            }
        }
    }
}


void fillStopWords(std::ifstream &inFile,
                   std::unordered_set<std::string> &stopwords) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    std::string line;
    while (true) {
        getline(inFile, line);
        if (inFile.fail()) break;
        stopwords.insert(line);
    }
}

void rateReviews(std::ifstream &testFile,
                 std::unordered_map<std::string, std::pair<long, long>> &dict,
                 std::ofstream &ratingsFile) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 20
    while (true) {

        // Save the next review to variable line
        std::string line;
        getline(testFile, line);

        // Break out of the loop when it reaches the end of the file
        if (testFile.fail()) break;

        // Split the line into individual words
        std::vector<std::string> words;
        splitLine(line, words);

        // The expected review score (before dividing by the # of words)
        double unaveragedReviewScore = 0;

        // For each word in the review
        for (std::string& word : words) {

            // Add the word's average score if it exists, otherwise add 2
            if (dict.count(word) > 0) {
                double averageScoreForWord = ((double)dict[word].first / (double)dict[word].second);
                unaveragedReviewScore = unaveragedReviewScore + averageScoreForWord;
            } else {
                unaveragedReviewScore = unaveragedReviewScore + 2;
            }
        }

        // The expected score for this review
        double expectedScore;

        // Take the per-word average of unaveragedReviewScore, unless there were 0 words
        if (words.size() == 0) {
            expectedScore = 2;
        } else {
            expectedScore = (double)unaveragedReviewScore / words.size();
        }

        // Write the expected score to the rating file
        ratingsFile << std::setprecision(2) << std::fixed << expectedScore << std::endl;
    }
}

void removeEmptyWords(std::vector<std::string> &tokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    std::vector <std::string>::iterator itr;
    for (itr = tokens.begin(); itr != tokens.end(); ++itr) {
        if ((*itr).size() == 0 and !(isdigit(atoi((*itr).c_str())))) {
        }
    }
}

void removePunctuation(std::vector<std::string> &inTokens,
                       std::vector<std::string> &outTokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 10
    int i = 0;
    for (std::string word : inTokens) {
        for (int i = 0, len = word.size(); i < len; i++) {
            if (ispunct(word[i])) {
                word.erase(i--, 1);
                outTokens.push_back(word);
                len = word.size();
            }
        }
    }
}

void removeSingleLetterWords(std::vector<std::string> &tokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    for (std::string word : tokens) {
        if (word.size() == 1 and !(isdigit(atoi(word.c_str())))) {
            std::vector<std::string>::iterator itr = tokens.begin();
            if (itr != tokens.end() and !(isdigit(atoi((*itr).c_str())))) tokens.erase(itr);
        }
    }

}

void removeStopWords(std::vector<std::string> &tokens,
                     std::unordered_set<std::string> &stopwords) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    for (const std::string& word : stopwords) {
        // Remove all occurrences of it in the review
        tokens.erase(std::remove(tokens.begin(), tokens.end(), word), tokens.end());
    }

}

void removeWhiteSpaces(std::vector<std::string> &tokens) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    // You may want to use the trim() method from the trim.*pp files in a2.
    for(std::string word : tokens) {
        trim(word);
    }
}

void replaceHyphensWithSpaces(std::string &line) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 5
    std::replace(line.begin(), line.end(), '-', ' ');
}

void splitLine(std::string &line, std::vector<std::string> &words) {
    // TODO: Implement this method.
    // approximate # of lines of code in Gerald's implementation: < 10
    std::stringstream ss;
    ss.str(line);
    std::string token;
    while (std::getline(ss, token, ' ')) {
        if (token.length() != 0) {
            words.push_back(token);
        }
    }
}
