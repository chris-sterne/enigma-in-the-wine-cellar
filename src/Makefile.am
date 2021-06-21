## Process this file with automake to produce Makefile.in

SUBDIRS =

AM_CPPFLAGS = \
	-DG_APPLICATION_ID='"org.chris_sterne.enigma_in_the_wine_cellar"' \
	-DG_RESOURCE_PREFIX='"/org/chris_sterne/enigma-in-the-wine-cellar/"' \
	-DE_PACKAGE_VERSION='"$(PACKAGE_VERSION)"' \
	-DPACKAGE_LOCALE_DIR=\""$(localedir)"\" \
	-DPACKAGE_SRC_DIR=\""$(srcdir)"\" \
	-DPACKAGE_DATA_DIR=\""$(pkgdatadir)"\" \
	$(ENIGMA_IN_THE_WINE_CELLAR_CFLAGS)

AM_CFLAGS = -Wall

bin_PROGRAMS = enigma-in-the-wine-cellar

enigma_in_the_wine_cellar_LDFLAGS =

enigma_in_the_wine_cellar_LDADD = $(ENIGMA_IN_THE_WINE_CELLAR_LIBS) -ldl -lGL

enigma_in_the_wine_cellar_SOURCES = \
	main.cpp \
  GLArea.cpp \
	Application.cpp \
	Window.cpp \
  Settings.cpp \
  CellarView.cpp \
	PlayerView.cpp \
  MapsView.cpp \
  SettingsView.cpp \
  AboutView.cpp \
  InventoryView.cpp \
  HelpView.cpp \
	MeshList.cpp \
	ViewCone.cpp \
	Map.cpp \
	MapObjectList.cpp \
  MapTeleporterList.cpp \
	MapItemList.cpp \
	MapPlayerList.cpp \
  ItemDialog.cpp \
  GameDialog.cpp \
  ErrorDialog.cpp \
	PlayRoom.cpp \
  Transition.cpp \
  FinePoint.cpp \
  Sounds.cpp \
	Resources.cpp \
  MapLocation.cpp \
  ScreenInput.cpp \
  MapController.cpp \
  MapControllerList.cpp \
  Connection.cpp \
	EnigmaWC.gresource.cpp
	
EnigmaWC.gresource.cpp: \
	EnigmaWC.gresource.xml
	-glib-compile-resources EnigmaWC.gresource.xml \
		--target=EnigmaWC.gresource.cc --generate-source

