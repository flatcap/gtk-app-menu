/* g++ menu.cc `pkg-config gtkmm-3.0 --cflags --libs` -o menu */

#include <gtkmm.h>

class App : public Gtk::Application {
public:
	App() : Gtk::Application("org.test.menu", Gio::APPLICATION_FLAGS_NONE), win(NULL) {}

protected:
	virtual void on_startup() {
		Gtk::Application::on_startup();

		Glib::RefPtr<Gio::SimpleAction> action = Gio::SimpleAction::create("hide");
		action->signal_activate().connect(sigc::mem_fun(*this, &App::on_hide));
		add_action(action);

		Glib::RefPtr<Gio::Menu> menu = Gio::Menu::create();
		menu->append("Hide", "app.hide");
		set_app_menu(menu);
	}

	virtual void on_activate() {
		if (!win) {
			win = new Gtk::ApplicationWindow();
			add_window(*win);
			win->signal_delete_event().connect(sigc::mem_fun(*this, &App::on_delete));
			hold();
		}
		win->show_all();
		win->set_show_menubar(true);
	}

private:
	void on_hide(const Glib::VariantBase &) {
		win->set_show_menubar(false);
		win->hide();
	}

	bool on_delete(GdkEventAny* event) {
		win->set_show_menubar(false);
		release();
		return false;
	}

	Gtk::ApplicationWindow *win;
};

int main(int argc, char **argv) {
	return App().run(argc, argv);
}
