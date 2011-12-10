## Process this file with automake to produce Makefile.in

AM_CPPFLAGS = \
	-DPACKAGE_LOCALE_DIR=\""$(prefix)/$(DATADIRNAME)/locale"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(datadir)"\" \
	-DPACKAGE_DOC_DIR=\""$(prefix)/doc"\" \
	$(GELIDE_CFLAGS)

AM_CFLAGS =\
	 -Wall\
	 -g

bin_PROGRAMS = gelide

gelide_SOURCES = \
	config.cpp \
	config.hpp \
	gelide.cpp \
	gelide.hpp \
	gelide_i18n.hpp \
	interface_configurable.hpp \
	core/dat_reader.cpp \
	core/dat_reader.hpp \
	core/dat_reader_clrmamepro.cpp \
	core/dat_reader_clrmamepro.hpp \
	core/dat_reader_factory.cpp \
	core/dat_reader_factory.hpp \
	core/dat_reader_logiqxxml.cpp \
	core/dat_reader_logiqxxml.hpp \
	core/dat_reader_mamexml.cpp \
	core/dat_reader_mamexml.hpp \
	core/default_systems.cpp \
	core/emulator.cpp \
	core/emulator.hpp \
	core/game.cpp \
	core/game.hpp \
	core/rom.cpp \
	core/rom.hpp \
	core/system.cpp \
	core/system.hpp \
	core/system_manager.cpp \
	core/system_manager.hpp \
	ui/dialog_about.cpp \
	ui/dialog_about.hpp \
	ui/dialog_changelog.cpp \
	ui/dialog_changelog.hpp \
	ui/dialog_columns.cpp \
	ui/dialog_columns.hpp \
	ui/dialog_emulator_edit.cpp \
	ui/dialog_emulator_edit.hpp \
	ui/dialog_emulator_launcher.cpp \
	ui/dialog_emulator_launcher.hpp \
	ui/dialog_gamelist_generator.cpp \
	ui/dialog_gamelist_generator.hpp \
	ui/dialog_game_properties.cpp \
	ui/dialog_game_properties.hpp \
	ui/dialog_preferences.cpp \
	ui/dialog_preferences.hpp \
	ui/dialog_system_edit.cpp \
	ui/dialog_system_edit.hpp \
	ui/gelide_icons.cpp \
	ui/gelide_icons.hpp \
	ui/gelide_ui.cpp \
	ui/gelide_ui.hpp \
	ui/gelide_ui_actions.cpp \
	ui/info_pannel.cpp \
	ui/info_pannel.hpp \
	ui/model_columns.cpp \
	ui/model_columns.hpp \
	ui/statusbar.cpp \
	ui/statusbar.hpp \
	ui/treeview_emulators.cpp \
	ui/treeview_emulators.hpp \
	ui/treeview_filters.cpp \
	ui/treeview_filters.hpp \
	ui/treeview_games.cpp \
	ui/treeview_games.hpp \
	ui/treeview_systems.cpp \
	ui/treeview_systems.hpp \
	utils/crc32.cpp \
	utils/crc32.hpp \
	utils/process.cpp \
	utils/process.hpp \
	utils/tokenizer.cpp \
	utils/tokenizer.hpp \
	utils/utils.cpp \
	utils/utils.hpp \
	utils/xml_parser.cpp \
	utils/xml_parser.hpp \
	utils/xml_reader.cpp \
	utils/xml_reader.hpp \
	utils/xml_writer.cpp \
	utils/xml_writer.hpp

gelide_LDFLAGS = 

gelide_LDADD = $(GELIDE_LIBS)

# Limpieza de ficheros con "make maintainerclean"
MAINTAINERCLEANFILES = \
	Makefile \
	Makefile.in

