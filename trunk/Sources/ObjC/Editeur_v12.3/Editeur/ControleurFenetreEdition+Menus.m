//
//  ControleurFenetreEdition+Menus.m
//  Editeur
//
//  Created by L Tremblay on 12-04-20.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "ControleurFenetreEdition+Menus.h"
#import "DonneesFenetreEdition.h"

@implementation ControleurFenetreEdition (Menus)


// À modifier ici si jamais on veut vérifier si des éléments de menu sont légaux
// ou non.
- (BOOL) validateMenuItem:(NSMenuItem *)menuItem
{
    return YES;
}

- (void) newDocument:(id)sender
{
   [[NSApp delegate] creerFenetreEdition];
}

- (void) openDocument:(id)sender
{
    [donneesFenetreEdition importerEnXML];
}

- (void) saveDocument:(id)sender
{
    [donneesFenetreEdition exporterEnXML];
}



@end
