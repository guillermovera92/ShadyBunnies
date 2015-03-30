#ifndef App_h
#define App_h

#include <G3D/G3DAll.h>
#include "TurntableManipulator.h"


class App : public GApp {
public:

	App(const GApp::Settings& settings = GApp::Settings());

	virtual void onInit();
	virtual void onSimulation(RealTime rdt, SimTime sdt, SimTime idt);
	virtual void onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& surface);
	virtual void onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& surface2D);

	virtual bool onEvent(const GEvent& e);

protected:

	virtual void reloadShaders();

	double t;

	shared_ptr<Texture> diffuseRamp;
	shared_ptr<Texture> specularRamp;

	shared_ptr<Shader> shader;

	shared_ptr<ArticulatedModel> model;
	shared_ptr<TurntableManipulator> turntable;

	Vector4 lightPosition;
	bool drawLightVector;
	float diffuseOnOff;  // 1.0 when on, 0.0 when off
	float specularOnOff; // 1.0 when on, 0.0 when off
	float ambientOnOff;  // 1.0 when on, 0.0 when off
};

#endif