#ifndef BENCHMARK_H
#define BENCHMARK_H

#include "qvector.h"
#include <QObject>
#include <QScopedPointer>
#include <QElapsedTimer>
#include <QDebug>
#include <QVector2D>

// #define SCOPED

class PointerData
{

public:
    QVector2D pos;
    QVector<QVector2D> path;

    PointerData()
    {
        pos = QVector2D(0, 0);
        path.clear();
    }

    PointerData(const PointerData &_other)
    {
        pos = _other.pos;
        path.clear();
        path.append(_other.path);
    }
};

class Data
{

public:

#ifdef SCOPED
    QScopedPointer<PointerData> dPointer;
    Data()
    {
        dPointer.reset(new PointerData());
    }

    Data(const Data &_other)
    {
        dPointer.reset(new PointerData(*_other.dPointer.get()));
    }

    ~Data()
    {

    }
#else
    PointerData *dPointer;
    Data()
    {
        dPointer = new PointerData();
    }

    Data(const Data &_other)
    {
        dPointer = new PointerData(*_other.dPointer);
    }

    ~Data()
    {
        delete dPointer;
    }
#endif

};


class Benchmark : public QObject
{
    Q_OBJECT
public:
    explicit Benchmark(QObject *parent = nullptr);

signals:
    void vSendData(const Data _data);

};

Q_DECLARE_METATYPE(Data)

#endif // BENCHMARK_H
