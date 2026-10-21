#include <cctype>
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

#ifdef _WIN32
#include <windows.h>
#endif

namespace {
const char* const RESET = "\033[0m";
const char* const PURPLE = "\033[35m";
const char* const RED = "\033[31m";
const char* const YELLOW = "\033[33m";
const char* const GREEN = "\033[32m";
const char* const ORANGE = "\033[38;5;208m";
const char* const CYAN = "\033[36m";

struct Fruit {
    const char* name;
    const char* hint;
    const char* drawing[5];
    const char* colorCode;
};

const Fruit fruits[] = {
    {"uva", "Crece en racimos y cada bolita es pequeña y redonda.",
     {"        _v_", "       /   \\", "      ( o o )", "     ( o o o )", "      `-----'"}, PURPLE},
    {"manzana", "Es redonda, crujiente y tiene un pequeño tallo.",
     {"        |", "      .'|`.", "     /  |  \\", "    |  * *  |", "     `-----'"}, RED},
    {"platano", "Es larga y curva; se pela antes de comerla.",
     {"       ____", "     .'    `.", "    /        `.", "   |          |", "    `--------'"}, YELLOW},
    {"fresa", "Es pequeña, tiene hojitas arriba y semillas por fuera.",
     {"       /\\", "      /^^\\", "     / ooo \\", "    / ooooo \\", "    `-------'"}, RED},
    {"naranja", "Es redonda, tiene cáscara y gajos jugosos por dentro.",
     {"        _v_", "      .'   `.", "     /  .-.  \\", "    |  (   )  |", "     `.(   ).'"}, ORANGE},
    {"pera", "Es más ancha abajo y tiene un tallito arriba.",
     {"         |", "        / \\", "       (   )", "      (     )", "       `---'"}, GREEN}
};

void enableConsoleColors() {
#ifdef _WIN32
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);
    if (output == INVALID_HANDLE_VALUE) {
        return;
    }

    DWORD mode = 0;
    if (GetConsoleMode(output, &mode)) {
        SetConsoleMode(output, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    }
#endif
}

std::string lowercase(std::string text) {
    for (std::size_t i = 0; i < text.size(); ++i) {
        text[i] = static_cast<char>(
            std::tolower(static_cast<unsigned char>(text[i])));
    }
    return text;
}

bool containsFruit(const std::string& answer, const char* fruit) {
    if (answer.find(fruit) != std::string::npos) {
        return true;
    }
    if (std::string(fruit) == "platano") {
        return answer.find("plátano") != std::string::npos ||
               answer.find("banana") != std::string::npos;
    }
    return false;
}

void celebrate() {
    const char* const frames[] = {
        "       😄  ¡Muy bien!",
        "      🥳  ¡Lo adivinaste!",
        "   ⭐  🎉 ¡Excelente! 🎉  ⭐"
    };
    for (std::size_t i = 0; i < sizeof(frames) / sizeof(frames[0]); ++i) {
        std::cout << YELLOW << frames[i] << RESET << "\n" << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(350));
    }
}

bool showGame() {
    const std::size_t fruitCount = sizeof(fruits) / sizeof(fruits[0]);
    std::cout << CYAN
              << "\n  * * * LA DANZA DE LAS FRUTAS * * *\n"
              << RESET
              << "¡Hola! Mira el dibujo y adivina el nombre de cada fruta.\n"
              << "Solo tienes que escribir el nombre de la fruta.\n"
              << "Pulsa Enter para comenzar...";
    std::string input;
    std::getline(std::cin, input);

    for (std::size_t i = 0; i < fruitCount; ++i) {
        std::cout << "\n" << CYAN << "Fruta " << (i + 1) << " de "
                  << fruitCount << RESET << "\n"
                  << fruits[i].colorCode;
        for (int line = 0; line < 5; ++line) {
            std::cout << fruits[i].drawing[line] << "\n";
        }
        std::cout << RESET
                  << "¿Qué fruta es? ";

        while (true) {
            if (!std::getline(std::cin, input)) {
                return false;
            }
            input = lowercase(input);
            if (containsFruit(input, fruits[i].name)) {
                celebrate();
                break;
            }

            if (input.find("brocoli") != std::string::npos ||
                input.find("brócoli") != std::string::npos) {
                std::cout << "El brócoli es una verdura, no una fruta.\n";
            } else {
                std::cout << "Esa no es la fruta que buscamos. ¡No te preocupes!\n";
            }
            std::cout << CYAN << "Pista: " << fruits[i].hint << "\n"
                      << "¡Inténtalo de nuevo! ¿Qué fruta es? " << RESET;
        }
    }

    std::cout << YELLOW
              << "\n  * * * ¡BAILE DE FRUTAS COMPLETADO! * * *\n"
              << RESET
              << "¡Felicidades! ¡Ya reconoces muchas frutas!\n";
    return true;
}
} // namespace

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    enableConsoleColors();

    char playAgain = 's';
    while (playAgain == 's' || playAgain == 'S') {
        showGame();
        std::cout << "\n¿Quieres jugar otra vez? (s/n): ";
        std::string answer;
        if (!std::getline(std::cin, answer) || answer.empty()) {
            break;
        }
        playAgain = answer[0];
    }

    std::cout << "¡Hasta la próxima, pequeño explorador!\n";
    return 0;
}
