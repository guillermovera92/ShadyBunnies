#include <G3D/G3DAll.h>
#include <string>
#include "App.h"

#include "config.h"

using namespace std;

G3D_START_AT_MAIN();

int main(int argc, const char* argv[]) {
	(void)argc; (void)argv;
	GApp::Settings settings(argc, argv);

	settings.window.width       = 1024; 
	settings.window.height      = 768;
	settings.window.resizable = true;
	return App(settings).run();
}


