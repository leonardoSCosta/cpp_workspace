#include "bloco.h"
#include "qglobal.h"

Bloco::Bloco()
{
}

void Bloco::RotacionaH()
{
    QPoint pivot = ptBlocks.at(0);
    for (QPoint& pt : ptBlocks)
    {
        pt -= pivot;
        pt = QPoint(pt.y(), -pt.x()) + pivot;
    }
}

void Bloco::RotacionaAH()
{
    QPoint pivot = ptBlocks.at(0);
    for (QPoint& pt : ptBlocks)
    {
        pt -= pivot;
        pt = QPoint(-pt.y(), pt.x()) + pivot;
    }
}

void Bloco::desce()
{
    for (QPoint& _pt : ptBlocks)
    {
        _pt = _pt - QPoint(0, 1);
    }
}

void Bloco::translate(const QPoint& _to)
{
    for (QPoint& _pt : ptBlocks)
    {
        _pt = _pt + _to;
    }
}

bool Bloco::checkCollision(const QPoint& _test) const
{
    for (const QPoint& _pt : ptBlocks)
    {
        if ((_test - _pt).manhattanLength() <= 0)
        {
            return true;
        }
    }
    return false;
}

bool Bloco::isInside(const int _limit) const
{
    for (const QPoint& _pt : ptBlocks)
    {
        if (_pt.x() < 0 || _pt.x() >= _limit || _pt.y() < 0)
        {
            return false;
        }
    }
    return true;
}

QPoint Bloco::lowest() const
{
    QPoint pos(0, 100);
    for (const QPoint& pt : ptBlocks)
    {
        if (pt.y() < pos.y())
        {
            pos = pt;
        }
    }
    return pos;
}

QPoint Bloco::highest() const
{
    QPoint pos(0, -100);
    for (const QPoint& pt : ptBlocks)
    {
        if (pt.y() > pos.y())
        {
            pos = pt;
        }
    }
    return pos;
}

const QVector<QPoint> Bloco::ptGetBlockShape() const
{
    return qAsConst(ptBlocks);
}

TiposBlocos Bloco::type() const
{
    return IDBloco;
}

QColor Bloco::color() const
{
    return blockColor;
}

QSharedPointer<Bloco> Bloco::GerarBloco(QVector<TiposBlocos>& _generatedBlocks)
{
    TiposBlocos tipoBloco;
    if (_generatedBlocks.size() == N_BLOCOS)
    {
        _generatedBlocks.clear();
    }

    do
    {
        tipoBloco = static_cast<TiposBlocos>(
            QRandomGenerator::global()->bounded(N_BLOCOS));
    } while (_generatedBlocks.contains(tipoBloco));
    _generatedBlocks.push_back(tipoBloco);

    QSharedPointer<Bloco> bloco;

    switch (tipoBloco)
    {
    case BLOCO_I:
        bloco.reset(new BlocoI);
        break;
    case BLOCO_J:
        bloco.reset(new BlocoJ);
        break;
    case BLOCO_L:
        bloco.reset(new BlocoL);
        break;
    case BLOCO_O:
        bloco.reset(new BlocoO);
        break;
    case BLOCO_S:
        bloco.reset(new BlocoS);
        break;
    case BLOCO_T:
        bloco.reset(new BlocoT);
        break;
    case BLOCO_Z:
        bloco.reset(new BlocoZ);
        break;
    default:
        qDebug() << "ID de bloco inválido\n";
        bloco.reset(new BlocoI);
        break;
    }
    return bloco;
}

/*  ____  _     ___   ____ ___    ___
 * | __ )| |   / _ \ / ___/ _ \  |_ _|
 * |  _ \| |  | | | | |  | | | |  | |
 * | |_) | |__| |_| | |__| |_| |  | |
 * |____/|_____\___/ \____\___/  |___|
 */

BlocoI::BlocoI()
{
    IDBloco = BLOCO_I;
    // I (0, 3)
    // I (0, 2)
    // I (0, 1)
    // I (0, 0)
    ptBlocks.append(QPoint(0, 0));
    ptBlocks.append(QPoint(0, 1));
    ptBlocks.append(QPoint(0, 2));
    ptBlocks.append(QPoint(0, 3));

    blockColor = QColor::fromRgb(0, 240, 240);
}

BlocoI::~BlocoI()
{
}

/*  ____  _     ___   ____ ___        _
 * | __ )| |   / _ \ / ___/ _ \      | |
 * |  _ \| |  | | | | |  | | | |  _  | |
 * | |_) | |__| |_| | |__| |_| | | |_| |
 * |____/|_____\___/ \____\___/   \___/
 */

