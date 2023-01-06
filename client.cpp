#include "client.h"
#include "ui_client.h"
#include "types.h"
#include "help_functions.h"
#include "adminmodel.h"
#include <QFile>
#include <QFileDialog>
#include <QDir>
#include <QDateTime>
#include <QCompleter>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include <QDebug>

#define __NUMBER(x) QString::number(x)
#define JSON_NULL QJsonValue::Null

namespace client
{
    Client* client = nullptr;
    Ui::Client* ui = nullptr;
    
    QCompleter *completer = nullptr;
    
    void close()
    {
        client = nullptr;
        ui = nullptr;
    }
    
    void init()
    {
        //Init completer
        if (completer == nullptr)
        {
            QStringList wordlist;
            wordlist << "TEAM" << "job" << "rp" << "dark"
                     << "garrys" << "Garry\'s" << "mod"
                     << "role" << "play" << "police" << "mayor"
                     << "hobo" << "citizen" << "TEAM_POLICE"
                     << "TEAM_MAYOR" << "TEAM_CITIZEN" << "TEAM_HOBO"
                     << "TEAM_MOB" << "TEAM_GANG" << "TEAM_GUN"
                     << "TEAM_HITMAN" << "TEAM_MOBBOSS" << "TEAM_CHIEF"
                     << "TEAM_NONRP" << "TEAM_ADMIN" << "TEAM_MOD"
                     << "TEAM_MODERATOR" << "moderator" << "admin"
                     << "administrator" << "nonrp" << "printer"
                     << "money" << "salary" << "TEAM_SWAT" << "TEAM_METH"
                     << "TEAM_COOK" << "cook" << "food" << "car"
                     << "Facepuch" << "Amalgama" << "Discord" << "Steam"
                     << "Skype" << "Vk" << "Anime" << "School"
                     << "TEAM_TEACHER" << "TEAM_UMBRELLA" << "umbrella"
                     << "TEAM_" << "TEAM_MEDIC" << "TEAM_MAFIA" << "TEAM_HOBOBOSS"
                     << "TEAM_BOSS" << "TEAM_VIP" << "vip" << "premium"
                     << "donate" << "donator" << "/ooc" << "/advert"
                     << "/shop" << "TEAM_SHOP" << "TEAM_TRASHMAN"
                     << "TEAM_FBI" << "TEAM_AGENT" << "TEAM_HIT";
            
            completer = new QCompleter(wordlist, client);
            completer->setCaseSensitivity(Qt::CaseInsensitive);           
        }
    }

    void show()
    {
        if (client == nullptr)
        {
            client = new Client;
            QObject::connect(client, &Client::slot_close, close);
            client->show();
        }
        else
        {
            client->activateWindow();
        }
    }

    [[nodiscard]] bool isNull()
    {
        return client == nullptr;
    }

    namespace control
    {
        PBUTTON newButton(const QString &_name, const QString &_toolTip = QString(), bool _checkable = false, const QString _icon = QString());
        PTOOL newTool(const char* _icon, const QString &_toolTip, bool _checkable = false);
        QFrame* newSeparator(const QString _orientation);
        PINPUT newInput(const QString &_ph, const QString &_txt = QString(), const uint32 &_maxlen = 200, const QString &_mask = QString(), bool _security = false, Qt::Alignment _align = Qt::AlignLeft);
        PTITLE newTitle(const QString &_txt, Qt::AlignmentFlag _align = Qt::AlignRight);
        PLIST newList(const char* _tooltip = nullptr, QListWidget::Movement _move = QListWidget::Static);
        PSPACER newSpacer(const QString &_orientation);
        PCOMBO newCombo(const QStringList &_items, const uint32 &_maxVisible = 0);
        PCOMBO newCombo(QAbstractListModel *model, const uint32 &_maxVisible = 0);
        PCHECK newCheck(const QString &_txt, bool _default, bool _tristate = false);
        PNUMERIC newNumeric(const int32 &_min, const int32 &_max, const uint32 &_step = 1);
        PTEXT newText(const char* _ph = nullptr);
        PHBOX newHBox(QWidget* _parent);
        _struct_end* newStructEnd(QGridLayout* grid);
        template<typename T>void changeIcon(T* _obj, const char* _icon);
        void changeTabIcon(QWidget* _tab, const char* _icon);
    }
}

namespace viewer
{
    void show(QWidget* _parent);
    void append(void* struct_data);
    void flush();
}

namespace logs
{
    void show(QWidget*);
    void sendError(const char*);
    void sendWarning(const char*);
    void sendInfo(const char*);
    void sendSystem(const char*);
    
    extern QString logsFileName;
}

namespace settings
{
    void show(QWidget*);
    
    extern QString outputPath;
    extern QString preambule;
    extern bool enable_preambule;
}

namespace pmv
{
    void show();
    void close();
}

namespace darkrp
{
    QStringList agendas;
    QStringList ammo;
    QStringList categories;
    QStringList demotegroups;
    QStringList doorgroups;
    QStringList entities;
    QStringList food;
    QStringList groupchats;
    QStringList jobs;
    QStringList shipments;
    QStringList vehicles;
}

namespace meta
{
    extern const char* name;
    extern const char* version;
    extern const char* author;
}

static void clear_darkrp()
{
    darkrp::agendas.clear();
    darkrp::ammo.clear();
    darkrp::categories.clear();
    darkrp::demotegroups.clear();
    darkrp::doorgroups.clear();
    darkrp::entities.clear();
    darkrp::food.clear();
    darkrp::groupchats.clear();
    darkrp::jobs.clear();
    darkrp::shipments.clear();
    darkrp::vehicles.clear();
}

static void OnRequiredInputEmpty(PINPUT);
static void OnRequiredInputAppend(PINPUT);

Client::Client(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);
    client::ui = ui;
    init();
    init_connect();
}

void Client::startup(const char* _title, const char* _v, QSize _szW)
{
    QString title = _title;
    title.append(" ");
    
    this->setWindowTitle(title + _v);
    this->resize(_szW);
    this->move(0, 0);
    this->setWindowState(Qt::WindowState::WindowMaximized);
    logs::sendSystem("Start up succesful.");
}

