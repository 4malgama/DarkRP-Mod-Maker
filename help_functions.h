#ifndef HELP_FUNCTIONS_H
#define HELP_FUNCTIONS_H

#include "classes.h"

static QString convert_make_to_string(MakeMode make)
{
    switch(make)
    {
    case MAKE_AGENDAS:
        return "Agendas";
    case MAKE_AMMO:
        return "Ammo";
    case MAKE_CATEGORIES:
        return "Agendas";
    case MAKE_DEMOTEGROUPS:
        return "Ammo";
    case MAKE_DOORGROUPS:
        return "Agendas";
    case MAKE_FOOD:
        return "Ammo";
    case MAKE_GROUPCHATS:
        return "Agendas";
    case MAKE_ENTITIES:
        return "Entities";
    case MAKE_JOBS:
        return "Ammo";
    case MAKE_SHIPMENTS:
        return "Agendas";
    case MAKE_VEHICLES:
        return "Ammo";
    case MAKE_UNKNOWN:
    default:
        return "Unknown";
    }
}

static MakeMode convert_string_to_make(QString str)
{
    if (str == "Agendas")
        return MAKE_AGENDAS;
    else if (str == "Ammo")
        return MAKE_AMMO;
    else if (str == "Categories")
        return MAKE_CATEGORIES;
    else if (str == "Demote groups")
        return MAKE_DEMOTEGROUPS;
    else if (str == "Door groups")
        return MAKE_DOORGROUPS;
    else if (str == "Food")
        return MAKE_FOOD;
    else if (str == "Entities")
        return MAKE_ENTITIES;
    else if (str == "Group chats")
        return MAKE_GROUPCHATS;
    else if (str == "Jobs")
        return MAKE_JOBS;
    else if (str == "Shipments")
        return MAKE_SHIPMENTS;
    else if (str == "Vehicles")
        return MAKE_VEHICLES;
    else
        return MAKE_UNKNOWN;
}

static int convert_admin_to_int(QString src)
{
    if (src == "Admin or heigher")
        return 1;
    else if (src == "Superadmin or rcon")
        return 2;
    else if (src == "Rcon only")
        return 3;
    else
        return 0;
}

static void alert(char _type, const char* _header, QString _msg)
{
    switch(_type)
    {
    case 'e':
    {
        QMessageBox(QMessageBox::Critical, _header, _msg, QMessageBox::StandardButton::Close).exec();                
        break;
    }
    case 'w':
    {
        QMessageBox(QMessageBox::Warning, _header, _msg, QMessageBox::StandardButton::Ignore).exec();                
        break;
    }
    case 'i':
    {
        QMessageBox(QMessageBox::Information, _header, _msg, QMessageBox::StandardButton::Ok).exec();                
        break;
    }
    case 'q':
    case '?':
    {
        QMessageBox(QMessageBox::Question, _header, _msg, QMessageBox::StandardButton::Ok).exec();                
        break;
    }
    default:
        break;
    }
}

#endif // HELP_FUNCTIONS_H
