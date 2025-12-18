//
// Created by youse on 18/12/2025.
//

#ifndef INCLUDE_WORDLESOLVER_H
#define INCLUDE_WORDLESOLVER_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <algorithm> // for std::max

struct Feedback {
    std::string word; 
    std::string colors; 
};

class IGuessStrategy {
public:
    virtual ~IGuessStrategy() = default;
    virtual std::string pickWord(const std::vector<std::string>& candidates) = 0;
};

class WordleSolver {
private:
    std::vector<std::string> possibleSolutions; // The words that can be the answer (word-bank)
    // std::vector<std::string> validGuesses;   // Words allowed to guess (valid-words) - saved for later use
    IGuessStrategy* strategy;

    std::string getFeedbackForGuess(const std::string& target, const std::string& guess) {
        std::string generatedColors = "-----"; 
        std::map<char, int> targetFreq;

        for (char c : target) targetFreq[c]++;

        // Green Pass
        for (int i = 0; i < 5; ++i) {
            if (guess[i] == target[i]) {
                generatedColors[i] = 'G';
                targetFreq[guess[i]]--;
            }
        }
        // Yellow Pass
        for (int i = 0; i < 5; ++i) {
            char letter = guess[i];
            if (generatedColors[i] == 'G') continue; 
            
            if (targetFreq[letter] > 0) {
                generatedColors[i] = 'Y';
                targetFreq[letter]--;
            }
        }
        return generatedColors;
    }

public:
    // Constructor now takes just the solution path for now
    WordleSolver(const std::string& solutionPath, IGuessStrategy* initialStrategy) {
        this->strategy = initialStrategy;
        loadDictionary(solutionPath);
    }

    void loadDictionary(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filepath << std::endl;
            return;
        }

        std::string word;
        while (std::getline(file, word)) {
            // CSVs might have \r chars on Windows, clean them
            word.erase(std::remove(word.begin(), word.end(), '\r'), word.end());
            
            // Your CSVs are just plain lists, so this simple check is enough
            if (word.length() == 5) {
                possibleSolutions.push_back(word);
            }
        }
        file.close();
        std::cout << "Loaded " << possibleSolutions.size() << " possible solutions.\n";
    }

    void filterList(const Feedback& fb) {
        std::vector<std::string> nextList;
        for (const auto& candidate : possibleSolutions) {
            if (getFeedbackForGuess(candidate, fb.word) == fb.colors) {
                nextList.push_back(candidate);
            }
        }
        possibleSolutions = nextList;
        std::cout << "Remaining possibilities: " << possibleSolutions.size() << "\n";
    }

    std::string suggestNext() {
        return strategy->pickWord(possibleSolutions);
    }
};

#endif //INCLUDE_WORDLESOLVER_H
