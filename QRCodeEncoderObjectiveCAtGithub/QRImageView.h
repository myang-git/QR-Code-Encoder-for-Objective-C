//
//  QRImageView.h
//  QRCodeEncoderObjectiveCAtGithub
//
//  Created by Shiva Huang on 13/6/18.
//
//

#import <UIKit/UIKit.h>
#import "DataMatrix.h"

@interface QRImageView : UIView

@property (strong, nonatomic) DataMatrix *dataMatrix;
@property (assign, nonatomic) unsigned int borderWidth;

@end