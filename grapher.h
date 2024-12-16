#ifndef GRAPHER_H
#define GRAPHER_H

#include <QMainWindow>
#include <QPushButton>
#include <QRadioButton>
#include <QMdiSubWindow>
#include <QVBoxLayout>
#include <QStyle>
#include <QList>

#include "src/calculator/calculator.h"
#include "src/calculatorline.h"
#include "src/matrixline.h"

namespace input_handler {
class InputHandler;
}

QT_BEGIN_NAMESPACE
namespace Ui {
class Grapher;
}
QT_END_NAMESPACE

class Grapher : public QMainWindow
{
    Q_OBJECT

    friend class input_handler::InputHandler;

public:
    Grapher(QWidget *parent = nullptr);
    ~Grapher();

private slots:
    void on_settingsMenuButton_toggled(bool checked);
    void on_newMenuButton_toggled(bool checked);
    void on_newCalculatorButton_clicked();
    void on_newMatrixButton_clicked();
    void on_darkLightThemeButton_toggled(bool light_mode);
    void focusNextWindow();

private:
    QString light_theme, dark_theme;
    QList<QWidget*> objects;
    QList<calculator::CalculatorLine*> calculator_lines;
    QList<matrix::MatrixLine*> matrix_lines;
    ContextPtr context;
    Ui::Grapher* ui;
};
#endif // GRAPHER_H
