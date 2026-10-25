#include "BenchmarkDialog.hpp"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>


BenchmarkDialog::BenchmarkDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Результаты бенчмарка");
    resize(950, 500);

    setupUI();
}


BenchmarkDialog::~BenchmarkDialog()
{
}


void BenchmarkDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    model_ = new BenchmarkTableModel(this);
    tableView_ = new QTableView();
    tableView_->setModel(model_);
    tableView_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView_->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView_->setAlternatingRowColors(true);
    mainLayout->addWidget(tableView_, 1);

    summaryLabel_ = new QLabel("Нет данных");
    mainLayout->addWidget(summaryLabel_);

    QHBoxLayout* buttonsLayout = new QHBoxLayout();

    btnExportCsv_ = new QPushButton("Экспорт в CSV");
    connect(btnExportCsv_, &QPushButton::clicked, this, &BenchmarkDialog::onExportCsv);

    //btnPlot_ = new QPushButton("Построить график");
    //connect(btnPlot_, &QPushButton::clicked, this, &BenchmarkDialog::onPlot);

    btnClose_ = new QPushButton("Закрыть");
    connect(btnClose_, &QPushButton::clicked, this, &QDialog::accept);

    buttonsLayout->addWidget(btnExportCsv_);
    //buttonsLayout->addWidget(btnPlot_);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(btnClose_);

    mainLayout->addLayout(buttonsLayout);
}


void BenchmarkDialog::setResults(const QVector<BenchmarkRow> &rows)
{
    model_->setRows(rows);

    if (rows.isEmpty()) {
        summaryLabel_->setText("Нет данных");
    } else {
        summaryLabel_->setText(
            QString("Строк в таблице: %1 | Размерности N: %2")
                .arg(rows.size())
                .arg(rows.first().dataSize));
    }
}


void BenchmarkDialog::onExportCsv()
{
    if (model_->rowCount() == 0) {
        QMessageBox::information(this, "Экспорт", "Нет данных для экспорта.");
        return;
    }

    QString defaultPath = "results/benchmark_results.csv";
    QString filePath = QFileDialog::getSaveFileName(
        this,
        "Сохранить результаты бенчмарка",
        defaultPath,
        "CSV файлы (*.csv)");

    if (filePath.isEmpty()) return;

    if (model_->exportToCsv(filePath)) {
        QMessageBox::information(this, "Экспорт",
            QString("Результаты успешно сохранены в:\n%1").arg(filePath));
        summaryLabel_->setText(
            QString("Экспортировано в: %1")
                .arg(QFileInfo(filePath).fileName()));
    } else {
        QMessageBox::critical(this, "Ошибка экспорта",
            "Не удалось сохранить файл. Проверьте права доступа.");
    }
}


// void BenchmarkDialog::onPlot()
// {
//     if (model_->rowCount() == 0) {
//         QMessageBox::information(this, "График", "Нет данных для построения графика.");
//         return;
//     }

//     QMessageBox::information(
//         this,
//         "Построение графика",
//         "Модуль Qt Charts не подключён к проекту.\n\n"
//         "Воспользуйтесь кнопкой \"Экспорт в CSV\" и постройте график "
//         "в Excel, LibreOffice Calc или Python (matplotlib).\n\n"
//         "Для подключения графика в самом приложении:\n"
//         "1. Добавьте в .pro файл: QT += charts\n"
//         "2. Пересоберите проект.\n"
//         "3. Реализуйте построение графика в этом слоте.");
// }