namespace meta
{
    const char* name = "DarkRP Mod Maker";
    const char* version = "v1.3";
    const char* author = "Amalgama";
}

namespace modules
{
    namespace pm
    {
        void initPM();
        void close();
    }
}

namespace settings
{
    void load();
}

void main_begin()
{
    settings::load();
    modules::pm::initPM();
}

void main_end()
{
    modules::pm::close();
}
