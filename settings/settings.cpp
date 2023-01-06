#include "settings.h"
#include "ui_settings.h"
#include <QDir>
#include <QFileDialog>
#include <QSettings>

namespace settings
{
    Settings* settings = nullptr;
    
    void close()
    {
        settings = nullptr;
    }
    
    void show(QWidget* parent)
    {
        if (settings == nullptr)
        {
            settings = new Settings(parent);
            QObject::connect(settings, &Settings::signal_close, close);
        }
        else
        {
            settings->activateWindow();
        }
    }
    
    bool isNull()
    {
        return settings == nullptr;
    }
    
    QString outputPath;
    QString preambule;
    bool enable_preambule;
    bool enable_logs;
    int theme;
    
    void save()
    {
        QSettings globalSettings;
        globalSettings.setValue("outputPath", outputPath);
        globalSettings.setValue("preambule", preambule);
        globalSettings.setValue("enable_preambule", enable_preambule);
        globalSettings.setValue("enable_logs", enable_logs);
        globalSettings.setValue("theme", theme);
    }
    
    void load()
    {
        QSettings globalSettings;
        outputPath = globalSettings.value("outputPath", QDir::currentPath()).toString();
        preambule = globalSettings.value("preambule", "-- Made with DarkRP Mod Maker\n-- Discord: https://discord.gg/fhbgxJQUrC").toString();
        enable_preambule = globalSettings.value("enable_preambule", true).toBool();
        enable_logs = globalSettings.value("enable_logs", true).toBool();
        theme = globalSettings.value("theme", 0).toInt();
    }
}

Settings::Settings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->line_output->setText(settings::outputPath);
    ui->preambule->setPlainText(settings::preambule);
    ui->check_preambule->setChecked(settings::enable_preambule);
    ui->check_enableLogs->setChecked(settings::enable_logs);
    ui->preambule->setDisabled(!settings::enable_preambule);
    show();
}

Settings::~Settings()
{
    delete ui;
}

void Settings::closeEvent(QCloseEvent *)
{
    emit signal_close();
    delete this;
}

void Settings::on_check_preambule_stateChanged(int arg1)
{
    ui->preambule->setDisabled(!arg1);
}


void Settings::on_btn_cancel_clicked()
{
    this->close();
}


void Settings::on_btn_apply_clicked()
{
    if (ui->line_output->text().isEmpty() == false)
        settings::outputPath = ui->line_output->text();
    settings::enable_preambule = ui->check_preambule->isChecked();
    settings::enable_logs = ui->check_enableLogs->isChecked();
    settings::theme = ui->combo_theme->currentIndex();
    if (settings::enable_preambule) settings::preambule = ui->preambule->toPlainText();
    settings::save();
}


void Settings::on_btn_output_clicked()
{
    ui->line_output->setText(QFileDialog::getExistingDirectory());
}

