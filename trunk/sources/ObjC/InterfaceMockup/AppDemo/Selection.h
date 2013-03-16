//
//  Selection.h
//  Hockedu
//
//  Created by Samuel Ledoux on 2013-03-16.
//
//

#import <Foundation/Foundation.h>
#include "Vecteur.h"
@interface Selection : NSObject
{
    
    void* mSelectionVisitor;
    
}

- (void)getSelection;
- (id)init:(Vecteur2*) positionMin: (Vecteur2*) positionMax;
@end
