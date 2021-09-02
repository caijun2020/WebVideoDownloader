#include "ExeProcess.h"
#include <QDebug>

ExeProcess::ExeProcess()
{
    myProcess = new QProcess(this);
    connect(myProcess, SIGNAL(readyRead()),this, SLOT(readProcessOutputResult()));
}

ExeProcess::~ExeProcess()
{
    delete myProcess;
}

void ExeProcess::run(const QString &program, const QStringList &arguments, int blockTimeoutInMS)
{
    myProcess->start(program, arguments);

    // Wait until exe run complete
    myProcess->waitForFinished(blockTimeoutInMS);
}

void ExeProcess::readProcessOutputResult()
{
    qDebug() << "ExeProcess::readProcessOutput()";

    QByteArray output = myProcess->readAll();
    QString result = QString::fromLocal8Bit(output.data());

    //QString result = QString(output);
    //qDebug() << "ExeProcess:: output = " << output;
    //qDebug() << "ExeProcess:: result = " << result;

    if (!result.isEmpty())
    {
        // Emit signal
        emit resultReady(result);
    }

    qDebug() << "result =" << result;
}
