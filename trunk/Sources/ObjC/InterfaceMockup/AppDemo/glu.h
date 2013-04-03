//
//  gluLookAt.h
//  Hockedu
//
//  Created by Michael Ferris on 2013-04-02.
//
//

typedef float GLfloat;
typedef int GLint;

void gluLookAt(GLfloat eyex, GLfloat eyey, GLfloat eyez,
               GLfloat centerx, GLfloat centery, GLfloat centerz,
               GLfloat upx, GLfloat upy, GLfloat upz);

void gluPerspective(float fovy, float aspect, float zNear, float zFar);
void gluUnProject (GLfloat winX, GLfloat winY, GLfloat winZ, const GLfloat *model, const GLfloat *proj, const GLint *view, GLfloat* objX, GLfloat* objY, GLfloat* objZ);