#include <gtest/gtest.h>
#include "GameEngine.h"

TEST(PieceTest, RotateKeepsSize) {
    Piece p = {0, 0, {{1, 0}, {1, 1}}};
    int h = p.shape.size();
    int w = p.shape[0].size();
    rotate(p);
    EXPECT_EQ(p.shape.size(), w);
    EXPECT_EQ(p.shape[0].size(), h);
}

TEST(PieceTest, PieceWithinBounds) {
    for (int i = 0; i < 10; ++i) {
        Piece p = generate_piece();
        EXPECT_GE(p.x, 0);
        EXPECT_LE(p.x + p.shape[0].size(), 10);
        EXPECT_EQ(p.y, 0);
    }
}

TEST(GameStateTest, GameOverOnCollisionAtSpawn) {
    Piece p = generate_piece();
    apply_piece_to_field(p, 1);
    current = p;
    EXPECT_TRUE(game_over());
}