bool Client::push(QString _tabName)
{    
    if (_tabName == "Agendas")
    {        
        QString members;
        uint32 size = agendas.listeners_list->count();
        
        if (!size) return false;
            
        for (uint32 i{}; i < size; ++i)
        {
            if (i == (size - 1))
                members.append(agendas.listeners_list->item(i)->text());
            else
                members.append(agendas.listeners_list->item(i)->text() + ", ");
        }
                
        QString stringData = "AddAgenda(\"" + agendas.title_input->text() + "\", " + agendas.manager_input->text() + ", {" + members + "})\n";
        
        darkrp::agendas << stringData;
        logs::sendInfo("Pushed agenda.");
        return true;
    }
    else if (_tabName == "Ammo")
    {
        QString stringData =
                "DarkRP.createAmmoType(\"" + ammo.ammoType_input->text() + "\", {\n"
                "\tname = \"" + ammo.name_input->text() + "\",\n"
                "\tmodel = \"" + ammo.model_input->text() + "\",\n"
                "\tprice = " + QString::number(ammo.price_input->value()) + ",\n"
                "\tamountGiven = " + QString::number(ammo.amountGiven_input->value()) + "\n"
                "})\n";
        
        darkrp::ammo << stringData;
        logs::sendInfo("Pushed ammo.");
        return true;
    }
    else if (_tabName == "Categories")
    {
        if (categories.color_button->text().at(0).isDigit() == false)
            return false;
        
        QString category = categories.categories_combo->currentText();
        QString color = "Color(" + categories.color_button->text() + ")";
        QString name = categories.name_input->text();
        QString sort = QString::number(categories.sortOrder_numeric->value());
        QString expanded = (categories.startExpanded_check->isChecked() ? "true" : "false");
        
        if (category.isEmpty()) return false;
        if (name.isEmpty()) return false;
        
        QString stringData = "DarkRP.createCategory{\n"
                             "\tname = \"" + name + "\",\n"
                             "\tcategorises = \"" + category + "\",\n"
                             "\tstartExpanded = " + expanded + ",\n"
                             "\tcolor = " + color + ",\n"
                             "\tsortOrder = " + sort + ",\n}\n";
        
        darkrp::categories << stringData;
        logs::sendInfo("Pushed category.");
        return true;
    }
    else if (_tabName == "Demote groups")
    {
        QString name = demotegroups.name_input->text();
        QString members;
        
        uint32 size = demotegroups.members_list->count();
        
        if (!size) return false;        
        if (name.isEmpty()) return false;
        
        for (uint32 i{}; i < size; ++i)
        {
            if (i == (size - 1))
                members.append(demotegroups.members_list->item(i)->text());
            else
                members.append(demotegroups.members_list->item(i)->text() + ", ");
        }
        
        QString stringData = "DarkRP.createDemoteGroup(\"" + name + "\", {" + members + "})\n";
        
        darkrp::demotegroups << stringData;
        logs::sendInfo("Pushed demote group.");
        return true;
    }
    else if (_tabName == "Door groups")
    {
        QString name = doorgroups.name_input->text();
        QString members;
        
        uint32 size = doorgroups.members_list->count();
        
        if (!size) return false;
        if (name.isEmpty()) return false;
        
        for (uint32 i{}; i < size; ++i)
        {
            if (i == (size - 1))
                members.append(doorgroups.members_list->item(i)->text());
            else
                members.append(doorgroups.members_list->item(i)->text() + ", ");
        }
        
        QString stringData = "AddDoorGroup(\"" + name + "\", " + members + ")\n";
        
        darkrp::doorgroups << stringData;
        logs::sendInfo("Pushed door group.");
        return true;
    }
    else if (_tabName == "Entities")
    {
        QString allowTools = entities.allowTools_check->isChecked() ? "true" : "false";
        QString cmd = entities.cmd_input->text();
        QString entity = entities.entity_input->text();
        QString max = QString::number(entities.max_numeric->value());
        QString model = entities.model_input->text();
        QString name = entities.name_input->text();
        QString price = QString::number(entities.price_numeric->value());
        
        uint32 size = entities.allowed_list->count();
        
        if (cmd.isEmpty()) return false;
        if (entity.isEmpty()) return false;
        if (model.isEmpty()) return false;
        if (name.isEmpty()) return false;
        
        QString stringData;
        
        if (size)
        {
            QString allowed;
            
            for (uint32 i{}; i < size; ++i)
            {
                if (i == (size - 1))
                    allowed.append(entities.allowed_list->item(i)->text());
                else
                    allowed.append(entities.allowed_list->item(i)->text() + ", ");
            }
            
            stringData = "DarkRP.createEntity(\"" + name + "\", {\n"
                         "\tent = \"" + entity + "\",\n"
                         "\tmodel = \"" + model + "\",\n"
                         "\tprice = " + price + ",\n"
                         "\tmax = " + max + ",\n"
                         "\tcmd = \"" + cmd + "\",\n"
                         "\tallowTools = " + allowTools + ",\n"
                         "\tallowed = {" + allowed + "}\n})\n";
        }
        else
        {
            stringData = "DarkRP.createEntity(\"" + name + "\", {\n"
                         "\tent = \"" + entity + "\",\n"
                         "\tmodel = \"" + model + "\",\n"
                         "\tprice = " + price + ",\n"
                         "\tmax = " + max + ",\n"
                         "\tcmd = \"" + cmd + "\",\n"
                         "\tallowTools = " + allowTools + ",\n})\n";
        }
        
        darkrp::entities << stringData;
        logs::sendInfo("Pushed entity.");
        return true;
    }
    else if (_tabName == "Food")
    {
        QString energy = QString::number(food.energy_numecric->value());
        QString model = food.model_input->text();
        QString name = food.name_input->text();
        QString price = QString::number(food.price_numeric->value());
        QString req = food.requiresCook_check->isChecked() ? "true" : "false";
        
        if (model.isEmpty()) return false;
        if (name.isEmpty()) return false;
        
        QString stringData = "DarkRP.createFood(\"" + name + "\", {\n"
                             "\tmodel = \"" + model + "\",\n"
                             "\tenergy = " + energy + ",\n"
                             "\tprice = " + price + ",\n"
                             "\trequiresCook = " + req + "\n})\n";
        
        darkrp::food << stringData;
        logs::sendInfo("Pushed food.");
        return true;
    }
    else if (_tabName == "Group chats")
    {
        QString members;
        
        uint32 size = groupchats.members_list->count();
        
        if (!size) return false;
        
        for (uint32 i{}; i < size; ++i)
        {
            if (i == (size - 1))
                members.append(groupchats.members_list->item(i)->text());
            else
                members.append(groupchats.members_list->item(i)->text() + ", ");
        }
        
        QString stringData = "GAMEMODE:AddGroupChat(" + members + ")\n";
        
        darkrp::groupchats << stringData;
        logs::sendInfo("Pushed group chat.");
        return true;
    }
    else if (_tabName == "Jobs")
    {
        QString admin = QString::number(jobs.admin_combo->currentData(Qt::UserRole).toInt());
        QString demote = jobs.canDemote_check->isChecked() ? "true" : "false";
        QString category = jobs.category_input->text();
        QString team = jobs.codeName_input->text();
        QString color = jobs.color_button->text();
        QString cmd = jobs.command_input->text();
        QString license = jobs.hasLicense_check->isChecked() ? "true" : "false";
        QString max = QString::number(jobs.max_numeric->value());
        QString name = jobs.name_input->text();
        QString salary = QString::number(jobs.salary_numeric->value());
        QString vote = jobs.vote_check->isChecked() ? "true" : "false";
        QString description = jobs.description->toPlainText();
        
        QString models;
        QString weapons;
        
        uint32 sizeM = jobs.models_list->count();
        uint32 sizeW = jobs.weapons_list->count();
        
        if (sizeM < 1) return false;
        if (category.isEmpty()) return false;
        if (team.isEmpty()) return false;
        if (color.at(0).isDigit() == false) return false;
        if (cmd.isEmpty()) return false;
        if (name.isEmpty()) return false;
        
        // models
        for (uint32 i{}; i < sizeM; ++i)
        {
            if (i == (sizeM - 1))
                models.append("\t\"" + jobs.models_list->item(i)->text() + "\"");
            else
                models.append("\t\"" + jobs.models_list->item(i)->text() + "\",\n");
        }
        
        // weapons
        for (uint32 i{}; i < sizeW; ++i)
        {
            if (i == (sizeW - 1))
                weapons.append("\"" + jobs.weapons_list->item(i)->text() + "\"");
            else
                weapons.append("\"" + jobs.weapons_list->item(i)->text() + "\", ");
        }
        
        QString stringData = team + " = DarkRP.createJob(\"" + name + "\", {\n"
                                    "\tcolor = Color(" + color + "),\n"
                                    "\tmodel = {\n" + models + "},\n"
                                    "\tdescription = [[" + description + "]],\n"
                                    "\tweapons = {" + weapons + "},\n"
                                    "\tcommand = \"" + cmd + "\",\n"
                                    "\tmax = " + max + ",\n"
                                    "\tsalary = " + salary + ",\n"
                                    "\tadmin = " + admin + ",\n"
                                    "\tvote = " + vote + ",\n"
                                    "\thasLicense = " + license + ",\n"
                                    "\tcandemote = " + demote + ",\n"
                                    "\tcategory = \"" + category + "\"\n})\n\n";
        
        darkrp::jobs << stringData;
        logs::sendInfo("Pushed job.");
        return true;
    }
    else if (_tabName == "Shipments")
    {
        QString amount = QString::number(shipments.amount_numeric->value());
        QString category = shipments.category_input->text();
        QString entity = shipments.entity_input->text();
        QString model = shipments.model_input->text();
        QString name = shipments.name_input->text();
        QString noship = shipments.noShip_check->isChecked() ? "true" : "false";
        QString price = QString::number(shipments.price_numeric->value());
        QString priceSep = QString::number(shipments.priceSep_numeric->value());
        QString separate = shipments.separate_check->isChecked() ? "true" : "false";
        
        QString members;
        uint32 size = shipments.members_list->count();
        
        if (entity.isEmpty()) return false;
        if (model.isEmpty()) return false;
        if (name.isEmpty()) return false;
        
        for (uint32 i{}; i < size; ++i)
        {
            if (i == (size - 1))
                members.append(shipments.members_list->item(i)->text());
            else
                members.append(shipments.members_list->item(i)->text() + ", ");
        }
        
        QString stringData = "DarkRP.createShipment(\"" + name + "\", {\n"
                             "\tmodel = \"" + model + "\",\n"
                             "\tentity = \"" + entity + "\",\n"
                             "\tprice = " + price + ",\n"
                             "\tamount = " + amount + ",\n"
                             "\tseparate = " + separate + ",\n"
                             "\tpricesep = " + priceSep + ",\n"
                             "\tnoship = " + noship + ",\n"
                             "\tallowed = {" + members + "},\n"
                             "\tcategory = \"" + category + "\",\n"
                             "})\n\n";
        
        darkrp::shipments << stringData;
        logs::sendInfo("Pushed shipment.");
        return true;
    }
    else if (_tabName == "Vehicles")
    {
        QString angle = vehicles.angle_input->text();
        QString distance = __NUMBER(vehicles.distance_numeric->value());
        QString label = vehicles.label_input->text();
        QString model = vehicles.model_input->text();
        QString name = vehicles.name_input->text();
        QString price = __NUMBER(vehicles.price_numeric->value());
        
        QString members;
        uint32 size = vehicles.members_list->count();
        
        if (model.isEmpty()) return false;
        if (name.isEmpty()) return false;
        
        for (uint32 i{}; i < size; ++i)
        {
            if (i == (size - 1))
                members.append(vehicles.members_list->item(i)->text());
            else
                members.append(vehicles.members_list->item(i)->text() + ", ");
        }
        
        QString stringData = "DarkRP.createVehicle({\n"
                             "\tname = \"" + name + "\",\n"
                             "\tmodel = \"" + model +"\",\n"
                             "\tprice = " + price + ",\n"
                             "\tallowed = {" + members + "},\n"
                             "\tlabel = \"" + label + "\",\n"
                             "\tdistance = " + distance + ",\n"
                             "\tangle = (" + angle + "),\n"
                             "})\n\n";
        
        darkrp::vehicles << stringData;
        logs::sendInfo("Pushed vehicle.");
        return true;
    }
    return false;
}

void Client::clear(QString _tabName)
{
    if (_tabName == "Agendas")
    {
        agendas.title_input->clear();
        agendas.manager_input->clear();
        agendas.listeners_input->clear();
        agendas.listeners_list->clear();
    }
    else if (_tabName == "Ammo")
    {
        ammo.ammoType_input->clear();
        ammo.name_input->clear();
        ammo.amountGiven_input->setValue(0);
        ammo.model_input->clear();
        ammo.price_input->setValue(0);
    }
    else if (_tabName == "Categories")
    {
        categories.categories_combo->setCurrentIndex(0);
        categories.color_button->setText(categories.color_button->toolTip());
        categories.name_input->clear();
        categories.sortOrder_numeric->setValue(categories.sortOrder_numeric->minimum());
        categories.startExpanded_check->setChecked(true);
    }
    else if (_tabName == "Demote groups")
    {
        demotegroups.name_input->clear();
        demotegroups.members_input->clear();
        demotegroups.members_list->clear();
    }
    else if (_tabName == "Door groups")
    {
        doorgroups.name_input->clear();
        doorgroups.members_input->clear();
        doorgroups.members_list->clear();
    }
    else if (_tabName == "Food")
    {
        food.name_input->clear();
        food.model_input->clear();
        food.energy_numecric->setValue(food.energy_numecric->minimum());
        food.price_numeric->setValue(food.price_numeric->minimum());
        food.requiresCook_check->setChecked(false);
    }
    else if (_tabName == "Entities")
    {
        entities.name_input->clear();
        entities.allowed_input->clear();
        entities.allowed_list->clear();
        entities.cmd_input->clear();
        entities.entity_input->clear();
        entities.allowTools_check->setChecked(false);
        entities.model_input->clear();
        entities.max_numeric->setValue(entities.max_numeric->minimum());
        entities.price_numeric->setValue(entities.price_numeric->minimum());
    }
    else if (_tabName == "Group chats")
    {
        groupchats.members_input->clear();
        groupchats.members_list->clear();
    }
    else if (_tabName == "Jobs")
    {
        jobs.codeName_input->clear();
        jobs.name_input->clear();
        jobs.category_input->clear();
        jobs.command_input->clear();
        jobs.models_input->clear();
        jobs.models_list->clear();
        jobs.weapons_input->clear();
        jobs.weapons_list->clear();
        jobs.max_numeric->setValue(0);
        jobs.salary_numeric->setValue(jobs.salary_numeric->minimum());
        jobs.vote_check->setChecked(false);
        jobs.canDemote_check->setChecked(true);
        jobs.hasLicense_check->setChecked(false);
        jobs.admin_combo->setCurrentIndex(0);
        jobs.color_button->setText(jobs.color_button->toolTip());
        jobs.description->clear();
    }
    else if (_tabName == "Shipments")
    {
        shipments.name_input->clear();
        shipments.allowed_input->clear();
        shipments.category_input->clear();
        shipments.entity_input->clear();
        shipments.model_input->clear();
        shipments.amount_numeric->setValue(shipments.amount_numeric->minimum());
        shipments.separate_check->setChecked(false);
        shipments.noShip_check->setChecked(false);
        shipments.price_numeric->setValue(shipments.price_numeric->minimum());
        shipments.priceSep_numeric->setValue(shipments.priceSep_numeric->minimum());
        shipments.members_list->clear();
    }
    else if (_tabName == "Vehicles")
    {
        vehicles.name_input->clear();
        vehicles.allowed_input->clear();
        vehicles.angle_input->clear();
        vehicles.label_input->clear();
        vehicles.model_input->clear();
        vehicles.distance_numeric->setValue(vehicles.distance_numeric->minimum());
        vehicles.price_numeric->setValue(vehicles.price_numeric->minimum());
        vehicles.members_list->clear();
    }
    logs::sendInfo(QString("Cleared " + _tabName).toStdString().c_str());
}

void Client::setPM(QString _pm)
{
    jobs.models_input->setText(_pm);
}

QString Client::keyValue(QString _tabName) // unused
{
    if (_tabName == "Agendas")
        return agendas._codeName;
    else if (_tabName == "Ammo")
        return ammo._codeName;
    else if (_tabName == "Categories")
        return categories._codeName;
    else if (_tabName == "Demote groups")
        return demotegroups._codeName;
    else if (_tabName == "Door groups")
        return doorgroups._codeName;
    else if (_tabName == "Food")
        return food._codeName;
    else if (_tabName == "Entities")
        return entities._codeName;
    else if (_tabName == "Group chats")
        return groupchats._codeName;
    else if (_tabName == "Jobs")
        return jobs._codeName;
    else if (_tabName == "Shipments")
        return shipments._codeName;
    else if (_tabName == "Vehicles")
        return vehicles._codeName;
    else
        return "Unknown";
}

