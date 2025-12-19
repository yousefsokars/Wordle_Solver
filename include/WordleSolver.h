//
// Created by youse on 18/12/2025.
//

#ifndef WORDLE_SOLVER_H
#define WORDLE_SOLVER_H

#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <iostream>
#include <algorithm>

// Represents the result of a guess
struct Feedback {
    std::string word;
    std::string colors;
};

// Abstract Interface for Strategies
class IGuessStrategy {
public:
    virtual ~IGuessStrategy() = default;
    // UPDATED: Now takes both the remaining possibilities AND the full list of valid guesses
    virtual std::string pickWord(const std::vector<std::string>& candidates,
                                 const std::vector<std::string>& fullValidGuesses) = 0;
};

class WordleSolver {
private:
    std::vector<std::string> possibleSolutions; // The "Answers" (word-bank)
    std::vector<std::string> validGuesses;      // The "Fillers" (valid-words)
    IGuessStrategy* strategy;

public:
    // --- STATIC HELPER (Moved from private so Strategies can use it) ---
    static std::string simulateFeedback(const std::string& target, const std::string& guess) {
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

    WordleSolver(const std::string& bankPath, const std::string& validPath, IGuessStrategy* initialStrategy) {
        this->strategy = initialStrategy;
        possibleSolutions = loadFile(bankPath);
        validGuesses = loadFile(validPath); // Now we load the big list too

        // The validGuesses list must ALSO contain the solutions for the game to work
        // (Usually valid-words contains everything, but just in case, we can merge them if needed)
        std::cout << "Engine Ready: " << possibleSolutions.size() << " solutions, "
                  << validGuesses.size() << " valid guesses.\n";
    }

    std::vector<std::string> loadFile(const std::string& filepath) {
        std::vector<std::string> list;
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << filepath << std::endl;
            return list;
        }
        std::string word;
        while (std::getline(file, word)) {
            word.erase(std::remove(word.begin(), word.end(), '\r'), word.end());
            if (word.length() == 5) list.push_back(word);
        }
        return list;
    }

    void filterList(const Feedback& fb) {
        std::vector<std::string> nextList;
        for (const auto& candidate : possibleSolutions) {
            if (simulateFeedback(candidate, fb.word) == fb.colors) {
                nextList.push_back(candidate);
            }
        }
        possibleSolutions = nextList;
        std::cout << "Remaining possibilities: " << possibleSolutions.size() << "\n";
    }

    std::string suggestNext() {
        return strategy->pickWord(possibleSolutions, validGuesses);
    }
};

#endif
