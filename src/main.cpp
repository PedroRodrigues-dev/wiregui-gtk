#include <gtkmm.h>
#include "VPNManager.h"

int main(int argc, char* argv[]) {
    VPNManager vpnManager;
    vpnManager.run(argc, argv);

    return 0;
}