QString Client::nameValue(QString _tabName)
{
    if (_tabName == "Agendas")
        return agendas.title_input->text();
    else if (_tabName == "Ammo")
        return ammo.name_input->text();
    else if (_tabName == "Categories")
        return categories.name_input->text();
    else if (_tabName == "Demote groups")
        return demotegroups.name_input->text();
    else if (_tabName == "Door groups")
        return doorgroups.name_input->text();
    else if (_tabName == "Food")
        return food.name_input->text();
    else if (_tabName == "Entities")
        return entities.name_input->text();
    else if (_tabName == "Group chats")
        return groupchats.members_list->item(0)->text() + " ...";
    else if (_tabName == "Jobs")
        return jobs.codeName_input->text();
    else if (_tabName == "Shipments")
        return shipments.name_input->text();
    else if (_tabName == "Vehicles")
        return vehicles.name_input->text();
    else
        return "Unknown";
}

void Client::make()
{
    makeAgendas();
    makeAmmo();
    makeCategories();
    makeDemoteGroups();
    makeDoorGroups();
    makeEntities();
    makeFood();
    makeGroupChats();
    makeJobs();
    makeShipments();
    makeVehicles();
    logs::sendInfo("Make all succesful.");
}

void Client::makeAgendas()
{
    if (darkrp::agendas.empty())
    {
        logs::sendWarning("Agendas not found.");
        return;
    }
    
    QFile file(settings::outputPath + "\\agendas.lua");
    
    if (file.open(QFile::WriteOnly))
    {
        if (settings::enable_preambule)
        {
            file.write(settings::preambule.toUtf8() + "\n");
        }
        
        for (auto& agenda : darkrp::agendas)
        {
            file.write(agenda.toUtf8());
        }
        
        file.close();
        
        //alert('i', "Export succesful!", "File was been created: \"" + file.fileName() + "\".");
        logs::sendInfo("Make agendas succesful.");
    }
    else
    {
        QMessageBox(QMessageBox::Critical, "Error create file", file.errorString(), QMessageBox::StandardButton::Close).exec();
        logs::sendError(file.errorString().toStdString().c_str());
    }
}

void Client::makeAmmo()
{
    if (darkrp::ammo.empty())
    {
        logs::sendWarning("Ammo not found.");
        return;
    }
        
    QFile file(settings::outputPath + "\\ammo.lua");
    
    if(file.open(QFile::WriteOnly))
    {
        if (settings::enable_preambule)
        {
            file.write(settings::preambule.toUtf8() + "\n");
        }
        
        for (auto& ammo : darkrp::ammo)
        {
            file.write(ammo.toUtf8());
        }
        
        file.close();
        
        //alert('i', "Export succesful!", "File was been created: \"" + file.fileName() + "\".");
        logs::sendInfo("Make ammo succesful.");
    }
    else
    {
        QMessageBox(QMessageBox::Critical, "Error create file", file.errorString(), QMessageBox::StandardButton::Close).exec();        
        logs::sendError(file.errorString().toStdString().c_str());
    } 
}

void Client::makeCategories()
{
    if (darkrp::categories.empty())
    {
        logs::sendWarning("Categories not found.");
        return;
    }
        
    QFile file(settings::outputPath + "\\categories.lua");
    
    if(file.open(QFile::WriteOnly))
    {
        if (settings::enable_preambule)
        {
            file.write(settings::preambule.toUtf8() + "\n");
        }
        
        for (auto& category : darkrp::categories)
        {
            file.write(category.toUtf8());
        }
        
        file.close();
        
        //alert('i', "Export succesful!", "File was been created: \"" + file.fileName() + "\".");
        logs::sendInfo("Make category succesful.");
    }
    else
    {
        QMessageBox(QMessageBox::Critical, "Error create file", file.errorString(), QMessageBox::StandardButton::Close).exec();        
        logs::sendError(file.errorString().toStdString().c_str());
    } 
}

void Client::makeDemoteGroups()
{
    if (darkrp::demotegroups.empty())
    {
        logs::sendWarning("Demote groups not found.");
        return;
    }
        
    QFile file(settings::outputPath + "\\demotegroups.lua");
    
    if(file.open(QFile::WriteOnly))
    {
        if (settings::enable_preambule)
        {
            file.write(settings::preambule.toUtf8() + "\n");
        }
        
        for (auto& dg : darkrp::demotegroups)
        {
            file.write(dg.toUtf8());
        }
        
        file.close();
        
        //alert('i', "Export succesful!", "File was been created: \"" + file.fileName() + "\".");
        logs::sendInfo("Make demote group succesful.");
    }
    else
    {
        QMessageBox(QMessageBox::Critical, "Error create file", file.errorString(), QMessageBox::StandardButton::Close).exec();        
        logs::sendError(file.errorString().toStdString().c_str());
    } 
}

void Client::makeDoorGroups()
{
    if (darkrp::doorgroups.empty())
    {
        logs::sendWarning("Door groups not found.");
        return;
    }
        
    QFile file(settings::outputPath + "\\doorgroups.lua");
    
    if(file.open(QFile::WriteOnly))
    {
        if (settings::enable_preambule)
        {
            file.write(settings::preambule.toUtf8() + "\n");
        }
        
        for (auto& dg : darkrp::doorgroups)
        {
            file.write(dg.toUtf8());
        }
        
        file.close();
        
        //alert('i', "Export succesful!", "File was been created: \"" + file.fileName() + "\".");
        logs::sendInfo("Make door group succesful.");
    }
    else
    {
        QMessageBox(QMessageBox::Critical, "Error create file", file.errorString(), QMessageBox::StandardButton::Close).exec();        
        logs::sendError(file.errorString().toStdString().c_str());
    }
}

void Client::makeEntities()
{
    if (darkrp::entities.empty())
    {
        logs::sendWarning("Entities not found.");
        return;
    }
        
    QFile file(settings::outputPath + "\\entities.lua");
    
    if(file.open(QFile::WriteOnly))
    {
        if (settings::enable_preambule)
        {
            file.write(settings::preambule.toUtf8() + "\n");
        }
        
        for (auto& e : darkrp::entities)
        {
            file.write(e.toUtf8());
        }
        
        file.close();
        
        //alert('i', "Export succesful!", "File was been created: \"" + file.fileName() + "\".");
        logs::sendInfo("Make entity succesful.");
    }
    else
    {
        QMessageBox(QMessageBox::Critical, "Error create file", file.errorString(), QMessageBox::StandardButton::Close).exec();        
        logs::sendError(file.errorString().toStdString().c_str());
    }
}

void Client::makeFood()
{
    if (darkrp::food.empty())
    {
        logs::sendWarning("Food not found.");
        return;
    }
        
    QFile file(settings::outputPath + "\\food.lua");
    
    if(file.open(QFile::WriteOnly))
    {
        if (settings::enable_preambule)
        {
            file.write(settings::preambule.toUtf8() + "\n");
        }
        
        for (auto& f : darkrp::food)
        {
            file.write(f.toUtf8());
        }
        
        file.close();
        
        //alert('i', "Export succesful!", "File was been created: \"" + file.fileName() + "\".");
        logs::sendInfo("Make food succesful.");
    }
    else
    {
        QMessageBox(QMessageBox::Critical, "Error create file", file.errorString(), QMessageBox::StandardButton::Close).exec();        
        logs::sendError(file.errorString().toStdString().c_str());
    }
}

void Client::makeGroupChats()
{
    if (darkrp::groupchats.empty())
    {
        logs::sendWarning("Group chats not found.");
        return;
    }
        
    QFile file(settings::outputPath + "\\groupchats.lua");
    
    if(file.open(QFile::WriteOnly))
    {
        if (settings::enable_preambule)
        {
            file.write(settings::preambule.toUtf8() + "\n");
        }
        
        for (auto& gc : darkrp::groupchats)
        {
            file.write(gc.toUtf8());
        }
        
        file.close();
        
        //alert('i', "Export succesful!", "File was been created: \"" + file.fileName() + "\".");
        logs::sendInfo("Make group chat succesful.");
    }
    else
    {
        QMessageBox(QMessageBox::Critical, "Error create file", file.errorString(), QMessageBox::StandardButton::Close).exec();        
        logs::sendError(file.errorString().toStdString().c_str());
    }
}

void Client::makeJobs()
{
    if (darkrp::jobs.empty())
    {
        logs::sendWarning("Jobs not found.");
        return;
    }
        
    QFile file(settings::outputPath + "\\jobs.lua");
    
    if(file.open(QFile::WriteOnly))
    {
        if (settings::enable_preambule)
        {
            file.write(settings::preambule.toUtf8() + "\n");
        }
        
        for (auto& job : darkrp::jobs)
        {
            file.write(job.toUtf8());
        }
        
        file.close();
        
        //alert('i', "Export succesful!", "File was been created: \"" + file.fileName() + "\".");
        logs::sendInfo("Make job succesful.");
    }
    else
    {
        QMessageBox(QMessageBox::Critical, "Error create file", file.errorString(), QMessageBox::StandardButton::Close).exec();        
        logs::sendError(file.errorString().toStdString().c_str());
    }
}

void Client::makeShipments()
{
    if (darkrp::shipments.empty())
    {
        logs::sendWarning("Shipments not found.");
        return;
    }
        
    QFile file(settings::outputPath + "\\shipments.lua");
    
    if(file.open(QFile::WriteOnly))
    {
        if (settings::enable_preambule)
        {
            file.write(settings::preambule.toUtf8() + "\n");
        }
        
        for (auto& ship : darkrp::shipments)
        {
            file.write(ship.toUtf8());
        }
        
        file.close();
        
        //alert('i', "Export succesful!", "File was been created: \"" + file.fileName() + "\".");
        logs::sendInfo("Make shipment succesful.");
    }
    else
    {
        QMessageBox(QMessageBox::Critical, "Error create file", file.errorString(), QMessageBox::StandardButton::Close).exec();        
        logs::sendError(file.errorString().toStdString().c_str());
    }
}

void Client::makeVehicles()
{
    if (darkrp::vehicles.empty())
    {
        logs::sendWarning("Vehicles not found.");
        return;
    }
        
    QFile file(settings::outputPath + "\\vehicles.lua");
    
    if(file.open(QFile::WriteOnly))
    {
        if (settings::enable_preambule)
        {
            file.write(settings::preambule.toUtf8() + "\n");
        }
        
        for (auto& v : darkrp::vehicles)
        {
            file.write(v.toUtf8());
        }
        
        file.close();
        
        //alert('i', "Export succesful!", "File was been created: \"" + file.fileName() + "\".");
        logs::sendInfo("Make vehicle succesful.");
    }
    else
    {
        QMessageBox(QMessageBox::Critical, "Error create file", file.errorString(), QMessageBox::StandardButton::Close).exec();        
        logs::sendError(file.errorString().toStdString().c_str());
    } 
}

