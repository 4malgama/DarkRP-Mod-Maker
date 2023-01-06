#ifndef CLASSES_H
#define CLASSES_H

#include "types.h"

#include <QPushButton>
#include <QToolButton>
#include <QTextBrowser>
#include <QLineEdit>
#include <QLabel>
#include <QListWidget>
#include <QSpacerItem>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QColorDialog>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QTreeWidgetItem>
#include <QMessageBox>



typedef QPushButton BUTTON;
typedef QToolButton TOOL;
typedef QFrame FRAME;
typedef QLineEdit INPUT;
typedef QLabel TITLE;
typedef QListWidget LIST;
typedef QSpacerItem SPACER;
typedef QComboBox COMBO;
typedef QCheckBox CHECK;
typedef QSpinBox NUMERIC;
typedef QColorDialog COLOR;
typedef QPlainTextEdit TEXT;
typedef QHBoxLayout HBOX;

typedef FRAME LINE;
typedef FRAME SEPARATOR;

typedef BUTTON* PBUTTON;
typedef TOOL* PTOOL;
typedef FRAME* PFRAME;
typedef INPUT* PINPUT;
typedef TITLE* PTITLE;
typedef LIST* PLIST;
typedef SPACER* PSPACER;
typedef COMBO* PCOMBO;
typedef CHECK* PCHECK;
typedef NUMERIC* PNUMERIC;
typedef COLOR* PCOLOR;
typedef TEXT* PTEXT;
typedef HBOX* PHBOX;

typedef LIST _lua_table;


struct _gmod_color
{
    uint8 r;
    uint8 g;
    uint8 b;
    uint8 a = 255;
};

struct _struct_end : public QObject
{
    PTOOL push;
    PTOOL clear;
    
    PSPACER spacer;
    
    PHBOX end;
    
    _struct_end(QObject* parent = nullptr)
        : QObject(parent)
    {
        
    }
};

struct _darkrp
{
    QString _codeName;
    _struct_end* struct_end;
    QVector<PINPUT> _requiredInputs;
};

struct _darkrp_agendas : public _darkrp
{
    PTITLE title_text;
    PTITLE manager_text;
    PTITLE listeners_text;
    
    PINPUT title_input;
    PINPUT manager_input;
    PINPUT listeners_input;
    
    PLIST listeners_list;
};

struct _darkrp_ammo : public _darkrp
{
    PTITLE ammoType_text;
    PTITLE name_text;
    PTITLE model_text;
    PTITLE price_text;
    PTITLE amountGiven_text;
    
    PINPUT ammoType_input;
    PINPUT name_input;
    PINPUT model_input;
    
    PNUMERIC price_input;
    PNUMERIC amountGiven_input;
    
    PBUTTON customCheck_button;
    PSPACER spacer_1; 
};

struct _darkrp_categories : public _darkrp
{
    PTITLE name_text;
    PTITLE categories_text;
    PTITLE startExpanded_text;
    PTITLE color_text;
    PTITLE sortOrder_text;  
    
    PINPUT name_input;
    
    PNUMERIC sortOrder_numeric;  
    
    PCHECK startExpanded_check;
    
    PBUTTON canSee_button;
    PBUTTON color_button;
    
    PCOMBO categories_combo;
    
    PSPACER spacer_1;
};

struct _darkrp_demotegroups : public _darkrp
{
    PTITLE name_text;
    PTITLE members_text;
    
    PINPUT name_input;
    PINPUT members_input;
    
    PLIST members_list;
};

struct _darkrp_doorgroups : public _darkrp
{
    PTITLE name_text;
    PTITLE members_text;
    
    PINPUT name_input;
    PINPUT members_input;
    
    PLIST members_list;
};

struct _darkrp_entities : public _darkrp
{
    PTITLE name_text;
    PTITLE entity_text;
    PTITLE model_text;
    PTITLE price_text;
    PTITLE cmd_text;
    PTITLE max_text;
    PTITLE allowed_text;
    PTITLE allowTools_text;
    
    PINPUT name_input;
    PINPUT entity_input;
    PINPUT model_input;
    PINPUT cmd_input;
    PINPUT allowed_input;
    
    PNUMERIC price_numeric;
    PNUMERIC max_numeric;
    
    PCHECK allowTools_check;
    
    PLIST allowed_list;
};

