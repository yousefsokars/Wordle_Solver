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

// Represents the result of a guess
struct Feedback {
    std::string word;   // The word user guessed
    std::string colors; // e.g., "G-Y--"
};

// Abstract Interface for Strategies
class IGuessStrategy {
public:
    virtual ~IGuessStrategy() = default;
    virtual std::string pickWord(const std::vector<std::string>& candidates) = 0;
};

class WordleSolver {
private:
    std::vector<std::string> possibleSolutions;
    IGuessStrategy* strategy;

    // The Simulation Logic (The "Engine")
    std::string getFeedbackForGuess(const std::string& target, const std::string& guess) {
        std::string generatedColors = "-----";
        std::map<char, int> targetFreq;

        // 1. Count frequencies
        for (char c : target) targetFreq[c]++;

        // 2. Green Pass
        for (int i = 0; i < 5; ++i) {
            if (guess[i] == target[i]) {
                generatedColors[i] = 'G';
                targetFreq[guess[i]]--;
            }
        }

        // 3. Yellow Pass
        for (int i = 0; i < 5; ++i) {
            char letter = guess[i];
            if (generatedColors[i] == 'G') continue; // Skip greens

            if (targetFreq[letter] > 0) {
                generatedColors[i] = 'Y';
                targetFreq[letter]--;
            }
        }
        return generatedColors;
    }

public:
    WordleSolver(const std::string& filepath, IGuessStrategy* initialStrategy) {
        this->strategy = initialStrategy;
        loadDictionary(filepath);
    }

    void loadDictionary(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filepath << std::endl;
            return;
        }

        std::string word;
        while (std::getline(file, word)) {
            // Cleanup: remove potential commas or quotes if your CSV has them
            std::string cleanWord = "";
            for (char c : word) {
                if (isalpha(c)) cleanWord += tolower(c);
            }
            if (cleanWord.length() == 5) {
                possibleSolutions.push_back(cleanWord);
            }
        }
        file.close();
        std::cout << "Loaded " << possibleSolutions.size() << " words.\n";
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
