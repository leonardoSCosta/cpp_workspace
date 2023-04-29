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

    pmTela = QPixmap(ui->labelTela->size());
    fEscala = 30;
//    QSharedPointer<Bloco> blocoTeste = Bloco::GerarBloco();
//    qDebug() << "Gerando um bloco do tipo" << blocoTeste.get()->IDBloco;

    desenhaBloco(0, 0, Qt::gray);
    desenhaBloco(1, 0, Qt::gray);
    desenhaBloco(2, 0, Qt::gray);
    desenhaBloco(3, 0, Qt::gray);
    desenhaBloco(4, 0, Qt::gray);

    desenhaBloco(5, 10, Qt::red);

    desenhaBloco(4, 19, Qt::gray);
}

Jogo::~Jogo()
{
    delete ui;
}

void Jogo::desenhaBloco(int x, int y, QColor cor)
{
    QPainter pincel;

    pincel.begin(&pmTela);
    pincel.translate(QPointF(0, pmTela.height()));
    pincel.scale(fEscala, fEscala);

    pincel.setBrush(Qt::NoBrush);
    pincel.setPen(QPen(cor, 0.1));

    pincel.drawRect(QRectF(x, -(y + 0.1), 1, -1));

    pincel.end();

    ui->labelTela->setPixmap(pmTela);
}


void Jogo::clockTick()
{

}
