#ifndef VPNMANAGER_H
#define VPNMANAGER_H

#include <string>
#include <vector>
#include <gtkmm.h>

class VPNManager {
private:
    std::string wireguard_dir;
    std::string vpn_name;
    std::string vpn_status_now;
    std::vector<std::string> vpn_list;

public:
    VPNManager();

    void add_config_file_dialog();
    void activate_vpn();
    void deactivate_vpn();
    void vpn_status();
    std::string remove_extension(const std::string& filename);
    void update_vpn_list();
    bool vpn_list_is_changed();
    void run(int argc, char* argv[]);
};

#endif
