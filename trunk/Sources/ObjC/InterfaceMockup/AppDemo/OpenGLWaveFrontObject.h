//
//  OpenGLWaveFrontObject.h
//  Wavefront OBJ Loader
//
//  Created by Jeff LaMarche on 12/14/08.
//  Copyright 2008 Jeff LaMarche. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <OpenGLES/EAGL.h>
#import <OpenGLES/ES1/gl.h>
#import <OpenGLES/ES1/glext.h>
#import "OpengLWaveFrontCommon.h"
#import "OpenGLWaveFrontMaterial.h"

#define kGroupIndexVertex			0
#define kGroupIndexTextureCoordIndex	1

struct Extremas {
    Vertex3D xMin;
    Vertex3D xMax;
    Vertex3D yMin;
    Vertex3D yMax;
    Vertex3D zMin;
    Vertex3D zMax;
};

@interface OpenGLWaveFrontObject : NSObject {
	NSString			*sourceObjFilePath;
	NSString			*sourceMtlFilePath;
	
	GLuint				numberOfVertices;
	Vertex3D			*vertices;	
	GLuint				numberOfFaces;			// Total faces in all groups
	
	Vector3D			*surfaceNormals;		// length = numberOfFaces
	Vector3D			*vertexNormals;			// length = numberOfFaces (*3 vertices per triangle);
	
	GLfloat				*textureCoords;
	GLubyte				valuesPerCoord;			// 1, 2, or 3, representing U, UV, or UVW mapping, could be 4 but OBJ doesn't support 4
	
	NSDictionary		*materials;
	NSMutableArray		*groups;
	
    Vertex3D            minCoord;
    Vertex3D            maxCoord;
    Vertex3D            modelScale;
    
	Vertex3D			currentPosition;
	Rotation3D			currentRotation;
}
@property (nonatomic, retain) NSString *sourceObjFilePath;
@property (nonatomic, retain) NSString *sourceMtlFilePath;
@property (nonatomic, retain) NSDictionary *materials;
@property (nonatomic, retain) NSMutableArray *groups;
@property Vertex3D currentPosition;
@property Vertex3D minCoord;
@property Vertex3D maxCoord;
@property Vertex3D modelScale;
@property Rotation3D currentRotation;
- (id)initWithPath:(NSString *)path;
- (void)drawSelf;
- (void)calculateScale:(Vertex3D)expectedSize;
@end
