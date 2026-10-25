#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableView>
#include <QComboBox>

#include "IndexSystem.hpp"
#include "StudentTableModel.hpp"
#include "BenchmarkDialog.hpp"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onLoadCSV();
    void onAddStudent();
    void onSearchByYear();
    void onSearchByRange();
    void onShowAll();
    void onRunBenchmark();
    void onClearDatabase();

private:
    void setupUI();
    void clearTable();

    IndexSystem* system_ = nullptr;

    QTableView* tableView_ = nullptr;
    StudentTableModel* model_ = nullptr;

    QLineEdit* editFirstName_ = nullptr;
    QLineEdit* editLastName_ = nullptr;
    QLineEdit* editYear_ = nullptr;
    QLineEdit* editGroup_ = nullptr;

    QLineEdit* editSearchYear_ = nullptr;
    QLineEdit* editMinYear_ = nullptr;
    QLineEdit* editMaxYear_ = nullptr;

    QComboBox* comboStrategy_ = nullptr;
    QComboBox* comboIndexType_ = nullptr;

    QLabel* statusLabel_ = nullptr;
};