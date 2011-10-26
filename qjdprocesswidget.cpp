#include "qjdprocesswidget.h"
#include <QDebug>
QJDProcessWidget::QJDProcessWidget(QWidget *parent) :
        QTableView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    setFocusPolicy(Qt::NoFocus);
    this->setObjectName(QString::fromUtf8("processWidget"));
    this->setMinimumSize(QSize(0, 0));

    this->setFrameShape(QFrame::WinPanel);
    this->setFrameShadow(QFrame::Raised);
    this->setAutoScroll(false);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setAlternatingRowColors(true);
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setTextElideMode(Qt::ElideRight);
    this->setShowGrid(true);
    this->setGridStyle(Qt::SolidLine);
    this->setSortingEnabled(false);
    this->setWordWrap(true);
    this->setCornerButtonEnabled(true);

    this->resizeColumnsToContents();
}
