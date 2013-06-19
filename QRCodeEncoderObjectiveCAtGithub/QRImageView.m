//
//  QRImageView.m
//  QRCodeEncoderObjectiveCAtGithub
//
//  Created by Shiva Huang on 13/6/18.
//
//

#import "QRImageView.h"

@implementation QRImageView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
        self.borderWidth = 4;
    }
    return self;
}


// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    CGContextSetFillColorWithColor(context, [self.backgroundColor CGColor]);
    CGContextFillRect(context, rect);
    
    if(self.dataMatrix == nil)
        return;
    
    const int dataDimension = self.dataMatrix.dimension;
    const CGFloat imageDimension = MIN(self.frame.size.width, self.frame.size.height);
    const CGFloat pixelPerDot = imageDimension / (dataDimension + self.borderWidth * 2);
    CGFloat yMargin = (self.frame.size.height - imageDimension) / 2.0;
    CGFloat xMargin = (self.frame.size.width - imageDimension) / 2.0;
    CGFloat yOffset = 0.0;
    
    // Drawing code
    CGContextSetFillColorWithColor(context, [[UIColor whiteColor] CGColor]);
    CGContextFillRect(context, CGRectMake(xMargin, yMargin, imageDimension, imageDimension));
    
    CGContextSetFillColorWithColor(context, [[UIColor blackColor] CGColor]);
    yMargin += self.borderWidth * pixelPerDot;
    xMargin += self.borderWidth * pixelPerDot;
    
    for(int my=0; my<dataDimension; my++) {
        yOffset = yMargin + my * pixelPerDot;
        
        for(int mx=0; mx<dataDimension; mx++) {
            if([self.dataMatrix valueAt:mx y:my]) {
                CGContextAddRect(context, CGRectMake(xMargin + mx * pixelPerDot, yOffset, pixelPerDot, pixelPerDot));
            }
        }
    }
    
    CGContextFillPath(context);
}

@end
