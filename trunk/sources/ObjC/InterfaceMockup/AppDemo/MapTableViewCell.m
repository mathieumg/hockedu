// TweetTableViewCell.m
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

#import "MapTableViewCell.h"

#import "Map.h"

#import "UIImageView+AFNetworking.h"

@implementation MapTableViewCell {
@private
    __strong Map *_map;
}

@synthesize map = _map;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (!self) {
        return nil;
    }
    
    self.textLabel.adjustsFontSizeToFitWidth = YES;
    self.textLabel.textColor = [UIColor darkGrayColor];
    self.detailTextLabel.font = [UIFont systemFontOfSize:12.0f];
    self.detailTextLabel.numberOfLines = 0;
    self.selectionStyle = UITableViewCellSelectionStyleGray;
    
    return self;
}

- (void)setMap:(Map *)map {
    _map = map;
    
    self.textLabel.text = _map.mapName;
    NSDate* lastModifiedDate = [NSDate dateWithTimeIntervalSince1970: _map.mapLastModified];
    NSMutableString* tempDescription = [[NSMutableString alloc] initWithString:_map.mapDescription];
    
    if( [tempDescription isEqualToString:@""] )
    {
        [tempDescription appendString:@"No description"];
    }
    
    NSDateFormatter *dateFormat = [[NSDateFormatter alloc] init];
    [dateFormat setDateFormat:@"yyyy-MM-dd"];
    
    NSDateFormatter *timeFormat = [[NSDateFormatter alloc] init];
    [timeFormat setTimeStyle:NSDateFormatterShortStyle];
    
    [timeFormat setDateFormat:@"HH:mm a"];
    
    NSString* dateString = [dateFormat stringFromDate:lastModifiedDate];
    NSString* timeString = [timeFormat stringFromDate:lastModifiedDate];
    [tempDescription appendString:@" - Last modified on: "];
    [tempDescription appendString:dateString];
    [tempDescription appendString:@" at "];
    [tempDescription appendString:timeString];
    self.detailTextLabel.text = tempDescription;
    //self.textLabel.text = @"Caca";

    //self.textLabel.text = @"mathieumg";
    //self.detailTextLabel.text = @"Some details...";
    //[self.imageView setImageWithURL:_post.user.avatarImageURL placeholderImage:[UIImage imageNamed:@"profile-image-placeholder"]];
    
    [self setNeedsLayout];
}

+ (CGFloat)heightForCellWithMap:(Map *)map {
    //CGSize sizeToFit = [post.text sizeWithFont:[UIFont systemFontOfSize:12.0f] constrainedToSize:CGSizeMake(220.0f, CGFLOAT_MAX) lineBreakMode:UILineBreakModeWordWrap];
    
    //return fmaxf(70.0f, sizeToFit.height + 45.0f);
    return 65.0f;
}

#pragma mark - UIView

- (void)layoutSubviews {
    [super layoutSubviews];
    
    //self.imageView.frame = CGRectMake(10.0f, 10.0f, 50.0f, 50.0f);
    self.textLabel.frame = CGRectMake(10.0f, 10.0f, 700.0f, 20.0f);
    
    CGRect detailTextLabelFrame = CGRectOffset(self.textLabel.frame, 0.0f, 25.0f);
    detailTextLabelFrame.size.height = [[self class] heightForCellWithMap:_map] - 45.0f;
    //detailTextLabelFrame.size.height = 45.0f;
    self.detailTextLabel.frame = detailTextLabelFrame;
}

@end
