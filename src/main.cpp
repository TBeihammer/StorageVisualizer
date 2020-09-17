#include "StorageVisualizerMainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StorageVisualizerMainWindow w;
    w.show();
    return a.exec();
}
