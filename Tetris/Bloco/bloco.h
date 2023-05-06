#ifndef BLOCO_H
#define BLOCO_H

#include <QColor>
#include <QDebug>
#include <QPoint>
#include <QRandomGenerator>
#include <QSharedPointer>
#include <QVector>
#include <QtMath>

#define BLOCK_SIZE 4

/**
 * @brief Tipos possíveis de blocos
 */
typedef enum TiposBlocos
{
    BLOCO_I = 0,
    BLOCO_J,
    BLOCO_L,
    BLOCO_O,
    BLOCO_S,
    BLOCO_T,
    BLOCO_Z,
    N_BLOCOS // Indica a quantidade máxima de blocos (7)
} TiposBlocos;

class Bloco
{
protected:
    TiposBlocos IDBloco;
    QVector<QPoint> ptBlocks;
    QColor blockColor;

public:
    Bloco();
    virtual ~Bloco(){};

    /**
     * @brief Rotaciona o bloco no sentido horário.
     */
    void RotacionaH();

    /**
     * @brief Rotaciona o bloco no sentido anti-horário.
     */
    void RotacionaAH();

    /**
     * @brief Retorna a forma do bloco.
     *
     * @return Vetor com a posição de cada quadrado do bloco.
     */
    const QVector<QPoint> ptGetBlockShape() const;

    TiposBlocos type() const;

    QColor color() const;

    void desce();

    void translate(const QPoint& _to);

    bool checkCollision(const QPoint& _test) const;

    bool isInside(const int _limit) const;

    QPoint lowest() const;

    QPoint highest() const;

    /**
     * @brief Gera um bloco aleatório.
     *
     * @return
     */
    static QSharedPointer<Bloco> GerarBloco(
        QVector<TiposBlocos>& _generatedBlocks);
};

/*  ____  _     ___   ____ ___    ___
 * | __ )| |   / _ \ / ___/ _ \  |_ _|
 * |  _ \| |  | | | | |  | | | |  | |
 * | |_) | |__| |_| | |__| |_| |  | |
 * |____/|_____\___/ \____\___/  |___|
 */

class BlocoI : public Bloco
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

class BlocoJ : public Bloco
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

class BlocoL : public Bloco
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

class BlocoO : public Bloco
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

class BlocoS : public Bloco
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

class BlocoT : public Bloco
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

class BlocoZ : public Bloco
{

public:
    BlocoZ();
    ~BlocoZ();

    void RotacionaH();
    void RotacionaAH();
};

#endif // BLOCO_H
