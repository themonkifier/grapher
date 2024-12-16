#include "inputhandler.h"

input_handler::InputHandler::InputHandler(Grapher* grapher, QObject* parent)
    : QObject{parent}
    , grapher(grapher)
{}

bool input_handler::InputHandler::eventFilter(QObject* obj, QEvent* event) {
    if (event->type() == QEvent::KeyPress) {
        switch (static_cast<QKeyEvent*>(event)->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:
            grapher->on_newCalculatorButton_clicked();
            break;
        case Qt::Key_Alt:
            grapher->on_newMatrixButton_clicked();
            break;
        case Qt::Key_Tab:
            grapher->focusNextWindow();
            break;
        default:
            return QObject::eventFilter(obj, event);
        }

        return true;  // default wasn't triggered
    }
    return QObject::eventFilter(obj, event);
}
