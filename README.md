Install
=========================

To integrate the QR Code Encoder static library with your iOS application, follow the steps below:

    1. Drag the Xcode project package QRCodeEncoderObjectiveCAtGithub.xcodeproj to your project navigator

    2. Click on your project. Choose the target. Then go to the Build Phases page. 
       Expand "Target Dependencies" and add qrencoder to the list

    3. In the same Build Phases page, expand "Link Binary With Libraries", add libqrencoder.a to the list
    
    4. Switch to the Build Settings page, go down to the Search Paths section. (if you have trouble finding it, type "header search" in the search field)
       Add the absolute path to the source code directory to the path list.
       
    5. Add "CoreGraphics" framework to frameworks
    
    6. Build the project and you are good to go!

How to Use
=========================

Here is the typical workflow:

    1. Import QREncoder.h
   
    2. Call +[QREncoder encodeWithECLevel:version:string:] to encode a string to a square matrix of booleans.
   
    3. Call +[QREncoder renderDataMatrix:imageDimension:] to render the matrix generated in step 2.

Take a look at QRCodeEncoderDemoViewController.mm for encoding and rendering examples.
Or build and run the target QRCodeEncoderDemo to see things in action.

Also, since the encoder references C++ code, ALL implementation files need to have .mm as the extension.

License
=========================

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

