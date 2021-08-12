#include "oglview.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    oqlview w;
    w.show();
    return a.exec();
}
