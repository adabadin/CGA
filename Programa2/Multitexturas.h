#ifndef __MULTITEXTURAS_H__
#define __MULTITEXTURAS_H__

#include "main.h"

// These are for our multitexture defines
#define GL_TEXTURE0_ARB                     0x84C0
#define GL_TEXTURE1_ARB                     0x84C1
#define GL_TEXTURE2_ARB						0x84C2
#define GL_TEXTURE3_ARB						0x84C3
#define GL_TEXTURE4_ARB						0x84C4
#define GL_TEXTURE5_ARB						0x84C5
#define GL_TEXTURE6_ARB						0x84C6

#define GL_COMBINE_ARB						0x8570
#define GL_RGB_SCALE_ARB					0x8573

// Here are the multitexture function prototypes
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FARBPROC) (GLenum target, GLfloat s, GLfloat t);
typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum target);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREPROC) (GLenum target);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum target);
	
// Here we extern our function pointers for multitexturing capabilities
extern PFNGLMULTITEXCOORD2FARBPROC			glMultiTexCoord2fARB;
extern PFNGLACTIVETEXTUREARBPROC			glActiveTextureARB;
extern PFNGLCLIENTACTIVETEXTUREPROC			glClientActiveTexture;
extern PFNGLCLIENTACTIVETEXTUREARBPROC		glClientActiveTextureARB;

class CMultitexturas
{
	public:
		CMultitexturas();
		~CMultitexturas(); 
		
		void InitMultitext(HWND hWnd);

	private:
		
};

#endif

