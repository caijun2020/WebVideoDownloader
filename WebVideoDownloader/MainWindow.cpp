#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "QUtilityBox.h"
#include <QDateTime>
#include <QFileDialog>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    widgetFontType("Arial"),
    widgetFontSize(16),
    m_downloadExe("youtube-dl.exe"),
    m_downloadPath("./Download"),
    m_scrollGridLayout(new QGridLayout),
    mfgInfoWidget(new MfgInfo)
{
    ui->setupUi(this);

    // Init UI elements
    initUI();

    // Default setting file
    currentSetting = new QSettings("config.ini", QSettings::IniFormat);

    // Load Settings from ini file
    loadSettingFromIniFile();

    // Init Widget Font type and size
    initWidgetFont();

    // Init Widget Style
    initWidgetStyle();

    m_myProcess = new ExeProcess();
    connect(m_myProcess, SIGNAL(resultReady(QString)),this, SLOT(readProcessOutputResult(QString)));

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(showMfgInfoUI()));

    // Set Window Title
    this->setWindowTitle(tr("Web Video Downloader"));

    // Set Menu Bar Version Info
    ui->menuVersion->addAction("V1.0 2021-Jan-28");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete currentSetting;
    delete m_myProcess;
    delete m_scrollGridLayout;
    delete mfgInfoWidget;
    deInitUI();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    Q_UNUSED(e);

    mfgInfoWidget->close();

    //exit(0);
}

void MainWindow::showMfgInfoUI()
{
    mfgInfoWidget->show();
    mfgInfoWidget->raise();
}

void MainWindow::initWidgetFont()
{
    ui->pushButton_download->setFont(QFont(widgetFontType, widgetFontSize, QFont::Normal));
    ui->pushButton_clear->setFont(QFont(widgetFontType, widgetFontSize, QFont::Normal));
    ui->pushButton_clearAll->setFont(QFont(widgetFontType, widgetFontSize, QFont::Normal));

    for(int i = 0; i < m_urlLabelList.size(); i++)
    {
        m_urlLabelList.at(i)->setFont(QFont(widgetFontType, widgetFontSize, QFont::Normal));
    }

    for(int i = 0; i < m_urlClearButtonList.size(); i++)
    {
        m_urlClearButtonList.at(i)->setFont(QFont(widgetFontType, widgetFontSize, QFont::Normal));
    }
}

void MainWindow::initWidgetStyle()
{
    ui->label_downloadPath->hide();
    ui->lineEdit_downloadPath->hide();
    ui->pushButton_browsePath->hide();
}

void MainWindow::updateLogData(QString logStr, bool timeStampFlag, bool endWithEnter)
{
    QDateTime time = QDateTime::currentDateTime();
    QString timeStr = time.toString("[yyyy-MM-dd hh:mm:ss:zzz] ");

    if(timeStampFlag)
    {
        // Add time stamp
        logStr.prepend(timeStr);
    }

    //logFile->addLogToFile(logStr);
    if(endWithEnter)
    {
        logStr.append("\r\n");
    }

    ui->textEdit_log->insertPlainText(logStr); //Display the log in the textBrowse
    ui->textEdit_log->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
}

void MainWindow::readProcessOutputResult(QString str)
{
    if (!str.isEmpty())
    {
        // Update Log
        updateLogData(str, true, false);
    }
}


