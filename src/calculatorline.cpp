#include "calculatorline.h"
#include <sstream>

namespace calculator {

CalculatorLine::CalculatorLine(ContextPtr context, QLayout* layout, QWidget* parent)
    : QWidget{parent},
      context{context},
      input{new QLineEdit},
      scalar_output{new QLabel},
      table_output{new QTableWidget},
      layout{layout},
      current_widget{scalar_output}
{
    layout->addWidget(input);
    layout->addWidget(scalar_output);

    connect(input, SIGNAL(textChanged(QString)), SLOT(recalculate(QString)));
}

CalculatorLine::~CalculatorLine() {
    delete input;
    if (current_widget != scalar_output) {
        delete scalar_output;
    } else {
        delete table_output;
    }
}

void CalculatorLine::recalculate(QString new_input) {
    std::stringstream text;

    try {
        Variable result = calculator.eval(new_input.toStdString(), context);

        if (result.index() == 0) {
            text << to_string(std::get<complex>(result));
        } else if (result.index() == 1) {
            Matrix& mat = std::get<Matrix>(result);

            table_output->setRowCount(mat.get_rows());
            table_output->setColumnCount(mat.get_columns());
            // populate table_output
            for (int i = 0; i < mat.get_rows(); ++i) {
                for (int j = 0; j < mat.get_columns(); ++j) {
                    QString text = QString::fromStdString(to_string(mat[i][j]));
                    table_output->setItem(i, j, new QTableWidgetItem(text));
                }
            }

            if (current_widget != table_output) {
                layout->removeWidget(current_widget);
                layout->addWidget(table_output);
                current_widget = table_output;
                scalar_output->hide();
                table_output->show();
            }
            return;
        }
    } catch (const std::runtime_error& e) {
        text << e.what();
    }

    if (current_widget != scalar_output) {
        layout->removeWidget(current_widget);
        layout->addWidget(scalar_output);
        current_widget = scalar_output;
        table_output->hide();
        scalar_output->show();
    }
    scalar_output->setText(QString::fromStdString(text.str()));
}

} // namespace calculator
