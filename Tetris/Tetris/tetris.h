#ifndef TETRIS_H
#define TETRIS_H

#include "Bloco/bloco.h"
#include "qvector.h"

#define TETRIS_WIDTH 10
#define TETRIS_HEIGHT 20
#define SPAWN_POINT QPoint(TETRIS_WIDTH/2, TETRIS_HEIGHT-1)

class Tetris
{
    QSharedPointer<Bloco> currentBlock;
    QVector<QVector<bool>> matrix;

public: 
    Tetris();

    void tick();
    const Bloco* getBlock() const;
    const QVector<QVector<bool>> getGameMatrix() const;
    bool checkPlacement();
    void placeBlock();
};

#endif // TETRIS_H
