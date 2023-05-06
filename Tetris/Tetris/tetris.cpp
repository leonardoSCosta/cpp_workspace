#include "tetris.h"

#include "qvector.h"

Tetris::Tetris()
{
    matrix.resize(TETRIS_WIDTH);
    generatedBlocks.clear();

    for (QVector<bool>& row : matrix)
    {
        row.resize(TETRIS_HEIGHT);
    }
}

int Tetris::tick()
{
    if (!currentBlock)
    {
        currentBlock = Bloco::GerarBloco(generatedBlocks);
        currentBlock->translate(SPAWN_POINT);
    }
    else if (checkPlacement())
    {
        if (not moveBlock({0, -1}))
        {
            placeBlock();
        }
    }
    else
    {
        placeBlock();
    }

    return clearRow();
}

const QVector<QVector<bool>> Tetris::getGameMatrix() const
{
    return matrix;
}

const Bloco* Tetris::getBlock() const
{
    return currentBlock.get();
}

bool Tetris::checkPlacement()
{
    bool placementOk = true;
    QPoint minPos = currentBlock->lowest();

    if (minPos.y() == 0)
    {
        placementOk = false;
    }
    else
    {
        int n = 0;
        for (QVector<bool>& rows : matrix)
        {
            if (rows.at(minPos.y() - 1) &&
                currentBlock->checkCollision({n, minPos.y()}))
            {
                placementOk = false;
                break;
            }
            ++n;
        }
    }

    return placementOk;
}

void Tetris::placeBlock()
{
    if (currentBlock->highest().y() >= TETRIS_HEIGHT)
    {
        qDebug() << "GAME OVER";
        emit gameOver();
    }
    else
    {
        for (const QPoint& pt : currentBlock->ptGetBlockShape())
        {
            matrix[pt.x()][pt.y()] = true;
        }
        currentBlock.reset();
    }
}

bool Tetris::blockCollision()
{
    if (currentBlock)
    {
        for (const QPoint& pt : currentBlock->ptGetBlockShape())
        {
            if (pt.x() >= 0 && pt.x() < TETRIS_WIDTH && pt.y() >= 0 &&
                pt.y() < TETRIS_HEIGHT)
            {
                if (matrix.at(pt.x()).at(pt.y()) == true)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

bool Tetris::moveBlock(QPoint _direction)
{
    bool ok = true;
    if (currentBlock)
    {
        userMovement = _direction;
        if (checkPlacement() || _direction.y() == 0)
        {
            if (userMovement != QPoint(-10, -10))
            {
                currentBlock->translate(userMovement);

                if (currentBlock->isInside(TETRIS_WIDTH) == false ||
                    blockCollision())
                {
                    currentBlock->translate(-userMovement);
                    ok = false;
                }
                userMovement.setX(-10);
                userMovement.setY(-10);
            }
        }
    }
    return ok;
}

bool Tetris::rotateBlock(char _direction)
{
    bool ok = true;
    if (currentBlock)
    {
        switch (_direction)
        {
        case 'Q': {
            currentBlock->RotacionaAH();
            while (currentBlock->isInside(TETRIS_WIDTH) == false ||
                   blockCollision())
            {
                currentBlock->RotacionaH();
                currentBlock->translate({1, 0});
                currentBlock->RotacionaAH();
            }
        }
        break;
        case 'E': {
            currentBlock->RotacionaH();
            if (currentBlock->isInside(TETRIS_WIDTH) == false ||
                blockCollision())
            {
                currentBlock->RotacionaAH();
                currentBlock->translate({-1, 0});
                currentBlock->RotacionaH();
            }
        }
        break;
        }
    }
    return ok;
}

int Tetris::clearRow()
{
    int points = 0;
    for (int row = 0; row < TETRIS_HEIGHT; ++row)
    {
        bool fullRow = true;
        for (int col = 0; col < TETRIS_WIDTH; ++col)
        {
            fullRow = matrix.at(col).at(row) & fullRow;
        }

        if (fullRow)
        {
            points += 10;
            // Remove linha
            for (int col = 0; col < TETRIS_WIDTH; ++col)
            {
                matrix[col][row] = false;
            }

            // Desloca linhas
            for (int i = row + 1; i < TETRIS_HEIGHT; ++i, ++row)
            {
                for (int col = 0; col < TETRIS_WIDTH; ++col)
                {
                    matrix[col][row] = matrix[col][i];
                }
            }
            row = 0;
        }
    }
    return points;
}
