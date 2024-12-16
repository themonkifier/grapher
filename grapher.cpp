#include "grapher.h"
#include "./ui_grapher.h"

Grapher::Grapher(QWidget *parent)
    : QMainWindow{parent}
    , ui{new Ui::Grapher}
{
    context = std::make_shared<Context>();

    ui->setupUi(this);

    ui->settingsMenuButton->setCheckable(true);
    ui->settingsGroupBox->setVisible(false);

    ui->newMenuButton->setCheckable(true);
    ui->newGroupBox->setVisible(false);

    on_newCalculatorButton_clicked();

    ui->darkLightThemeButton->setCheckable(true);

    ui->mainArea->setLayout(new QGridLayout);
    ui->mainArea->setBackground(QColor(0, 0, 0, 0));

    ui->darkLightThemeButton->setText("🌞");

    dark_theme = QString::fromUtf8(" {border: 1px solid rgb(37, 37, 37); background-color: rgb(25, 25, 25); color: rgb(232, 232, 232); font: italic 12pt \"Ubuntu\";}");
    light_theme = QString::fromUtf8(" {border: 1px solid rgb(220, 220, 220); background-color: rgb(232, 232, 232); color: rgb(25, 25, 25); font: italic 12pt \"Ubuntu\";}");
    objects = QList<QWidget*>{
        ui->centralwidget,
        ui->darkLightThemeButton,
        ui->newMenuButton,
        ui->settingsMenuButton,
        ui->newCalculatorButton,
        ui->newMatrixButton,
        ui->newGroupBox,
        ui->settingsGroupBox,
        ui->mainArea
    };

    for (QWidget* widget : objects) {
        widget->setStyleSheet(
            QString::fromUtf8(widget->metaObject()->className()) + dark_theme
        );
    }
}

Grapher::~Grapher()
{
    for (calculator::CalculatorLine* line : calculator_lines) {
        delete line;
    }

    for (matrix::MatrixLine* line : matrix_lines) {
        delete line;
    }

    delete ui->mainArea->layout();
    delete ui;
}

void Grapher::on_settingsMenuButton_toggled(bool checked)
{
    if (checked) {
        ui->settingsMenuButton->setText("-");
    } else {
        ui->settingsMenuButton->setText("+");
    }

    ui->settingsGroupBox->setVisible(checked);
}


void Grapher::on_newMenuButton_toggled(bool checked)
{
    if (checked) {
        ui->newMenuButton->setText("-");
    } else {
        ui->newMenuButton->setText("...");
    }

    ui->newGroupBox->setVisible(checked);
}

void Grapher::on_newCalculatorButton_clicked()
{
    QWidget* box = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout{box};

    calculator::CalculatorLine* calc = new calculator::CalculatorLine{context, layout};
    calculator_lines.append(calc);

    QMdiSubWindow* new_window = ui->mainArea->addSubWindow(box);
    new_window->setWindowFlags(
        new_window->windowFlags() &
        ~Qt::WindowMaximizeButtonHint
    );  // disable maximize button
    new_window->setWindowFlags(
        new_window->windowFlags() |
        Qt::CustomizeWindowHint |
        Qt::WindowMinimizeButtonHint |
        Qt::WindowCloseButtonHint
    );  // enable closing and minimizing
    new_window->setAttribute(Qt::WA_DeleteOnClose);
    new_window->show();
}

void Grapher::on_newMatrixButton_clicked()
{
    QWidget* box = new QWidget;
    QVBoxLayout* layout = new QVBoxLayout(box);

    matrix::MatrixLine* mat = new matrix::MatrixLine(3, 3, context, layout);
    matrix_lines.append(mat);

    QMdiSubWindow* new_window = ui->mainArea->addSubWindow(box);
    new_window->setWindowFlags(
        new_window->windowFlags() &
        ~Qt::WindowMaximizeButtonHint
    );  // disable maximize button
    new_window->setWindowFlags(
        new_window->windowFlags() |
        Qt::CustomizeWindowHint |
        Qt::WindowMinimizeButtonHint |
        Qt::WindowCloseButtonHint
    );  // enable closing and minimizing
    new_window->setAttribute(Qt::WA_DeleteOnClose);
    new_window->show();
}

void Grapher::on_darkLightThemeButton_toggled(bool light_mode)
{
    if (light_mode) {
        ui->darkLightThemeButton->setText("🌕");

        for (QWidget* widget : objects) {
            widget->setStyleSheet(
                QString::fromUtf8(widget->metaObject()->className()) +
                light_theme
            );
        }
    } else {
        ui->darkLightThemeButton->setText("🌞");

        for (QWidget* widget : objects) {
            widget->setStyleSheet(
                QString::fromUtf8(widget->metaObject()->className()) +
                dark_theme
            );
        }
    }
}

void Grapher::focusNextWindow() {
    ui->mainArea->activateNextSubWindow();
}
