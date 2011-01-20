#include "qjdprocesswidget.h"
#include <QDebug>
QJDProcessWidget::QJDProcessWidget(QWidget *parent) :
        QTableView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);

    this->setObjectName(QString::fromUtf8("processWidget"));
    this->setMinimumSize(QSize(0, 0));
    QPalette palette;
    QLinearGradient gradient(1, 0, 0.623398, 0.983);
    gradient.setSpread(QGradient::PadSpread);
    gradient.setCoordinateMode(QGradient::ObjectBoundingMode);
    gradient.setColorAt(0, QColor(80, 170, 255, 255));
    gradient.setColorAt(1, QColor(85, 85, 255, 255));
    QBrush brush(gradient);
    palette.setBrush(QPalette::Active, QPalette::Highlight, brush);
    QBrush brush1(QColor(255, 255, 255, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::HighlightedText, brush1);
    QBrush brush2(QColor(255, 255, 220, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::AlternateBase, brush2);
    QLinearGradient gradient1(1, 0, 0.623398, 0.983);
    gradient1.setSpread(QGradient::PadSpread);
    gradient1.setCoordinateMode(QGradient::ObjectBoundingMode);
    gradient1.setColorAt(0, QColor(80, 170, 255, 255));
    gradient1.setColorAt(1, QColor(85, 85, 255, 255));
    QBrush brush3(gradient1);
    palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush3);
    palette.setBrush(QPalette::Inactive, QPalette::HighlightedText, brush1);
    palette.setBrush(QPalette::Inactive, QPalette::AlternateBase, brush2);
    QLinearGradient gradient2(1, 0, 0.623398, 0.983);
    gradient2.setSpread(QGradient::PadSpread);
    gradient2.setCoordinateMode(QGradient::ObjectBoundingMode);
    gradient2.setColorAt(0, QColor(80, 170, 255, 255));
    gradient2.setColorAt(1, QColor(85, 85, 255, 255));
    QBrush brush4(gradient2);
    palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush4);
    palette.setBrush(QPalette::Disabled, QPalette::HighlightedText, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::AlternateBase, brush2);
    this->setPalette(palette);
    QFont font1;
    font1.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
    font1.setPointSize(10);
    font1.setBold(false);
    font1.setItalic(false);
    font1.setWeight(50);
    this->setFont(font1);
    this->setFocusPolicy(Qt::StrongFocus);
    this->setStyleSheet(QString::fromUtf8("gridline-color: rgb(200, 200, 200);\n"
                                             "selection-color: rgb(255, 255, 255);\n"
                                             "selection-background-color: qlineargradient(spread:pad, x1:1, y1:0, x2:0.623398, y2:0.983, stop:0 rgba(80, 170, 255, 255), stop:1 rgba(85, 85, 255, 255));"));
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
}


//void QJDProcessWidget::resizeEvent(QResizeEvent *event)
//{
//    qDebug()<<this->size();
//}
