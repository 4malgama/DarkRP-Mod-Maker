#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMainWindow>

namespace Ui {
class Settings;
}

class Settings : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    
signals:
    void signal_close();
    
private slots:
    void on_check_preambule_stateChanged(int arg1);
    
    void on_btn_cancel_clicked();
    
    void on_btn_apply_clicked();
    
    void on_btn_output_clicked();
    
private:
    Ui::Settings *ui;
    virtual void closeEvent(QCloseEvent*) override;
};

#endif // SETTINGS_H
