#pragma once
#include "Renderer.h"
#include "SerialRenderer.h"
#include "Structure.h"

#define ARENA_WIDTH 16
#define ARENA_HEIGHT 16

class Game
{
public:
    Tile tiles[ARENA_WIDTH * ARENA_HEIGHT];

    int snakeX, snakeY, speedX, speedY;

    int score;
    bool appleShine;
    float snakeMoveTime;
    float appleBlinkTime;

    Game()
    {
        reset();
    }

    void reset()
    {
        memset(tiles, 0, sizeof(Tile) * ARENA_WIDTH * ARENA_HEIGHT);
        appleShine = false;
        snakeMoveTime = 0;
        appleBlinkTime = 0;
        snakeX = 0;
        snakeY = 0;
        speedX = 0;
        speedY = 0;

        score = 0;
        snakeX = 0;
        snakeY = 0;
        speedX = 1;
        speedY = 0;

        spawnApple();
    }

    float getRawSnakeLength()
    {
        return (score + 1);
    }

    float getSnakeLength()
    {
        float rawLength = getRawSnakeLength();
        return 1.0f / (rawLength * getSnakeMoveRate());
    }

    float getSnakeMoveRate()
    {
        return 1.0f / (score * 0.25f + 2);
    }

    float getAppleBlinkRate()
    {
        return getSnakeMoveRate() / 2.0f;
    }

    Tile *getTile(int x, int y)
    {
        return x + tiles + y * ARENA_WIDTH;
    }

    void moveSnake()
    {
        snakeX += speedX;
        snakeY += speedY;

        if (snakeX >= ARENA_WIDTH)
            snakeX = 0;
        else if (snakeX < 0)
            snakeX = ARENA_WIDTH - 1;

        if (snakeY >= ARENA_HEIGHT)
            snakeY = 0;
        else if (snakeY < 0)
            snakeY = ARENA_HEIGHT - 1;

        Tile *tile = getTile(snakeX, snakeY);
        if (tile->type == Empty)
        {
            tile->type = Snake;
        }
        else if (tile->type == Apple)
        {
            score++;
            tile->type = Snake;
            spawnApple();
        }

        if (tile->type == Snake)
            tile->duration = 1.0 / getSnakeLength();
    }

    void updateDirection()
    {
        //char key = keypad.getKey();
        //switch (key) {
        //  case '2': // up
        //    dir = 'U';
        //    break;
        //  case '4': // left
        //    dir = 'L';
        //    break;
        //  case '8': // down
        //    dir = 'D';
        //    break;
        //  case '6': // right
        //    dir = 'R';
        //    break;
        //}
    }

    void setDirection(char direction)
    {
        switch (direction)
        {
        case 'U':
            speedX = 0;
            speedY = -1;
            break;

        case 'L':
            speedX = -1;
            speedY = 0;
            break;

        case 'D':
            speedX = 0;
            speedY = 1;
            break;

        case 'R':
            speedX = 1;
            speedY = 0;
            break;
        }
    }

    void spawnApple()
    {
        int maxTries = 0;

    TrySpawn:
        int appleX = random(ARENA_WIDTH);
        int appleY = random(ARENA_HEIGHT);

        Tile *tile = getTile(appleX, appleY);
        if (tile->type == Empty)
        {
            tile->type = Apple;
            return;
        }

        maxTries++;
        if (maxTries < 10)
            goto TrySpawn;
    }

    void updateTiles(float deltaTime)
    {
        for (int y = 0; y < ARENA_HEIGHT; y++)
        {
            Tile *tileRow = tiles + y * ARENA_WIDTH;

            for (int x = 0; x < ARENA_WIDTH; x++)
            {
                Tile *tile = &tileRow[x];

                switch (tile->type)
                {
                case Snake:
                    if (tile->duration <= 0)
                    {
                        tile->type = Empty;
                        tile->duration = 0;
                    }
                    else
                    {
                        tile->duration -= deltaTime;
                    }
                    break;
                }
            }
        }
    }

    void tick(float deltaTime)
    {
        //updateDirection();

        snakeMoveTime += deltaTime;
        if (snakeMoveTime >= getSnakeMoveRate())
        {
            snakeMoveTime -= getSnakeMoveRate();
            moveSnake();
        }

        appleBlinkTime += deltaTime;
        if (appleBlinkTime >= getAppleBlinkRate())
        {
            appleBlinkTime -= getAppleBlinkRate();
            appleShine = !appleShine;
        }

        updateTiles(deltaTime);
    }

    void drawTiles(Renderer *renderer)
    {
        for (int y = 0; y < ARENA_HEIGHT; y++)
        {
            Tile *tileRow = tiles + y * ARENA_WIDTH;

            for (int x = 0; x < ARENA_WIDTH; x++)
            {
                Tile *tile = &tileRow[x];

                switch (tile->type)
                {
                case Snake:
                    renderer->setPixel(x, y, true);
                    break;

                case Apple:
                    if (appleShine)
                        renderer->setPixel(x, y, true);
                    else
                        renderer->setPixel(x, y, false);
                    break;

                case Empty:
                default:
                    renderer->setPixel(x, y, false);
                    break;
                }
            }
        }
    }

    int xd;

    void draw(Renderer *renderer)
    {
        drawTiles(renderer);
    }
};
