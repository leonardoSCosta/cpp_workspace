#include "bloco.h"
#include "qglobal.h"

Bloco::Bloco()
{

}

void Bloco::Desce()
{

}

const QVector<QPoint> Bloco::ptGetBlockShape() const
{
    return qAsConst(ptBlocks);
}

QSharedPointer<Bloco> Bloco::GerarBloco()
{
    TiposBlocos tipoBloco = static_cast<TiposBlocos>(QRandomGenerator::global()->bounded(N_BLOCOS));
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
}

BlocoI::~BlocoI()
{

}

void BlocoI::RotacionaH()
{

}

void BlocoI::RotacionaAH()
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
}

BlocoJ::~BlocoJ()
{

}

void BlocoJ::RotacionaH()
{

}

void BlocoJ::RotacionaAH()
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
}

BlocoL::~BlocoL()
{

}

void BlocoL::RotacionaH()
{

}

void BlocoL::RotacionaAH()
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
}

BlocoO::~BlocoO()
{

}

void BlocoO::RotacionaH()
{

}

void BlocoO::RotacionaAH()
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
}

BlocoS::~BlocoS()
{

}

void BlocoS::RotacionaH()
{

}

void BlocoS::RotacionaAH()
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
}

BlocoT::~BlocoT()
{

}

void BlocoT::RotacionaH()
{

}

void BlocoT::RotacionaAH()
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
}

BlocoZ::~BlocoZ()
{

}

void BlocoZ::RotacionaH()
{

}

void BlocoZ::RotacionaAH()
{

}
