//
//  BackgroundLayer.h
//  Hockedu
//
//  Created by Mathieu Marengere Gosselin on 2013-04-07.
//
//

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>

@interface BackgroundLayer : NSObject

+(CAGradientLayer*) greyGradient;
+(CAGradientLayer*) blueGradient;

@end