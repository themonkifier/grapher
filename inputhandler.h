#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "grapher.h"

#include <QObject>
#include <QEvent>
#include <QKeyEvent>

#include <unordered_map>

namespace input_handler {

class InputHandler : public QObject
{
    Q_OBJECT
public:
    explicit InputHandler(Grapher* grapher = nullptr, QObject* parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent* event);

private:
    Grapher* grapher;
};

} // namespace input_handler

#endif // INPUTHANDLER_H
