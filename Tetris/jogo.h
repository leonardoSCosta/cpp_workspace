#ifndef JOGO_H
#define JOGO_H

#include <QMainWindow>
#include "Tetris/tetris.h"
#include <QTimer>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class Jogo; }
QT_END_NAMESPACE

#define TEMPO_CLOCK 200

class Jogo : public QMainWindow
{
    Q_OBJECT

public:
    Jogo(QWidget *parent = nullptr);
    ~Jogo();

private:
    Ui::Jogo *ui;
    QSharedPointer<QTimer> clock;
    QPixmap pmTela;
    float fEscalaX;
    float fEscalaY;
    int labelOffset;

    Tetris tetrisGame;

    void desenhaBloco(int x, int y, QColor cor);
    void setup();

private slots:

    /**
     * @brief Base de tempo para o jogo
     */
    void clockTick();
};
#endif // JOGO_H
