//
// Created by youse on 18/12/2025.
//

#ifndef INCLUDE_STRATEGIES_H
#define INCLUDE_STRATEGIES_H

#include "WordleSolver.h"
#include <map>

class AggressiveStrategy : public IGuessStrategy {
public:
    std::string pickWord(const std::vector<std::string>& candidates) override {
        if (candidates.empty()) return "";

        // 1. Calculate letter frequency in remaining words
        std::map<char, int> freqMap;
        for (const auto& word : candidates) {
            for (char c : word) freqMap[c]++;
        }

        // 2. Score words based on frequency
        std::string bestWord = "";
        int bestScore = -1;

        for (const auto& word : candidates) {
            int currentScore = 0;
            // Simple scoring: sum of letter frequencies
            // A better aggressive heuristic counts UNIQUE letters only to maximize info
            std::string uniqueChars = "";
            for(char c : word) {
                if(uniqueChars.find(c) == std::string::npos) {
                    currentScore += freqMap[c];
                    uniqueChars += c;
                }
            }

            if (currentScore > bestScore) {
                bestScore = currentScore;
                bestWord = word;
            }
        }
        return bestWord;
    }
};

#endif //INCLUDE_STRATEGIES_H
