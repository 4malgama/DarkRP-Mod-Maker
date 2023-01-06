#include "playermodelviewer.h"
#include "client.h"
#include "ui_playermodelviewer.h"
#include "../classes.h"

#include <QDebug>

namespace client
{
    extern Client* client;
}

namespace pmv
{
    struct __pair
    {
        PlayerModel* pm;
        __Point2 pos;
    };

    PlayerModelViewer* pmv = nullptr;
    QList<__pair> models;
    
    void slot_close()
    {
        pmv = nullptr;
    }
    
    void show()
    {
        if (pmv == nullptr)
        {
            pmv = new PlayerModelViewer;
            QObject::connect(pmv, &PlayerModelViewer::event_close, slot_close);
        }
        else
        {
            pmv->activateWindow();
        }
    }
    
    void append(const char* _mdl, const QIcon& _icon)
    {
        PlayerModel *model = new PlayerModel;
        model->model = QString(_mdl);
        model->setIcon(_icon);
        model->setIconSize(QSize(200, 256));
        model->setAutoRaise(true);
        model->setSizePolicy(QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Expanding);
        model->setToolTip(_mdl);
        
        __Point2 pos;
        
        if (models.isEmpty()) pos.x = pos.y = 0;
        else
        {
            __Point2 last = models.last().pos;
            
            if (last.x != 3)
            {
                pos.y = last.y;
                pos.x = last.x + 1;
            }
            else
            {
                pos.y = last.y + 1;
                pos.x = 0;
            }
        }
        
        models.append({ model, pos });
    }
    
    void end()
    {
        for (auto i : models)
        {
            delete i.pm;
        }
    }
    
    void close()
    {
        if (pmv != nullptr)
        {
            pmv->close();
        }
    }
}

PlayerModelViewer::PlayerModelViewer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PlayerModelViewer)
{
    ui->setupUi(this);
    show();
    awake();
    update();
}

PlayerModelViewer::~PlayerModelViewer()
{
    for (auto i = a.cbegin(); i != a.cend(); ++i)
    {
        delete (*i);
    }
    delete ui;
}

void PlayerModelViewer::awake()
{
    if (pmv::models.isEmpty())
        return;
    
    //fill array of new PM
    for (auto i = pmv::models.begin(); i != pmv::models.end(); ++i)
    {
        pmv::__pair* pair = new pmv::__pair;
        
        pair->pm = new PlayerModel;
        
        pair->pm->model = i->pm->model;
        pair->pm->setIcon(i->pm->icon());
        pair->pm->setIconSize(i->pm->iconSize());
        pair->pm->setSizePolicy(i->pm->sizePolicy());
        pair->pm->setToolTip(i->pm->toolTip());
        pair->pm->setCursor(Qt::PointingHandCursor);
        pair->pm->setAutoRaise(i->pm->autoRaise());
        
        pair->pos.x = i->pos.x;
        pair->pos.y = i->pos.y;
        
        this->a.append(pair);
    }
    
    if (this->a.isEmpty()) return;
    
    //connecting
    for (int i{}; i < a.count(); i++)
    {
        QObject::connect(a[i]->pm, &TOOL::clicked, this, [i, this] { client::client->setPM(a[i]->pm->model); });
    }
}

void PlayerModelViewer::update()
{
    if (a.isEmpty()) return;
    
    //creating in UI
    for (auto i = a.cbegin(); i != a.cend(); ++i)
    {
        ui->gridLayout_2->addWidget((*i)->pm, (*i)->pos.y, (*i)->pos.x);
    }
}

void PlayerModelViewer::closeEvent(QCloseEvent *)
{
    emit event_close();
    delete this;
}
