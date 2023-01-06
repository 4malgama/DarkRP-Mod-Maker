#include "logs.h"
#include "ui_logs.h"
#include "../classes.h"
#include <QTime>
#include <QFile>
#include <QDir>

#include <QDebug>

namespace settings
{
    extern bool enable_logs;
}

namespace logs
{
    Logs* logs = nullptr;
    QList<Note> buffer;
    QString logsFileName;
    
    void close()
    {
        logs = nullptr;
    }
    
    void show(QWidget* parent)
    {
        if (logs == nullptr)
        {
            logs = new Logs(parent);
            QObject::connect(logs, &Logs::slot_close, close);
            logs->show();
        }
        else
        {
            logs->activateWindow();
        }
    }
    
    void clear()
    {
        logs->clear();
    }
    
    bool isNull()
    {
        return logs == nullptr;
    }
    
    void fileUpdate(const Note* note)
    {
        QString pathToLogs = QDir::currentPath() + "/logs";
        
        if (QDir(pathToLogs).exists() == false)
            QDir().mkdir("logs");
        
        QFile file(pathToLogs + "/" + logsFileName);
        
        if (file.open(QIODevice::WriteOnly | QIODevice::Append))
        {
            QTextStream ts(&file);
            
            ts << "[ " << note->time << " | " << note->type << " | " << note->info << " ]\n";
            
            ts.flush();
            file.close();
        }
        else
        {
            qDebug() << file.errorString();
        }
    }
    
    void sendError(const char* _info)
    {
        if (settings::enable_logs == false)
            return;
        
        Note note;
        note.time = QTime::currentTime().toString("hh:mm:ss");
        note.type = "Error";
        note.info = QString(_info);
        note.color = QColor(247, 27, 97);
        
        buffer.append(note);
        
        if (logs != nullptr)
        {
            logs->push(&note);
        }
        
        fileUpdate(&note);
    }
    
    void sendWarning(const char* _info)
    {
        if (settings::enable_logs == false)
            return;
        
        Note note;
        note.time = QTime::currentTime().toString("hh:mm:ss");
        note.type = "Warning";
        note.info = QString(_info);
        note.color = QColor(247, 200, 27);
        
        buffer.append(note);
        
        if (logs != nullptr)
        {
            logs->push(&note);
        }
        
        fileUpdate(&note);
    }
    
    void sendInfo(const char* _info)
    {
        if (settings::enable_logs == false)
            return;
        
        Note note;
        note.time = QTime::currentTime().toString("hh:mm:ss");
        note.type = "Info";
        note.info = QString(_info);
        note.color = QColor(31, 124, 251);
        
        buffer.append(note);
        
        if (logs != nullptr)
        {
            logs->push(&note);
        }
        
        fileUpdate(&note);
    }
    
    void sendSystem(const char* _info)
    {
        if (settings::enable_logs == false)
            return;
        
        Note note;
        note.time = QTime::currentTime().toString("hh:mm:ss");
        note.type = "System";
        note.info = QString(_info);
        note.color = QColor(69, 172, 44);
        
        buffer.append(note);
        
        if (logs != nullptr)
        {
            logs->push(&note);
        }
        
        fileUpdate(&note);
    }
}

Logs::Logs(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Logs)
{
    ui->setupUi(this);
    
    if (logs::buffer.empty() == false)
    {
        for (Note& note : logs::buffer)
        {
            push(&note);
        }
    }
}

Logs::~Logs()
{
    delete ui;
}

void Logs::push(Note *note)
{
    QTreeWidgetItem* item = new QTreeWidgetItem;
    item->setText(0, note->time);
    item->setText(1, note->type);
    item->setText(2, note->info);
    item->setForeground(0, note->color);
    item->setForeground(1, note->color);
    item->setForeground(2, note->color);
    ui->treeWidget->addTopLevelItem(item);
    checkNote(item);
}

void Logs::clear()
{
    ui->treeWidget->clear();
    logs::buffer.clear();
}

void Logs::closeEvent(QCloseEvent *)
{
    emit slot_close();
    delete this;    
}

void Logs::checkNote(QTreeWidgetItem *e)
{
    if (e->text(1) == "System")
    {
        e->setHidden(!ui->check_system->isChecked());
    }
    else if (e->text(1) == "Warning")
    {
        e->setHidden(!ui->check_warnings->isChecked());
    }
    else if (e->text(1) == "Error")
    {
        e->setHidden(!ui->check_errors->isChecked());
    }
}

void Logs::on_btn_clear_clicked()
{
    clear();
}

//2 on
//0 off

void Logs::on_check_system_stateChanged(int arg1)
{
    uint32 size = ui->treeWidget->topLevelItemCount();
    for (uint32 i{}; i < size; ++i)
    {
        auto* e = ui->treeWidget->topLevelItem(i);
        if (e->text(1) == "System")
        {
            e->setHidden(!arg1);
        }
    }
}


void Logs::on_check_warnings_stateChanged(int arg1)
{
    uint32 size = ui->treeWidget->topLevelItemCount();
    for (uint32 i{}; i < size; ++i)
    {
        auto* e = ui->treeWidget->topLevelItem(i);
        if (e->text(1) == "Warning")
        {
            e->setHidden(!arg1);
        }
    }
}

