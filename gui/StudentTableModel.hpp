#pragma once

#include <QAbstractTableModel>
#include <QVector>
#include <QString>

struct StudentRow {
    QString firstName;
    QString lastName;
    int birthYear;
    QString group;
};

class StudentTableModel : public QAbstractTableModel {
    Q_OBJECT

public:
    explicit StudentTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setStudents(const QVector<StudentRow> &students);
    void clear();

    StudentRow getStudent(int row) const;

private:
    QVector<StudentRow> m_students;
};