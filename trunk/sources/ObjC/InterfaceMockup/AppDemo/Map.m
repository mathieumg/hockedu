// Post.m
//
// Copyright (c) 2012 Mattt Thompson (http://mattt.me/)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#import "Map.h"
#import "AFAppDotNetAPIClient.h"
#import "Model.h"
@implementation Map
@synthesize mapID = _mapID;
@synthesize mapName = _mapName;
@synthesize mapDescription = _mapDescription;
@synthesize mapLastModified = _mapLastModified;
//@synthesize mapContent = _mapContent;

- (id)initWithAttributes:(NSDictionary *)attributes {
    self = [super init];
    if (!self) {
        return nil;
    }
    
    _mapID = [[attributes valueForKeyPath:@"id"] integerValue];
    _mapName = [attributes valueForKeyPath:@"name"];
    _mapDescription = [attributes valueForKeyPath:@"description"];
    _mapLastModified = [[attributes valueForKeyPath:@"last_modified_time"] integerValue];
    
    return self;
}

#pragma mark -

//,@"608b9a09de61fea254bbebdcadc0fe8c38ae2ccb", @"auth_key",

+ (void)globalTimelinePostsWithBlock:(void (^)(NSArray *maps, NSError *error))block :(NSInteger) userId {
    [[AFAppDotNetAPIClient sharedClient] postPath:@"remote/listmaps" parameters:@{@"user_id":[[NSNumber numberWithInt:userId] stringValue]} success:^(AFHTTPRequestOperation *operation, id JSON) {
        
        NSArray *mapsFromResponse = [JSON valueForKeyPath:@"maps"];
        NSMutableArray *mutableMaps = [NSMutableArray arrayWithCapacity:[mapsFromResponse count]];
        for (NSDictionary *attributes in mapsFromResponse) {
            Map *map = [[Map alloc] initWithAttributes:attributes];
            [mutableMaps addObject:map];
        }
        
        if (block) {
            block([NSArray arrayWithArray:mutableMaps], nil);
        }
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        if (block) {
            block([NSArray array], error);
        }
    }];
}

+ (void)downloadMap:(void (^)(NSArray *maps, NSError *error))block :(NSInteger) mapId {
    [[AFAppDotNetAPIClient sharedClient] postPath:@"remote/getmap" parameters:@{@"map_id":[[NSNumber numberWithInt:mapId] stringValue]} success:^(AFHTTPRequestOperation *operation, id JSON) {
        
        //_mapContent = [JSON valueForKeyPath:@"content"];
        
        //NSLog(@"Good");
        
       NSLog(@"Map content: %@", [JSON valueForKeyPath:@"content"]);
        
        [Model loadField:[JSON valueForKeyPath:@"content"]];
        
        //NSArray *mapFromResponse = [JSON valueForKeyPath:@"content"];
        
        //NSArray *mapsFromResponse = [JSON valueForKeyPath:@"maps"];
        /*
        NSMutableArray *mutableMaps = [NSMutableArray arrayWithCapacity:[mapsFromResponse count]];
        for (NSDictionary *attributes in mapsFromResponse) {
            Map *map = [[Map alloc] initWithAttributes:attributes];
            [mutableMaps addObject:map];
        }
        */
        
        /*
        if (block) {
            block([NSArray arrayWithArray:mutableMaps], nil);
        }
        */
        
       
        
    } failure:^(AFHTTPRequestOperation *operation, NSError *error) {
        
        /*
        if (block) {
            block([NSArray array], error);
        }
         */
        
          NSLog(@"Bad");
        
    }];
}

@end
