#include "ushell_core.h"
#include "uart_access.h"

int main(void)
{
    //uart_setup();
    Microshell::getShellPtr(pluginEntry(), "root")->Run();
    return 0;
}

extern "C" void app_main()
{
    main();
}

