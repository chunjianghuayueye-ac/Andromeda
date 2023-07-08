#include "RenderObject.hpp"

using namespace andromeda::graphics;

void RenderObject::loadTextures(bool generate_minimap,bool release_image)
{
	for(int i=0;i<textures.getLength();++i)
		textures[i].load(i,0,generate_minimap,release_image);
}

void RenderObject::useTextures()
{
	for(int i=0;i<textures.getLength();++i)
		textures[i].use(i);
}
