#include "updater.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/icons/icon.ico"));
    Updater w;
    w.show();

    return a.exec();
}
