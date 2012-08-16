#ifndef PROJECTMODEL_H
#define PROJECTMODEL_H

#include <QAbstractItemModel>
#include <QVector>

class Project;

class ProjectModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit ProjectModel(int *hoursBeforeEndOfBudgetWarning, QObject *parent = 0);
    ~ProjectModel();

    Project* AddProject(const QString &name, Project *parent = 0);
    Project* GetProject(const QModelIndex &index) const;
    Project *Root() const;

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    bool insertRow(int row, const QModelIndex &parent = QModelIndex());
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());

    static QModelIndex TopLevelIndex(const QModelIndex &index);

signals:
    void totalOrPlannedHoursEdited(const QModelIndex &index);

public slots:
    void updateHoursOfAllParents(const QModelIndex &index);

private:
    Project *root;

    int *hoursBeforeEndOfBudgetWarning;

    void AddProject(Project *p);
    void RemoveProject(Project *p);
};

#endif // PROJECTMODEL_H
