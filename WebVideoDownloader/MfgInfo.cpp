#include "mfginfo.h"
#include "ui_mfginfo.h"

#include <QPainter>
#include <QPixmap>
#include <QDebug>

MfgInfo::MfgInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MfgInfo)
{
    ui->setupUi(this);

    // Set Window Title
    this->setWindowTitle( tr("About Info") );

    // Always show on top
    this->setWindowFlags(Qt::WindowStaysOnTopHint);
}

MfgInfo::~MfgInfo()
{
    this->close();
    delete ui;
}

void MfgInfo::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load("./resource/logo.png");

    // Display the LOGO in the middle of widget_logo
    painter.drawPixmap((this->ui->widget_logo->width()-pix.width())/2, (this->ui->widget_logo->height()-pix.height())/2, pix);
}
