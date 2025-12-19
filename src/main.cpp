//
// Created by youse on 18/12/2025.
//

#include <iostream>
#include "WordleSolver.h"
#include "Strategies.h"

int main() {
    // 1. Ask for Playstyle
    int choice;
    std::cout << "Select Playstyle:\n1. Aggressive (Try to win fast)\n2. Safe (Never get trapped)\n> ";
    std::cin >> choice;

    IGuessStrategy* strategy = nullptr;
    AggressiveStrategy agg;
    SafeStrategy safe;

    if (choice == 1) strategy = &agg;
    else strategy = &safe;

    // 2. Initialize Solver with BOTH files
    // Ensure paths are correct relative to your .exe
    WordleSolver solver("data/word-bank.csv", "data/valid-words.csv", strategy);

    std::cout << "--- Wordle Assistant ---\n";
    std::string suggestion = solver.suggestNext();
    std::cout << "Suggested starter: " << suggestion << "\n";

    while (true) {
        std::string guess, colors;

        std::cout << "\n--------------------------------\n";
        std::cout << "Enter word you played: ";
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

        suggestion = solver.suggestNext();
        if (suggestion.empty()) {
            std::cout << "No solutions left!\n";
            break;
        }
        std::cout << ">> Best Next Guess: " << suggestion << "\n";
    }
    return 0;
}