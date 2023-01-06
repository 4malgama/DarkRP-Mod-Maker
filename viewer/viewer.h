#ifndef VIEWER_H
#define VIEWER_H

#include <QMainWindow>

#include "../classes.h"

namespace Ui {
class Viewer;
}

class Viewer : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Viewer(QWidget *parent = nullptr);
    ~Viewer();
    void addElement(void*);
    void clear();
    
signals:
    void event_close();
    
private:
    Ui::Viewer *ui;
    void closeEvent(QCloseEvent*);
};

#endif // VIEWER_H
