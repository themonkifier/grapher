#ifndef CALCULATORLINE_H
#define CALCULATORLINE_H

#include "calculator/calculator.h"
#include "calculator/lexer.h"
#include "calculator/util/context.h"
#include "calculator/util/errors.h"
#include "calculator/matrix/matrix.h"

#include <QWidget>
#include <QLayout>
#include <QLineEdit>
#include <QLabel>
#include <QTableWidget>

namespace calculator {

class CalculatorLine : public QWidget
{
    Q_OBJECT

public:
    explicit CalculatorLine(ContextPtr context, QLayout* layout, QWidget* parent = nullptr);
    ~CalculatorLine();

private slots:
    void recalculate(QString new_input);

private:
    Calculator calculator;
    std::shared_ptr<Context> context;
    QLineEdit* input;
    QLabel* scalar_output;
    QTableWidget* table_output;
    QLayout* layout;
    QWidget* current_widget;
};

} // namespace calculator

#endif // CALCULATORLINE_H
