#include "jogo.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Jogo w;
    w.show();
    return a.exec();
}
