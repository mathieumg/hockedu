//
//  NoeudComposite.h
//  Editeur
//
//  Created by L Tremblay on 12-02-29.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Noeud.h"

@interface NoeudComposite : Noeud
{
@protected
    NSMutableArray *enfants;
}

@property (strong) NSMutableArray *enfants;

- (NoeudComposite *) initWithCapacity:(NSUInteger) capacite;
- (void) afficheInformations;

@end
