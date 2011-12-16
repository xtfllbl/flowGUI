#include "qjdargu.h"


/// 保存一些时常需要改动的参数，集合起来方便修改
QJDArgu::QJDArgu(QObject *parent) :
    QObject(parent)
{
    FlowProcess="MainFlow";
//    FlowProcess="geany";
    HistoryLogRefreshInterval=3000;

    flowGuiPath=QApplication::applicationDirPath();
}
