#include "grapher.h"
#include "inputhandler.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Grapher grapher;

    input_handler::InputHandler input_handler(&grapher);
    grapher.installEventFilter(&input_handler);

    grapher.show();
    return app.exec();
}
