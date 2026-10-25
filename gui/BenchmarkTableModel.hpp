#pragma once
#include <QAbstractTableModel>
#include <QVector>
#include <QString>

struct BenchmarkRow {
    QString dataType;
    int     dataSize = 0;
    
    double  treeBuildTime_ms   = 0.0;
    double  sortedBuildTime_ms = 0.0;
    
    double  treeSearchTime_ms         = 0.0;
    double  sortedSearchTimeMid_ms    = 0.0;
    double  sortedSearchTimeGolden_ms = 0.0;
};

class BenchmarkTableModel : public QAbstractTableModel {
    Q_OBJECT
public:
    explicit BenchmarkTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setRows(const QVector<BenchmarkRow> &rows);
    void clear();

    bool exportToCsv(const QString &filePath) const;
    const QVector<BenchmarkRow>& rows() const { return m_rows; }

private:
    QVector<BenchmarkRow> m_rows;
};