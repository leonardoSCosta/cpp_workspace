#include <QCoreApplication>
#include "benchmark.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Benchmark benchmark;

    return a.exec();
}
