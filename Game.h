#pragma once
#include "LedRenderer.h"

#define ARENA_WIDTH 16
#define ARENA_HEIGHT 16

enum TileType : byte
{
    Empty,
    Snake,
    Apple
};

struct Tile
{
    TileType type;
    float duration;
};

class Game
{
public:
    LedRenderer *renderer;

    Tile tiles[ARENA_WIDTH * ARENA_HEIGHT];

    char dir;
    int snakeX, snakeY, speedX, speedY;

    bool appleShine;
    int score = 5;

    float snakeMoveTime;
    float appleBlinkTime;

    Game(LedRenderer *renderer);
    float rawSnakeLength();
    float snakeLength();
    float snakeMoveRate();
    float appleBlinkRate();
    void tick(float deltaTime);
    void moveSnake();
    void checkDirection();
    void updateTiles(float deltaTime);
    void drawTiles();
    void spawnApple();
    Tile *getTile(int x, int y);
};
