//
//  Triangle.m
//  Editeur
//
//  Created by L Tremblay on 12-05-08.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Triangle.h"

int nbPointsTriangle = 3;
float pointsTriangle[9] = {-50.0, -50.0, 0.0,
                            0.0,  50.0, 0.0,
                           50.0, -50.0, 0.0};

@implementation Triangle

-(id) init
{
    self = [super init];
    if (self)
    {
        nbPointsModele = nbPointsTriangle;
        modele = &pointsTriangle[0];
    }
    return self;
}

-(void) exporterEnXML:(NSXMLElement *)parent
{
    NSXMLElement *unElement = [[NSXMLElement alloc] initWithName:NSStringFromClass([self class])];
    [unElement addAttribute:[NSXMLNode attributeWithName:@"Position_x" stringValue:[[NSNumber numberWithFloat:self.position.x] stringValue]]];
    [unElement addAttribute:[NSXMLNode attributeWithName:@"Position_y" stringValue:[[NSNumber numberWithFloat:self.position.y] stringValue]]];
    [unElement addAttribute:[NSXMLNode attributeWithName:@"Echelle" stringValue:[[NSNumber numberWithFloat:self.echelle] stringValue]]];
    [unElement addAttribute:[NSXMLNode attributeWithName:@"Angle" stringValue:[[NSNumber numberWithFloat:self.angle] stringValue]]];
    [parent addChild:unElement];    
}

@end
