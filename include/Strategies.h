//
// Created by youse on 18/12/2025.
//

#ifndef STRATEGIES_H
#define STRATEGIES_H

#include "WordleSolver.h"
#include <map>
#include <algorithm>
#include <climits>

// aggressive (just tries to match letters)
class AggressiveStrategy : public IGuessStrategy {
public:
    std::string pickWord(const std::vector<std::string>& candidates,
                         const std::vector<std::string>& fullValidGuesses) override {

        if (candidates.empty()){return "";}
        std::map<char, int> freqMap;
        for(const auto& word : candidates){
            for(char c : word){freqMap[c]++;}
        }
        std::string bestWord= "";
        int bestScore= -1;
        for(const auto& word : candidates){
            int currentScore= 0;
            std::string uniqueChars= "";
            for(char c : word){
                if(uniqueChars.find(c)== std::string::npos) {
                    currentScore+= freqMap[c];
                    uniqueChars+= c;
                }
            }
            if(currentScore > bestScore){
                bestScore= currentScore;
                bestWord= word;
            }
        }
        return bestWord;
    }
};

// safe mode (uses minimax to never lose)
class SafeStrategy : public IGuessStrategy {
public:
    std::string pickWord(const std::vector<std::string>& candidates,
                         const std::vector<std::string>& fullValidGuesses) override {

        if(candidates.empty()){return "";}
        if(candidates.size() == 1){return candidates[0];}

        // current threshold is 100 (using aggressive until less than 100)
        if(candidates.size()> 50){
            AggressiveStrategy fallback;
            return fallback.pickWord(candidates, fullValidGuesses);
        }

        std::string bestGuess= "";
        int minWorstCase= INT_MAX;

        std::cout<< "[SafeMode] Thinking (Running Minimax on "<< candidates.size()<< " candidates)... ";

        // we check valid guesses to find the best "splitter"
        for(const auto& guess : fullValidGuesses){

            std::map<std::string, int> outcomes;
            int maxBucketSize= 0;

            for(const auto& solution : candidates) {
                std::string feedback = WordleSolver::simulateFeedback(solution, guess);
                outcomes[feedback]++;

                if(outcomes[feedback]> maxBucketSize){
                    maxBucketSize= outcomes[feedback];
                }
            }

            if(maxBucketSize < minWorstCase){
                minWorstCase= maxBucketSize;
                bestGuess= guess;
            }
        }
        std::cout << "Done.\n";
        return bestGuess;
    }
};

#endif
