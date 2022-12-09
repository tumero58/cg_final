#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <vector>
#include <GL/glew.h>

class FrameBuffer
{
  public:
	FrameBuffer(int width, int height);
	void bindTexture(int slot);
	void Bind(int width, int height);
	void Unbind();

  private:
	unsigned int textureID;
	unsigned int depthID;
	unsigned int bufferID;
	int initTexture(int width, int height);
	int initDepth(int width, int height);
	int initRender(int width, int height);
};
#endif