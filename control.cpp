#include "client.h"
#include "types.h"
#include "ui_client.h"
#include "help_functions.h"
#include <QDateTime>

#include <QDebug>

namespace viewer
{
    void append(void* struct_data);
}

namespace client
{
    extern Client* client;
    extern Ui::Client* ui;
    extern QCompleter *completer;

    namespace control
    {
        QString imagesPath = ":/silk/silk/";
    
        PBUTTON newButton(const QString &_name, const QString &_toolTip = QString(), bool _checkable = false, const QString _icon = QString())
        {
            QIcon _image = QIcon(imagesPath + _icon);
            
            if (_toolTip.isEmpty()) const_cast<QString&>(_toolTip) = _name;
            
            QPushButton* button = new QPushButton(client);
            button->setText(_name);
            button->setCheckable(_checkable);
            if (_icon.isEmpty() == false)
                button->setIcon(_image);
            button->setToolTip(_toolTip);
            button->setCursor(Qt::PointingHandCursor);
            return button;
        }
        
        PTOOL newTool(const char* _icon, const QString &_toolTip, bool _checkable = false)
        {
            QIcon _image = QIcon(imagesPath + _icon);
            
            QToolButton* button = new QToolButton(client);
            button->setIcon(_image);
            button->setIconSize(QSize(16, 16));
            button->setCheckable(_checkable);
            button->setToolTip(_toolTip);
            button->setCursor(Qt::PointingHandCursor);
            return button;
        }
        
        QFrame* newSeparator(const QString &_orientation)
        {            
            if (_orientation.toLower() == "vertical")
            {
                QFrame* line = new QFrame(client);
                line->setFrameShape(QFrame::VLine);
                return line;
            }
            else if (_orientation.toLower() == "horizontal")
            {
                QFrame* line = new QFrame(client);
                line->setFrameShape(QFrame::HLine);
                return line;
            }
            return nullptr;
        }
        
        PINPUT newInput(const QString &_ph, const QString &_txt = QString(), const uint32 &_maxlen = 200, const QString &_mask = QString(), bool _security = false, Qt::Alignment _align = Qt::AlignLeft)
        {
            QLineEdit* input = new QLineEdit(client);
            input->setPlaceholderText(_ph);
            input->setText(_txt);
            input->setEchoMode(_security ? QLineEdit::Password : QLineEdit::Normal);
            input->setAlignment(_align);
            input->setMaxLength(_maxlen);
            input->setClearButtonEnabled(true);
            input->setInputMask(_mask);
            if (completer != nullptr)
            {
                input->setCompleter(completer);
            }
            return input;
        }
        
        PTITLE newTitle(const QString &_txt, Qt::AlignmentFlag _align = Qt::AlignRight)
        {
            QLabel* text = new QLabel(_txt, client);
            return text->setAlignment(_align), text;
        }
        
        PLIST newList(const char* _tooltip = nullptr, QListWidget::Movement _move = QListWidget::Static)
        {
            QListWidget* list = new QListWidget(client);
            list->setMovement(_move);
            list->setMinimumHeight(240);
            if (_tooltip != nullptr)
                list->setToolTip(_tooltip);
            list->setCursor(Qt::CrossCursor);
            return list;
        }
        
        PSPACER newSpacer(const QString &_orientation)
        {
            if (_orientation.toLower() == "vertical")
            {
                return new QSpacerItem(0, 1, QSizePolicy::Minimum, QSizePolicy::Expanding);
            }
            else if (_orientation.toLower() == "horizontal")
            {
                return new QSpacerItem(1, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
            }
            return nullptr;
        }
        
        PCOMBO newCombo(const QStringList &_items, const uint32 &_maxVisible = 0)
        {
            QComboBox* combo = new QComboBox(client);
            combo->addItems(_items);
            combo->setMaxVisibleItems((_maxVisible == 0) ? (_items.size() + 1) : _maxVisible);
            combo->setCursor(Qt::PointingHandCursor);
            return combo;
        }
        
        PCOMBO newCombo(QAbstractListModel *model, const uint32 &_maxVisible = 0)
        {
            QComboBox* combo = new QComboBox(client);
            combo->setModel(model);
            combo->setMaxVisibleItems((_maxVisible == 0) ? (model->rowCount()) : _maxVisible);
            combo->setCursor(Qt::PointingHandCursor);
            return combo;
        }
        
        PCHECK newCheck(const QString &_txt, bool _default, bool _tristate = false)
        {
            QCheckBox* check = new QCheckBox(client);
            check->setChecked(_default);
            check->setText(_txt);
            check->setTristate(_tristate);
            check->setCursor(Qt::PointingHandCursor);
            return check;
        }
        
        PNUMERIC newNumeric(const int32 &_min, const int32 &_max, const uint32 &_step = 1)
        {
            QSpinBox* numeric = new QSpinBox(client);
            numeric->setMinimum(_min);
            numeric->setMaximum(_max);
            numeric->setSingleStep(_step);
            return numeric;
        }
        
        PTEXT newText(const char* _ph = nullptr)
        {
            PTEXT text = new TEXT(client);
            if (_ph != nullptr)
                text->setPlaceholderText(_ph);
            return text;
        }
                
        PHBOX newHBox(QWidget* _parent)
        {
            PHBOX box = new HBOX(_parent);
            return box;
        }
        
        _struct_end* newStructEnd(QGridLayout* grid)
        {
            _struct_end* end = new _struct_end(client);
            
            end->push = newTool("add.png", "Push");
            end->clear = newTool("delete.png", "Clear");
            end->spacer = newSpacer("horizontal");
            end->end = newHBox(grid->widget());
            
            end->end->addSpacerItem(end->spacer);
            end->end->addWidget(end->clear);
            end->end->addWidget(end->push);
            
            grid->addLayout(end->end, 0, 1);
            
            QObject::connect(end->push, &TOOL::clicked, [] {
                QString title = ui->tabWidget->tabText(ui->tabWidget->currentIndex());
                if (client->push(title) == true)
                {
                    struct
                    {
                        QString make;
                        QString name;
                        QString time;
                    } Arguments = { client->keyValue(title), client->nameValue(title), QDateTime::currentDateTime().toString("hh:mm / dd.MM") };
                    viewer::append(&Arguments);
                    client->clear(title);
                }
            });
            
            QObject::connect(end->clear, &TOOL::clicked, [] {
                client->clear(ui->tabWidget->tabText(ui->tabWidget->currentIndex()));
            });

            return end;
        }
                
        template<typename T>
        void changeIcon(T* _obj, const char* _icon)
        {
            _obj->setIcon(QIcon(imagesPath + _icon));
        }
        
        void changeTabIcon(QWidget* _tab, const char* _icon)
        {
            if (ui == nullptr) return;
            
            if (_tab == nullptr) return;
            
            int _i = ui->tabWidget->indexOf(_tab);
            ui->tabWidget->setTabIcon(_i, QIcon(imagesPath + _icon));
        }            
    }
}
