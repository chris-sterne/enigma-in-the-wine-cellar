## Process this file with automake to produce Makefile.in

SUBDIRS = meshes

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

enigma_in_the_wine_cellar_LDADD = $(ENIGMA_IN_THE_WINE_CELLAR_LIBS)

enigma_in_the_wine_cellar_SOURCES = \
	main.cc \
	Application.cc \
	Application.gresource.cc \
	PlayerView.cc \
	PlayerView.gresource.cc \
	ItemsView.cc \
	ItemsView.gresource.cc \
	WorldsView.cc \
	WorldsView.gresource.cc \
	HelpView.cc \
	HelpView.gresource.cc \
	AboutView.cc \
	AboutView.gresource.cc \
	Object.cc \
	Object.gresource.cc \
	Mesh.cc \
	Mesh.gresource.cc \
	Sound.cc \
	Sound.gresource.cc \
	World.cc \
	Boundary.cc \
	Location.cc \
	FineLocation.cc \
	UserAction.cc \
	Viewer.cc \
	Player.cc \
	Matrix4.cc \
	ObjectList.cc \
	ItemList.cc \
	ItemList.gresource.cc \
	Item.cc \
	PlayerList.cc \
	WineCellar.cc \
	BlockWall.cc \
	StoneWall.cc \
	Ladder.cc \
	LadderEnd.cc \
	Blocker.cc \
	Mover.cc \
	Turner.cc \
	Mushrooms.cc \
	CorkScrew.cc \
	WineBottle.cc \
	Bread.cc \
	Cheese.cc \
	Knife.cc \
	Apple.cc \
	WineGlass.cc \
	PlaceMat.cc \
	Grapes.cc \
	Skull.cc \
	EdgeDrain.cc \
	AirVent.cc

Application.gresource.cc: \
	Application.gresource.xml \
	Application.ui \
	Alert.png
	-glib-compile-resources Application.gresource.xml \
		--target=Application.gresource.cc --generate-source

PlayerView.gresource.cc: \
	PlayerView.gresource.xml \
	PlayerView.ui
	-glib-compile-resources PlayerView.gresource.xml \
		--target=PlayerView.gresource.cc --generate-source

ItemsView.gresource.cc: \
	ItemsView.gresource.xml \
	ItemsView.ui
	-glib-compile-resources ItemsView.gresource.xml \
		--target=ItemsView.gresource.cc --generate-source

WorldsView.gresource.cc: \
	WorldsView.gresource.xml \
	WorldsView.ui
	-glib-compile-resources WorldsView.gresource.xml \
		--target=WorldsView.gresource.cc --generate-source

HelpView.gresource.cc: \
	HelpView.gresource.xml \
	HelpView.ui HelpView.txt
	-glib-compile-resources HelpView.gresource.xml \
		--target=HelpView.gresource.cc --generate-source

AboutView.gresource.cc: \
	AboutView.gresource.xml \
	AboutView.ui \
	AboutView.txt \
	AboutView.png
	-glib-compile-resources AboutView.gresource.xml \
		--target=AboutView.gresource.cc --generate-source

ItemList.gresource.cc: \
	ItemList.gresource.xml \
	ItemList.ui
	-glib-compile-resources ItemList.gresource.xml \
		--target=ItemList.gresource.cc --generate-source

Object.gresource.cc: \
	Object.gresource.xml \
	images/Apple.png \
	images/Bread.png \
	images/Cheese.png \
	images/CorkScrew.png \
	images/Grapes.png \
	images/Knife.png \
	images/Mushrooms.png \
	images/None.png \
	images/PlaceMat.png \
	images/Skull.png \
	images/WineBottle.png \
	images/WineGlass.png
	-glib-compile-resources Object.gresource.xml \
		--target=Object.gresource.cc --generate-source

Mesh.gresource.cc: \
	Mesh.gresource.xml \
	VertexShader.glsl \
	FragmentShader.glsl \
	meshes/WineCellar_A.msh \
	meshes/BlockWall_N.msh \
	meshes/BlockWall_A.msh \
	meshes/StoneWall_A.msh \
	meshes/Ladder_A.msh \
	meshes/LadderEnd_A.msh \
	meshes/Mushrooms_A.msh \
	meshes/CorkScrew_A.msh \
	meshes/WineBottle_A.msh \
	meshes/Bread_A.msh \
	meshes/Cheese_A.msh \
	meshes/Knife_A.msh \
	meshes/Apple_A.msh \
	meshes/WineGlass_A.msh \
	meshes/PlaceMat_A.msh \
	meshes/Grapes_A.msh \
	meshes/Skull_A.msh \
	meshes/EdgeDrain_A.msh \
	meshes/AirVent_A.msh
	-glib-compile-resources Mesh.gresource.xml \
		--target=Mesh.gresource.cc --generate-source
		
Sound.gresource.cc: \
	Sound.gresource.xml \
	sounds/Bump.ogg \
	sounds/ItemFound.ogg \
	sounds/SkullFound.ogg
	-glib-compile-resources Sound.gresource.xml \
		--target=Sound.gresource.cc --generate-source