void Client::_make(MakeMode mode)
{    
    switch (mode)
    {
    case MAKE_ALL:
    {
        make();
        break;
    }
    case MAKE_AGENDAS:
    {
        makeAgendas();
        break;
    }
    case MAKE_AMMO:
    {
        makeAmmo();
        break;
    }
    case MAKE_CATEGORIES:
    {
        makeCategories();
        break;
    }
    case MAKE_DEMOTEGROUPS:
    {
        makeDemoteGroups();
        break;
    }
    case MAKE_DOORGROUPS:
    {
        makeDoorGroups();
        break;
    }
    case MAKE_ENTITIES:
    {
        makeEntities();
        break;
    }
    case MAKE_FOOD:
    {
        makeFood();
        break;
    }
    case MAKE_GROUPCHATS:
    {
        makeGroupChats();
        break;
    }
    case MAKE_JOBS:
    {
        makeJobs();
        break;
    }
    case MAKE_SHIPMENTS:
    {
        makeShipments();
        break;
    }
    case MAKE_VEHICLES:
    {
        makeVehicles();
        break;
    }
    case MAKE_UNKNOWN:
    default:
        break;
    }
}

Client::~Client()
{
    delete ui;
    pmv::close();
}

void Client::closeEvent(QCloseEvent*)
{
    emit slot_close();
    delete this;
}

