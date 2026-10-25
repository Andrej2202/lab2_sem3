#include "StudentTableModel.hpp"

StudentTableModel::StudentTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int StudentTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_students.size();
}

int StudentTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 4;
}

QVariant StudentTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    const StudentRow &student = m_students[index.row()];

    switch (index.column()) {
    case 0:
        return student.firstName;
    case 1:
        return student.lastName;
    case 2:
        return student.birthYear;
    case 3:
        return student.group;
    default:
        return QVariant();
    }
}

QVariant StudentTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return "Имя";
        case 1:
            return "Фамилия";
        case 2:
            return "Год рождения";
        case 3:
            return "Группа";
        default:
            return QVariant();
        }
    }
    return QVariant();
}

void StudentTableModel::setStudents(const QVector<StudentRow> &students)
{
    beginResetModel();
    m_students = students;
    endResetModel();
}

void StudentTableModel::clear()
{
    beginResetModel();
    m_students.clear();
    endResetModel();
}

StudentRow StudentTableModel::getStudent(int row) const
{
    if (row >= 0 && row < m_students.size()) {
        return m_students[row];
    }
    return StudentRow();
}