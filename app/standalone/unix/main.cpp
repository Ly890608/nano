#include "nano/nano.h"

int main(int argc, char *argv[])
{
    Nano::Application app;
    app.OnInit();
    app.OnUpdate();
    app.OnRender();
    app.OnDestroy();
    return 0;
}
