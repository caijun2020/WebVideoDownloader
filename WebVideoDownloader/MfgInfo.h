#ifndef MFGINFO_H
#define MFGINFO_H

#include <QWidget>

namespace Ui {
class MfgInfo;
}

class MfgInfo : public QWidget
{
    Q_OBJECT
    
public:
    explicit MfgInfo(QWidget *parent = 0);
    ~MfgInfo();
    
private:
    Ui::MfgInfo *ui;

protected:
    void paintEvent(QPaintEvent * e);

};

#endif // MFGINFO_H