void MainWindow::on_pushButton_browsePath_clicked()
{
    QString defaultLocalDir = ui->lineEdit_downloadPath->text();

    QString directory = QFileDialog::getExistingDirectory( this,
                                                 tr("Select the download Directory"),
                                                 defaultLocalDir,
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    if (!directory.isEmpty())
    {
        ui->lineEdit_downloadPath->setText(directory);
        m_downloadPath = directory;

        // Update to ini setting file
        updateDownloadPathToIniFile();
    }
}

void MainWindow::on_pushButton_download_clicked()
{
    QStringList arguments;
    arguments.clear();

    for(int i = 0; i < m_urlList.size(); i++)
    {
        if(!m_urlList.at(i)->text().isEmpty())
        {
            arguments.append(m_urlList.at(i)->text());
        }
    }

    if(!arguments.isEmpty())
    {
        m_myProcess->run(m_downloadExe, arguments, 10);
    }
}

void MainWindow::on_pushButton_clear_clicked()
{
    ui->textEdit_log->clear();
}

void MainWindow::loadSettingFromIniFile()
{
    QUtilityBox toolBox;

    // Load Font type and size
    currentSetting->beginGroup("SystemSetting");

    if(currentSetting->contains("FontType"))
    {
        // Load FontType
        widgetFontType = currentSetting->value("FontType").toString();
    }
    else
    {
        // Init the default value
        currentSetting->setValue("FontType", widgetFontType);
    }

    if(currentSetting->contains("FontSize"))
    {
        // Load FontSize
        widgetFontSize = currentSetting->value("FontSize").toInt();
    }
    else
    {
        // Init the default value
        currentSetting->setValue("FontSize", widgetFontSize);
    }

    if(currentSetting->contains("DownloadDir"))
    {
        // Load storage path
        m_downloadPath = currentSetting->value("DownloadDir").toString();
    }
    else
    {
        // Init the default value
        currentSetting->setValue("DownloadDir", m_downloadPath);
    }
    ui->lineEdit_downloadPath->setText(m_downloadPath);
    toolBox.mkdir(m_downloadPath);

    currentSetting->endGroup();
}


void MainWindow::updateDownloadPathToIniFile()
{
    currentSetting->beginGroup("SystemSetting");
    if(currentSetting->contains("DownloadDir"))
    {
        if(!m_downloadPath.isEmpty())
        {
            currentSetting->setValue("DownloadDir", m_downloadPath);
        }
    }

    currentSetting->endGroup();
}

void MainWindow::initUI()
{
    m_urlLabelList.clear();
    m_urlList.clear();
    m_urlClearButtonList.clear();

    for(int i = 0; i < MAX_URL_NUM; i++)
   {
       QHBoxLayout *pHLayout = new QHBoxLayout();

       QLabel *pLabel = new QLabel();
       pLabel->setText(QString::fromLocal8Bit("URL%1").arg(i+1));
       m_urlLabelList.append(pLabel);

       QLineEdit *pLineEdit = new QLineEdit();
       m_urlList.append(pLineEdit);

       QPushButton *pBtn = new QPushButton();
       pBtn->setObjectName(QString::fromUtf8("pushButton_clearUrl%1").arg(i+1));
       pBtn->setText(QString::fromLocal8Bit("Clear"));
       m_urlClearButtonList.append(pBtn);

       pHLayout->addWidget(pLabel);
       pHLayout->addWidget(pLineEdit);
       pHLayout->addWidget(pBtn);

       pHLayout->setStretch(0, 1);
       pHLayout->setStretch(1, 8);
       pHLayout->setStretch(2, 1);

       m_scrollGridLayout->addLayout(pHLayout, i, 0);
   }

   ui->scrollArea->widget()->setLayout(m_scrollGridLayout);

    // Connect signals and slots
   if(m_urlClearButtonList.size() <= MAX_URL_NUM)
   {
       connect(m_urlClearButtonList.at(0), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl1_clicked()));
       connect(m_urlClearButtonList.at(1), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl2_clicked()));
       connect(m_urlClearButtonList.at(2), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl3_clicked()));
       connect(m_urlClearButtonList.at(3), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl4_clicked()));
       connect(m_urlClearButtonList.at(4), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl5_clicked()));
       connect(m_urlClearButtonList.at(5), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl6_clicked()));
       connect(m_urlClearButtonList.at(6), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl7_clicked()));
       connect(m_urlClearButtonList.at(7), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl8_clicked()));
       connect(m_urlClearButtonList.at(8), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl9_clicked()));
       connect(m_urlClearButtonList.at(9), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl10_clicked()));

       connect(m_urlClearButtonList.at(10), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl11_clicked()));
       connect(m_urlClearButtonList.at(11), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl12_clicked()));
       connect(m_urlClearButtonList.at(12), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl13_clicked()));
       connect(m_urlClearButtonList.at(13), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl14_clicked()));
       connect(m_urlClearButtonList.at(14), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl15_clicked()));
       connect(m_urlClearButtonList.at(15), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl16_clicked()));
       connect(m_urlClearButtonList.at(16), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl17_clicked()));
       connect(m_urlClearButtonList.at(17), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl18_clicked()));
       connect(m_urlClearButtonList.at(18), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl19_clicked()));
       connect(m_urlClearButtonList.at(19), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl20_clicked()));

       connect(m_urlClearButtonList.at(20), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl21_clicked()));
       connect(m_urlClearButtonList.at(21), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl22_clicked()));
       connect(m_urlClearButtonList.at(22), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl23_clicked()));
       connect(m_urlClearButtonList.at(23), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl24_clicked()));
       connect(m_urlClearButtonList.at(24), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl25_clicked()));
       connect(m_urlClearButtonList.at(25), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl26_clicked()));
       connect(m_urlClearButtonList.at(26), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl27_clicked()));
       connect(m_urlClearButtonList.at(27), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl28_clicked()));
       connect(m_urlClearButtonList.at(28), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl29_clicked()));
       connect(m_urlClearButtonList.at(29), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl30_clicked()));

       connect(m_urlClearButtonList.at(30), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl31_clicked()));
       connect(m_urlClearButtonList.at(31), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl32_clicked()));
       connect(m_urlClearButtonList.at(32), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl33_clicked()));
       connect(m_urlClearButtonList.at(33), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl34_clicked()));
       connect(m_urlClearButtonList.at(34), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl35_clicked()));
       connect(m_urlClearButtonList.at(35), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl36_clicked()));
       connect(m_urlClearButtonList.at(36), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl37_clicked()));
       connect(m_urlClearButtonList.at(37), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl38_clicked()));
       connect(m_urlClearButtonList.at(38), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl39_clicked()));
       connect(m_urlClearButtonList.at(39), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl40_clicked()));

       connect(m_urlClearButtonList.at(40), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl41_clicked()));
       connect(m_urlClearButtonList.at(41), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl42_clicked()));
       connect(m_urlClearButtonList.at(42), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl43_clicked()));
       connect(m_urlClearButtonList.at(43), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl44_clicked()));
       connect(m_urlClearButtonList.at(44), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl45_clicked()));
       connect(m_urlClearButtonList.at(45), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl46_clicked()));
       connect(m_urlClearButtonList.at(46), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl47_clicked()));
       connect(m_urlClearButtonList.at(47), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl48_clicked()));
       connect(m_urlClearButtonList.at(48), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl49_clicked()));
       connect(m_urlClearButtonList.at(49), SIGNAL(clicked()), this, SLOT(pushButton_clearUrl50_clicked()));
   }
}

