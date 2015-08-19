#include "embeddedsys-emulator.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    EmbeddedSysEmu w;
    w.show();
    
    return a.exec();
}
