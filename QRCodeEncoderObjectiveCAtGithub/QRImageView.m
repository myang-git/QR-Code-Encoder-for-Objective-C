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
    }
    return self;
}


// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect
{
    CGContextRef context = UIGraphicsGetCurrentContext();
    
    CGContextSetFillColorWithColor(context, [[UIColor whiteColor] CGColor]);
    CGContextFillRect(context, rect);
    
    if(self.dataMatrix == nil)
        return;
    
    const int dataDimension = self.dataMatrix.dimension;
    const CGFloat ratio = self.frame.size.width / (dataDimension + 8);
    CGFloat yOffset = 0.0;
    
    // Drawing code
    
    CGContextSetFillColorWithColor(context, [[UIColor blackColor] CGColor]);
    
    for(int my=0; my<dataDimension; my++) {
        yOffset = (my+4) * ratio;
        
        for(int mx=0; mx<dataDimension; mx++) {
            if([self.dataMatrix valueAt:mx y:my]) {
                CGContextAddRect(context, CGRectMake((mx+4)*ratio, yOffset, ratio, ratio));
            }
        }
    }
    
    CGContextFillPath(context);
}

@end
