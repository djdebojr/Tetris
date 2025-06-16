#include "GameEngine.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <fstream>
#include <cstdlib>
#include <windows.h>

int score = 0;
int speed = 500;
int level = 1;
bool paused = false;
int highScore = 0;
Piece current;

const int width = 10;   
const int height = 20;  


std::vector<std::vector<int>> field(height, std::vector<int>(width, 0));


std::vector<std::vector<std::vector<int>>> tetrominoes = {
    {{1, 1, 1, 1}},
    {{1, 1}, {1, 1}},
    {{0, 1, 0}, {1, 1, 1}},
    {{1, 0, 0}, {1, 1, 1}},
    {{0, 0, 1}, {1, 1, 1}},
    {{0, 1, 1}, {1, 1, 0}},
    {{1, 1, 0}, {0, 1, 1}}
};

extern int score;
extern int speed;
extern int level;
extern bool paused;
extern int highScore;
extern Piece current;


void print_cell(int value) {
    std::cout << (value ? "#" : ".");
}


void draw_border() {
    std::cout << "+";
    for (int i = 0; i < width; ++i) std::cout << "-";
    std::cout << "+\n";
}


void draw_info_panel() {
    std::cout << "Score: " << score << "\n";
    std::cout << "Level: " << level << "\n";
    std::cout << "High Score: " << highScore << "\n";
    if (paused) std::cout << "[PAUSED]\n";
    std::cout << "Controls: A-left D-right W-rotate S-down P-pause\n";
}


void draw() {
    system("cls");
    draw_border();
    for (int i = 0; i < height; ++i) {
        std::cout << "|";
        for (int j = 0; j < width; ++j) {
            bool drawn = false;
            for (int y = 0; y < current.shape.size(); ++y) {
                for (int x = 0; x < current.shape[y].size(); ++x) {
                    if (current.shape[y][x]) {
                        int cx = current.x + x;
                        int cy = current.y + y;
                        if (cy == i && cx == j) {
                            std::cout << "#";
                            drawn = true;
                        }
                    }
                }
            }
            if (!drawn)
                print_cell(field[i][j]);
        }
        std::cout << "|\n";
    }
    draw_border();
    draw_info_panel();
}


bool check_collision(const Piece& p) {
    for (int i = 0; i < p.shape.size(); ++i) {
        for (int j = 0; j < p.shape[i].size(); ++j) {
            if (p.shape[i][j]) {
                int newX = p.x + j;
                int newY = p.y + i;
                if (newX < 0 || newX >= width || newY >= height || field[newY][newX])
                    return true;
            }
        }
    }
    return false;
}


void apply_piece_to_field(const Piece& p, int value) {
    for (int i = 0; i < p.shape.size(); ++i) {
        for (int j = 0; j < p.shape[i].size(); ++j) {
            if (p.shape[i][j]) {
                field[p.y + i][p.x + j] = value;
            }
        }
    }
}


void merge_piece(const Piece& p) {
    apply_piece_to_field(p, 1);
}


void clear_lines() {
    for (int i = height - 1; i >= 0; --i) {
        bool full = std::all_of(field[i].begin(), field[i].end(), [](int v) { return v != 0; });
        if (full) {
            field.erase(field.begin() + i);
            field.insert(field.begin(), std::vector<int>(width, 0));
            score += 100;
            if (score > highScore) highScore = score;
            if (score % 500 == 0 && speed > 50) speed -= 50;
            ++i;
        }
    }
}


Piece generate_piece() {
    Piece p;
    p.shape = tetrominoes[rand() % tetrominoes.size()];
    p.x = width / 2 - p.shape[0].size() / 2;
    p.y = 0;
    return p;
}


void rotate(Piece& p) {
    std::vector<std::vector<int>> rotated(p.shape[0].size(), std::vector<int>(p.shape.size()));
    for (int i = 0; i < p.shape.size(); ++i)
        for (int j = 0; j < p.shape[0].size(); ++j)
            rotated[j][p.shape.size() - i - 1] = p.shape[i][j];
    Piece test = p;
    test.shape = rotated;
    if (!check_collision(test))
        p.shape = rotated;
}


void move_piece(Piece& p, char dir) {
    Piece temp = p;
    if (dir == 'a') temp.x--;
    else if (dir == 'd') temp.x++;
    else if (dir == 's') temp.y++;
    else if (dir == 'w') rotate(temp);

    if (!check_collision(temp))
        p = temp;
}


void input() {
    if (_kbhit()) {
        char key = _getch();
        if (key == 'p') {
            paused = !paused;
            return;
        }
        if (!paused)
            move_piece(current, key);
    }
}


bool game_over() {
    return check_collision(current);
}


void update_game() {
    if (paused) return;
    Piece temp = current;
    temp.y++;
    if (!check_collision(temp)) {
        current = temp;
    } else {
        merge_piece(current);
        clear_lines();
        current = generate_piece();
        if (game_over()) {
            draw();
            std::cout << "Game Over!\n";
            std::ofstream out("highscore.txt");
            if (out) out << highScore;
            out.close();
            exit(0);
        }
    }
}

void show_menu() {
    std::cout << "Select difficulty:\n1. Easy\n2. Medium\n3. Hard\n> ";
    std::cin >> level;
    if (level == 1) speed = 500;
    else if (level == 2) speed = 300;
    else speed = 100;
}


void load_high_score() {
    std::ifstream in("highscore.txt");
    if (in) in >> highScore;
    in.close();
}
