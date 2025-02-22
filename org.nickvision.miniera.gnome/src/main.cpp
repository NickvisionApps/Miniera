#include "application.h"

using namespace Nickvision::Miniera::GNOME;

int main(int argc, char* argv[])
{
    Application app{ argc, argv };
    return app.run();
}