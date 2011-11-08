#ifndef QJDFILESAVELINE_H
#define QJDFILESAVELINE_H

#include <QObject>
#include <QLineEdit>
#include <QToolButton>

class QJDFileSaveLine : public QWidget
{
    Q_OBJECT
public:
    explicit QJDFileSaveLine(QWidget *parent = 0);
    QString id();

signals:
    void sigLineEditChanged(QString text, QString id);

public slots:
    QString setRandString(QString & randString);

    void fileButtonPressed();
    void setText(QString text);

private slots:
    void emitEditFinished();
private:

    QString _id;
    QLineEdit *lineEdit;
    QToolButton *fileButton;

    QString fileName;
};

#endif // QJDFILESAVELINE_H
