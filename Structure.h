#pragma once

enum SerialOutgoingMessageType : byte
{
    Setup = 0,
    Print = 1,
    DisplayData = 2
};

enum SerialIncomingMessageType : byte
{
    SetupAcknowledge = 0,
    MoveDirection = 1
};

enum TileType : byte
{
    Empty = 0,
    Snake = 1,
    Apple = 2
};

struct Tile
{
    TileType type;
    float duration;
};
