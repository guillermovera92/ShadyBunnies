#include "App.h"
#include "config.h"

App::App(const GApp::Settings& settings) : GApp(settings) {
	renderDevice->setColorClearValue(Color3(0.2, 0.2, 0.2));
	renderDevice->setSwapBuffersAutomatically(true);
	drawLightVector = false;
	ambientOnOff = 1.0;
	diffuseOnOff = 1.0;
	specularOnOff = 1.0;
	t = 0.0;
}


void App::onInit() {
	// Turn on the developer HUD
	createDeveloperHUD();
	debugWindow->setVisible(false);
	developerWindow->setVisible(false);
	developerWindow->cameraControlWindow->setVisible(false);
	showRenderingStats = false;


	// This load shaders from disk, we do it once when the program starts up, but
	// since you don't need to recompile to reload shaders, you can even do this
	// inteactively as you debug your shaders!  Press the R key to reload them
	// while your program is running!
	reloadShaders();

	// This loads the bunny model from a file and initializes G3D's ArticulatedModel class to store it  
	model = ArticulatedModel::fromFile(BUNNY_FILE);

	// This is a simple manipulator for moving the camera around in the scene based on mouse movement
	turntable = shared_ptr<TurntableManipulator>(new TurntableManipulator());
	turntable->setEnabled(true);
	setCameraManipulator(turntable);
	addWidget(turntable);
	turntable->setCenterPosition(Vector3(-0.3,0.8,0));

	//Loading textures
	diffuseRamp = Texture::fromFile(DIFFUSE_RAMP, ImageFormat::AUTO(), Texture::DIM_2D); //refers to lightingToon
	specularRamp = Texture::fromFile(SPECULAR_RAMP, ImageFormat::AUTO(), Texture::DIM_2D); //refers to ligthingNormal

}


bool App::onEvent(const GEvent& e) {
	if (GApp::onEvent(e)) {
		return true;
	}
	// Press R to reload the shaders
	if (e.type == GEventType::KEY_DOWN && e.key.keysym.unicode == 'R') {
		reloadShaders();
		return true;
	}
	// Press L to toggle displaying the lighting direction for debugging purposes
	if (e.type == GEventType::KEY_DOWN && e.key.keysym.unicode == 'L') {
		drawLightVector = !drawLightVector;
		return true;
	}
	// Press S to toggle specular lighting on/off
	if (e.type == GEventType::KEY_DOWN && e.key.keysym.unicode == 'S') {
		if (specularOnOff == 1.0) {
			specularOnOff = 0.0;
		}
		else {
			specularOnOff = 1.0;
		}
		return true;
	}
	// Press D to toggle diffuse lighting on/off
	if (e.type == GEventType::KEY_DOWN && e.key.keysym.unicode == 'D') {
		if (diffuseOnOff == 1.0) {
			diffuseOnOff = 0.0;
		}
		else {
			diffuseOnOff = 1.0;
		}
		return true;
	}
	// Press A to toggle ambient lighting on/off
	if (e.type == GEventType::KEY_DOWN && e.key.keysym.unicode == 'A') {
		if (ambientOnOff == 1.0) {
			ambientOnOff = 0.0;
		}
		else {
			ambientOnOff = 1.0;
		}
		return true;
	}
	return false;
}


void App::onSimulation(RealTime rdt, SimTime sdt, SimTime idt) {
	GApp::onSimulation(rdt, sdt, idt);

	t += rdt;

	// Make the light orbit around the bunny so we can see the lighting change in response to the light position
	float radius = 10.0;
	lightPosition = Vector4(cos(t*0.6)*sin(t*0.5)*radius,
		cos(t*0.3)*sin(t*0.2)*radius,
		cos(t*0.1)*sin(t*0.4)*radius,
		1.0);
}


void App::reloadShaders() {
	shader = Shader::fromFiles(VERTEX_SHADER, FRAGMENT_SHADER);
}


