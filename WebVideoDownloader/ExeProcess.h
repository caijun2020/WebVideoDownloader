#ifndef EXEPROCESS_H
#define EXEPROCESS_H

#include <QProcess>

class ExeProcess : public QObject
{
    Q_OBJECT
public:
    ExeProcess();
    ~ExeProcess();

    void run(const QString &program, const QStringList &arguments, int blockTimeoutInMS = 10000);

signals:
    void resultReady(QString);

private slots:
    void readProcessOutputResult();

private:
    // Process pointer
    QProcess *myProcess;
};

#endif // EXEPROCESS_H
