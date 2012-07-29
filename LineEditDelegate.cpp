#include "LineEditDelegate.h"
#include "ProjectModel.h"
#include "Project.h"

#include <QLineEdit>
#include <QPainter>

LineEditDelegate::LineEditDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QWidget *LineEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(parent);
    Q_UNUSED(option);
    Q_UNUSED(index);

    QLineEdit *editor = new QLineEdit(parent);

    return editor;
}

void LineEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString currentModelText = index.model()->data(index).toString();

    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

    lineEdit->setText(currentModelText);
}

void LineEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *lineEdit = static_cast<QLineEdit*>(editor);

    QString currentTextLineEdit = lineEdit->text();

    model->setData(index, currentTextLineEdit);
}

void LineEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(index);

    editor->setGeometry(option.rect);
}

void LineEditDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    int col = index.column();

    const ProjectModel *model = static_cast<const ProjectModel*>(index.model());

    Project *currentProject = model->GetProject(index);

    bool projectIsConsistent = currentProject->CheckConsistency();

    QStyleOptionViewItemV4 newOption(option);

    if(!projectIsConsistent && (col == 1 || col == 2))
    {
        newOption.palette.setColor(QPalette::Text, QColor(255, 0, 0));
    }

    QStyledItemDelegate::paint(painter, newOption, index);
}
