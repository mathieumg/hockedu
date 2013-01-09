//
//  Tableau.m
//  Editeur
//
//  Created by L Tremblay on 12-05-07.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Tableau.h"

@implementation Tableau

-(void) exporterEnXML:(NSXMLElement *)parent
{
    NSXMLElement *unElement = [[NSXMLElement alloc] initWithName:NSStringFromClass([self class])];
    [parent addChild:unElement];        
    [super exporterEnXML:unElement];
}


@end