struct _darkrp_food : public _darkrp
{
    PTITLE name_text;
    PTITLE model_text;
    PTITLE energy_text;
    PTITLE price_text;
    PTITLE requiresCook_text;
    
    PINPUT name_input;
    PINPUT model_input;
    
    PNUMERIC energy_numecric;
    PNUMERIC price_numeric;
    
    PCHECK requiresCook_check;
    
    PBUTTON customCheck_button;
    
    PSPACER spacer_1;
};

struct _darkrp_groupchats : public _darkrp
{
    PTITLE members_text;
    
    PINPUT members_input;
    
    PTOOL function_tool;
    
    PLIST members_list;
};

struct _darkrp_jobs : public _darkrp
{
    PTITLE codeName_text;
    PTITLE name_text;
    PTITLE color_text;
    PTITLE models_text;
    PTITLE description_text;
    PTITLE weapons_text;
    PTITLE command_text;
    PTITLE max_text;
    PTITLE salary_text;
    PTITLE admin_text;
    PTITLE vote_text;
    PTITLE hasLicense_text;
    PTITLE canDemote_text;
    PTITLE category_text;
    
    PINPUT codeName_input;
    PINPUT name_input;
    PINPUT models_input;
    PINPUT weapons_input;
    PINPUT command_input;
    PINPUT category_input;
    
    PTEXT description;
    
    PCOMBO admin_combo;
    
    PNUMERIC max_numeric;
    PNUMERIC salary_numeric;
    
    PCHECK vote_check;
    PCHECK hasLicense_check;
    PCHECK canDemote_check;
    
    PLIST weapons_list;
    PLIST models_list;
    
    PBUTTON color_button;
    PBUTTON customCheck_button;
};

struct _darkrp_shipments : public _darkrp
{
    PTITLE name_text;
    PTITLE model_text;
    PTITLE entity_text;
    PTITLE price_text;
    PTITLE amount_text;
    PTITLE separate_text;
    PTITLE priceSep_text;
    PTITLE noShip_text;
    PTITLE allowed_text;
    PTITLE category_text;
    
    PINPUT name_input;
    PINPUT model_input;
    PINPUT entity_input;
    PINPUT allowed_input;
    PINPUT category_input;
    
    PCHECK separate_check;
    PCHECK noShip_check;
    
    PNUMERIC price_numeric;
    PNUMERIC amount_numeric;
    PNUMERIC priceSep_numeric;
    
    PLIST members_list;
};

struct _darkrp_vehicles : public _darkrp
{
    PTITLE name_text;
    PTITLE model_text;
    PTITLE price_text;
    PTITLE allowed_text; //opt
    PTITLE label_text; //opt
    PTITLE distance_text; //opt
    PTITLE angle_text; //opt
    PTITLE onBought_text; //opt
    
    PINPUT name_input;
    PINPUT model_input;
    PINPUT allowed_input;
    PINPUT label_input;
    PINPUT angle_input;
    
    PNUMERIC price_numeric;
    PNUMERIC distance_numeric;
    
    PBUTTON onBought_button;
    PBUTTON customCheck_button; //opt
    
    PLIST members_list;
};

enum MakeMode
{
    MAKE_ALL,
    MAKE_AGENDAS,
    MAKE_AMMO,
    MAKE_CATEGORIES,
    MAKE_DEMOTEGROUPS,
    MAKE_DOORGROUPS,
    MAKE_ENTITIES,
    MAKE_FOOD,
    MAKE_GROUPCHATS,
    MAKE_JOBS,
    MAKE_SHIPMENTS,
    MAKE_VEHICLES,
    MAKE_UNKNOWN
};

enum class AdminValue
{
    ALL = 0,
    ADMIN_OR_HEIGHER = 1,
    SUPERADMIN_OR_RCON = 2,
    RCON_ONLY = 3
};

struct _gmod_item : public QTreeWidgetItem
{
    QString makeModeType;
    QString name;
    QString time;
};

struct Note
{
    QString time;
    QString type;
    QString info;
    QColor color;
};

struct __Point2
{
    uint32 x;
    uint32 y;
};

struct PlayerModel : public QToolButton
{
    QString model;
    
    PlayerModel(QWidget* parent = nullptr)
        : QToolButton(parent)
    {
        
    }
};

#endif // CLASSES_H
