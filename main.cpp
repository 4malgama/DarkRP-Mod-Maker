#include <QApplication>

namespace client
{
    void show();
    void init();
}

namespace meta
{
    extern const char* name;
    extern const char* version;
    extern const char* author;
}

void main_begin();
void main_end();

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
    QCoreApplication::setOrganizationName(meta::author);
    QCoreApplication::setApplicationName(meta::name);
    
    main_begin();
    
    client::init();
    client::show();
    
    a.exec();
    
    main_end();
    
    return 0;
}
