#pragma once

#include <QDialog>
#include <QTableView>
#include <QPushButton>
#include <QLabel>

#include "BenchmarkTableModel.hpp"

class BenchmarkDialog : public QDialog {
    Q_OBJECT

public:
    explicit BenchmarkDialog(QWidget *parent = nullptr);
    ~BenchmarkDialog() override;

    void setResults(const QVector<BenchmarkRow> &rows);

private slots:
    void onExportCsv();
    //void onPlot();

private:
    void setupUI();

    QTableView*        tableView_ = nullptr;
    BenchmarkTableModel* model_   = nullptr;

    QPushButton*       btnExportCsv_ = nullptr;
    QPushButton*       btnPlot_      = nullptr;
    QPushButton*       btnClose_     = nullptr;

    QLabel*            summaryLabel_ = nullptr;
};