void MainWindow::deInitUI()
{
    for(int i = 0; i < m_urlLabelList.size(); i++)
    {
        delete m_urlLabelList.at(i);
    }
    m_urlLabelList.clear();

    for(int i = 0; i < m_urlList.size(); i++)
    {
        delete m_urlList.at(i);
    }
    m_urlList.clear();

    for(int i = 0; i < m_urlClearButtonList.size(); i++)
    {
        delete m_urlClearButtonList.at(i);
    }
    m_urlClearButtonList.clear();
}

void MainWindow::on_pushButton_clearAll_clicked()
{
    for(int i = 0; i < m_urlList.size(); i++)
    {
        m_urlList.at(i)->clear();
    }
}

void MainWindow::clearUrl(int num)
{
    qDebug() << "clearUrl num=" << num;
    if(num < 1 || num > MAX_URL_NUM)
    {
        return;
    }

    m_urlList.at(num - 1)->clear();
}

void MainWindow::pushButton_clearUrl1_clicked()
{
    clearUrl(1);
}

void MainWindow::pushButton_clearUrl2_clicked()
{
    clearUrl(2);
}

void MainWindow::pushButton_clearUrl3_clicked()
{
    clearUrl(3);
}

void MainWindow::pushButton_clearUrl4_clicked()
{
    clearUrl(4);
}

void MainWindow::pushButton_clearUrl5_clicked()
{
    clearUrl(5);
}

void MainWindow::pushButton_clearUrl6_clicked()
{
    clearUrl(6);
}

