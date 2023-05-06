#ifndef JOGO_H
#define JOGO_H

#include "Tetris/tetris.h"
#include <QMainWindow>
#include <QPainter>
#include <QTimer>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui
{
class Jogo;
}
QT_END_NAMESPACE

#define TEMPO_CLOCK 500

class Jogo : public QMainWindow
{
    Q_OBJECT

  public:
    Jogo(QWidget* parent = nullptr);
    ~Jogo();

  private:
    Ui::Jogo* ui;
    QSharedPointer<QTimer> clock;
    QPixmap pmTela;
    float fEscalaX;
    float fEscalaY;
    int labelOffset;

    Tetris tetrisGame;

    void desenhaBloco(int x, int y, QColor cor, QColor inside);
    void desenhaJogo();
    void setup();

  private slots:

    /**
     * @brief Base de tempo para o jogo
     */
    void clockTick();
    void on_actionQ_triggered();
    void on_actionE_triggered();
    void on_actionA_triggered();
    void on_actionS_triggered();
    void on_actionD_triggered();

    void isGameOver();
};
#endif // JOGO_H
