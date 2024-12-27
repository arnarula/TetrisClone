#pragma once

#include <unordered_map>
#include <array>
#include <string>

#include "raylib.h"

enum PieceType
{
    L_PIECE,
    J_PIECE,
    I_PIECE,
    S_PIECE,
    Z_PIECE,
    T_PIECE,
    O_PIECE
};

constexpr unsigned char pieceAlpha = 170;
const inline std::unordered_map<PieceType, Color> pieceColors = {
    {L_PIECE, Color{255, 0, 0, pieceAlpha}},
    {J_PIECE, Color{0, 255, 0, pieceAlpha}},
    {I_PIECE, Color{0, 0, 255, pieceAlpha}},
    {S_PIECE, Color{255, 255, 0, pieceAlpha}},
    {Z_PIECE, Color{0, 255, 255, pieceAlpha}},
    {T_PIECE, Color{255, 0, 255, pieceAlpha}},
    {O_PIECE, Color{100, 100, 100, pieceAlpha}}
};

const inline std::unordered_map<PieceType, std::array<std::string, 4>> pieceShapes = {
    {L_PIECE, {
        "0X00",
        "0X00",
        "0XX0",
        "0000"
    }},
    {J_PIECE, {
        "00X0",
        "00X0",
        "0XX0",
        "0000"
    }},
    {I_PIECE, {
        "0000",
        "XXXX",
        "0000",
        "0000"
    }},
    {S_PIECE, {
        "0000",
        "00XX",
        "0XX0",
        "0000"
    }},
    {Z_PIECE, {
        "0000",
        "0XX0",
        "00XX",
        "0000"
    }},
    {T_PIECE, {
        "0000",
        "0X00",
        "XXX0",
        "0000"
    }},
    {O_PIECE, {
        "0000",
        "0XX0",
        "0XX0",
        "0000"
    }}
};

struct Piece
{
    PieceType pieceType;
    std::array<std::string, 4> blocks; // 4 x 4 block representation of the piece

    int x, y;
    bool hasLanded;

    Piece() : x(0), y(0), hasLanded(false)
    {
        pieceType = static_cast<PieceType>(GetRandomValue(0, (int) pieceColors.size() - 1));
        blocks = pieceShapes.at(pieceType);
    }

    Piece(PieceType _pieceType) : x(0), y(0), hasLanded(false)
    {
        pieceType = _pieceType;
        blocks = pieceShapes.at(pieceType);
    }

    void SetPosition(int newX, int newY)
    {
        x = newX;
        y = newY;
    }
};
