#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <gtkmm.h>
#include "VPNManager.h"
#include "CustomFileChooserDialog.h"

namespace fs = std::filesystem;

VPNManager::VPNManager()
    : wireguard_dir("/etc/wireguard"), vpn_name("Selecione uma VPN") {}

void VPNManager::add_config_file_dialog() {
    Gtk::Window window;
    CustomFileChooserDialog dialog("Selecione um arquivo", window, wireguard_dir);

    dialog.run();
}

void VPNManager::activate_vpn() {
    if (vpn_name != "Selecione uma VPN") {
        std::string command = "sudo wg-quick up " + vpn_name;
        system(command.c_str());
    }
}

void VPNManager::deactivate_vpn() {
    for (std::string vpn : vpn_list) {
        std::string command = "sudo wg-quick down " + vpn;
        system(command.c_str());
    }
}

void VPNManager::vpn_status() {
    std::string temp_filename = "/tmp/wiregui-gtk.temp";

    std::string full_command = "sudo wg show > " + temp_filename;

    std::system(full_command.c_str());

    std::ifstream file(temp_filename);
    std::string output((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    std::remove(temp_filename.c_str());

    vpn_status_now = output;
}

std::string VPNManager::remove_extension(const std::string& filename) {
    std::string extension = ".conf";
    if (filename.size() >= extension.size() && filename.substr(filename.size() - extension.size()) == extension) {
        return filename.substr(0, filename.size() - extension.size());
    }
    return filename;
}

void VPNManager::update_vpn_list() {
    vpn_list.clear();

    try {
        for (const auto& entry : fs::directory_iterator(wireguard_dir)) {
            vpn_list.push_back(remove_extension(entry.path().filename()));
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro ao listar vpns: " << e.what() << std::endl;
    }
}

bool VPNManager::vpn_list_is_changed() {
    std::vector<std::string> vpn_list_now;

    try {
        for (const auto& entry : fs::directory_iterator(wireguard_dir)) {
            vpn_list_now.push_back(remove_extension(entry.path().filename()));
        }
    } catch (const std::exception& e) {
        std::cerr << "Erro ao listar vpns: " << e.what() << std::endl;
    }

    return vpn_list_now != vpn_list;
}

void VPNManager::run(int argc, char* argv[]) {
    if (geteuid() != 0) {
        std::string command = "sudo ";
        for (int i = 0; i < argc; ++i) {
            command += argv[i];
            command += ' ';
        }
        std::exit(system(command.c_str()));
    }

    auto app = Gtk::Application::create(argc, argv);

    Gtk::Window window;
    window.set_default_size(500, 350);
    window.set_resizable(false);

    Gtk::Button add_config_button("Adicionar configuração");
    add_config_button.signal_clicked().connect(sigc::mem_fun(*this, &VPNManager::add_config_file_dialog));
    add_config_button.set_margin_top(20);

    Gtk::ComboBoxText vpn_selector;
    vpn_selector.append("Selecione uma VPN");
    vpn_selector.set_active(0);
    update_vpn_list();
    for (std::string vpn : vpn_list) {
        vpn_selector.append(vpn);
    }

    vpn_selector.signal_changed().connect([&]() {
        vpn_name = remove_extension(vpn_selector.get_active_text());
    });

    Gtk::Button activate_vpn_button("Ativar");
    activate_vpn_button.signal_clicked().connect(sigc::mem_fun(*this, &VPNManager::activate_vpn));
    activate_vpn_button.set_visible(true);

    Gtk::Button deactivate_vpn_button("Desativar");
    deactivate_vpn_button.signal_clicked().connect(sigc::mem_fun(*this, &VPNManager::deactivate_vpn));
    deactivate_vpn_button.set_visible(false);

    Gtk::Label vpn_status_label("");

    Gtk::Grid grid;
    grid.set_row_spacing(10);
    grid.set_column_spacing(10);
    grid.attach(add_config_button, 0, 0, 1, 1);
    grid.attach(vpn_selector, 0, 1, 1, 1);
    grid.attach(activate_vpn_button, 0, 2, 1, 1);
    grid.attach(deactivate_vpn_button, 0, 2, 1, 1);
    grid.attach(vpn_status_label, 0, 3, 1, 1);

    Gtk::Alignment alignment(0.5, 0, 0, 0);
    alignment.add(grid);

    window.add(alignment);
    window.show_all();

    auto application_loop = [&]() {
        vpn_status();

        if (vpn_status_now != "") {
            vpn_selector.set_visible(false);
            activate_vpn_button.set_visible(false);
            deactivate_vpn_button.set_visible(true);
            vpn_status_label.set_visible(true);
            vpn_status_label.set_label(vpn_status_now);
        } else {
            if (vpn_list_is_changed()) {
                vpn_selector.remove_all();
                vpn_selector.append("Selecione uma VPN");
                vpn_selector.set_active(0);
                update_vpn_list();
                for (std::string vpn : vpn_list) {
                    vpn_selector.append(vpn);
                }
            }

            vpn_status_label.set_visible(false);
            vpn_selector.set_visible(true);
            activate_vpn_button.set_visible(true);
            deactivate_vpn_button.set_visible(false);
            vpn_status_label.set_visible(false);
            vpn_status_label.set_label("");
        }

        return true;
    };

    Glib::signal_timeout().connect(application_loop, 100);

    app->run(window);
}
