

#import "QREncoder.h"


@implementation QREncoder

+ (NSData*)AESEncryptString:(NSString*)string withPassphrase:(NSString*)passphrase {
    if (passphrase.length>kCCKeySizeAES256) {
        throw [NSException exceptionWithName:@"invalid passphrase exception" reason:[NSString stringWithFormat:@"passphrase too long: %d", passphrase.length] userInfo:nil];
    }
    const char* cstrPassphraseOriginal = [passphrase cStringUsingEncoding:NSUTF8StringEncoding];
    char cstrPassphrasePadded[kCCKeySizeAES256 + 1];
    memset(cstrPassphrasePadded, 0, sizeof(cstrPassphrasePadded));
    memcpy(cstrPassphrasePadded, cstrPassphraseOriginal, [passphrase length]);
    const char* dataIn = [string cStringUsingEncoding:NSUTF8StringEncoding];
    size_t dataInLength = [string length];
    size_t dataOutLength = dataInLength + kCCBlockSizeAES128;
    void* dataOut = malloc(dataOutLength);
    size_t encryptedDataLength = 0;
    CCCryptorStatus status = CCCrypt(kCCEncrypt, kCCAlgorithmAES128, kCCOptionPKCS7Padding, cstrPassphrasePadded, kCCKeySizeAES256, NULL, dataIn, dataInLength, dataOut, dataOutLength, &encryptedDataLength);
    NSData* encryptedData = nil;
    if (status==kCCSuccess) {
        encryptedData = [NSData dataWithBytes:dataOut length:encryptedDataLength];
    }
    free(dataOut);
    return encryptedData;
}

