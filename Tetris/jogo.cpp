#include "jogo.h"

#include "ui_jogo.h"
#include <cstdlib>

Jogo::Jogo(QWidget* parent) : QMainWindow(parent), ui(new Ui::Jogo)
{
    ui->setupUi(this);

    clock.reset(new QTimer);
    connect(clock.get(), &QTimer::timeout, this, &Jogo::clockTick);
    clock.get()->start(TEMPO_CLOCK);

    labelOffset = 8;
    connect(&tetrisGame, &Tetris::gameOver, this, &Jogo::isGameOver);
}

Jogo::~Jogo()
{
    delete ui;
}

void Jogo::desenhaBloco(int x, int y, QColor cor, QColor inside)
{
    QPainter pincel;

    pincel.begin(&pmTela);
    pincel.translate(
        QPointF(labelOffset / 4.0, pmTela.height() - labelOffset / 4.0));
    pincel.scale(fEscalaX, fEscalaY);

    pincel.setBrush(inside);
    pincel.setPen(QPen(cor, 0.1));

    pincel.drawRect(QRectF(x, -(y + 2 / fEscalaY), 1, -1));

    pincel.end();
}

void Jogo::desenhaJogo()
{
    setup();

    if (tetrisGame.getBlock())
    {
        for (const QPoint& pt : tetrisGame.getBlock()->ptGetBlockShape())
        {
            desenhaBloco(pt.x(), pt.y(), Qt::gray,
                         tetrisGame.getBlock()->color());
        }
    }

    int x = 0, y = 0;
    for (const QVector<bool>& rows : tetrisGame.getGameMatrix())
    {
        y = 0;
        for (const bool& occupied : rows)
        {
            if (occupied)
            {
                desenhaBloco(x, y, Qt::gray, QColor::fromRgb(80, 250, 123));
            }
            y++;
        }
        x++;
    }

    ui->labelTela->setPixmap(pmTela);
}

void Jogo::setup()
{
    if (ui->labelTela->size() != pmTela.size())
    {
        pmTela = QPixmap(ui->labelTela->size());
    }
    pmTela.fill(Qt::black);
    fEscalaX = 1.f * (pmTela.size().width() - labelOffset) / TETRIS_WIDTH;
    fEscalaY = 1.f * (pmTela.size().height() - labelOffset) / TETRIS_HEIGHT;
}

void Jogo::clockTick()
{
    int score = ui->lcdNumber->value();
    score += tetrisGame.tick();
    ui->lcdNumber->display(score);
    desenhaJogo();
}

void Jogo::on_actionQ_triggered()
{
    tetrisGame.rotateBlock('Q');
    desenhaJogo();
}

void Jogo::on_actionE_triggered()
{
    tetrisGame.rotateBlock('E');
    desenhaJogo();
}

void Jogo::on_actionA_triggered()
{
    tetrisGame.moveBlock(QPoint(-1, 0));
    desenhaJogo();
}

void Jogo::on_actionS_triggered()
{
    tetrisGame.moveBlock(QPoint(0, -1));
    desenhaJogo();
}

void Jogo::on_actionD_triggered()
{
    tetrisGame.moveBlock(QPoint(1, 0));
    desenhaJogo();
}

void Jogo::isGameOver()
{
    QMessageBox msgBox;
    msgBox.setText(QString("Game Over 😓😥\nSua pontuação máxima foi %1")
                       .arg(ui->lcdNumber->value()));
    msgBox.exec();
    exit(0);
}
