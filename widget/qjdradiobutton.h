#ifndef QJDRADIOBUTTON_H
#define QJDRADIOBUTTON_H

#include <QRadioButton>

/// 与其他控件不一样, 麻烦就麻烦在多个来整合一个
class QJDRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    explicit QJDRadioButton(QWidget *parent = 0);

    QString id();
signals:
    void sigRadioChecked(QString check, QString id, QString otherID);

public slots:
    void setOtherID(QString id);
private:
    QString _id;
    QString otherID;
    QString setRandString(QString & randString);
private slots:
    void emitStateChanged(bool);
};

#endif // QJDRADIOBUTTON_H
