#ifndef QJDLINEEDIT_H
#define QJDLINEEDIT_H

#include <QLineEdit>

class QJDLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QJDLineEdit(QWidget *parent = 0);

    QString id();
    void setBoxID(QString id);
signals:
    void sigLineEditChanged(QString text, QString id);
    void sigLineEditChanged(QString text, QString boxID, QString id);
public slots:
private:
    QString _id;
    QString _boxID;
    QString setRandString(QString & randString);
private slots:
    void emitEditFinished();
};

#endif // QJDLINEEDIT_H
