//
// Created by youse on 18/12/2025.
//

#include <iostream>
#include "../include/WordleSolver.h" 
#include "../include/Strategies.h"

int main() {
    AggressiveStrategy aggStrat;

    // IMPORTANT: Point this to your word-bank.csv
    // If running from the root folder, use "data/word-bank.csv"
    WordleSolver solver("data/word-bank.csv", &aggStrat);

    std::cout << "--- Wordle Assistant (Aggressive Mode) ---\n";
    
    // Calculate first guess dynamically or hardcode
    std::string firstSuggestion = solver.suggestNext();
    std::cout << "Suggested starter: " << firstSuggestion << "\n"; 

    while (true) {
        std::string guess, colors;
        
        std::cout << "\n--------------------------------\n";
        std::cout << "Enter word you played (or 'exit'): ";
        std::cin >> guess;
        if (guess == "exit") break;
        
        std::cout << "Enter feedback (G=Green, Y=Yellow, -=Gray): ";
        std::cin >> colors;

        if (colors == "GGGGG") {
            std::cout << "Congratulations!\n";
            break;
        }

        Feedback fb {guess, colors};
        solver.filterList(fb);
        
        std::string suggestion = solver.suggestNext();
        if (suggestion.empty()) {
            std::cout << "No solutions left! (Check your feedback input)\n";
            break;
        }
        std::cout << ">> Best Next Guess: " << suggestion << "\n";
    }
    return 0;
}