void Client::init()
{
    using namespace client::control;
    logs::logsFileName = QDateTime::currentDateTime().toString("dd-MM_hh-mm-ss.log");
    logs::sendSystem("Init start...");
    
    startup( meta::name, meta::version , { 1150, 720 } );
    
    changeTabIcon(ui->tabAgendas, "table.png");
    changeTabIcon(ui->tabAmmo, "bomb.png");
    changeTabIcon(ui->tabCategories, "folder.png");
    changeTabIcon(ui->tabDemoteGroups, "user_delete.png");
    changeTabIcon(ui->tabDoorGroups, "door.png");
    changeTabIcon(ui->tabEntities, "basket.png");
    changeTabIcon(ui->tabFood, "cake.png");
    changeTabIcon(ui->tabGroupChats, "comments.png");
    changeTabIcon(ui->tabJobs, "user_gray.png");
    changeTabIcon(ui->tabShipments, "box.png");
    changeTabIcon(ui->tabVehicles, "car.png");
    
    /// Agendas ///
    
    agendas._codeName = "Agenda";
    
    agendas.title_text = newTitle("Title:");
    agendas.manager_text = newTitle("Manager:");
    agendas.listeners_text = newTitle("Listeners:");
    
    agendas.title_input = newInput("Police agenda");
    agendas.manager_input = newInput("TEAM_MAYOR");
    agendas.listeners_input = newInput("TEAM_CHIEF, TEAM_POLICE, ...");
    
    agendas.listeners_list = newList("Listeners");

    agendas.struct_end = newStructEnd(ui->gridAgendas);
    
    agendas._requiredInputs << agendas.title_input << agendas.manager_input;
    
    logs::sendSystem("Struct agendas loaded.");
            
    /// Ammo ///
    
    ammo._codeName = "Ammo";
    
    ammo.ammoType_text = newTitle("Type:");
    ammo.name_text = newTitle("Name:");
    ammo.model_text = newTitle("Model:");
    ammo.price_text = newTitle("Price:");
    ammo.amountGiven_text = newTitle("Amount given:");
    
    ammo.ammoType_input = newInput("pistol");
    ammo.name_input = newInput("Pistol ammo");
    ammo.model_input = newInput("models/Items/BoxSRounds.mdl");
    ammo.price_input = newNumeric(0, 999999, 10);
    ammo.amountGiven_input = newNumeric(0, 999999);
    
    ammo.customCheck_button = newButton("Custom check function", "Custom check function on lua language", false, "script_edit.png");
    
    ammo.spacer_1 = newSpacer("vertical");
    
    ammo.struct_end = newStructEnd(ui->gridAmmo);
    
    ammo._requiredInputs << ammo.ammoType_input << ammo.name_input << ammo.model_input;
    
    logs::sendSystem("Struct ammo loaded.");
    
    /// Categories ///
    
    categories._codeName = "Category";
    
    categories.name_text = newTitle("Name:");    
    categories.categories_text = newTitle("Category:");
    categories.startExpanded_text = newTitle("Start expanded:");
    categories.color_text = newTitle("Color:");
    categories.sortOrder_text = newTitle("Sort order:");
    
    categories.name_input = newInput("Citizens");

    categories.startExpanded_check = newCheck("", true);

    categories.categories_combo = newCombo({"jobs", "entities", "shipments", "weapons", "vehicles", "ammo"});
    
    categories.sortOrder_numeric = newNumeric(0, 999);
        
    categories.canSee_button = newButton("Can see function", "Can see function on Lua language", false, "script_edit.png");
    categories.color_button = newButton("Select color", "", false, "color_wheel.png");
    
    categories.spacer_1 = newSpacer("vertical");
    
    categories.struct_end = newStructEnd(ui->gridCategories);
    
    categories._requiredInputs << categories.name_input;
    
    logs::sendSystem("Struct categories loaded.");
    
    /// Demotegroups ///
    
    demotegroups._codeName = "Demote group";
    
    demotegroups.name_text = newTitle("Name of the group:");
    demotegroups.members_text = newTitle("Members:");
    
    demotegroups.name_input = newInput("Cops");
    demotegroups.members_input = newInput("TEAM_POLICE, TEAM_CHIEF");
    
    demotegroups.members_list = newList("Demote teams");
    
    demotegroups.struct_end = newStructEnd(ui->gridDemoteGroups);
    
    demotegroups._requiredInputs << demotegroups.name_input;
    
    logs::sendSystem("Struct demotegroups loaded.");
    
    /// Doorgroups ///
    
    doorgroups._codeName = "Door group";
    
    doorgroups.name_text = newTitle("Name of the group:");
    doorgroups.members_text = newTitle("Teams:");
    
    doorgroups.name_input = newInput("Cops and Mayor only");
    doorgroups.members_input = newInput("TEAM_CHIEF, TEAM_POLICE, TEAM_MAYOR");
    
    doorgroups.members_list = newList("Door teams");
    
    doorgroups.struct_end = newStructEnd(ui->gridDoorGroups);
    
    doorgroups._requiredInputs << doorgroups.name_input;
    
    logs::sendSystem("Struct doorgroups loaded.");
    
    /// Entities ///
    
    entities._codeName = "Entity";
    
    entities.name_text = newTitle("Display name:");
    entities.entity_text = newTitle("Entity:");
    entities.model_text = newTitle("Model:");
    entities.price_text = newTitle("Price:");
    entities.max_text = newTitle("Max:");
    entities.cmd_text = newTitle("Command:");
    entities.allowed_text = newTitle("Allowed teams:");
    entities.allowTools_text = newTitle("Allow tools:");
    
    entities.name_input = newInput("Money printer");
    entities.entity_input = newInput("money_printer");
    entities.model_input = newInput("models/props_c17/consolebox01a.mdl");
    entities.cmd_input = newInput("buymoneyprinter");
    entities.allowed_input = newInput("TEAM_GANG, TEAM_MOB");
    
    entities.allowTools_check = newCheck("", false);
    
    entities.price_numeric = newNumeric(0, 99999, 5);
    entities.max_numeric = newNumeric(0, 100);
    
    entities.allowed_list = newList("Allowed teams");
    
    entities.struct_end = newStructEnd(ui->gridEntities);
    
    entities._requiredInputs << entities.name_input << entities.entity_input << entities.model_input << entities.cmd_input;
    
    logs::sendSystem("Struct entities loaded.");
    
    /// Food ///
    
    food._codeName = "Food";
    
    food.name_text = newTitle("Name:");
    food.model_text = newTitle("Model:");
    food.energy_text = newTitle("Energy:");
    food.price_text = newTitle("Price:");
    food.requiresCook_text = newTitle("Requires cook:");
    
    food.name_input = newInput("Banana");
    food.model_input = newInput("models/props/cs_italy/bananna.mdl");
    
    food.energy_numecric = newNumeric(0, 999, 5);
    food.price_numeric = newNumeric(0, 99999);
    
    food.requiresCook_check = newCheck("", false);
    
    food.customCheck_button = newButton("Custom check function", "Custom Lua function", false, "script_edit.png");
    
    food.spacer_1 = newSpacer("vertical");
    
    food.struct_end = newStructEnd(ui->gridFood);
    
    food._requiredInputs << food.name_input << food.model_input;
    
    logs::sendSystem("Struct food loaded.");
    
    /// Groupchats ///
    
    groupchats._codeName = "Group of chat";
    
    groupchats.members_text = newTitle("Members:");
    
    groupchats.members_input = newInput("TEAM_HOBO");
    
    groupchats.members_list = newList("Chat members");
    
    groupchats.function_tool = newTool("script_edit.png", "Custom Lua function");
    
    groupchats.struct_end = newStructEnd(ui->gridGroupChats);
        
    logs::sendSystem("Struct groupchats loaded.");
    
    /// Jobs ///
    
    jobs._codeName = "Job";
    
    jobs.codeName_text = newTitle("Team:");
    jobs.name_text = newTitle("Display name:");
    jobs.color_text = newTitle("Color:");
    jobs.models_text = newTitle("Models:");
    jobs.description_text = newTitle("Description:");
    jobs.weapons_text = newTitle("Weapons:");
    jobs.command_text = newTitle("Command:");
    jobs.max_text = newTitle("Maximum:");
    jobs.salary_text = newTitle("Salary:");
    jobs.admin_text = newTitle("Admin:");
    jobs.vote_text = newTitle("Vote:");
    jobs.hasLicense_text = newTitle("Has license:");
    jobs.canDemote_text = newTitle("Can demote:");
    jobs.category_text = newTitle("Category:");
    
    jobs.codeName_input = newInput("TEAM_CITIZEN");
    jobs.name_input = newInput("Citizen");
    jobs.models_input = newInput("models/player/Group01/Female_01.mdl");
        jobs.models_input->setContextMenuPolicy(Qt::CustomContextMenu);
    jobs.weapons_input = newInput("arrest_stick");
    jobs.command_input = newInput("citizen");
    jobs.category_input = newInput("Citizens");
    
    jobs.max_numeric = newNumeric(0, 128);
    jobs.salary_numeric = newNumeric(0, 1000000, 50);
    
    //jobs.admin_combo = newCombo( { "All", "Admin or higher", "Superadmin or rcon", "Rcon only" } );
    jobs.admin_combo = newCombo(new AdminModel(this));
    
    jobs.vote_check = newCheck("", false);
    jobs.hasLicense_check = newCheck("", false);
    jobs.canDemote_check = newCheck("", true);
    
    jobs.description = newText();
    
    jobs.color_button = newButton("Select color", "", false, "color_wheel.png");
    jobs.customCheck_button = newButton("Custon check Lua function", "", false, "script_edit.png");
    
    jobs.models_list = newList("Models");
    jobs.weapons_list = newList("Weapons");
    
    jobs.struct_end = newStructEnd(ui->gridJobs);
    
    jobs._requiredInputs << jobs.codeName_input << jobs.name_input << jobs.command_input << jobs.category_input;
    
    logs::sendSystem("Struct jobs loaded.");
    
    /// Shipments ///
    
    shipments._codeName = "Shipment";
    
    shipments.name_text = newTitle("Name:");
    shipments.model_text = newTitle("Model:");
    shipments.entity_text = newTitle("Entity:");
    shipments.price_text = newTitle("Price:");
    shipments.amount_text = newTitle("Amount:");
    shipments.separate_text = newTitle("Separate:");
    shipments.priceSep_text = newTitle("Price separate:");
    shipments.noShip_text = newTitle("No Shipment:");
    shipments.allowed_text = newTitle("Allowed:");
    shipments.category_text = newTitle("Category:");
    
    shipments.name_input = newInput("Desert eagle");
    shipments.model_input = newInput("models/weapons/w_pist_deagle.mdl");
    shipments.entity_input = newInput("weapon_deagle2");
    shipments.allowed_input = newInput("TEAM_GUN");
    shipments.category_input = newInput("Pistols");
    
    shipments.price_numeric = newNumeric(0, 999999, 100);
    shipments.amount_numeric = newNumeric(1, 999);
    shipments.priceSep_numeric = newNumeric(0, 999999, 100);
    
    shipments.separate_check = newCheck("", false);
    shipments.noShip_check = newCheck("", false);
    
    shipments.members_list = newList("Allowed teams");
    
    shipments.struct_end = newStructEnd(ui->gridShipments);
    
    shipments._requiredInputs << shipments.name_input << shipments.model_input << shipments.entity_input << shipments.category_input;
    
    logs::sendSystem("Struct shipments loaded.");
    
    /// Vehicles ///
    
    vehicles._codeName = "Vehicle";
    
    vehicles.name_text = newTitle("Name:");
    vehicles.model_text = newTitle("Model:");
    vehicles.price_text = newTitle("Price");
    vehicles.allowed_text = newTitle("Allowed:");
    vehicles.label_text = newTitle("Label (Display name):");
    vehicles.distance_text = newTitle("Distance:");
    vehicles.angle_text = newTitle("Angle:");
    vehicles.onBought_text = newTitle("On bought:");
    
    vehicles.name_input = newInput("Bus");
    vehicles.model_input = newInput("models/borderlands/bus/bus.mdl");
    vehicles.allowed_input = newInput("TEAM_MOB, TEAM_GUNGSTER");
    vehicles.label_input = newInput("Ultra bus (leave empty, for using Name)");
    vehicles.angle_input = newInput("0, 90, 0", "", 32, "999, 999, 999");
    
    vehicles.price_numeric = newNumeric(0, 999999, 100);
    vehicles.distance_numeric = newNumeric(0, 999999, 10);
    
    vehicles.onBought_button = newButton("Lua function", "", false, "script_edit.png");
    vehicles.customCheck_button = newButton("Custom check function", "", false, "script_edit.png");

    vehicles.members_list = newList("Allowed teams");
    
    vehicles.struct_end = newStructEnd(ui->gridVehicles);
    
    vehicles._requiredInputs << vehicles.name_input << vehicles.model_input;
    
    logs::sendSystem("Struct vehicles loaded.");
    
    
    /// W I D G E T S ///
                
    ui->gridAgendas->addWidget(agendas.title_text, 1, 0);
    ui->gridAgendas->addWidget(agendas.manager_text, 2, 0);
    ui->gridAgendas->addWidget(agendas.listeners_text, 3, 0);
    ui->gridAgendas->addWidget(agendas.title_input, 1, 1);
    ui->gridAgendas->addWidget(agendas.manager_input, 2, 1);
    ui->gridAgendas->addWidget(agendas.listeners_input, 3, 1);
    ui->pageAgendas->addWidget(agendas.listeners_list, 1);
    
    ui->gridAmmo->addWidget(ammo.ammoType_text, 1, 0);
    ui->gridAmmo->addWidget(ammo.name_text, 2, 0);
    ui->gridAmmo->addWidget(ammo.model_text, 3, 0);
    ui->gridAmmo->addWidget(ammo.price_text, 4, 0);
    ui->gridAmmo->addWidget(ammo.amountGiven_text, 5, 0);
    ui->gridAmmo->addWidget(ammo.ammoType_input, 1, 1);
    ui->gridAmmo->addWidget(ammo.name_input, 2, 1);
    ui->gridAmmo->addWidget(ammo.model_input, 3, 1);
    ui->gridAmmo->addWidget(ammo.price_input, 4, 1);
    ui->gridAmmo->addWidget(ammo.amountGiven_input, 5, 1);
    ui->pageAmmo->addWidget(ammo.customCheck_button, 1);
    ui->pageAmmo->addItem(ammo.spacer_1);
    
    ui->gridCategories->addWidget(categories.name_text, 1, 0);
    ui->gridCategories->addWidget(categories.categories_text, 2, 0);
    ui->gridCategories->addWidget(categories.startExpanded_text, 3, 0);
    ui->gridCategories->addWidget(categories.color_text, 4, 0);
    ui->gridCategories->addWidget(categories.sortOrder_text, 5, 0);
    ui->gridCategories->addWidget(categories.name_input, 1, 1);
    ui->gridCategories->addWidget(categories.categories_combo, 2, 1);
    ui->gridCategories->addWidget(categories.startExpanded_check, 3, 1);
    ui->gridCategories->addWidget(categories.color_button, 4 ,1);
    ui->gridCategories->addWidget(categories.sortOrder_numeric, 5, 1);
    ui->pageCategories->addWidget(categories.canSee_button, 1);
    ui->pageCategories->addItem(categories.spacer_1);
    
    ui->gridDemoteGroups->addWidget(demotegroups.name_text, 1, 0);
    ui->gridDemoteGroups->addWidget(demotegroups.members_text, 2, 0);
    ui->gridDemoteGroups->addWidget(demotegroups.name_input, 1, 1);
    ui->gridDemoteGroups->addWidget(demotegroups.members_input, 2, 1);
    ui->pageDemoteGroups->addWidget(demotegroups.members_list, 1);
    
    ui->gridDoorGroups->addWidget(doorgroups.name_text, 1, 0);
    ui->gridDoorGroups->addWidget(doorgroups.members_text, 2, 0);
    ui->gridDoorGroups->addWidget(doorgroups.name_input, 1, 1);
    ui->gridDoorGroups->addWidget(doorgroups.members_input, 2, 1);
    ui->pageDoorGroups->addWidget(doorgroups.members_list, 1);
    
    ui->gridEntities->addWidget(entities.name_text, 1, 0);
    ui->gridEntities->addWidget(entities.entity_text, 2, 0);
    ui->gridEntities->addWidget(entities.model_text, 3, 0);
    ui->gridEntities->addWidget(entities.price_text, 4, 0);
    ui->gridEntities->addWidget(entities.max_text, 5, 0);
    ui->gridEntities->addWidget(entities.cmd_text, 6, 0);
    ui->gridEntities->addWidget(entities.allowed_text, 7, 0);
    ui->gridEntities->addWidget(entities.allowTools_text, 8, 0);
    ui->gridEntities->addWidget(entities.name_input, 1, 1);
    ui->gridEntities->addWidget(entities.entity_input, 2, 1);
    ui->gridEntities->addWidget(entities.model_input, 3, 1);
    ui->gridEntities->addWidget(entities.price_numeric, 4, 1);
    ui->gridEntities->addWidget(entities.max_numeric, 5, 1);
    ui->gridEntities->addWidget(entities.cmd_input, 6, 1);
    ui->gridEntities->addWidget(entities.allowed_input, 7, 1);
    ui->gridEntities->addWidget(entities.allowTools_check, 8, 1);
    ui->pageEntities->addWidget(entities.allowed_list, 1);
    
    ui->gridFood->addWidget(food.name_text, 1, 0);
    ui->gridFood->addWidget(food.model_text, 2, 0);
    ui->gridFood->addWidget(food.energy_text, 3, 0);
    ui->gridFood->addWidget(food.price_text, 4, 0);
    ui->gridFood->addWidget(food.requiresCook_text, 5, 0);
    ui->gridFood->addWidget(food.name_input, 1, 1);
    ui->gridFood->addWidget(food.model_input, 2, 1);
    ui->gridFood->addWidget(food.energy_numecric, 3, 1);
    ui->gridFood->addWidget(food.price_numeric, 4, 1);
    ui->gridFood->addWidget(food.requiresCook_check, 5, 1);
    ui->pageFood->addWidget(food.customCheck_button, 1);
    ui->pageFood->addItem(food.spacer_1);
    
    ui->gridGroupChats->addWidget(groupchats.members_text, 1, 0);
    ui->gridGroupChats->addWidget(groupchats.members_input, 1, 1);
    ui->gridGroupChats->addWidget(groupchats.function_tool, 1, 2);
    ui->pageGroupChats->addWidget(groupchats.members_list, 1);
    
    ui->gridJobs->addWidget(jobs.codeName_text, 1, 0);
    ui->gridJobs->addWidget(jobs.name_text, 2, 0);
    ui->gridJobs->addWidget(jobs.color_text, 3, 0);
    ui->gridJobs->addWidget(jobs.models_text, 4, 0);
    ui->gridJobs->addWidget(jobs.description_text, 5, 0);
    ui->gridJobs->addWidget(jobs.weapons_text, 6, 0);
    ui->gridJobs->addWidget(jobs.command_text, 7, 0);
    ui->gridJobs->addWidget(jobs.max_text, 8, 0);
    ui->gridJobs->addWidget(jobs.salary_text, 9, 0);
    ui->gridJobs->addWidget(jobs.admin_text, 10, 0);
    ui->gridJobs->addWidget(jobs.vote_text, 11, 0);
    ui->gridJobs->addWidget(jobs.hasLicense_text, 12, 0);
    ui->gridJobs->addWidget(jobs.canDemote_text, 13, 0);
    ui->gridJobs->addWidget(jobs.category_text, 14, 0);
    //
    ui->gridJobs->addWidget(jobs.codeName_input, 1, 1);
    ui->gridJobs->addWidget(jobs.name_input, 2, 1);
    ui->gridJobs->addWidget(jobs.color_button, 3, 1);
    ui->gridJobs->addWidget(jobs.models_input, 4, 1);
    ui->gridJobs->addWidget(jobs.description, 5, 1);
    ui->gridJobs->addWidget(jobs.weapons_input, 6, 1);
    ui->gridJobs->addWidget(jobs.command_input, 7, 1);
    ui->gridJobs->addWidget(jobs.max_numeric, 8, 1);
    ui->gridJobs->addWidget(jobs.salary_numeric, 9, 1);
    ui->gridJobs->addWidget(jobs.admin_combo, 10, 1);
    ui->gridJobs->addWidget(jobs.vote_check, 11, 1);
    ui->gridJobs->addWidget(jobs.hasLicense_check, 12, 1);
    ui->gridJobs->addWidget(jobs.canDemote_check, 13, 1);
    ui->gridJobs->addWidget(jobs.category_input, 14, 1);    
    //
    ui->pageJobs->addWidget(jobs.customCheck_button, 1);    
    {
        QHBoxLayout* l = new QHBoxLayout(ui->pageJobs->widget());   // easy
        l->addWidget(jobs.weapons_list, 0);
        l->addWidget(jobs.models_list, 1);
        ui->pageJobs->addLayout(l);
    }
    
    ui->gridShipments->addWidget(shipments.name_text, 1, 0);
    ui->gridShipments->addWidget(shipments.model_text, 2, 0);
    ui->gridShipments->addWidget(shipments.entity_text, 3, 0);
    ui->gridShipments->addWidget(shipments.price_text, 4, 0);
    ui->gridShipments->addWidget(shipments.amount_text, 5, 0);
    ui->gridShipments->addWidget(shipments.separate_text, 6, 0);
    ui->gridShipments->addWidget(shipments.priceSep_text, 7, 0);
    ui->gridShipments->addWidget(shipments.noShip_text, 8, 0);
    ui->gridShipments->addWidget(shipments.allowed_text, 9, 0);
    ui->gridShipments->addWidget(shipments.category_text, 10, 0);
    //
    ui->gridShipments->addWidget(shipments.name_input, 1, 1);
    ui->gridShipments->addWidget(shipments.model_input, 2, 1);
    ui->gridShipments->addWidget(shipments.entity_input, 3, 1);
    ui->gridShipments->addWidget(shipments.price_numeric, 4, 1);
    ui->gridShipments->addWidget(shipments.amount_numeric, 5, 1);
    ui->gridShipments->addWidget(shipments.separate_check, 6, 1);
    ui->gridShipments->addWidget(shipments.priceSep_numeric, 7, 1);
    ui->gridShipments->addWidget(shipments.noShip_check, 8, 1);
    ui->gridShipments->addWidget(shipments.allowed_input, 9, 1);
    ui->gridShipments->addWidget(shipments.category_input, 10, 1);
    //
    ui->pageShipments->addWidget(shipments.members_list, 1);
    
    ui->gridVehicles->addWidget(vehicles.name_text, 1, 0);
    ui->gridVehicles->addWidget(vehicles.model_text, 2, 0);
    ui->gridVehicles->addWidget(vehicles.price_text, 3, 0);
    ui->gridVehicles->addWidget(vehicles.allowed_text, 4, 0);
    ui->gridVehicles->addWidget(vehicles.label_text, 5, 0);
    ui->gridVehicles->addWidget(vehicles.distance_text, 6, 0);
    ui->gridVehicles->addWidget(vehicles.angle_text, 7, 0);
    ui->gridVehicles->addWidget(vehicles.onBought_text, 8, 0);
    //
    ui->gridVehicles->addWidget(vehicles.name_input, 1, 1);
    ui->gridVehicles->addWidget(vehicles.model_input, 2, 1);
    ui->gridVehicles->addWidget(vehicles.price_numeric, 3, 1);
    ui->gridVehicles->addWidget(vehicles.allowed_input, 4, 1);
    ui->gridVehicles->addWidget(vehicles.label_input, 5, 1);
    ui->gridVehicles->addWidget(vehicles.distance_numeric, 6, 1);
    ui->gridVehicles->addWidget(vehicles.angle_input, 7, 1);
    ui->gridVehicles->addWidget(vehicles.onBought_button, 8, 1);
    ui->pageVehicles->addWidget(vehicles.customCheck_button, 1);
    ui->pageVehicles->addWidget(vehicles.members_list, 2);
    
    logs::sendSystem("All widgets ready to use.");
}

