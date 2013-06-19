

#import "QRCodeEncoderDemoViewController.h"


@implementation QRCodeEncoderDemoViewController

- (void)dealloc
{
    [super dealloc];
}

#pragma mark - View lifecycle

- (void)viewDidLoad
{
    [super viewDidLoad];
    
    UIScrollView *scrollView = [[UIScrollView alloc] initWithFrame:self.view.frame];
    
    //the qrcode is square. now we make it 250 pixels wide
    int qrcodeImageDimension = 250;
    
    //the string can be very long
    NSString* aVeryLongURL = @"http://thelongestlistofthelongeststuffatthelongestdomainnameatlonglast.com/";
    
    //first encode the string into a matrix of bools, TRUE for black dot and FALSE for white. Let the encoder decide the error correction level and version
    DataMatrix* qrMatrix = [QREncoder encodeWithECLevel:QR_ECLEVEL_AUTO version:QR_VERSION_AUTO string:aVeryLongURL];
    
    //then render the matrix
    UIImage* qrcodeImage = [QREncoder renderDataMatrix:qrMatrix imageDimension:qrcodeImageDimension];
    
    //put the image into the view
    UIImageView* qrcodeImageView = [[UIImageView alloc] initWithImage:qrcodeImage];
    CGRect parentFrame = self.view.frame;
    
    //put the image on a scroll view
    CGFloat margin = (parentFrame.size.width - qrcodeImageDimension) / 2.0;
    CGRect qrcodeImageViewFrame = CGRectMake(margin, margin, qrcodeImageDimension, qrcodeImageDimension);
    [qrcodeImageView setFrame:qrcodeImageViewFrame];
    
    //you can also show the matrix using QRImageView
    CGRect qrcodeImageViewFrame2 = CGRectMake(margin, margin * 2 + qrcodeImageDimension, qrcodeImageDimension, qrcodeImageDimension);
    QRImageView* qrcodeImageView2 = [[QRImageView alloc] initWithFrame:qrcodeImageViewFrame2];
    
    //you can set the border arounding the QR code to make scanning easier, the default with is 4 dots
    qrcodeImageView2.borderWidth = 4;
    
    //assign the matrix
    qrcodeImageView2.dataMatrix = qrMatrix;
    
    //and that's it! The upper one is a bitmap image shown in UIImageView, the lower one is a QRImageView showing the same data matrix.
    [scrollView addSubview:qrcodeImageView];
    [scrollView addSubview:qrcodeImageView2];
    [scrollView setContentSize:CGSizeMake(parentFrame.size.width, qrcodeImageDimension * 2 + margin * 3)];
    [self.view addSubview:scrollView];
    
    [qrcodeImageView release];
    [qrcodeImageView2 release];
    [scrollView release];
}

@end
