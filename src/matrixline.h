#ifndef MATRIXLINE_H
#define MATRIXLINE_H

#include "calculator/matrix/matrix.h"
#include "calculator/lexer.h"
#include "calculator/util/errors.h"

#include <QWidget>
#include <QLayout>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QSpinBox>

namespace matrix {

class MatrixLine : public QWidget
{
    Q_OBJECT

public:
    explicit MatrixLine(std::size_t rows, std::size_t columns, ContextPtr context, QLayout* layout, QWidget* parent = nullptr);
    ~MatrixLine();

private slots:
    void rename(QString new_name);
    void resize_rows(int rows);
    void resize_columns(int columns);
    void recalculate(int row, int column);

private:
    void resize(size_t rows, size_t columns);

    std::size_t rows, columns;
    Matrix matrix;
    QString name;

    QLayout* layout;
    QLineEdit* name_input;
    QTableWidget* table;
    QSpinBox* rows_spinbox, * columns_spinbox;
    QLabel* output;
};

} // namespace matrix

#endif // MATRIX_MATRIXLINE_H
