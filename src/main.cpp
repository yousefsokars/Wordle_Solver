//
// Created by youse on 18/12/2025.
//

#include <iostream>
#include "../include/WordleSolver.h" // Note the relative path
#include "../include/Strategies.h"

int main() {
    // 1. Initialize Strategy
    AggressiveStrategy aggStrat;

    // 2. Initialize Solver with path to your CSV/TXT
    // Make sure the path matches where you run the executable from!
    WordleSolver solver("../data/words.csv", &aggStrat);

    std::cout << "--- Wordle Assistant (Aggressive Mode) ---\n";
    std::cout << "Suggested starter: crane\n";

    while (true) {
        std::string guess, colors;

        std::cout << "\n--------------------------------\n";
        std::cout << "Enter word you played (or 'exit'): ";
        std::cin >> guess;
        if (guess == "exit") break;

        std::cout << "Enter feedback (G=Green, Y=Yellow, -=Gray): ";
        std::cin >> colors;

        if (colors == "GGGGG") {
            std::cout << "Congratulations! solved.\n";
            break;
        }

        // Logic Step
        Feedback fb {guess, colors};
        solver.filterList(fb);

        std::string suggestion = solver.suggestNext();
        if (suggestion.empty()) {
            std::cout << "No words match your feedback! (Did you make a typo?)\n";
            break;
        }
        std::cout << ">> Best Next Guess: " << suggestion << "\n";
    }
    return 0;
}