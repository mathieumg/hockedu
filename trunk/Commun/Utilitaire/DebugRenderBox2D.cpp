/*
* Copyright (c) 2006-2007 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#include "DebugRenderBox2D.h"

#include "glew.h"

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include "..\Arbre\HUD\HUDTexte.h"
#include "Utilitaire.h"
using namespace std;

const float DebugZ = 25.f;

void DebugRenderBox2D::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex3f(vertices[i].x, vertices[i].y,DebugZ);
	}
	glEnd();
}

void DebugRenderBox2D::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex3f(vertices[i].x, vertices[i].y,DebugZ);
	}
	glEnd();
	glDisable(GL_BLEND);

	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex3f(vertices[i].x, vertices[i].y,DebugZ);
	}
	glEnd();
}

void DebugRenderBox2D::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex3f(v.x, v.y,DebugZ);
		theta += k_increment;
	}
	glEnd();
}

void DebugRenderBox2D::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex3f(v.x, v.y,DebugZ);
		theta += k_increment;
	}
	glEnd();
	glDisable(GL_BLEND);

	theta = 0.0f;
	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex3f(v.x, v.y,DebugZ);
		theta += k_increment;
	}
	glEnd();

	b2Vec2 p = center + radius * axis;
	glBegin(GL_LINES);
	glVertex3f(center.x, center.y,DebugZ);
	glVertex3f(p.x, p.y,DebugZ);
	glEnd();
}

void DebugRenderBox2D::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex3f(p1.x, p1.y,DebugZ);
	glVertex3f(p2.x, p2.y,DebugZ);
	glEnd();
}

void DebugRenderBox2D::DrawTransform(const b2Transform& xf)
{
	b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.4f;
	glBegin(GL_LINES);
	
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(p1.x, p1.y,DebugZ);
	p2 = p1 + k_axisScale * xf.q.GetXAxis();
	glVertex3f(p2.x, p2.y,DebugZ);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(p1.x, p1.y,DebugZ);
	p2 = p1 + k_axisScale * xf.q.GetYAxis();
	glVertex3f(p2.x, p2.y,DebugZ);

	glEnd();
}

void DebugRenderBox2D::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	glPointSize(size);
	glBegin(GL_POINTS);
	glColor3f(color.r, color.g, color.b);
	glVertex3f(p.x, p.y,DebugZ);
	glEnd();
	glPointSize(1.0f);
}

void DebugRenderBox2D::DrawString(int x, int y, const char *string, ...)
{
    checkf(0, "Not working yet");
    // Not tested yet !!
    static HUDTexte texte("",Vecteur4f(0.9f, 0.6f, 0.6f));
	char buffer[128];

	va_list arg;
	va_start(arg, string);
	vsprintf(buffer, string, arg);
	va_end(arg);

    texte.setMessage(buffer);
    texte.repeindre();

// 
// 	glMatrixMode(GL_PROJECTION);
// 	glPushMatrix();
// 	glLoadIdentity();
// 	int w = glutGet(GLUT_WINDOW_WIDTH);
// 	int h = glutGet(GLUT_WINDOW_HEIGHT);
// 	gluOrtho2D(0, w, h, 0);
// 	glMatrixMode(GL_MODELVIEW);
// 	glPushMatrix();
// 	glLoadIdentity();
// 
// 	glColor3f(0.9f, 0.6f, 0.6f);
// 	glRasterPos2i(x, y);
// 	int32 length = (int32)strlen(buffer);
// 	for (int32 i = 0; i < length; ++i)
// 	{
// 		glutBitmapCharacter(GLUT_BITMAP_8_BY_13, buffer[i]);
// 	}
// 
// 	glPopMatrix();
// 	glMatrixMode(GL_PROJECTION);
// 	glPopMatrix();
// 	glMatrixMode(GL_MODELVIEW);
// 	
}

void DebugRenderBox2D::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	glColor3f(c.r, c.g, c.b);
	glBegin(GL_LINE_LOOP);
	glVertex3f(aabb->lowerBound.x, aabb->lowerBound.y,DebugZ);
	glVertex3f(aabb->upperBound.x, aabb->lowerBound.y,DebugZ);
	glVertex3f(aabb->upperBound.x, aabb->upperBound.y,DebugZ);
	glVertex3f(aabb->lowerBound.x, aabb->upperBound.y,DebugZ);
	glEnd();
}
