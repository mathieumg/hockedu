//
//  VueOpenGL.h
//  Editeur
//
//  Created by L Tremblay on 12-02-20.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import <AppKit/AppKit.h>

@class ControleurFenetreEdition;
//@class DonneesEditeur;
@class DonneesFenetreEdition;

@interface VueOpenGL : NSOpenGLView
{
    IBOutlet ControleurFenetreEdition *controleurFenetreEdition;
}

@property(strong, nonatomic) IBOutlet DonneesFenetreEdition *donneesFenetreEdition;

- (void)dessineVue;
- (void)dessineRectangleElastique;
- (void)effectuerSelection;
- (void)mettre_A_JourProjection;
@end
