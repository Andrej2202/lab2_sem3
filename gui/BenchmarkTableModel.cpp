#include "BenchmarkTableModel.hpp"
#include <QFile>
#include <QTextStream>

BenchmarkTableModel::BenchmarkTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int BenchmarkTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_rows.size();
}

int BenchmarkTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 7;
}

QVariant BenchmarkTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole) {
        return QVariant();
    }

    const BenchmarkRow &r = m_rows[index.row()];

    switch (index.column()) {
    case 0: return r.dataType;
    case 1: return r.dataSize;
    case 2: return QString::number(r.treeBuildTime_ms,         'f', 3);
    case 3: return QString::number(r.sortedBuildTime_ms,       'f', 3);
    case 4: return QString::number(r.treeSearchTime_ms,        'f', 3);
    case 5: return QString::number(r.sortedSearchTimeMid_ms,   'f', 3);
    case 6: return QString::number(r.sortedSearchTimeGolden_ms,'f', 3);
    default:
        return QVariant();
    }
}

QVariant BenchmarkTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return "Тип данных";
        case 1: return "N";
        case 2: return "Построение дерева (мс)";
        case 3: return "Построение массива (мс)";
        case 4: return "Поиск в дереве (мс)";
        case 5: return "Массив: пополам (мс)";
        case 6: return "Массив: золотое сечение (мс)";
        default:
            return QVariant();
        }
    }
    return QVariant();
}

void BenchmarkTableModel::setRows(const QVector<BenchmarkRow> &rows)
{
    beginResetModel();
    m_rows = rows;
    endResetModel();
}

void BenchmarkTableModel::clear()
{
    beginResetModel();
    m_rows.clear();
    endResetModel();
}

bool BenchmarkTableModel::exportToCsv(const QString &filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        return false;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    out << "Тип данных;N;"
        << "Построение дерева (мс);Построение массива (мс);"
        << "Поиск в дереве (мс);Массив: пополам (мс);Массив: золотое сечение (мс)\n";

    for (const auto &r : m_rows) {
        out << r.dataType << ";"
            << r.dataSize << ";"
            << QString::number(r.treeBuildTime_ms,         'f', 3) << ";"
            << QString::number(r.sortedBuildTime_ms,       'f', 3) << ";"
            << QString::number(r.treeSearchTime_ms,        'f', 3) << ";"
            << QString::number(r.sortedSearchTimeMid_ms,   'f', 3) << ";"
            << QString::number(r.sortedSearchTimeGolden_ms,'f', 3) << "\n";
    }

    file.close();
    return true;
}