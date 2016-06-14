#include <QApplication>
#include "tsofinder.h"

//THIS FILE CONTAINS main()

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TSOFinder w(argc,argv);
    w.show();

    return a.exec();
}
