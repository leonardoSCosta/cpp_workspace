#include "tetris.h"
#include "qvector.h"

Tetris::Tetris()
{
    matrix.resize(TETRIS_WIDTH);

    for(QVector<bool> &row : matrix)
    {
        row.resize(TETRIS_HEIGHT);
    }
}


void Tetris::tick()
{
  if(!currentBlock) 
  {
    currentBlock = Bloco::GerarBloco();
    currentBlock->translate(SPAWN_POINT);
  }
  else if (checkPlacement()) 
  {
    currentBlock->desce();
  }
  else
  {
    placeBlock();
  }
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

  if(minPos.y() == 0)
  {
    placementOk = false;
  }
  else
  {
    int n = 0;
    for(QVector<bool> &rows : matrix)
    {
      if(rows.at(minPos.y()-1) && currentBlock->checkCollision({n, minPos.y()}))
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
  if(currentBlock->highest().y() >= TETRIS_HEIGHT)
  {
      // GAME OVER
  }
  else
  {
    for(const QPoint &pt : currentBlock->ptGetBlockShape())
    {
      matrix[pt.x()][pt.y()] = true;
    }
    currentBlock.reset();
  }
}
