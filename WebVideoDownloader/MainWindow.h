#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>
#include <QList>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include "ExeProcess.h"
#include "MfgInfo.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public:
    enum
    {
        MAX_URL_NUM = 50
    };

protected slots:
    void closeEvent(QCloseEvent *e);
    void readProcessOutputResult(QString str);

private slots:
    void on_pushButton_browsePath_clicked();

    void on_pushButton_download_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_clearAll_clicked();

    void showMfgInfoUI();

    void pushButton_clearUrl1_clicked();
    void pushButton_clearUrl2_clicked();
    void pushButton_clearUrl3_clicked();
    void pushButton_clearUrl4_clicked();
    void pushButton_clearUrl5_clicked();
    void pushButton_clearUrl6_clicked();
    void pushButton_clearUrl7_clicked();
    void pushButton_clearUrl8_clicked();
    void pushButton_clearUrl9_clicked();
    void pushButton_clearUrl10_clicked();
    void pushButton_clearUrl11_clicked();
    void pushButton_clearUrl12_clicked();
    void pushButton_clearUrl13_clicked();
    void pushButton_clearUrl14_clicked();
    void pushButton_clearUrl15_clicked();
    void pushButton_clearUrl16_clicked();
    void pushButton_clearUrl17_clicked();
    void pushButton_clearUrl18_clicked();
    void pushButton_clearUrl19_clicked();
    void pushButton_clearUrl20_clicked();
    void pushButton_clearUrl21_clicked();
    void pushButton_clearUrl22_clicked();
    void pushButton_clearUrl23_clicked();
    void pushButton_clearUrl24_clicked();
    void pushButton_clearUrl25_clicked();
    void pushButton_clearUrl26_clicked();
    void pushButton_clearUrl27_clicked();
    void pushButton_clearUrl28_clicked();
    void pushButton_clearUrl29_clicked();
    void pushButton_clearUrl30_clicked();
    void pushButton_clearUrl31_clicked();
    void pushButton_clearUrl32_clicked();
    void pushButton_clearUrl33_clicked();
    void pushButton_clearUrl34_clicked();
    void pushButton_clearUrl35_clicked();
    void pushButton_clearUrl36_clicked();
    void pushButton_clearUrl37_clicked();
    void pushButton_clearUrl38_clicked();
    void pushButton_clearUrl39_clicked();
    void pushButton_clearUrl40_clicked();
    void pushButton_clearUrl41_clicked();
    void pushButton_clearUrl42_clicked();
    void pushButton_clearUrl43_clicked();
    void pushButton_clearUrl44_clicked();
    void pushButton_clearUrl45_clicked();
    void pushButton_clearUrl46_clicked();
    void pushButton_clearUrl47_clicked();
    void pushButton_clearUrl48_clicked();
    void pushButton_clearUrl49_clicked();
    void pushButton_clearUrl50_clicked();

private:
    Ui::MainWindow *ui;

    QSettings *currentSetting;  // Store current setting with ini file
    QString widgetFontType; // Store the font type of widget
    int widgetFontSize;     // Store the font size of widget

    ExeProcess *m_myProcess;

    QString m_downloadExe;   // file name of youtube-dl
    QString m_downloadPath;  // Download path

    QList<QLabel *> m_urlLabelList; // Store QLabel
    QList<QLineEdit *> m_urlList; // Store QLineEdit
    QList<QPushButton *> m_urlClearButtonList; // Store QPushButton
    QGridLayout *m_scrollGridLayout;

    MfgInfo *mfgInfoWidget; // The Widget to display the About Info of the Software

    void initWidgetFont();  // Init the Font type and size of the widget
    void initWidgetStyle(); // Init Icon of the widget

    // Update Log
    void updateLogData(QString logStr, bool timeStampFlag = true, bool endWithEnter = true);

    // Load setting from ini file
    void loadSettingFromIniFile();

    // Update download path to setting ini file
    void updateDownloadPathToIniFile();

    // Init UI elements
    void initUI();

    // Delete UI elements to free memory
    void deInitUI();

    void clearUrl(int num);
};
#endif // MAINWINDOW_H