BlocoJ::BlocoJ()
{
    IDBloco = BLOCO_J;
    // J     (0, 1)
    // J J J (0, 0) (1,0) (2,0)
    ptBlocks.append(QPoint(0, 0));
    ptBlocks.append(QPoint(1, 0));
    ptBlocks.append(QPoint(2, 0));
    ptBlocks.append(QPoint(0, 1));

    blockColor = QColor::fromRgb(0, 0, 240);
}

BlocoJ::~BlocoJ()
{
}

/*  ____  _     ___   ____ ___    _
 * | __ )| |   / _ \ / ___/ _ \  | |
 * |  _ \| |  | | | | |  | | | | | |
 * | |_) | |__| |_| | |__| |_| | | |___
 * |____/|_____\___/ \____\___/  |_____|
 */

BlocoL::BlocoL()
{
    IDBloco = BLOCO_L;
    //     L               (2, 1)
    // L L L (0, 0) (1, 0) (2, 0)
    ptBlocks.append(QPoint(0, 0));
    ptBlocks.append(QPoint(1, 0));
    ptBlocks.append(QPoint(2, 0));
    ptBlocks.append(QPoint(2, 1));

    blockColor = QColor::fromRgb(240, 160, 0);
}

BlocoL::~BlocoL()
{
}

/*  ____  _     ___   ____ ___     ___
 * | __ )| |   / _ \ / ___/ _ \   / _ \
 * |  _ \| |  | | | | |  | | | | | | | |
 * | |_) | |__| |_| | |__| |_| | | |_| |
 * |____/|_____\___/ \____\___/   \___/
 */

BlocoO::BlocoO()
{
    IDBloco = BLOCO_O;
    // O O (0, 1) (1, 1)
    // O O (0, 0) (1, 0)
    ptBlocks.append(QPoint(0, 0));
    ptBlocks.append(QPoint(1, 0));
    ptBlocks.append(QPoint(0, 1));
    ptBlocks.append(QPoint(1, 1));

    blockColor = QColor::fromRgb(240, 240, 0);
}

BlocoO::~BlocoO()
{
}

/*  ____  _     ___   ____ ___    ____
 * | __ )| |   / _ \ / ___/ _ \  / ___|
 * |  _ \| |  | | | | |  | | | | \___ \
 * | |_) | |__| |_| | |__| |_| |  ___) |
 * |____/|_____\___/ \____\___/  |____/
 */

BlocoS::BlocoS()
{
    IDBloco = BLOCO_S;
    //   S S        (1, 1) (2, 1)
    // S S   (0, 0) (1, 0)
    ptBlocks.append(QPoint(0, 0));
    ptBlocks.append(QPoint(1, 0));
    ptBlocks.append(QPoint(1, 1));
    ptBlocks.append(QPoint(2, 1));

    blockColor = QColor::fromRgb(0, 240, 0);
}

BlocoS::~BlocoS()
{
}

/*  ____  _     ___   ____ ___    _____
 * | __ )| |   / _ \ / ___/ _ \  |_   _|
 * |  _ \| |  | | | | |  | | | |   | |
 * | |_) | |__| |_| | |__| |_| |   | |
 * |____/|_____\___/ \____\___/    |_|
 */

BlocoT::BlocoT()
{
    IDBloco = BLOCO_T;
    //   T          (1, 1)
    // T T T (0, 0) (1, 0) (2, 0)
    ptBlocks.append(QPoint(0, 0));
    ptBlocks.append(QPoint(1, 1));
    ptBlocks.append(QPoint(1, 0));
    ptBlocks.append(QPoint(2, 0));

    blockColor = QColor::fromRgb(160, 0, 240);
}

BlocoT::~BlocoT()
{
}

/*  ____  _     ___   ____ ___    _____
 * | __ )| |   / _ \ / ___/ _ \  |__  /
 * |  _ \| |  | | | | |  | | | |   / /
 * | |_) | |__| |_| | |__| |_| |  / /_
 * |____/|_____\___/ \____\___/  /____|
 */

BlocoZ::BlocoZ()
{
    IDBloco = BLOCO_Z;
    // Z Z   (0, 1) (1, 1)
    //   Z Z        (1, 0) (2, 0)
    ptBlocks.append(QPoint(0, 1));
    ptBlocks.append(QPoint(1, 1));
    ptBlocks.append(QPoint(1, 0));
    ptBlocks.append(QPoint(2, 0));

    blockColor = QColor::fromRgb(240, 0, 0);
}

BlocoZ::~BlocoZ()
{
}
