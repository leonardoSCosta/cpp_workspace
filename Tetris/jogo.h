#ifndef JOGO_H
#define JOGO_H

#include <QMainWindow>
#include "Bloco/bloco.h"
#include <QTimer>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class Jogo; }
QT_END_NAMESPACE

#define TEMPO_CLOCK 16

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
    float fEscala;

    void desenhaBloco(int x, int y, QColor cor);

private slots:

    /**
     * @brief Base de tempo para o jogo
     */
    void clockTick();
};
#endif // JOGO_H
