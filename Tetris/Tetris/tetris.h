#ifndef TETRIS_H
#define TETRIS_H

#include "Bloco/bloco.h"
#include <QObject>
#include <QVector>

#define TETRIS_WIDTH 10
#define TETRIS_HEIGHT 20
#define SPAWN_POINT QPoint(TETRIS_WIDTH / 2, TETRIS_HEIGHT - 1)

class Tetris : public QObject
{
    Q_OBJECT

    QSharedPointer<Bloco> currentBlock;
    QVector<QVector<bool>> matrix;
    QPoint userMovement;
    QVector<TiposBlocos> generatedBlocks;

  public:
    Tetris();

    int tick();
    const Bloco* getBlock() const;
    const QVector<QVector<bool>> getGameMatrix() const;
    bool checkPlacement();
    void placeBlock();
    bool blockCollision();
    bool moveBlock(QPoint _direction);
    bool rotateBlock(char _direction);

    int clearRow();

signals:
    void gameOver();
};

#endif // TETRIS_H