+ (NSString*)AESDecryptString:(NSData*)string withPassphrase:(NSString*)passphrase {
    if (passphrase.length>kCCKeySizeAES256) {
        throw [NSException exceptionWithName:@"invalid passphrase exception" reason:[NSString stringWithFormat:@"passphrase too long: %d", passphrase.length] userInfo:nil];
    }
    const char* cstrPassphraseOriginal = [passphrase cStringUsingEncoding:NSASCIIStringEncoding];
    char cstrPassphrase[kCCKeySizeAES256 + 1];
    memset(cstrPassphrase, 0, sizeof(cstrPassphrase));
    memcpy(cstrPassphrase, cstrPassphraseOriginal, [passphrase length]);
    const void* dataIn = [string bytes];
    size_t dataInLength = [string length];
    size_t dataOutLength = dataInLength + kCCBlockSizeAES128;
    void* dataOut = malloc(dataOutLength);
    size_t decryptedDataLength = 0;
    CCCryptorStatus status = CCCrypt(kCCDecrypt, kCCAlgorithmAES128, kCCOptionPKCS7Padding, cstrPassphrase, kCCKeySizeAES256, NULL, dataIn, dataInLength, dataOut, dataOutLength, &decryptedDataLength);
    NSString* decryptedString = nil;
    if (status==kCCSuccess) {
        NSData* data = [NSData dataWithBytes:dataOut length:decryptedDataLength];
        decryptedString = [[[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding] autorelease];
    }
    free(dataOut);
    return decryptedString;
}

+ (DataMatrix*)encodeCStringWithECLevel:(int)ecLevel version:(int)version cstring:(const char*)cstring {
    CQR_Encode* encoder = new CQR_Encode;
    encoder->EncodeData(ecLevel, version, true, -1, cstring);
    int dimension = encoder->m_nSymbleSize;
    DataMatrix* matrix = [[[DataMatrix alloc] initWith:dimension] autorelease];
    for (int y=0; y<dimension; y++) {
        for (int x=0; x<dimension; x++) {
            int v = encoder->m_byModuleData[y][x];
            bool bk = v==1;
            [matrix set:bk x:y y:x];
        }
    }
    
    delete encoder;
    
    return matrix;

}

+ (DataMatrix*)encodeWithECLevel:(int)ecLevel version:(int)version string:(NSString *)string AESPassphrase:(NSString*)AESPassphrase {
    NSData* encryptedString = [QREncoder AESEncryptString:string withPassphrase:AESPassphrase];
    const unsigned int len = [encryptedString length];
    char cstring[len + 1];
    bzero(cstring, len + 1);
    [encryptedString getBytes:cstring length:len];
    DataMatrix* matrix = [QREncoder encodeCStringWithECLevel:ecLevel version:version cstring:cstring];
    return matrix;
}

+ (DataMatrix*)encodeWithECLevel:(int)ecLevel version:(int)version string:(NSString *)string {
    const char* cstring = [string cStringUsingEncoding:NSUTF8StringEncoding];
    DataMatrix* matrix = [QREncoder encodeCStringWithECLevel:ecLevel version:version cstring:cstring];
    return matrix;
}

+ (void)drawDotAt:(int)x y:(int)y offset:(int)offset intensity:(unsigned char)intensity pixelPerDot:(int)pixelPerDot imageDimension:(int)imageDimension rawData:(unsigned char*)rawData {
    const int bytesPerPixel = 4;
    const int bytesPerLine = bytesPerPixel * imageDimension;

    int startX = pixelPerDot * x * bytesPerPixel + bytesPerPixel * offset;
    int startY = pixelPerDot * y + offset;
    int endX = startX + pixelPerDot * bytesPerPixel;
    int endY = startY + pixelPerDot;
    
    for (int my = startY; my < endY; my++) {
        for (int mx = startX; mx < endX; mx+=bytesPerPixel) {
            rawData[bytesPerLine * my + mx    ] = intensity;    //red
            rawData[bytesPerLine * my + mx + 1] = intensity;    //green
            rawData[bytesPerLine * my + mx + 2] = intensity;    //blue
            rawData[bytesPerLine * my + mx + 3] = 255;          //alpha
        }
    }
    
    
}

+ (UIImage*)renderDataMatrix:(DataMatrix*)matrix imageDimension:(int)imageDimension {
    
    const int bitsPerPixel = BITS_PER_BYTE * BYTES_PER_PIXEL;
    const int bytesPerLine = BYTES_PER_PIXEL * imageDimension;
    const int rawDataSize = imageDimension * imageDimension * BYTES_PER_PIXEL;
    unsigned char* rawData = (unsigned char*)malloc(rawDataSize);

    memset(rawData, WHITE, sizeof(unsigned char) * rawDataSize);
    
    int matrixDimension = [matrix dimension];
    int pixelPerDot = imageDimension / matrixDimension;
    int offset = (int)((imageDimension - pixelPerDot * matrixDimension) / 2);

    for (int y=0; y<matrixDimension; y+=1) {
        for (int x=0; x<matrixDimension; x+=1) {
            bool isDark = [matrix valueAt:x y:y];
            if (isDark) {
                char intensity = isDark ? 0 : WHITE;
                [QREncoder drawDotAt:x y:y offset:offset intensity:intensity pixelPerDot:pixelPerDot imageDimension:imageDimension rawData:rawData];
            }
        }
    }
    
    CGDataProviderRef provider = CGDataProviderCreateWithData(NULL, 
                                                              rawData, 
                                                              rawDataSize, 
                                                              NULL);
    
    CGColorSpaceRef colorSpaceRef = CGColorSpaceCreateDeviceRGB();
    CGBitmapInfo bitmapInfo = kCGBitmapByteOrderDefault;
    CGColorRenderingIntent renderingIntent = kCGRenderingIntentDefault;
    CGImageRef imageRef = CGImageCreate(imageDimension,
                                        imageDimension,
                                        BITS_PER_BYTE,
                                        bitsPerPixel,
                                        bytesPerLine,
                                        colorSpaceRef,
                                        bitmapInfo,
                                        provider,
                                        NULL,NO,renderingIntent);
    
    UIImage *newImage = [UIImage imageWithCGImage:imageRef];

    CGImageRelease(imageRef);
    CGColorSpaceRelease(colorSpaceRef);
    CGDataProviderRelease(provider);
    
    return newImage;
}


@end