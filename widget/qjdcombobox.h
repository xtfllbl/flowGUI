#ifndef QJDCOMBOBOX_H
#define QJDCOMBOBOX_H

#include <QComboBox>

class QJDComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit QJDComboBox(QWidget *parent = 0);

    QString id();
signals:
    void sigIndexChanged(QString text, QString id);

public slots:
private:
    QString _id;
    QString setRandString(QString & randString);
private slots:
    void emitIndexChanged(QString);
};

#endif // QJDCOMBOBOX_H
