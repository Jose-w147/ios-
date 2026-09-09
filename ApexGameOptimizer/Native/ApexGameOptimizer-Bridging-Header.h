#ifndef ApexGameOptimizer_Bridging_Header_h
#define ApexGameOptimizer_Bridging_Header_h

#import <Foundation/Foundation.h>

// C++ function declarations accessible from Swift via bridging header
// These map to the native_bridge.cpp JNI functions, adapted for iOS

#ifdef __cplusplus
extern "C" {
#endif

// Hardware & System
int getSystemCores(void);
const char* getSystemMetrics(void);
const char* runRamCacheBenchmark(void);
const char* runSystemDiagnosticTest(void);

// Gyroscope
void updateGyro360(float gx, float gy, float gz, float dt);
float getYaw360(void);
void resetGyro360(void);

// Module 0-10
const char* runDynamicSpreadTest(int state);
const char* runRecoilTest(int shotNumber);
const char* runWorldToScreenTest(float wx, float wy, float wz);
const char* runSensitivityScalingTest(float currentFOV);
const char* runCollisionTest(void);
const char* runVector3DTest(void);
const char* runSmoothingTest(void);
const char* runNativeBenchmark(void);
const char* runVectorBenchmark(void);
const char* runLeadPredictionTest(float tx, float ty, float tz, float vx, float vy, float vz);
const char* runTouchPredictionTest(float cx, float cy, float dx, float dy);

// Module 11-20
const char* runBezierAimTest(void);
const char* runCpuGovernorTest(void);
const char* runAdvancedRecoilTest(int shotNumber, float recoilStrengthPercent);
const char* runAdvancedSpreadTest(int stateIndex, float spreadReductionPercent);
const char* runTouchSamplingTest(int targetHzMode);
const char* runKalmanFilterTest(float cx, float cy, float vx, float vy);
const char* runGyroFilterTest(float yaw, float pitch, float roll);
const char* runDisplaySyncTest(int targetHz);
const char* runRamDefragTest(void);
const char* runCpuAffinityTest(void);

// Module 21-30
const char* runElevationAngleTest(float dist);
const char* runEngineVerificationTest(void);
const char* runMultiTargetTest(void);
const char* runRecoilResetTest(void);
const char* runSubPixelTouchTest(void);
const char* runTouchPressureTest(void);
const char* runGpuDevfreqTest(void);
const char* runAudioLatencyTest(void);
const char* runShaderCacheTest(void);
const char* runSecurityGuardTest(void);

// Module 31-40
const char* runAiNeuralPredictorTest(void);
const char* runAiQuantumEngineTest(void);
const char* runScopeStabilizerTest(float magnification);
const char* runBulletVelocityTest(float speed, float dist);
const char* runMagnetSnapTest(void);
const char* runFpsStabilizerTest(int targetFps);
const char* runTouchJitterTest(void);
const char* runVulkanPipelineTest(void);
const char* runDualArenaDefragTest(void);
const char* runSplineTouchTest(void);

// Module 41-53
const char* runAudioSpectrogramTest(void);
const char* runRapidFireTapTest(void);
const char* runTargetLockPersistenceTest(void);
const char* runGyroSensitivityScaleTest(float mag);
const char* runGamePresetTest(int gameId);
const char* runThermalAnticipatorTest(void);
const char* runInputBufferFlushTest(void);
const char* runQuantumHealthCheckTest(void);
const char* runCachelineAllocatorTest(void);
const char* runHapticAudioFusionTest(void);
const char* runPolymorphicProtectorTest(void);
const char* runAdaptiveGyroKalmanTest(void);
const char* runZeroStutterShaderMatrixTest(void);
const char* runDynamicResolutionScalerTest(void);
const char* runBatteryThrottleShieldTest(void);
const char* runAntiAliasingFxaaTest(void);
const char* runSubMillisecondInputBufferTest(void);
const char* runMultiThreadedPhysicsTest(void);
const char* runVerticalFlickCurveTest(float velY);
const char* runWeaponRecoilProfileTest(int categoryInt, int shotNumber);
const char* runBinauralAudioTrackingTest(float leftDb, float rightDb);

#ifdef __cplusplus
}
#endif

#endif
