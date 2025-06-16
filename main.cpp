#include "GameEngine.h"
#include <chrono>
#include <thread>
#include <cstdlib>


int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    show_menu();
    load_high_score();
    current = generate_piece();

    while (true) {
        draw();
        input();
        update_game();
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
    }
    return 0;
}
