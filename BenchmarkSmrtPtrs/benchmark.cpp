#include "benchmark.h"
#include "qglobal.h"
#include <cstdlib>

Benchmark::Benchmark(QObject *parent) : QObject(parent)
{
/*    qRegisterMetaType<Data>();

    double t = 0, tmin = 10, tmax = 0;
    QElapsedTimer timer;
    int N = 100e3;

    for(int i = 0; i < 20; ++i)
    {
        for(int n = 0; n < N; ++n)
        {
            timer.start();

            emit vSendData(Data());

            t += timer.nsecsElapsed()/1e9;
            tmin = qMin(tmin, timer.nsecsElapsed()/1e9);
            tmax = qMax(tmax, timer.nsecsElapsed()/1e9);
        }
        t /= N;

        qDebug() << QString("%1) T, Tmin, Tmax = (%2, %3, %4)").arg(i).arg(t).arg(tmin).arg(tmax);
    }
    exit(0);*/
    QSharedPointer<int> smrtPtr;
    smrtPtr.reset(new int);

    QWeakPointer<int> ptr = smrtPtr;

    smrtPtr.reset(new int);

    *ptr.toStrongRef() = 10;

    qDebug() << *ptr.toStrongRef() << *smrtPtr.get();
    exit(0);
}
