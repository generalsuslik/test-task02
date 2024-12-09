#include <QApplication>

#include "GridApp.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    GridApp gridApp;
    gridApp.setWindowTitle("Grid app");
    gridApp.resize(800, 600);
    gridApp.show();

    return QApplication::exec();
}