//
//  GPUImageCoreVideoInput.h
//  GPUImage
//
//  Created by Karl von Randow on 3/11/13.
//  Copyright (c) 2013 Brad Larson. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <CoreMedia/CoreMedia.h>

#import "GPUImageOutput.h"

extern const GLfloat kColorConversion601[];
extern const GLfloat kColorConversion709[];
extern NSString *const kGPUImageYUVVideoRangeConversionForRGFragmentShaderString;
extern NSString *const kGPUImageYUVVideoRangeConversionForLAFragmentShaderString;


//Delegate Protocal for Face Detection.
@protocol GPUImageVideoCameraDelegate <NSObject>

@optional
- (void)willOutputSampleBuffer:(CMSampleBufferRef)sampleBuffer;
@end


@interface GPUImageCoreVideoInput : GPUImageOutput <AVCaptureVideoDataOutputSampleBufferDelegate> {
    
@protected
    dispatch_semaphore_t frameRenderingSemaphore;
    
    BOOL captureAsYUV;
    BOOL capturePaused;
    
@private
    CVOpenGLESTextureCacheRef coreVideoTextureCache;
    
    NSUInteger numberOfFramesCaptured;
    CGFloat totalFrameTimeDuringCapture;
    
    GPUImageRotationMode outputRotation;
    
    GLuint luminanceTexture, chrominanceTexture;
    
    __unsafe_unretained id<GPUImageVideoCameraDelegate> _delegate;
}

/// This enables the benchmarking mode, which logs out instantaneous and average frame times to the console
@property(readwrite, nonatomic) BOOL runBenchmark;

@property(readwrite, nonatomic) GPUImageRotationMode outputRotation;

@property(readwrite, nonatomic) BOOL capturePaused;

@property(nonatomic, assign) id<GPUImageVideoCameraDelegate> delegate;

- (id)initWithCaptureAsYUV:(BOOL)aCaptureAsYUV;

/** Process a video sample
 @param sampleBuffer Buffer to process
 */
- (void)processVideoSampleBuffer:(CMSampleBufferRef)sampleBuffer;

+ (GPUImageRotationMode)rotationForImageOrientation:(UIInterfaceOrientation)imageOrientation
                              captureDevicePosition:(AVCaptureDevicePosition)position
                               horizontallyMirrored:(BOOL)horizontallyMirrored;

/// @name Benchmarking

/** When benchmarking is enabled, this will keep a running average of the time from uploading, processing, and final recording or display
 */
- (CGFloat)averageFrameDurationDuringCapture;

- (void)resetBenchmarkAverage;

@end
