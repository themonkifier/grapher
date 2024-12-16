#include "matrixline.h"
#include <sstream>

namespace matrix {

MatrixLine::MatrixLine(std::size_t rows, std::size_t columns, ContextPtr context, QLayout* layout, QWidget* parent)
    : QWidget{parent},
      rows{rows},
      columns{columns},
      matrix{rows, columns, context},
      name{},
      layout{layout},
      name_input{new QLineEdit},
      table{new QTableWidget(rows, columns, parent)},
      rows_spinbox{new QSpinBox},
      columns_spinbox{new QSpinBox},
      output{new QLabel}
{
    layout->addWidget(name_input);
    layout->addWidget(rows_spinbox);
    layout->addWidget(columns_spinbox);
    layout->addWidget(table);
    layout->addWidget(output);

    name_input->setPlaceholderText("Matrix name");

    rows_spinbox->setMinimum(1);
    rows_spinbox->setSingleStep(1);
    rows_spinbox->setValue(rows);

    columns_spinbox->setMinimum(1);
    columns_spinbox->setSingleStep(1);
    columns_spinbox->setValue(columns);

    connect(name_input, SIGNAL(textChanged(QString)), SLOT(rename(QString)));
    connect(rows_spinbox, SIGNAL(valueChanged(int)), SLOT(resize_rows(int)));
    connect(columns_spinbox, SIGNAL(valueChanged(int)), SLOT(resize_columns(int)));
    connect(table, SIGNAL(cellChanged(int, int)), SLOT(recalculate(int, int)));
}

MatrixLine::~MatrixLine() {
    delete layout;
    delete table;
    delete name_input;
    delete output;
}

void MatrixLine::rename(QString new_name) {
    matrix.reset(new_name.toStdString(), matrix);
    name = new_name;
}

void MatrixLine::resize_rows(int rows) {
    resize(rows, columns);
}

void MatrixLine::resize_columns(int columns) {
    resize(rows, columns);
}

void MatrixLine::resize(size_t rows, size_t columns) {
    this->rows = rows;
    this->columns = columns;

    table->setRowCount(rows);
    table->setColumnCount(columns);

    matrix.resize(rows, columns);
}

void MatrixLine::recalculate(int row, int column) {
    std::stringstream text;
    try {
        std::string new_input = table->item(row, column)->text().toStdString();
        complex result = matrix.set(row, column, new_input);
        matrix.reset(name.toStdString(), matrix);
        text << to_string(result);
    } catch (const std::invalid_argument& e) {
        if (std::strcmp(e.what(), "stof") == 0) {
            text << "'-' feels so lonely :(";
        }
    } catch (const std::runtime_error& e) {
        text << e.what() << " at (row " << (row + 1) << ", column " << (column + 1) << ")";
    }

    output->setText(QString::fromStdString(text.str()));
}

} // namespace matrix