void App::onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& surface3D) {
	rd->clear();

	// Always good practice to start with a pushState
	rd->pushState();

	// Make lighting vary smoothly over the surface
	rd->setShadeMode(RenderDevice::SHADE_SMOOTH);

	// Tell ArticulatedModel to "pose" the model based on the current transformation and then
	// store it as a surface3D.  The surface3D is the datastructure that holds all the triangles
	// etc. that we will actually draw --  Note, there are many other ways to do this, but this 
	// is the easiest approach to load a model from a file and then draw it using G3D10.
	model->pose(surface3D);



	// Properties of the material the bunny is made out of (the "K" terms in the equations discussed in class)
	// These values should make the bunny look like it is made out of a metal, like brass
	Color3 ambientReflectionCoeff = Color3(0.33, 0.22, 0.03);
	Color3 diffuseReflectionCoeff = Color3(0.78, 0.57, 0.11);
	Color3 specularReflectionCoeff = Color3(0.99, 0.94, 0.80);
	float specularExponent = 27.9;

	/* For toon shading, you want all the color to come from the texture, so you can just use a white bunny like this:
	Color3 ambientReflectionCoeff = Color3(1,1,1);
	Color3 diffuseReflectionCoeff = Color3(1,1,1);
	Color3 specularReflectionCoeff = Color3(1,1,1);
	float specularExponent = 50.0;
	*/

	// Properties of the light source (the "I" terms in the equations discussed in class)
	// These values are for a white light so the r,g,b intensities are all the same
	// Note: lightPosition is another important property of the light; it is set inside onSimulation()
	Color3 ambientLightIntensity = Color3(0.3, 0.3, 0.3);
	Color3 diffuseLightIntensity = Color3(0.6, 0.6, 0.6);
	Color3 specularLightIntensity = Color3(1.0, 1.0, 1.0);

	// Multiply these light intensities by the OnOff terms below to turn each commonent on the lighting on/off based on keystrokes
	ambientLightIntensity *= ambientOnOff;
	diffuseLightIntensity *= diffuseOnOff;
	specularLightIntensity *= specularOnOff;

	Args args;
	// TODO: Pass these parameters into your shader programs... in shader programs these are called "uniform variables"
    
    // Passing light intensity parameters to shader
    args.setUniform("ambientLightIntensity", ambientLightIntensity);
    args.setUniform("diffuseLightIntensity", diffuseLightIntensity);
    args.setUniform("specularLightIntensity", specularLightIntensity);

    // Passing material coefficients to shader
    args.setUniform("ambientReflectionCoeff", ambientReflectionCoeff);
    args.setUniform("diffuseReflectionCoeff", diffuseReflectionCoeff);
    args.setUniform("specularReflectionCoeff", specularReflectionCoeff);
    args.setUniform("specularExponent", specularExponent);
    
    //TODO: Set shader light properties (intensity)
    args.setUniform("lightPosition", lightPosition);
    
    // Pass texture to sample from
    args.setUniform("textureSamplerDiffuse", diffuseRamp, Sampler::video());
    args.setUniform("textureSamplerSpecular", specularRamp, Sampler::video());

    Vector3 eyePosWorld = activeCamera()->frame().translation;
    args.setUniform("eyePosWorld", eyePosWorld);

	// This tells OpenGL to draw the actual triangles that make up the bunny.
	// All the vertices and pixels that make up the bunny will
	// be passed through the shader programs, which run on the graphics card hardware.
	// Send model geometry to the graphics card
    CoordinateFrame cframe;
    for (int i = 0; i < surface3D.size(); ++i) {

        // Downcast to UniversalSurface to access its fields
        shared_ptr<UniversalSurface> surface = dynamic_pointer_cast<UniversalSurface>(surface3D[i]);
        if (notNull(surface)) {
            surface->getCoordinateFrame(cframe);
            rd->setObjectToWorldMatrix(cframe);

			// The following call sets the attribute arrays and index stream for the vertex buffer to fill
			// in the g3d_Vertex and g3d_Normal in variables in the shader.
			surface->gpuGeom()->setShaderArgs(args);
			// The previous call is equivalent to doing:
			//args.setAttributeArray("g3d_Vertex", surface->gpuGeom()->vertex);
			//args.setAttributeArray("g3d_Normal", surface->gpuGeom()->normal);
			//args.setIndexArray(surface->gpuGeom()->index);


            // Render the geometry using the shader
            rd->apply(shader, args);
        }
    }


	// Good practice to pop the state here since we just finished doing some complex rendering calls
	rd->popState();


	// For debugging purposes, let's draw a sphere to reprsent the "light bulb" in the scene, that way
	// we can make sure the lighting on the bunny makes sense given the position of the light source.
	Draw::sphere(Sphere(lightPosition.xyz(), 0.1), rd, Color3::yellow(), Color4::clear());

	// Another useful aid for debugging: draw the vector to the light source
	if (drawLightVector) {
		Draw::cylinder(Cylinder(Vector3(-0.3,0.8,0), lightPosition.xyz(),0.01), rd, Color3::yellow(), Color4::clear());
	}

	// Call to make the GApp show the output of debugDraw
	drawDebugShapes();
}


void App::onGraphics2D(RenderDevice* rd, Array<Surface2D::Ref>& posed2D) {
	Surface2D::sortAndRender(rd, posed2D);
}



