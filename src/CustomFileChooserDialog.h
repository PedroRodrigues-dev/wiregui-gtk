#ifndef CUSTOMFILECHOOSERDIALOG_H
#define CUSTOMFILECHOOSERDIALOG_H

#include <gtkmm.h>

class CustomFileChooserDialog : public Gtk::FileChooserDialog {
public:
    CustomFileChooserDialog(const Glib::ustring &title, Gtk::Window &parent, std::string wireguard_dir);
};

#endif