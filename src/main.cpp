#include <QApplication>
#include <QDebug>

#include "qutepart.h"


int main(int argc, char** argv) {
    QApplication app(argc, argv);
    Qutepart qutepart;
    qutepart.show();

    qWarning() << "exec";
    return app.exec();
}
