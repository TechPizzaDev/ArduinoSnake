#pragma once
#include "Renderer.h"
#include "SerialRenderer.h"
#include "Structure.h"

#define ARENA_WIDTH 16
#define ARENA_HEIGHT 16

#define LEFT_BUTTON 11
#define RIGHT_BUTTON 9
#define UP_BUTTON 12
#define DOWN_BUTTON 10

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
            setSnakeDuration(tile);
        }
        else if (tile->type == Apple)
        {
            score++;
            tile->type = Snake;
            setSnakeDuration(tile);
            spawnApple();
        }
        else if (tile->type == Snake)
        {
            snakeCollision(snakeX, snakeY);
        }
    }

    void setSnakeDuration(Tile* tile) 
    {
        tile->duration = 1.0 / getSnakeLength();
    }

    void snakeCollision(int snakeX, int snakeY)
    {
        reset();
    }

    void readDirectionInput()
    {
        return;

        bool up = digitalRead(UP_BUTTON) != 0;
        bool down = digitalRead(DOWN_BUTTON) != 0;
        bool left = digitalRead(LEFT_BUTTON) != 0;
        bool right = digitalRead(RIGHT_BUTTON) != 0;

        if (up)
        {
            setDirection('U');
        }
        else if (down)
        {
            setDirection('D');
        }
        else if (left)
        {
            setDirection('L');
        }
        else if (right)
        {
            setDirection('R');
        }
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
        readDirectionInput();

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

        //Serial.write(SerialOutgoingMessageType::Print);
        //Serial.print(digitalRead(UP_BUTTON));
        //Serial.print("-");
        //Serial.print(digitalRead(DOWN_BUTTON));
        //Serial.print("-");
        //Serial.print(digitalRead(LEFT_BUTTON));
        //Serial.print("-");
        //Serial.print(digitalRead(RIGHT_BUTTON));
        //Serial.println();
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