void Client::init_connect()
{
    //agendas
    
    QObject::connect(agendas.listeners_input, &INPUT::returnPressed, this, [this] {
        QString _text = agendas.listeners_input->text();
        if (_text.isEmpty()) return;
        agendas.listeners_list->addItem(_text);
        agendas.listeners_input->clear();
    });
    
    QObject::connect(agendas.listeners_list, &LIST::itemDoubleClicked, this, [this] (QListWidgetItem* item) {
        agendas.listeners_list->takeItem(agendas.listeners_list->row(item));
    });
    
    /*
    QObject::connect(agendas.struct_end->push, &TOOL::clicked, this, [this] {
        if (push("Agendas") == true)        
        {
            struct
            {
                QString make;
                QString name;
                QString time;
            } Arguments = { "Agendas", agendas._codeName, QDateTime::currentDateTime().toString("hh:mm / dd.MM") };
            
            viewer::append(&Arguments);
            clear("Agendas");
        }
    });

    QObject::connect(agendas.struct_end->clear, &TOOL::clicked, this, [this] {
        clear("Agendas");
    });
    */
    
    if (!agendas._requiredInputs.isEmpty())
    {
        for (auto* i : agendas._requiredInputs)
        {
            QObject::connect(i, &INPUT::textEdited, this, [i] {
                if (i->text().isEmpty())
                {
                    OnRequiredInputEmpty(i);
                }
                else
                {
                    OnRequiredInputAppend(i);
                }
            });
        }
    }
    
    //ammo
    
    if (!ammo._requiredInputs.isEmpty())
    {
        for (auto* i : ammo._requiredInputs)
        {
            QObject::connect(i, &INPUT::textEdited, this, [i] {
                if (i->text().isEmpty())
                {
                    OnRequiredInputEmpty(i);
                }
                else
                {
                    OnRequiredInputAppend(i);
                }
            });
        }
    }
    
    //categories
    
    QObject::connect(categories.color_button, &BUTTON::clicked, this, [this] {
        PCOLOR color = new COLOR(this);
        QObject::connect(color, &COLOR::colorSelected, this, [this] (const QColor &color) {
            categories.color_button->setText(
                        QString::number(color.red()) + ", " +
                        QString::number(color.green()) + ", " +
                        QString::number(color.blue()) + ", " +
                        QString::number(color.alpha()));
        });
        color->show();
    });
    
    if (!categories._requiredInputs.isEmpty())
    {
        for (auto* i : categories._requiredInputs)
        {
            QObject::connect(i, &INPUT::textEdited, this, [i] {
                if (i->text().isEmpty())
                {
                    OnRequiredInputEmpty(i);
                }
                else
                {
                    OnRequiredInputAppend(i);
                }
            });
        }
    }
    
    //demotegroups
    
    QObject::connect(demotegroups.members_input, &INPUT::returnPressed, this, [this] {
        QString _text = demotegroups.members_input->text();
        if (_text.isEmpty()) return;
        demotegroups.members_list->addItem(_text);
        demotegroups.members_input->clear();
    });
    
    QObject::connect(demotegroups.members_list, &LIST::itemDoubleClicked, this, [this] (QListWidgetItem* item) {
        demotegroups.members_list->takeItem(demotegroups.members_list->row(item));
    });
    
    if (!demotegroups._requiredInputs.isEmpty())
    {
        for (auto* i : demotegroups._requiredInputs)
        {
            QObject::connect(i, &INPUT::textEdited, this, [i] {
                if (i->text().isEmpty())
                {
                    OnRequiredInputEmpty(i);
                }
                else
                {
                    OnRequiredInputAppend(i);
                }
            });
        }
    }
    
    //doorgroups
    
    QObject::connect(doorgroups.members_input, &INPUT::returnPressed, this, [this] {
        QString _text = doorgroups.members_input->text();
        if (_text.isEmpty()) return;
        doorgroups.members_list->addItem(_text);
        doorgroups.members_input->clear();
    });
    
    QObject::connect(doorgroups.members_list, &LIST::itemDoubleClicked, this, [this] (QListWidgetItem* item) {
        doorgroups.members_list->takeItem(doorgroups.members_list->row(item));
    });
    
    if (!doorgroups._requiredInputs.isEmpty())
    {
        for (auto* i : doorgroups._requiredInputs)
        {
            QObject::connect(i, &INPUT::textEdited, this, [i] {
                if (i->text().isEmpty())
                {
                    OnRequiredInputEmpty(i);
                }
                else
                {
                    OnRequiredInputAppend(i);
                }
            });
        }
    }
    
    //entities
    
    QObject::connect(entities.allowed_input, &INPUT::returnPressed, this, [this] {
        QString _text = entities.allowed_input->text();
        if (_text.isEmpty()) return;
        entities.allowed_list->addItem(_text);
        entities.allowed_input->clear();
    });
    
    QObject::connect(entities.allowed_list, &LIST::itemDoubleClicked, this, [this] (QListWidgetItem* item) {
        entities.allowed_list->takeItem(entities.allowed_list->row(item));
    });
    
    if (!entities._requiredInputs.isEmpty())
    {
        for (auto* i : entities._requiredInputs)
        {
            QObject::connect(i, &INPUT::textEdited, this, [i] {
                if (i->text().isEmpty())
                {
                    OnRequiredInputEmpty(i);
                }
                else
                {
                    OnRequiredInputAppend(i);
                }
            });
        }
    }
    
    //food
    
    if (!food._requiredInputs.isEmpty())
    {
        for (auto* i : food._requiredInputs)
        {
            QObject::connect(i, &INPUT::textEdited, this, [i] {
                if (i->text().isEmpty())
                {
                    OnRequiredInputEmpty(i);
                }
                else
                {
                    OnRequiredInputAppend(i);
                }
            });
        }
    }
    
    //groupchats
    
    QObject::connect(groupchats.members_input, &INPUT::returnPressed, this, [this] {
        QString _text = groupchats.members_input->text();
        if (_text.isEmpty()) return;
        groupchats.members_list->addItem(_text);
        groupchats.members_input->clear();
    });
    
    QObject::connect(groupchats.members_list, &LIST::itemDoubleClicked, this, [this] (QListWidgetItem* item) {
        groupchats.members_list->takeItem(groupchats.members_list->row(item));
    });
    
    if (!groupchats._requiredInputs.isEmpty())
    {
        for (auto* i : groupchats._requiredInputs)
        {
            QObject::connect(i, &INPUT::textEdited, this, [i] {
                if (i->text().isEmpty())
                {
                    OnRequiredInputEmpty(i);
                }
                else
                {
                    OnRequiredInputAppend(i);
                }
            });
        }
    }
    
    //jobs
    
    QObject::connect(jobs.models_input, &INPUT::returnPressed, this, [this] {
        QString _text = jobs.models_input->text();
        if (_text.isEmpty()) return;
        jobs.models_list->addItem(_text);
        jobs.models_input->clear();
    });
    
    QObject::connect(jobs.models_input, &INPUT::customContextMenuRequested, this, [this] {
        QMenu menu;
        connect(menu.addAction("Open list"), &QAction::triggered, this, [] { pmv::show(); });
        QPoint pos = QCursor::pos();
        //QSize size = menu.sizeHint();
        menu.move(pos);
        menu.exec();
    });
    
    QObject::connect(jobs.models_list, &LIST::itemDoubleClicked, this, [this] (QListWidgetItem* item) {
        jobs.models_list->takeItem(jobs.models_list->row(item));
    });
    
    QObject::connect(jobs.weapons_input, &INPUT::returnPressed, this, [this] {
        QString _text = jobs.weapons_input->text();
        if (_text.isEmpty()) return;
        jobs.weapons_list->addItem(_text);
        jobs.weapons_input->clear();
    });
    
    QObject::connect(jobs.weapons_list, &LIST::itemDoubleClicked, this, [this] (QListWidgetItem* item) {
        jobs.weapons_list->takeItem(jobs.weapons_list->row(item));
    });
    
    QObject::connect(jobs.color_button, &BUTTON::clicked, this, [this] {
        PCOLOR color = new COLOR(this);
        QObject::connect(color, &COLOR::colorSelected, this, [this] (const QColor &color) {
            jobs.color_button->setText(
                        QString::number(color.red()) + ", " +
                        QString::number(color.green()) + ", " +
                        QString::number(color.blue()) + ", " +
                        QString::number(color.alpha()));
        });
        color->show();
    });
    
    if (!jobs._requiredInputs.isEmpty())
    {
        for (auto* i : jobs._requiredInputs)
        {
            QObject::connect(i, &INPUT::textEdited, this, [i] {
                if (i->text().isEmpty())
                {
                    OnRequiredInputEmpty(i);
                }
                else
                {
                    OnRequiredInputAppend(i);
                }
            });
        }
    }
    
    //shipments
    
    QObject::connect(shipments.allowed_input, &INPUT::returnPressed, this, [this] {
        QString _text = shipments.allowed_input->text();
        if (_text.isEmpty()) return;
        shipments.members_list->addItem(_text);
        shipments.allowed_input->clear();
    });
    
    QObject::connect(shipments.members_list, &LIST::itemDoubleClicked, this, [this] (QListWidgetItem* item) {
        shipments.members_list->takeItem(shipments.members_list->row(item));
    });
    
    if (!shipments._requiredInputs.isEmpty())
    {
        for (auto* i : shipments._requiredInputs)
        {
            QObject::connect(i, &INPUT::textEdited, this, [i] {
                if (i->text().isEmpty())
                {
                    OnRequiredInputEmpty(i);
                }
                else
                {
                    OnRequiredInputAppend(i);
                }
            });
        }
    }
    
    //vehicles
    
    QObject::connect(vehicles.allowed_input, &INPUT::returnPressed, this, [this] {
        QString _text = vehicles.allowed_input->text();
        if (_text.isEmpty()) return;
        vehicles.members_list->addItem(_text);
        vehicles.allowed_input->clear();
    });
    
    QObject::connect(vehicles.members_list, &LIST::itemDoubleClicked, this, [this] (QListWidgetItem* item) {
        vehicles.members_list->takeItem(vehicles.members_list->row(item));
    });
    
    if (!vehicles._requiredInputs.isEmpty())
    {
        for (auto* i : vehicles._requiredInputs)
        {
            QObject::connect(i, &INPUT::textEdited, this, [i] {
                if (i->text().isEmpty())
                {
                    OnRequiredInputEmpty(i);
                }
                else
                {
                    OnRequiredInputAppend(i);
                }
            });
        }
    }
    
    logs::sendSystem("All objects was been connected.");
}

