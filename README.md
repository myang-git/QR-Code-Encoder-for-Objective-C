Install
=========================

To integrate the QR Code Encoder static library with your iOS application, follow the steps below:

    1. Drag the Xcode project package QRCodeEncoderObjectiveCAtGithub.xcodeproj to your project navigator

    2. Click on your project. Choose the target. Then go to the Build Phases page. 
       Expand "Target Dependencies" and add qrencoder to the list

    3. In the same Build Phases page, expand "Link Binary With Libraries", add libqrencoder.a to the list

    4. Build the project and you are good to go!

How to Use
=========================

Here is the typical workflow:

    1. Import QREncoder.h
   
    2. Call QREncoder +encodeWithECLevel:(int)ecLevel version:(int)version string:(NSString*)string 
       to encode a string to a square matrix of booleans.
   
    3. Call QREncoder +renderDataMatrix:(DataMatrix*)matrix imageDimension:(int)imageDimension 
       to render the matrix generated from step 2.

Take a look at QRCodeEncoderDemoViewController.mm for encoding and rendering examples.

Also, since the encoder references C++ code, ALL implementation files need to have .mm as the extension.
