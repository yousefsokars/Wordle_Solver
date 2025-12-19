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

//the result of a guess
struct Feedback {
    std::string word;
    std::string colors;
};

// abs strat
class IGuessStrategy {
public:
    virtual ~IGuessStrategy() = default;
    virtual std::string pickWord(const std::vector<std::string>& candidates,
                                 const std::vector<std::string>& fullValidGuesses) = 0;
};

class WordleSolver {
private:
    std::vector<std::string> possibleSolutions;
    std::vector<std::string> validGuesses;
    IGuessStrategy* strategy;

public:

    static std::string simulateFeedback(const std::string& target, const std::string& guess) {
        std::string generatedColors = "-----";
        std::map<char, int> targetFreq;

        for(char c : target){targetFreq[c]++;}


        for(int i = 0; i< 5; ++i){
            if(guess[i]== target[i]){
                generatedColors[i]= 'G';
                targetFreq[guess[i]]--;
            }
        }
        for(int i= 0; i< 5;++i){
            char letter= guess[i];
            if(generatedColors[i]== 'G'){continue;}
            if(targetFreq[letter]> 0) {
                generatedColors[i]= 'Y';
                targetFreq[letter]--;
            }
        }
        return generatedColors;
    }

    WordleSolver(const std::string& bankPath, const std::string& validPath, IGuessStrategy* initialStrategy) {
        this->strategy = initialStrategy;
        possibleSolutions = loadFile(bankPath);
        validGuesses = loadFile(validPath);

        std::cout << "Engine Ready: "<< possibleSolutions.size()<< " solutions, "
                  << validGuesses.size()<< " valid guesses.\n";
    }

    std::vector<std::string> loadFile(const std::string& filepath) {
        std::vector<std::string> list;
        std::ifstream file(filepath);
        if(!file.is_open()){
            std::cerr << "Error: Could not open file " << filepath << std::endl;
            return list;
        }
        std::string word;
        while(std::getline(file, word)){
            word.erase(std::remove(word.begin(), word.end(), '\r'), word.end());
            if(word.length()== 5){list.push_back(word);}
        }
        return list;
    }

    void filterList(const Feedback& fb) {
        std::vector<std::string> nextList;
        for(const auto& candidate : possibleSolutions){
            if(simulateFeedback(candidate, fb.word)== fb.colors){
                nextList.push_back(candidate);
            }
        }
        possibleSolutions= nextList;
        std::cout<< "Remaining possibilities: "<< possibleSolutions.size()<< "\n";
    }

    std::string suggestNext(){
        return strategy->pickWord(possibleSolutions, validGuesses);
    }
};

#endif