void Client::on_actionExport_current_triggered()
{    
    QString _tabName = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
    logs::sendInfo(QString("Trying export " + _tabName).toStdString().c_str());
    MakeMode _mode = convert_string_to_make(_tabName);
    
    _make(_mode);
}


void Client::on_actionViewer_triggered()
{
    viewer::show(this);
}


void Client::on_actionLogs_triggered()
{
    logs::show(this);
}


void Client::on_actionExport_all_triggered()
{
    make();
    viewer::flush();
    clear_darkrp();
}


void Client::on_actionSettings_triggered()
{
    settings::show(this);
}


void Client::on_actionNew_triggered()
{
    viewer::flush();
    clear_darkrp();
    logs::sendInfo("Created new file.");
}


void Client::on_actionClose_triggered()
{
    this->close();
}

static void OnRequiredInputEmpty(PINPUT _pInput)
{
    _pInput->setStyleSheet("QLineEdit{"
                           "background-color: rgb(255, 50, 70);"
                           "border: 1px solid rgb(100, 20, 20);"
                           "}");
}

static void OnRequiredInputAppend(PINPUT _pInput)
{
    _pInput->setStyleSheet("");
}

void Client::on_actionSave_current_triggered()
{
    MakeMode mode = convert_string_to_make(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
    
    QString path = QFileDialog::getSaveFileName(this, "Save name", QDir::homePath());
    
    if (path.isEmpty())
    {
        logs::sendWarning("Save path is empty.");
        return;
    }
    
    if (path.endsWith(".json") == false)
        path.append(".json");
    
    switch(mode)
    {
    case MAKE_AGENDAS:
    {
        QFile file(path);
        if (file.open(QFile::WriteOnly))        
        {
            QJsonObject obj;
            QJsonArray listeners;
            
            for (int i{}; i < agendas.listeners_list->count(); ++i)
            {
                auto* item = agendas.listeners_list->item(i);
                listeners.append(item->text());
            }            
            
            obj["id"] = 0;
            obj["listeners"] = listeners;            
            obj["manager"] = agendas.manager_input->text();
            obj["title"] = agendas.title_input->text();
            
            QJsonDocument doc;
            doc.setObject(obj);
            
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
            
            logs::sendInfo("Save agendas succesful.");
        }
        else
        {
            logs::sendError(file.errorString().toStdString().c_str());
        }
        
        break;
    }
    case MAKE_AMMO:
    {
        QFile file(path);
        if (file.open(QFile::WriteOnly))        
        {
            QJsonObject obj;
            
            obj["id"] = 1;
            obj["ammoType"] = ammo.ammoType_input->text();
            obj["amountGiven"] = ammo.amountGiven_input->value();
            obj["customCheck"] = JSON_NULL;  //null
            obj["model"] = ammo.model_input->text();
            obj["name"] = ammo.name_input->text();
            obj["price"] = ammo.price_input->value();
            
            QJsonDocument doc;
            doc.setObject(obj);            
            
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
            
            logs::sendInfo("Save ammo succesful.");
        }
        else
        {
            logs::sendError(file.errorString().toStdString().c_str());
        }
        
        break;
    }
    case MAKE_CATEGORIES:
    {
        QFile file(path);
        if (file.open(QFile::WriteOnly))        
        {
            QJsonObject obj;
            
            obj["id"] = 2;
            obj["category"] = categories.categories_combo->currentIndex();
            obj["color"] = categories.color_button->text();
            obj["name"] = categories.name_input->text();
            obj["sortOrder"] = categories.sortOrder_numeric->value();
            obj["startExpanded"] = categories.startExpanded_check->isChecked();
            
            QJsonDocument doc;
            doc.setObject(obj);            
            
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
            
            logs::sendInfo("Save categories succesful.");
        }
        else
        {
            logs::sendError(file.errorString().toStdString().c_str());
        }
        
        break;
    }
    case MAKE_DEMOTEGROUPS:
    {
        QFile file(path);
        if (file.open(QFile::WriteOnly))        
        {
            QJsonObject obj;
            
            obj["id"] = 3;
            obj["name"] = demotegroups.name_input->text();
            
            QJsonArray arr;
            
            for (int i{}; i < demotegroups.members_list->count(); ++i)
            {
                auto* item = demotegroups.members_list->item(i);
                arr.append(item->text());
            }
            
            obj["members"] = arr;
            
            QJsonDocument doc;
            doc.setObject(obj);            
            
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
            
            logs::sendInfo("Save demotegroups succesful.");
        }
        else
        {
            logs::sendError(file.errorString().toStdString().c_str());
        }
        
        break;
    }
    case MAKE_DOORGROUPS:
    {
        QFile file(path);
        if (file.open(QFile::WriteOnly))        
        {
            QJsonObject obj;
            
            obj["id"] = 4;
            obj["name"] = doorgroups.name_input->text();
            
            QJsonArray arr;
            
            for (int i{}; i < doorgroups.members_list->count(); ++i)
            {
                auto* item = doorgroups.members_list->item(i);
                arr.append(item->text());
            }
            
            obj["members"] = arr;
            
            QJsonDocument doc;
            doc.setObject(obj);            
            
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
            
            logs::sendInfo("Save doorgroups succesful.");
        }
        else
        {
            logs::sendError(file.errorString().toStdString().c_str());
        }
        
        break;
    }
    case MAKE_ENTITIES:
    {
        QFile file(path);
        if (file.open(QFile::WriteOnly))        
        {
            QJsonObject obj;
            
            obj["id"] = 5;
            
            QJsonArray arr;
            
            for (int i{}; i < entities.allowed_list->count(); ++i)
            {
                auto* item = entities.allowed_list->item(i);
                arr.append(item->text());
            }
            
            obj["allowed"] = arr;
            obj["allowTools"] = entities.allowTools_check->isChecked();
            obj["cmd"] = entities.cmd_input->text();
            obj["entity"] = entities.entity_input->text();
            obj["max"] = entities.max_numeric->value();
            obj["model"] = entities.model_input->text();
            obj["name"] = entities.name_input->text();
            obj["price"] = entities.price_numeric->value();
            
            QJsonDocument doc;
            doc.setObject(obj);            
            
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
            
            logs::sendInfo("Save entities succesful.");
        }
        else
        {
            logs::sendError(file.errorString().toStdString().c_str());
        }
        
        break;
    }
    case MAKE_FOOD:
    {
        QFile file(path);
        if (file.open(QFile::WriteOnly))        
        {
            QJsonObject obj;
            
            obj["id"] = 6;           
            obj["customCheck"] = JSON_NULL;
            obj["energy"] = food.energy_numecric->value();
            obj["model"] = food.model_input->text();
            obj["name"] = food.name_input->text();
            obj["price"] = food.price_numeric->value();
            obj["requiredCook"] = food.requiresCook_check->isChecked();
            
            QJsonDocument doc;
            doc.setObject(obj);            
            
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
            
            logs::sendInfo("Save food succesful.");
        }
        else
        {
            logs::sendError(file.errorString().toStdString().c_str());
        }
        
        break;
    }
    case MAKE_GROUPCHATS:
    {
        QFile file(path);
        if (file.open(QFile::WriteOnly))        
        {
            QJsonObject obj;
            
            obj["id"] = 7;           
            obj["customCheck"] = JSON_NULL;
            
            QJsonArray arr;
            
            for (int i{}; i < groupchats.members_list->count(); ++i)
            {
                auto* item = groupchats.members_list->item(i);
                arr.append(item->text());
            }
            
            obj["members"] = arr;
            
            QJsonDocument doc;
            doc.setObject(obj);            
            
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
            
            logs::sendInfo("Save groupchats succesful.");
        }
        else
        {
            logs::sendError(file.errorString().toStdString().c_str());
        }
        
        break;
    }
    case MAKE_JOBS:
    {
        QFile file(path);
        if (file.open(QFile::WriteOnly))        
        {
            QJsonObject obj;
            
            obj["id"] = 8;           
            obj["admin"] = jobs.admin_combo->currentIndex();
            obj["canDemote"] = jobs.canDemote_check->isChecked();
            obj["category"] = jobs.category_input->text();
            obj["codeName"] = jobs.codeName_input->text();
            obj["color"] = jobs.color_button->text();
            obj["command"] = jobs.command_input->text();
            obj["customCheck"] = JSON_NULL;
            obj["description"] = jobs.description->toPlainText();
            obj["hasLicense"] = jobs.hasLicense_check->isChecked();
            obj["max"] = jobs.max_numeric->value();
            
            QJsonArray models;
            
            for (int i{}; i < jobs.models_list->count(); ++i)
            {
                auto* item = jobs.models_list->item(i);
                models.append(item->text());
            }
            
            obj["models"] = models;
            obj["name"] = jobs.name_input->text();
            obj["salary"] = jobs.salary_numeric->value();
            obj["vote"] = jobs.vote_check->isChecked();
            
            QJsonArray weapons;
            
            for (int i{}; i < jobs.weapons_list->count(); ++i)
            {
                auto* item = jobs.weapons_list->item(i);
                weapons.append(item->text());
            }
            
            obj["weapons"] = weapons;
            
            QJsonDocument doc;
            doc.setObject(obj);            
            
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
            
            logs::sendInfo("Save jobs succesful.");
        }
        else
        {
            logs::sendError(file.errorString().toStdString().c_str());
        }
        
        break;
    }
    case MAKE_SHIPMENTS:
    {
        QFile file(path);
        if (file.open(QFile::WriteOnly))        
        {
            QJsonObject obj;
            
            obj["id"] = 9;           
            obj["amount"] = shipments.amount_numeric->value();
            obj["category"] = shipments.category_input->text();
            obj["entity"] = shipments.entity_input->text();
            
            QJsonArray arr;
            
            for (int i{}; i < shipments.members_list->count(); ++i)
            {
                auto* item = shipments.members_list->item(i);
                arr.append(item->text());
            }
            
            obj["members"] = arr;
            obj["model"] = shipments.model_input->text();
            obj["name"] = shipments.name_input->text();
            obj["noShip"] = shipments.noShip_check->isChecked();
            obj["price"] = shipments.price_numeric->value();
            obj["priceSep"] = shipments.priceSep_numeric->value();
            obj["separate"] = shipments.separate_check->isChecked();
            
            QJsonDocument doc;
            doc.setObject(obj);            
            
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
            
            logs::sendInfo("Save shipments succesful.");
        }
        else
        {
            logs::sendError(file.errorString().toStdString().c_str());
        }
        
        break;
    }
    case MAKE_VEHICLES:
    {
        QFile file(path);
        if (file.open(QFile::WriteOnly))        
        {
            QJsonObject obj;
            
            obj["id"] = 10;           
            obj["angle"] = vehicles.angle_input->text();
            obj["customCheck"] = JSON_NULL;
            obj["distance"] = vehicles.distance_numeric->value();
            obj["label"] = vehicles.label_input->text();
            
            QJsonArray arr;
            
            for (int i{}; i < vehicles.members_list->count(); ++i)
            {
                auto* item = vehicles.members_list->item(i);
                arr.append(item->text());
            }
            
            obj["members"] = arr;
            obj["model"] = vehicles.model_input->text();
            obj["name"] = vehicles.name_input->text();
            obj["onBought"] = JSON_NULL;
            obj["price"] = vehicles.price_numeric->value();
            
            QJsonDocument doc;
            doc.setObject(obj);
            
            file.write(doc.toJson(QJsonDocument::Indented));
            file.close();
            
            logs::sendInfo("Save vehicles succesful.");
        }
        else
        {
            logs::sendError(file.errorString().toStdString().c_str());
        }
        
        break;
    }
    case MAKE_UNKNOWN:
    default:
        break;
    }
}


void Client::on_actionSave_all_triggered()
{
    QString path = QFileDialog::getSaveFileName(this, "Save name", QDir::homePath());
    
    if (path.isEmpty())
    {
        logs::sendWarning("Save path is empty.");
        return;
    }
    
    if (path.endsWith(".json") == false)
        path.append(".json");
    
    QFile file(path);
    if (file.open(QFile::WriteOnly))
    {
        QJsonObject mainObject;
        mainObject["id"] = JSON_NULL;
        
        QJsonArray objects;
        
        /// START ///
        
        //agendas
        {
            QJsonObject obj;
            QJsonArray listeners;
            
            for (int i{}; i < agendas.listeners_list->count(); ++i)
            {
                auto* item = agendas.listeners_list->item(i);
                listeners.append(item->text());
            }            
            
            obj["id"] = 0;
            obj["listeners"] = listeners;            
            obj["manager"] = agendas.manager_input->text();
            obj["title"] = agendas.title_input->text();
            
            objects.append(obj);
        }
        
        //ammo
        {
            QJsonObject obj;
            
            obj["id"] = 1;
            obj["ammoType"] = ammo.ammoType_input->text();
            obj["amountGiven"] = ammo.amountGiven_input->value();
            obj["customCheck"] = JSON_NULL;  //null
            obj["model"] = ammo.model_input->text();
            obj["name"] = ammo.name_input->text();
            obj["price"] = ammo.price_input->value();
            
            objects.append(obj);
        }
        
        //categories
        {
            QJsonObject obj;
            
            obj["id"] = 2;
            obj["category"] = categories.categories_combo->currentIndex();
            obj["color"] = categories.color_button->text();
            obj["name"] = categories.name_input->text();
            obj["sortOrder"] = categories.sortOrder_numeric->value();
            obj["startExpanded"] = categories.startExpanded_check->isChecked();
            
            objects.append(obj);
        }
        
        //demotegroups
        {
            QJsonObject obj;
            
            obj["id"] = 3;
            obj["name"] = demotegroups.name_input->text();
            
            QJsonArray arr;
            
            for (int i{}; i < demotegroups.members_list->count(); ++i)
            {
                auto* item = demotegroups.members_list->item(i);
                arr.append(item->text());
            }
            
            obj["members"] = arr;
            
            objects.append(obj);
        }
        
        //doorgroups
        {
            QJsonObject obj;
            
            obj["id"] = 4;
            obj["name"] = doorgroups.name_input->text();
            
            QJsonArray arr;
            
            for (int i{}; i < doorgroups.members_list->count(); ++i)
            {
                auto* item = doorgroups.members_list->item(i);
                arr.append(item->text());
            }
            
            obj["members"] = arr;
            
            objects.append(obj);
        }
        
        //entities
        {
            QJsonObject obj;
            
            obj["id"] = 5;
            
            QJsonArray arr;
            
            for (int i{}; i < entities.allowed_list->count(); ++i)
            {
                auto* item = entities.allowed_list->item(i);
                arr.append(item->text());
            }
            
            obj["allowed"] = arr;
            obj["allowTools"] = entities.allowTools_check->isChecked();
            obj["cmd"] = entities.cmd_input->text();
            obj["entity"] = entities.entity_input->text();
            obj["max"] = entities.max_numeric->value();
            obj["model"] = entities.model_input->text();
            obj["name"] = entities.name_input->text();
            obj["price"] = entities.price_numeric->value();
            
            objects.append(obj);
        }
        
        //food
        {
            QJsonObject obj;
            
            obj["id"] = 6;           
            obj["customCheck"] = JSON_NULL;
            obj["energy"] = food.energy_numecric->value();
            obj["model"] = food.model_input->text();
            obj["name"] = food.name_input->text();
            obj["price"] = food.price_numeric->value();
            obj["requiredCook"] = food.requiresCook_check->isChecked();
            
            objects.append(obj);
        }
        
        //groupchats
        {
            QJsonObject obj;
            
            obj["id"] = 7;           
            obj["customCheck"] = JSON_NULL;
            
            QJsonArray arr;
            
            for (int i{}; i < groupchats.members_list->count(); ++i)
            {
                auto* item = groupchats.members_list->item(i);
                arr.append(item->text());
            }
            
            obj["members"] = arr;
            
            objects.append(obj);
        }
        
        //jobs
        {
            QJsonObject obj;
            
            obj["id"] = 8;           
            obj["admin"] = jobs.admin_combo->currentIndex();
            obj["canDemote"] = jobs.canDemote_check->isChecked();
            obj["category"] = jobs.category_input->text();
            obj["codeName"] = jobs.codeName_input->text();
            obj["color"] = jobs.color_button->text();
            obj["command"] = jobs.command_input->text();
            obj["customCheck"] = JSON_NULL;
            obj["description"] = jobs.description->toPlainText();
            obj["hasLicense"] = jobs.hasLicense_check->isChecked();
            obj["max"] = jobs.max_numeric->value();
            
            QJsonArray models;
            
            for (int i{}; i < jobs.models_list->count(); ++i)
            {
                auto* item = jobs.models_list->item(i);
                models.append(item->text());
            }
            
            obj["models"] = models;
            obj["name"] = jobs.name_input->text();
            obj["salary"] = jobs.salary_numeric->value();
            obj["vote"] = jobs.vote_check->isChecked();
            
            QJsonArray weapons;
            
            for (int i{}; i < jobs.weapons_list->count(); ++i)
            {
                auto* item = jobs.weapons_list->item(i);
                weapons.append(item->text());
            }
            
            obj["weapons"] = weapons;
            
            objects.append(obj);
        }
        
        //shipments
        {
            QJsonObject obj;
            
            obj["id"] = 9;           
            obj["amount"] = shipments.amount_numeric->value();
            obj["category"] = shipments.category_input->text();
            obj["entity"] = shipments.entity_input->text();
            
            QJsonArray arr;
            
            for (int i{}; i < shipments.members_list->count(); ++i)
            {
                auto* item = shipments.members_list->item(i);
                arr.append(item->text());
            }
            
            obj["members"] = arr;
            obj["model"] = shipments.model_input->text();
            obj["name"] = shipments.name_input->text();
            obj["noShip"] = shipments.noShip_check->isChecked();
            obj["price"] = shipments.price_numeric->value();
            obj["priceSep"] = shipments.priceSep_numeric->value();
            obj["separate"] = shipments.separate_check->isChecked();
            
            objects.append(obj);
        }
        
        //vehicles
        {
            QJsonObject obj;
            
            obj["id"] = 10;           
            obj["angle"] = vehicles.angle_input->text();
            obj["customCheck"] = JSON_NULL;
            obj["distance"] = vehicles.distance_numeric->value();
            obj["label"] = vehicles.label_input->text();
            
            QJsonArray arr;
            
            for (int i{}; i < vehicles.members_list->count(); ++i)
            {
                auto* item = vehicles.members_list->item(i);
                arr.append(item->text());
            }
            
            obj["members"] = arr;
            obj["model"] = vehicles.model_input->text();
            obj["name"] = vehicles.name_input->text();
            obj["onBought"] = JSON_NULL;
            obj["price"] = vehicles.price_numeric->value();
            
            objects.append(obj);
        }
        
        /// END ///
        
        mainObject["objects"] = objects;
        
        QJsonDocument doc;
        doc.setObject(mainObject);
        
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        
        logs::sendInfo("Save all successful.");
    }
    else
    {
        logs::sendError(file.errorString().toStdString().c_str());
    }
}


void Client::on_actionOpen_triggered()
{
    QString path = QFileDialog::getOpenFileName(this, "Open file", QDir::homePath(), "JSON (*.json)");
    
    if (path.isEmpty()) return;
    
    QFile file(path);
    if (file.open(QFile::ReadOnly))
    {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject mainObject = doc.object();
        
        QJsonValue type = mainObject.value("id");
        
        // only one tab
        if (type != JSON_NULL)
        {
            const int id = type.toInt();
            
            /// 0 agendas
            /// 1 ammo
            /// 2 categories
            /// 3 demotegroups
            /// 4 doorgroups
            /// 5 entities
            /// 6 food
            /// 7 groupchats
            /// 8 jobs
            /// 9 shipments
            /// 10 vehicles
            
            switch(id)
            {
            case 0:
            {
                QString title = mainObject.value("title").toString();
                QString manager = mainObject.value("manager").toString();
                
                QJsonArray arr = mainObject.value("listeners").toArray();
                
                agendas.title_input->setText(title);
                agendas.manager_input->setText(manager);
                
                for (auto i : arr)
                {
                    agendas.listeners_list->addItem(i.toString());
                }
                
                break;
            }
            case 1:
            {
                QString ammoType = mainObject.value("ammoType").toString();
                int amountGiven = mainObject.value("amountGiven").toInt();
                QString model = mainObject.value("model").toString();
                QString name = mainObject.value("name").toString();
                int price = mainObject.value("price").toInt();
                
                ammo.ammoType_input->setText(ammoType);
                ammo.amountGiven_input->setValue(amountGiven);
                ammo.model_input->setText(model);
                ammo.name_input->setText(name);
                ammo.price_input->setValue(price);
                
                break;
            }
            case 2:
            {
                int category = mainObject.value("category").toInt();
                QString color = mainObject.value("color").toString();
                QString name = mainObject.value("name").toString();
                int sortOrder = mainObject.value("sortOrder").toInt();
                bool startExpanded = mainObject.value("startExpanded").toBool();
                
                categories.categories_combo->setCurrentIndex(category);
                categories.color_button->setText(color);
                categories.name_input->setText(name);
                categories.sortOrder_numeric->setValue(sortOrder);
                categories.startExpanded_check->setChecked(startExpanded);
                
                ui->tabWidget->setCurrentIndex(id);
                
                break;
            }
            default:
                logs::sendError("Open file is failure. Error save ID.");
                break;
            }
        }
        //all tabs
        else
        {
            
        }
        
        file.close();
        
        logs::sendInfo("Open successful.");
    }
    else
    {
        logs::sendError(file.errorString().toStdString().c_str());
    }
}

