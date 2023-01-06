#include "viewer.h"
#include "ui_viewer.h"
#include "help_functions.h"

#include <QDebug>

namespace viewer
{
    Viewer* viewer = nullptr;
    
    QList<_gmod_item> stack;
    
    void open()
    {
        if (stack.isEmpty()) return;
        
        for (auto i = stack.cbegin(); i != stack.cend(); ++i)
        {
            struct
            {
                QString type;
                QString name;
                QString time;
            } Arguments = {
                i->makeModeType,
                i->name,
                i->time
            };
            viewer->addElement(&Arguments);
        }
    }
    
    void slot_close()
    {
        viewer = nullptr;
    }
    
    void show(QWidget* _parent)
    {
        if (viewer == nullptr)
        {
            viewer = new Viewer(_parent);
            QObject::connect(viewer, &Viewer::event_close, slot_close);
            viewer->show();
            open();
        }
        else
        {
            viewer->activateWindow();
        }
    }
    
    void append(void* struct_data)
    {
        struct Data
        {
            QString a;
            QString b;
            QString c;
        };
        Data* data = static_cast<Data*>(struct_data);
        
        _gmod_item item;
        item.makeModeType = data->a;
        item.name = data->b;
        item.time = data->c;
        
        stack.append(item);
        
        if (viewer != nullptr)
        {
            struct
            {
                QString type;
                QString name;
                QString time;
            } arguments = {
                item.makeModeType,
                item.name,
                item.time
            };
            viewer->addElement(&arguments);
        }
    }
    
    bool isNull()
    {
        return viewer == nullptr;
    }
        
    void flush()
    {
        if (viewer != nullptr)
            viewer->clear();    
        stack.clear();
    }
}

Viewer::Viewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Viewer)
{
    ui->setupUi(this);
    this->setWindowTitle("Viewer");
}

Viewer::~Viewer()
{
    delete ui;
}

void Viewer::addElement(void *struct_data)
{
    struct Element
    {
        QString type;
        QString name;
        QString time;
    };
    Element* e = static_cast<Element*>(struct_data);
    
    _gmod_item* item = new _gmod_item;
    item->makeModeType = e->type;
    item->name = e->name;
    item->time = e->time;
    
    item->setText(0, item->makeModeType);
    item->setText(1, item->name);
    item->setText(2, item->time);
    
    ui->tree->addTopLevelItem(item);
}

void Viewer::clear()
{
    ui->tree->clear();
}

void Viewer::closeEvent(QCloseEvent*)
{
    emit event_close();
    delete this;
}
