#ifndef BLOCO_H
#define BLOCO_H

#include "qpoint.h"
#include "qvector.h"
#include <QVector>
#include <QPoint>
#include <QRandomGenerator>
#include <QSharedPointer>
#include <QDebug>

#define BLOCK_SIZE 4

/**
 * @brief Tipos possíveis de blocos
 */
typedef enum TiposBlocos
{
    BLOCO_I = 0,
    BLOCO_J    ,
    BLOCO_L    ,
    BLOCO_O    ,
    BLOCO_S    ,
    BLOCO_T    ,
    BLOCO_Z    ,
    N_BLOCOS // Indica a quantidade máxima de blocos (7)
}TiposBlocos;

class Bloco
{
public:
    Bloco();
    virtual ~Bloco() { };

    TiposBlocos IDBloco;
    QVector<QPoint> ptBlocks;

    /**
     * @brief Rotaciona o bloco no sentido horário.
     *
     * Este métido é virtual, pois cada bloco será rotacionado de uma forma,
     * mas todos os blocos devem rotacionar, ou seja, todos os blocos irão
     * possuir o método RotacionaH() para rotacioná-lo no sentido horário, mas, a
     * implementação será diferente para cada um. O modificador *virtual* força
     * o programador a implementar este método quando esta classe for herdada.
     */
    virtual void RotacionaH() = 0;

    /**
     * @brief Rotaciona o bloco no sentido anti-horário.
     */
    virtual void RotacionaAH() = 0;

    /**
     * @brief Retorna a forma do bloco.
     *
     * @return Vetor com a posição de cada quadrado do bloco.
     */
    const QVector<QPoint> ptGetBlockShape() const;

    /**
     * @brief Desce o bloco em uma unidade.
     *
     * Neste caso, pode não ser necessário implementar um método Desce() para
     * cada tipo de bloco, por conta disso este método não precisa ser virtual.
     */
    void Desce();

    /**
     * @brief Gera um bloco aleatório.
     *
     * @return
     */
    static QSharedPointer<Bloco> GerarBloco();
};

/*  ____  _     ___   ____ ___    ___
 * | __ )| |   / _ \ / ___/ _ \  |_ _|
 * |  _ \| |  | | | | |  | | | |  | |
 * | |_) | |__| |_| | |__| |_| |  | |
 * |____/|_____\___/ \____\___/  |___|
 */

class BlocoI: public Bloco
{

public:
    BlocoI();
    ~BlocoI();

    void RotacionaH();
    void RotacionaAH();
};

/*  ____  _     ___   ____ ___        _
 * | __ )| |   / _ \ / ___/ _ \      | |
 * |  _ \| |  | | | | |  | | | |  _  | |
 * | |_) | |__| |_| | |__| |_| | | |_| |
 * |____/|_____\___/ \____\___/   \___/
 */

class BlocoJ: public Bloco
{

public:
    BlocoJ();
    ~BlocoJ();

    void RotacionaH();
    void RotacionaAH();
};

/*  ____  _     ___   ____ ___    _
 * | __ )| |   / _ \ / ___/ _ \  | |
 * |  _ \| |  | | | | |  | | | | | |
 * | |_) | |__| |_| | |__| |_| | | |___
 * |____/|_____\___/ \____\___/  |_____|
 */

class BlocoL: public Bloco
{

public:
    BlocoL();
    ~BlocoL();

    void RotacionaH();
    void RotacionaAH();
};

/*  ____  _     ___   ____ ___     ___
 * | __ )| |   / _ \ / ___/ _ \   / _ \
 * |  _ \| |  | | | | |  | | | | | | | |
 * | |_) | |__| |_| | |__| |_| | | |_| |
 * |____/|_____\___/ \____\___/   \___/
 */

class BlocoO: public Bloco
{

public:
    BlocoO();
    ~BlocoO();

    void RotacionaH();
    void RotacionaAH();
};

/*  ____  _     ___   ____ ___    ____
 * | __ )| |   / _ \ / ___/ _ \  / ___|
 * |  _ \| |  | | | | |  | | | | \___ \
 * | |_) | |__| |_| | |__| |_| |  ___) |
 * |____/|_____\___/ \____\___/  |____/
 */

class BlocoS: public Bloco
{

public:
    BlocoS();
    ~BlocoS();

    void RotacionaH();
    void RotacionaAH();
};

/*  ____  _     ___   ____ ___    _____
 * | __ )| |   / _ \ / ___/ _ \  |_   _|
 * |  _ \| |  | | | | |  | | | |   | |
 * | |_) | |__| |_| | |__| |_| |   | |
 * |____/|_____\___/ \____\___/    |_|
 */

class BlocoT: public Bloco
{

public:
    BlocoT();
    ~BlocoT();

    void RotacionaH();
    void RotacionaAH();
};

/*  ____  _     ___   ____ ___    _____
 * | __ )| |   / _ \ / ___/ _ \  |__  /
 * |  _ \| |  | | | | |  | | | |   / /
 * | |_) | |__| |_| | |__| |_| |  / /_
 * |____/|_____\___/ \____\___/  /____|
 */

class BlocoZ: public Bloco
{

public:
    BlocoZ();
    ~BlocoZ();

    void RotacionaH();
    void RotacionaAH();
};

#endif // BLOCO_H
