#include "jogo.h"
#include "ui_jogo.h"

Jogo::Jogo(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Jogo)
{
    ui->setupUi(this);

    clock.reset(new QTimer);
    connect(clock.get(), &QTimer::timeout, this, &Jogo::clockTick);
    clock.get()->start(TEMPO_CLOCK);

    labelOffset = 8;
}

Jogo::~Jogo()
{
    delete ui;
}

void Jogo::desenhaBloco(int x, int y, QColor cor)
{
    QPainter pincel;

    pincel.begin(&pmTela);
    pincel.translate(QPointF(labelOffset / 4.0,
                             pmTela.height() - labelOffset / 4.0));
    pincel.scale(fEscalaX, fEscalaY);

    pincel.setBrush(Qt::NoBrush);
    pincel.setPen(QPen(cor, 0.1));

    pincel.drawRect(QRectF(x, -(y + 2/fEscalaY), 1, -1));

    pincel.end();
}

void Jogo::setup()
{
    if(ui->labelTela->size() != pmTela.size()) 
    {
        pmTela = QPixmap(ui->labelTela->size());
    }
    pmTela.fill(Qt::black);
    fEscalaX = 1.f * (pmTela.size().width() - labelOffset) / TETRIS_WIDTH;
    fEscalaY = 1.f * (pmTela.size().height() - labelOffset) / TETRIS_HEIGHT;
}


void Jogo::clockTick()
{
    setup();

    tetrisGame.tick();

    if(tetrisGame.getBlock()) 
    {
        for(const QPoint &pt : tetrisGame.getBlock()->ptGetBlockShape()) 
        {
            desenhaBloco(pt.x(), pt.y(), tetrisGame.getBlock()->color());
        }
    }

    int x = 0, y = 0;
    for(const QVector<bool> &rows : tetrisGame.getGameMatrix()) 
    {
        y = 0;
        for(const bool& occupied : rows)
        {
            if(occupied)
            {
                desenhaBloco(x, y, Qt::gray);
            }
            y++;
        }
        x++;
    }

    ui->labelTela->setPixmap(pmTela);
}
