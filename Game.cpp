#include "Game.h"

Game::Game(LedRenderer *renderer)
{
    this->renderer = renderer;
    this->spawnApple();
}

float Game::rawSnakeLength()
{
    return (score + 1);
}

float Game::snakeLength()
{
    float rawLength = rawSnakeLength();
    return 1.0 / (rawLength * snakeMoveRate());
}

float Game::snakeMoveRate()
{
    float rawMoveRate = 1.0 / (score * 0.3 + 1);
    return rawMoveRate;
}

float Game::appleBlinkRate()
{
    return snakeMoveRate() / 4;
}

void Game::tick(float deltaTime)
{
    checkDirection();

    snakeMoveTime += deltaTime;
    if (snakeMoveTime >= snakeMoveRate())
    {
        snakeMoveTime -= snakeMoveRate();
        moveSnake();

        int b1 = digitalRead(10);
        int b2 = digitalRead(11);
        int b3 = digitalRead(12);
        int up = digitalRead(13);
        Serial.print(b1);
        Serial.print(" ");
        Serial.print(b2);
        Serial.print(" ");
        Serial.print(b3);
        Serial.print(" ");
        Serial.println(up);
    }

    appleBlinkTime += deltaTime;
    if (appleBlinkTime >= appleBlinkRate())
    {
        appleBlinkTime -= appleBlinkRate();
        appleShine = !appleShine;
    }

    updateTiles(deltaTime);

    drawTiles();
}

void Game::moveSnake()
{
    snakeX += speedX;
    snakeY += speedY;

    if (snakeX >= ARENA_WIDTH)
    {
        snakeX = 0;
    }
    else if (snakeX < 0)
    {
        snakeX = ARENA_WIDTH - 1;
    }

    if (snakeY >= ARENA_HEIGHT)
    {
        snakeY = 0;
    }
    else if (snakeY < 0)
    {
        snakeY = ARENA_HEIGHT - 1;
    }

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
        tile->duration = 1.0 / rawSnakeLength();
}

void Game::checkDirection()
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

    dir = Serial.read();
    switch (dir)
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

void Game::updateTiles(float deltaTime)
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

void Game::drawTiles()
{
    for (int y = 0; y < ARENA_HEIGHT; y++)
    {
        Tile *tileRow = tiles + y * ARENA_WIDTH;

        for (int x = 0; x < ARENA_WIDTH; x++)
        {
            Tile tile = tileRow[x];

            switch (tile.type)
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

void Game::spawnApple()
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
    {
        goto TrySpawn;
    }
}

Tile *Game::getTile(int x, int y)
{
    return tiles + y * ARENA_WIDTH + x;
}
