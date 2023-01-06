#ifndef PLAYERMODELVIEWER_H
#define PLAYERMODELVIEWER_H

#include <QMainWindow>

class PlayerModel;

namespace pmv {
struct __pair;
}

namespace Ui {
class PlayerModelViewer;
}

class PlayerModelViewer : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit PlayerModelViewer(QWidget *parent = nullptr);
    ~PlayerModelViewer();
    void awake();
    void update();
    
signals:
    void event_close();
    
private:
    Ui::PlayerModelViewer *ui;
    virtual void closeEvent(QCloseEvent*) override;
    
    QList<pmv::__pair*> a;
};

#endif // PLAYERMODELVIEWER_H
