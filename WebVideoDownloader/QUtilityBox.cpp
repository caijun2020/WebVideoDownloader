/**********************************************************************
PACKAGE:        Utility
FILE:           QUtilityBox.cpp
COPYRIGHT (C):  All rights reserved.

PURPOSE:        General tool box, provide buffer convert function and etc.
**********************************************************************/

#include "QUtilityBox.h"
#include <QStringList>
#include <QRegExp>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

QUtilityBox::QUtilityBox()
{
}

QUtilityBox::~QUtilityBox()
{
}

uint32_t QUtilityBox::convertHexStringToDataBuffer(uint8_t *convertedDataBuffer, const QString &inputStr)
{
    int len = 0;
    bool ok;

    QStringList dataList;
    dataList = inputStr.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    len = dataList.size();
    for(int i = 0; i < dataList.size(); i++)
    {
        //qDebug() << "dataList.at(" << i << ") = " << dataList.at(i);
        *(convertedDataBuffer + i) = dataList.at(i).toInt(&ok, 16);
    }

    return len;
}

uint32_t QUtilityBox::convertHexStringToDataBuffer(uint16_t *convertedDataBuffer, const QString &inputStr)
{
    int len = 0;
    bool ok;

    QStringList dataList;
    dataList = inputStr.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    len = dataList.size();
    for(int i = 0; i < dataList.size(); i++)
    {
        //qDebug() << "dataList.at(" << i << ") = " << dataList.at(i);
        *(convertedDataBuffer + i) = dataList.at(i).toInt(&ok, 16);
    }

    return len;
}

uint32_t QUtilityBox::convertDecStringToDataBuffer(uint8_t *convertedDataBuffer, const QString &inputStr)
{
    int len = 0;
    bool ok;

    QStringList dataList;
    dataList = inputStr.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    len = dataList.size();
    for(int i = 0; i < dataList.size(); i++)
    {
        //qDebug() << "dataList.at(" << i << ") = " << dataList.at(i);
        *(convertedDataBuffer + i) = dataList.at(i).toInt(&ok, 10);
    }

    return len;
}

uint32_t QUtilityBox::convertDecStringToDataBuffer(uint16_t *convertedDataBuffer, const QString &inputStr)
{
    int len = 0;
    bool ok;

    QStringList dataList;
    dataList = inputStr.split(QRegExp("\\s+"), QString::SkipEmptyParts);

    len = dataList.size();
    for(int i = 0; i < dataList.size(); i++)
    {
        //qDebug() << "dataList.at(" << i << ") = " << dataList.at(i);
        *(convertedDataBuffer + i) = dataList.at(i).toInt(&ok, 10);
    }

    return len;
}

QString QUtilityBox::convertDataToHexString(const QByteArray &data)
{
    QString retStr = "";

    for(int i = 0; i < data.size(); i++)
    {
    #if 1
        retStr.append(QString::number((uint8_t)data.at(i), 16).rightJustified(2, '0').toUpper());
        retStr.append(" ");
    #else
        char tmpBuf[2048] = {0};
        sprintf(tmpBuf, "%02X ", (uint8_t)data.at(i));
        retStr.append(tmpBuf);
    #endif
    }

    return retStr;
}

QString QUtilityBox::convertDataToHexString(const uint8_t *data, int len)
{
    QByteArray tmpData((char *)data, len);

    return convertDataToHexString(tmpData);
}

void QUtilityBox::mkdir(QString path)
{
    QDir dir;

    // If the folder not exist, create it
    if(!dir.exists(path))
    {
        dir.mkpath(path);
    }
}

QFileInfoList QUtilityBox::getFolderInfo(const QString &path)
{
    QStringList nameFilters;
    nameFilters.clear();

    return getFolderInfo(path, nameFilters);
}

QFileInfoList QUtilityBox::getFolderInfo(const QString &path, QStringList filter)
{
    QDir dir(path);

    QFileInfoList fileDirs = dir.entryInfoList(filter, QDir::Dirs|QDir::Files|QDir::Readable, QDir::Name);

    return fileDirs;
}

QFileInfoList QUtilityBox::getFileList(QString &path)
{
    QStringList nameFilters;
    nameFilters.clear();

    return getFileList(path, nameFilters);
}

QFileInfoList QUtilityBox::getFileList(QString &path, QStringList filter)
{
    QDir dir(path);
    QFileInfoList file_list = dir.entryInfoList(filter, QDir::Files | QDir::Hidden | QDir::NoSymLinks, QDir::Name);
    QFileInfoList folder_list = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);

    for(int i = 0; i < folder_list.size(); i++)
    {
         QString name = folder_list.at(i).absoluteFilePath();
         QFileInfoList child_file_list = getFileList(name, filter);
         file_list.append(child_file_list);
    }

    sortFileListByInt(file_list);

    return file_list;
}

QFileInfoList QUtilityBox::sortFileListByInt(QFileInfoList &list)
{
    for(int i = 0; i < list.size(); i++)
    {
        //qDebug() << "list.at("<< i << ") = " << list.at(i).fileName();
    }

    qSort(list.begin(), list.end(), QUtilityBox::fileNameLessThan);

    //qDebug() << "after sort";
    for(int i = 0; i < list.size(); i++)
    {
        //qDebug() << "list.at("<< i << ") = " << list.at(i).fileName();
    }

    return list;
}

QByteArray QUtilityBox::convertFloat32ToInt16(const QByteArray data)
{
    QByteArray out;
    const int FLOAT32_BYTES = 4;
    out.clear();

    if(!data.isEmpty())
    {
        for(int i = 0; i < (data.size() / FLOAT32_BYTES); i++)
        {
            QByteArray tmp = data.mid(i * FLOAT32_BYTES, FLOAT32_BYTES);

            float *pfloat = (float *)tmp.data();
            int16_t value = (int16_t)(*pfloat);

            // Do not work tmp.toFloat();
            //bool ok;
            //int16_t value = (int16_t)(tmp.toFloat(&ok));
            //qDebug("int16 value = %d, float32 value = %f", value, tmp.toFloat());

            //qDebug("int16 value = %d, float32 *pfloat = %f", value, *pfloat);
            out.append((char*)&value, sizeof(int16_t));
        }
    }

    //qDebug("out.size() = %d, data.size() = %d", out.size(), data.size());

    return out;
}


bool QUtilityBox::intLessThan(const QString &s1, const QString &s2)
{
    return s1.toInt() < s2.toInt();
}

bool QUtilityBox::fileNameLessThan(const QFileInfo &s1, const QFileInfo &s2)
{
    QString nameS1 = s1.baseName();
    QString nameS2 = s2.baseName();

    return nameS1.toInt() < nameS2.toInt();
}
