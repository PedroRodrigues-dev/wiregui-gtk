#include <iostream>
#include <filesystem>
#include <unistd.h>
#include <cstdlib>
#include <string>
#include <fstream>
#include <gtkmm.h>
#include "CustomFileChooserDialog.h"

namespace fs = std::filesystem;

CustomFileChooserDialog::CustomFileChooserDialog(const Glib::ustring &title, Gtk::Window &parent, std::string wireguard_dir)
    : Gtk::FileChooserDialog(title, Gtk::FILE_CHOOSER_ACTION_OPEN)
{

    set_transient_for(parent);
    Gtk::Button* button_add = add_button("Adicionar", Gtk::ResponseType::RESPONSE_ACCEPT);

    button_add->signal_clicked().connect([&]() {
        std::string selected_file = get_filename();

        fs::path source_path(selected_file);
        fs::path destination_path = fs::path(wireguard_dir) / source_path.filename();

        try {
            fs::copy_file(source_path, destination_path, fs::copy_options::overwrite_existing);
        } catch (const std::exception& e) {
            std::cerr << "Erro ao adicionar configuração: " << e.what() << std::endl;
        }

        hide();
    });

    auto filter_conf = Gtk::FileFilter::create();
    filter_conf->set_name("Arquivo de Configuração");
    filter_conf->add_pattern("*.conf");
    add_filter(filter_conf);
}