void MainWindow::pushButton_clearUrl7_clicked()
{
    clearUrl(7);
}

void MainWindow::pushButton_clearUrl8_clicked()
{
    clearUrl(8);
}

void MainWindow::pushButton_clearUrl9_clicked()
{
    clearUrl(9);
}

void MainWindow::pushButton_clearUrl10_clicked()
{
    clearUrl(10);
}

void MainWindow::pushButton_clearUrl11_clicked()
{
    clearUrl(11);
}

void MainWindow::pushButton_clearUrl12_clicked()
{
    clearUrl(12);
}

void MainWindow::pushButton_clearUrl13_clicked()
{
    clearUrl(13);
}

void MainWindow::pushButton_clearUrl14_clicked()
{
    clearUrl(14);
}

void MainWindow::pushButton_clearUrl15_clicked()
{
    clearUrl(15);
}

void MainWindow::pushButton_clearUrl16_clicked()
{
    clearUrl(16);
}

void MainWindow::pushButton_clearUrl17_clicked()
{
    clearUrl(17);
}

void MainWindow::pushButton_clearUrl18_clicked()
{
    clearUrl(18);
}

void MainWindow::pushButton_clearUrl19_clicked()
{
    clearUrl(19);
}

void MainWindow::pushButton_clearUrl20_clicked()
{
    clearUrl(20);
}

void MainWindow::pushButton_clearUrl21_clicked()
{
    clearUrl(21);
}

void MainWindow::pushButton_clearUrl22_clicked()
{
    clearUrl(22);
}

void MainWindow::pushButton_clearUrl23_clicked()
{
    clearUrl(23);
}

void MainWindow::pushButton_clearUrl24_clicked()
{
    clearUrl(24);
}

void MainWindow::pushButton_clearUrl25_clicked()
{
    clearUrl(25);
}

void MainWindow::pushButton_clearUrl26_clicked()
{
    clearUrl(26);
}

void MainWindow::pushButton_clearUrl27_clicked()
{
    clearUrl(27);
}

void MainWindow::pushButton_clearUrl28_clicked()
{
    clearUrl(28);
}

void MainWindow::pushButton_clearUrl29_clicked()
{
    clearUrl(29);
}

void MainWindow::pushButton_clearUrl30_clicked()
{
    clearUrl(30);
}

void MainWindow::pushButton_clearUrl31_clicked()
{
    clearUrl(31);
}

void MainWindow::pushButton_clearUrl32_clicked()
{
    clearUrl(32);
}

void MainWindow::pushButton_clearUrl33_clicked()
{
    clearUrl(33);
}

void MainWindow::pushButton_clearUrl34_clicked()
{
    clearUrl(34);
}

void MainWindow::pushButton_clearUrl35_clicked()
{
    clearUrl(35);
}

void MainWindow::pushButton_clearUrl36_clicked()
{
    clearUrl(36);
}

void MainWindow::pushButton_clearUrl37_clicked()
{
    clearUrl(37);
}

void MainWindow::pushButton_clearUrl38_clicked()
{
    clearUrl(38);
}

void MainWindow::pushButton_clearUrl39_clicked()
{
    clearUrl(39);
}

void MainWindow::pushButton_clearUrl40_clicked()
{
    clearUrl(40);
}

void MainWindow::pushButton_clearUrl41_clicked()
{
    clearUrl(41);
}

void MainWindow::pushButton_clearUrl42_clicked()
{
    clearUrl(42);
}

void MainWindow::pushButton_clearUrl43_clicked()
{
    clearUrl(43);
}

void MainWindow::pushButton_clearUrl44_clicked()
{
    clearUrl(44);
}

void MainWindow::pushButton_clearUrl45_clicked()
{
    clearUrl(45);
}

void MainWindow::pushButton_clearUrl46_clicked()
{
    clearUrl(46);
}

void MainWindow::pushButton_clearUrl47_clicked()
{
    clearUrl(47);
}

void MainWindow::pushButton_clearUrl48_clicked()
{
    clearUrl(48);
}

void MainWindow::pushButton_clearUrl49_clicked()
{
    clearUrl(49);
}

void MainWindow::pushButton_clearUrl50_clicked()
{
    clearUrl(50);
}
