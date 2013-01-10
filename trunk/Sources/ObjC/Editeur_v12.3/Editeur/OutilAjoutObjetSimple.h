//
//  OutilAjoutObjetSimple.h
//  Editeur
//
//  Created by L Tremblay on 12-05-07.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#import "Outil.h"

@interface OutilAjoutObjetSimple : Outil
{
    NSString *sousType;
}

- (void) definirSousType:(NSString *) unSousType;

@end
