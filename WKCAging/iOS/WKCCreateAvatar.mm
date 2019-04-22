//
//  WKCCreateAvatar.m
//  WKCAgingView
//
//  Created by WeiKunChao on 2019/3/27.
//  Copyright © 2019 SecretLisa. All rights reserved.
//

#import "WKCCreateAvatar.h"
#include "mpsynth.h"


@implementation WKCCreateAvatar
{
    NSString * _agingSkin;
    NSString * _agingMask;
    NSString * _agingResource;
    
    int _usrImageW;
    int _usrImageH;
    unsigned char * _usrImage;
}

- (instancetype)init
{
    if (self = [super init])
    {
        _agingSkin = WKCFutureAgingPath();;
        _agingMask = [NSTemporaryDirectory() stringByAppendingPathComponent:@"agingmask"];
        _agingResource = WKCFutureResPath();
    }
    return self;
}

- (void)postError:(WKCAvatarError)error
{
    if (self.delegate && [self.delegate respondsToSelector:@selector(agingAvatarError:)])
    {
        [self.delegate agingAvatarError:error];
    }
}

- (void)createAvatar:(UIImage *)image
{
    [self createAvatar:image
                  skin:_agingSkin
               maskOut:_agingMask
         agingResource:_agingResource];
}

- (void)createAvatar:(UIImage *)image
                skin:(NSString *)pathSkin
             maskOut:(NSString *)pathMask
       agingResource:(NSString *)res
{
    _usrImageW = image.size.width;
    _usrImageH = image.size.height;
    
    CGImageRef inCgImage = [image CGImage];
    size_t bytesPerRow = CGImageGetBytesPerRow(inCgImage);
    CGDataProviderRef inDataProvider = CGImageGetDataProvider(inCgImage);
    CFDataRef inData = CGDataProviderCopyData(inDataProvider);
    UInt8 * inPixels = (UInt8*)CFDataGetBytePtr(inData);
    
    if((_usrImage = (unsigned char *)malloc(_usrImageW * _usrImageH * 3)) == NULL) {
        CFRelease(inData);
        return;
    }
    
    for(int y = 0; y != _usrImageH; ++ y)
    {
        for(int x = 0; x != _usrImageW; ++ x)
        {
            UInt8 * buf = inPixels + y * bytesPerRow + x * 4;
            _usrImage[(y*_usrImageW+x)*3+0] = buf[0];
            _usrImage[(y*_usrImageW+x)*3+1] = buf[1];
            _usrImage[(y*_usrImageW+x)*3+2] = buf[2];
        }
    }
    
    CFRelease(inData);
    [NSThread detachNewThreadSelector:@selector(createFaceData) toTarget:self withObject:nil];
}

- (void)createFaceData
{
    motionportrait::MpSynth *synth = new motionportrait::MpSynth();
    motionportrait::mpFaceObject faceObject;
    
    int stat = synth -> Init([_agingResource UTF8String]);
    if(stat)
    {
        [self postError:WKCAvatarErrorRecogintionEngine];
        return;
    }
    
    motionportrait::MpSynth::Img inImg;
    inImg.w = _usrImageW;
    inImg.h = _usrImageH;
    inImg.rgb = _usrImage;
    inImg.alpha = NULL;
    
    stat = synth->Detect(inImg);
    
    if(stat) {
        [self postError:WKCAvatarErrorRecogintionFailure];
        delete synth;
        free(_usrImage);
        return;
    } else {
        motionportrait::MpSynth::Mpfp mpfp;
        synth -> GetMpfp(mpfp);
        
        synth -> SetParami(motionportrait::MpSynth::HAIRIMAGE, motionportrait::MpSynth::FORMAT_BIN) ;
        synth -> SetParami(motionportrait::MpSynth::TEX_SIZE, 512) ;
        synth -> SetParami(motionportrait::MpSynth::MODEL_SIZE, 256) ;
        synth -> SetParamf(motionportrait::MpSynth::FACE_SIZE, 0.6) ;
        synth -> SetParamf(motionportrait::MpSynth::FACE_POS, 0.5) ;
        synth -> SetParami(motionportrait::MpSynth::CROP_MARGIN, 0);
        synth -> SetMpfp(mpfp);
        stat = synth -> Synth(inImg, &faceObject);
        
        if(stat) {
            [self postError:WKCAvatarErrorSynthesizeFailure];
        } else {
            NSString *skinY = [_agingSkin stringByAppendingPathComponent:@"young_skin"];
            NSString *skinO = [_agingSkin stringByAppendingPathComponent:@"old_skin"];
            NSString *maskY = [_agingMask stringByAppendingString:@"_young"];
            NSString *maskO = [_agingMask stringByAppendingString:@"_old"];
            synth -> SetMpfp(mpfp);
            stat |= synth -> GenAgingMask(inImg, [skinY UTF8String], [maskY UTF8String]);
            synth -> SetMpfp(mpfp);
            stat |= synth -> GenAgingMask(inImg, [skinO UTF8String], [maskO UTF8String]);
        }
        delete synth;
        free(_usrImage);
        
        if (0 == stat)
        {
            if ([self.delegate respondsToSelector:@selector(agingAvatarFinished:)])
            {
               [self.delegate agingAvatarFinished:faceObject];
            }
        }
        else
        {
            [self postError:WKCAvatarErrorMaskFailure];
        }
    }
}


@end
