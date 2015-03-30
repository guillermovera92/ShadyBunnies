#ifndef config_h
#define config_h

#include<G3D/G3DAll.h>

const G3D::String DATA_DIR="/Users/bjackson/classes/comp394-s15/assignments/asgn4-ShadyBunnies/ShadyBunniesStudent/data/";
//const G3D::String DATA_DIR="C:\\Users\\bjackson\\Documents\\classes\\comp394-s15\\assignments\\asgn4-ShadyBunnies\\ShadyBunniesStudent\\data\\";

const G3D::String FRAGMENT_SHADER = DATA_DIR + "frag.pix";
const G3D::String VERTEX_SHADER = DATA_DIR + "vert.vrt";
const G3D::String BUNNY_FILE = DATA_DIR + "bunny.obj";
const G3D::String DIFFUSE_RAMP = DATA_DIR + "lightingToon.jpg";
const G3D::String SPECULAR_RAMP = DATA_DIR + "lightingNormal.jpg";


#endif
