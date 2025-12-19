//
// Created by youse on 18/12/2025.
//

#ifndef STRATEGIES_H
#define STRATEGIES_H

#include "WordleSolver.h"
#include <map>
#include <algorithm>
#include <climits>

// --- AGGRESSIVE (Keep your old logic, just updated signature) ---
class AggressiveStrategy : public IGuessStrategy {
public:
    std::string pickWord(const std::vector<std::string>& candidates,
                         const std::vector<std::string>& fullValidGuesses) override {
        // (Your existing frequency logic goes here, ignoring fullValidGuesses)
        // Copy-paste your previous logic but update the function signature
        if (candidates.empty()) return "";
        std::map<char, int> freqMap;
        for (const auto& word : candidates) {
            for (char c : word) freqMap[c]++;
        }
        std::string bestWord = "";
        int bestScore = -1;
        for (const auto& word : candidates) {
            int currentScore = 0;
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

// --- SAFE (MINIMAX) STRATEGY ---
class SafeStrategy : public IGuessStrategy {
public:
    std::string pickWord(const std::vector<std::string>& candidates,
                         const std::vector<std::string>& fullValidGuesses) override {

        if (candidates.empty()) return "";
        if (candidates.size() == 1) return candidates[0]; // Only one left? Just guess it!

        // PERFORMANCE OPTIMIZATION:
        // If we have tons of candidates (e.g. > 100), Minimax is too slow.
        // Just use Aggressive logic until the list is smaller.
        if (candidates.size() > 50) {
            AggressiveStrategy fallback;
            return fallback.pickWord(candidates, fullValidGuesses);
        }

        std::string bestGuess = "";
        int minWorstCase = INT_MAX;

        std::cout << "[SafeMode] Thinking (Running Minimax on " << candidates.size() << " candidates)... ";

        // We check valid guesses to find the best "splitter"
        // Note: checking ALL 12,000 words against ~50 candidates is ~600,000 ops. Fast enough.
        for (const auto& guess : fullValidGuesses) {

            std::map<std::string, int> outcomes;
            int maxBucketSize = 0;

            // SIMULATION: "If I play 'guess', how much does it split the remaining candidates?"
            for (const auto& solution : candidates) {
                std::string feedback = WordleSolver::simulateFeedback(solution, guess);
                outcomes[feedback]++;

                // Track the worst-case group size for this guess
                if (outcomes[feedback] > maxBucketSize) {
                    maxBucketSize = outcomes[feedback];
                }
            }

            // We want the guess that has the SMALLEST worst-case bucket
            if (maxBucketSize < minWorstCase) {
                minWorstCase = maxBucketSize;
                bestGuess = guess;
            }
        }
        std::cout << "Done.\n";
        return bestGuess;
    }
};

#endif
