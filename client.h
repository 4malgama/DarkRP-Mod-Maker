#ifndef CLIENT_H
#define CLIENT_H

#include <QMainWindow>
#include "classes.h"


QT_BEGIN_NAMESPACE
namespace Ui { class Client; }
QT_END_NAMESPACE

class Client : public QMainWindow
{
    Q_OBJECT

public:
    Client(QWidget *parent = nullptr);
    ~Client();
    bool push(QString);
    void clear(QString);
    void setPM(QString);
    QString keyValue(QString);
    QString nameValue(QString);

signals:
    void slot_close();

private slots:
    void on_actionExport_current_triggered();
    
    void on_actionViewer_triggered();
    
    void on_actionLogs_triggered();
    
    void on_actionExport_all_triggered();
    
    void on_actionSettings_triggered();
    
    void on_actionNew_triggered();
    
    void on_actionClose_triggered();
    
    void on_actionSave_current_triggered();
    
    void on_actionSave_all_triggered();
    
    void on_actionOpen_triggered();
    
private:
    Ui::Client *ui;
    void closeEvent(QCloseEvent*);
    void init();
    void init_connect();
    void startup(const char*, const char*, QSize);    
    void make();
    void makeAgendas();
    void makeAmmo();
    void makeCategories();
    void makeDemoteGroups();
    void makeDoorGroups();
    void makeEntities();
    void makeFood();
    void makeGroupChats();
    void makeJobs();
    void makeShipments();
    void makeVehicles();
    void _make(MakeMode);
    
private: 
    _darkrp_agendas agendas;
    _darkrp_ammo ammo;        
    _darkrp_categories categories;
    _darkrp_demotegroups demotegroups;
    _darkrp_doorgroups doorgroups;
    _darkrp_entities entities;
    _darkrp_food food;
    _darkrp_groupchats groupchats;
    _darkrp_jobs jobs;
    _darkrp_shipments shipments;
    _darkrp_vehicles vehicles;
};

#endif // CLIENT_H
