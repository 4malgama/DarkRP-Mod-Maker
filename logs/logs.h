#ifndef LOGS_H
#define LOGS_H

#include <QMainWindow>

class Note;
class QTreeWidgetItem;

namespace Ui {
class Logs;
}

class Logs : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Logs(QWidget *parent = nullptr);
    ~Logs();
    void push(Note*);
    void clear();
    
signals:
    void slot_close();
    
private slots:
    void on_btn_clear_clicked();
    
    void on_check_system_stateChanged(int arg1);
    
    void on_check_warnings_stateChanged(int arg1);
    
private:
    Ui::Logs *ui;
    virtual void closeEvent(QCloseEvent*) override;
    void checkNote(QTreeWidgetItem*);
};

#endif // LOGS_H
