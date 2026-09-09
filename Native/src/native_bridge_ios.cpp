#include <string>
#include <sstream>
#include <thread>
#include "performance_optimizer.hpp"

static Optimizer::GyroscopeCamera360 g_camera360;

extern "C" {

// === Hardware & System ===

int getSystemCores_raw(void) {
    unsigned int cores = std::thread::hardware_concurrency();
    return (cores > 0) ? static_cast<int>(cores) : 1;
}

const char* getSystemMetrics_raw(void) {
    static thread_local std::string r;
    auto stats = Optimizer::getHardwareStats();
    auto pm = Optimizer::getProcessMemoryStats();
    std::ostringstream ss;
    ss << "CPU: " << stats.cpuArchitecture << "\n"
       << "Cores: " << stats.cpuCores << "\n"
       << "RAM Total: " << stats.totalMemoryMb << " MB\n"
       << "RAM Livre: " << stats.freeMemoryMb << " MB\n";
    if (pm.rssKb > 0) ss << "RSS: " << (pm.rssKb / 1024) << " MB\n";
    ss << "NEON: " << (stats.supportsNeon ? "SIM" : "NAO");
    r = ss.str();
    return r.c_str();
}

const char* runRamCacheBenchmark_raw(void) {
    static thread_local std::string r;
    Optimizer::MemoryArena arena(2 * 1024 * 1024);
    arena.allocate(1024);
    arena.allocate(2048);
    auto cr = Optimizer::runCacheOptimizationBenchmark(1000000);
    std::ostringstream ss;
    ss << "=== Memory Optimization ===\n"
       << "Arena: " << (arena.usedSize() / 1024) << "KB used\n"
       << "AoS: " << cr.aosDurationMs << "ms\n"
       << "SoA: " << cr.soaDurationMs << "ms\n"
       << "Speedup: " << cr.speedupFactor << "x";
    r = ss.str();
    return r.c_str();
}

const char* runSystemDiagnosticTest_raw(void) {
    static thread_local std::string r;
    auto s = Optimizer::runSystemDiagnostic();
    std::ostringstream ss;
    ss << "=== System Diagnostic ===\n"
       << "Memory: " << (s.memoryPoolOk ? "OK" : "FAIL") << "\n"
       << "Threads: " << (s.threadPoolOk ? "OK" : "FAIL") << "\n"
       << "Gyro: " << (s.gyroEngineOk ? "OK" : "FAIL") << "\n"
       << "Vector: " << (s.vectorEngineOk ? "OK" : "FAIL") << "\n"
       << "Collision: " << (s.collisionEngineOk ? "OK" : "FAIL") << "\n"
       << "Time: " << s.diagnosticDurationMs << "ms";
    r = ss.str();
    return r.c_str();
}

// === Gyroscope ===

void updateGyro360(float gx, float gy, float gz, float dt) {
    g_camera360.updateGyro(gx, gy, gz, dt);
}

float getYaw360(void) {
    return g_camera360.getYaw360();
}

void resetGyro360(void) {
    g_camera360.reset();
}

// === Module functions ===

const char* runDynamicSpreadTest(int state) {
    static thread_local std::string r;
    auto s = static_cast<Optimizer::MovementState>(state);
    float spread = Optimizer::calculateCrosshairSpread(s);
    std::ostringstream ss;
    ss << "=== Dynamic Spread ===\nState: " << state
       << "\nSpread: " << spread;
    r = ss.str();
    return r.c_str();
}

const char* runRecoilTest(int shotNumber) {
    static thread_local std::string r;
    auto rc = Optimizer::calculateCameraRecoil(shotNumber, 0, 0, 0.016f);
    std::ostringstream ss;
    ss << "=== Recoil ===\nShot: " << shotNumber
       << "\nPitch: " << rc.pitchKick << "\nYaw: " << rc.yawKick;
    r = ss.str();
    return r.c_str();
}

const char* runWorldToScreenTest(float wx, float wy, float wz) {
    static thread_local std::string r;
    auto sp = Optimizer::projectWorldToScreen(
        Optimizer::Vector3D(wx, wy, wz),
        Optimizer::Vector3D(0, 0, 0), 90, 1080, 2400);
    std::ostringstream ss;
    ss << "=== WorldToScreen ===\n("
       << sp.screenX << ", " << sp.screenY
       << ") visible=" << sp.visibleOnScreen;
    r = ss.str();
    return r.c_str();
}

const char* runSensitivityScalingTest(float currentFOV) {
    static thread_local std::string r;
    float s = Optimizer::calculateFOVSensitivityScale(currentFOV);
    std::ostringstream ss;
    ss << "=== FOV Scale ===\nFOV: " << currentFOV
       << "\nScale: " << s;
    r = ss.str();
    return r.c_str();
}

const char* runCollisionTest(void) {
    static thread_local std::string r;
    Optimizer::BoundingBox3D a(Optimizer::Vector3D(0,0,0), Optimizer::Vector3D(1,1,1));
    Optimizer::BoundingBox3D b(Optimizer::Vector3D(0.5f,0.5f,0.5f), Optimizer::Vector3D(1.5f,1.5f,1.5f));
    bool hit = Optimizer::checkAABBCollision(a, b);
    std::ostringstream ss;
    ss << "=== Collision AABB ===\nHit: " << (hit ? "SIM" : "NAO");
    r = ss.str();
    return r.c_str();
}

const char* runVector3DTest(void) {
    static thread_local std::string r;
    auto res = Optimizer::runVectorMathBenchmark(100000);
    std::ostringstream ss;
    ss << "=== Vector3D NEON ===\nScalar: " << res.scalarDurationMs
       << "ms\nSIMD: " << res.simdDurationMs
       << "ms\nSpeedup: " << res.speedupRatio << "x";
    r = ss.str();
    return r.c_str();
}

const char* runSmoothingTest(void) {
    static thread_local std::string r;
    Optimizer::Vector3D cur(1, 0, 0), tgt(0, 1, 0);
    auto sm = Optimizer::smoothExponential(cur, tgt, 0.1f, 0.016f);
    std::ostringstream ss;
    ss << "=== Smoothing ===\nResult: ("
       << sm.x << ", " << sm.y << ", " << sm.z << ")";
    r = ss.str();
    return r.c_str();
}

const char* runNativeBenchmark(void) {
    static thread_local std::string r;
    auto res = Optimizer::runVectorMathBenchmark(500000);
    std::ostringstream ss;
    ss << "=== Native Benchmark ===\nSIMD Speedup: "
       << res.speedupRatio << "x";
    r = ss.str();
    return r.c_str();
}

const char* runVectorBenchmark(void) {
    static thread_local std::string r;
    auto res = Optimizer::runVectorMathBenchmark(1000000);
    std::ostringstream ss;
    ss << "=== Vector NEON ===\nSpeedup: "
       << res.speedupRatio << "x";
    r = ss.str();
    return r.c_str();
}

const char* runLeadPredictionTest(float tx, float ty, float tz, float vx, float vy, float vz) {
    static thread_local std::string r;
    auto lp = Optimizer::predictTargetLeadPosition(
        Optimizer::Vector3D(tx, ty, tz),
        Optimizer::Vector3D(vx, vy, vz),
        Optimizer::Vector3D(0, 0, 0));
    std::ostringstream ss;
    ss << "=== Lead Prediction ===\n("
       << lp.predictedPosition.x << ", " << lp.predictedPosition.y
       << ") TTI: " << lp.timeToImpactSeconds << "s";
    r = ss.str();
    return r.c_str();
}

const char* runTouchPredictionTest(float cx, float cy, float dx, float dy) {
    static thread_local std::string r;
    auto tp = Optimizer::predictTouchPosition(cx, cy, dx, dy);
    std::ostringstream ss;
    ss << "=== Touch Prediction ===\n("
       << tp.predictedRawX << ", " << tp.predictedRawY
       << ") Latency-: " << tp.latencyReductionMs << "ms";
    r = ss.str();
    return r.c_str();
}

const char* runBezierAimTest(void) {
    static thread_local std::string r;
    auto pts = Optimizer::generateBezierAimPath(0, 0, 500, 500);
    std::ostringstream ss;
    ss << "=== Bezier Aim ===\nPoints: " << pts.size();
    r = ss.str();
    return r.c_str();
}

const char* runCpuGovernorTest(void) {
    static thread_local std::string r;
    auto gov = Optimizer::optimizeCpuGovernorSettings();
    std::ostringstream ss;
    ss << "=== CPU Governor ===\nPolicy: " << gov.governorPolicy
       << "\nFreq: " << gov.cpuFrequencyGhz << "GHz"
       << "\nThermal: " << gov.cpuTempCelsius << "C";
    r = ss.str();
    return r.c_str();
}

const char* runAdvancedRecoilTest(int shotNumber, float recoilStrengthPercent) {
    static thread_local std::string r;
    auto rc = Optimizer::calculateAdvancedRecoilCompensation(shotNumber, recoilStrengthPercent);
    std::ostringstream ss;
    ss << "=== Advanced Recoil ===\nShot: " << shotNumber
       << "\nOrig: (" << rc.originalPitch << ", " << rc.originalYaw
       << ")\nComp: (" << rc.compensatedPitch << ", " << rc.compensatedYaw
       << ")\nReduction: " << rc.reductionPercentage << "%";
    r = ss.str();
    return r.c_str();
}

const char* runAdvancedSpreadTest(int stateIndex, float spreadReductionPercent) {
    static thread_local std::string r;
    auto s = static_cast<Optimizer::MovementState>(stateIndex);
    auto sr = Optimizer::calculateAdvancedSpreadReduction(s, spreadReductionPercent);
    std::ostringstream ss;
    ss << "=== Advanced Spread ===\nOrig: " << sr.originalSpreadRadius
       << "\nReduced: " << sr.reducedSpreadRadius
       << "\nBoost: " << sr.accuracyBoostPercent << "%";
    r = ss.str();
    return r.c_str();
}

const char* runTouchSamplingTest(int targetHzMode) {
    static thread_local std::string r;
    auto ts = Optimizer::optimizeTouchSamplingRate(targetHzMode);
    std::ostringstream ss;
    ss << "=== Touch Sampling ===\nTarget: " << ts.targetHz
       << "Hz\nLag: " << ts.inputLagMs << "ms";
    r = ss.str();
    return r.c_str();
}

const char* runKalmanFilterTest(float cx, float cy, float vx, float vy) {
    static thread_local std::string r;
    auto kf = Optimizer::runKalmanFilterAimTracking(cx, cy, vx, vy);
    std::ostringstream ss;
    ss << "=== Kalman Filter ===\nEst: (" << kf.estimatedX << ", " << kf.estimatedY
       << ")\nPred: (" << kf.predictedNextX << ", " << kf.predictedNextY
       << ")\nAcc: " << kf.accuracyScorePercent << "%";
    r = ss.str();
    return r.c_str();
}

const char* runGyroFilterTest(float yaw, float pitch, float roll) {
    static thread_local std::string r;
    auto gf = Optimizer::runAdaptiveGyroFilter(yaw, pitch, roll);
    std::ostringstream ss;
    ss << "=== Gyro Filter ===\nFiltered: ("
       << gf.filteredYaw << ", " << gf.filteredPitch << ", " << gf.filteredRoll
       << ")\nTremor: " << (gf.tremorFiltered ? "FILTERED" : "NONE");
    r = ss.str();
    return r.c_str();
}

const char* runDisplaySyncTest(int targetHz) {
    static thread_local std::string r;
    auto ds = Optimizer::runDisplayVrrSync(targetHz);
    std::ostringstream ss;
    ss << "=== Display Sync ===\nTarget: " << ds.targetRefreshRateHz
       << "Hz\nJitter-: " << ds.jitterReductionPercent << "%";
    r = ss.str();
    return r.c_str();
}

const char* runRamDefragTest(void) {
    static thread_local std::string r;
    auto rd = Optimizer::runNativeRamDefragmenter();
    std::ostringstream ss;
    ss << "=== RAM Defrag ===\nTrimmed: " << rd.memoryTrimmedBytes
       << " bytes\nFree: " << rd.freeRamMb << "MB";
    r = ss.str();
    return r.c_str();
}

const char* runCpuAffinityTest(void) {
    static thread_local std::string r;
    auto ca = Optimizer::runCpuCoreAffinityLocking();
    std::ostringstream ss;
    ss << "=== CPU Affinity ===\nCore: " << ca.pinnedCoreId
       << "\nOK: " << (ca.affinityLockSuccess ? "SIM" : "NAO");
    r = ss.str();
    return r.c_str();
}

const char* runElevationAngleTest(float dist) {
    static thread_local std::string r;
    auto ea = Optimizer::calculateHeadshotElevationAngle(dist);
    std::ostringstream ss;
    ss << "=== Elevation ===\nDist: " << dist
       << "m\nAngle: " << ea.elevationAngleDegrees << "deg";
    r = ss.str();
    return r.c_str();
}

const char* runEngineVerificationTest(void) {
    static thread_local std::string r;
    auto ev = Optimizer::verifyRealtimeEngineMetrics();
    std::ostringstream ss;
    ss << "=== Engine Verify ===\nCores: " << ev.activeCoresCount
       << "\nRSS: " << ev.processRssMb << "MB"
       << "\nSIMD: " << ev.simdSpeedupRatio << "x"
       << "\nLoop: " << ev.loopExecutionTimeMicroseconds << "us";
    r = ss.str();
    return r.c_str();
}

const char* runMultiTargetTest(void) {
    static thread_local std::string r;
    auto mt = Optimizer::runMultiTargetVectorTracking();
    std::ostringstream ss;
    ss << "=== Multi Target ===\nTracked: " << mt.trackedTargetsCount
       << "\nSnap: " << mt.snapTargetAngleDegrees << "deg";
    r = ss.str();
    return r.c_str();
}

const char* runRecoilResetTest(void) {
    static thread_local std::string r;
    auto rr = Optimizer::predictRecoilResetTime(5.0f);
    std::ostringstream ss;
    ss << "=== Recoil Reset ===\nTime: " << rr.recoilResetTimeMs
       << "ms\nBurstReady: " << (rr.readyForBurstFire ? "SIM" : "NAO");
    r = ss.str();
    return r.c_str();
}

const char* runSubPixelTouchTest(void) {
    static thread_local std::string r;
    auto sp = Optimizer::runSubPixelTouchInterpolator(500.0f, 600.0f);
    std::ostringstream ss;
    ss << "=== Sub-Pixel ===\n(" << sp.subPixelX << ", " << sp.subPixelY
       << ")\nJitter: " << (sp.jitterEliminated ? "ELIMINATED" : "NONE");
    r = ss.str();
    return r.c_str();
}

const char* runTouchPressureTest(void) {
    static thread_local std::string r;
    auto tp = Optimizer::runTouchPressureNormalizer(0.75f);
    std::ostringstream ss;
    ss << "=== Touch Pressure ===\nNorm: " << tp.normalizedPressure
       << "\nStable: " << (tp.pressureStabilized ? "SIM" : "NAO");
    r = ss.str();
    return r.c_str();
}

const char* runGpuDevfreqTest(void) {
    static thread_local std::string r;
    auto gd = Optimizer::runGpuDevfreqLock();
    std::ostringstream ss;
    ss << "=== GPU Devfreq ===\nFreq: " << gd.gpuFrequencyMhz
       << "MHz\nLocked: " << (gd.gpuLockActive ? "SIM" : "NAO");
    r = ss.str();
    return r.c_str();
}

const char* runAudioLatencyTest(void) {
    static thread_local std::string r;
    auto al = Optimizer::runAudioLatencyMinimizer();
    std::ostringstream ss;
    ss << "=== Audio Latency ===\nLatency: " << al.audioLatencyMs
       << "ms\nOptimized: " << (al.bufferOptimized ? "SIM" : "NAO");
    r = ss.str();
    return r.c_str();
}

const char* runShaderCacheTest(void) {
    static thread_local std::string r;
    auto sc = Optimizer::runShaderCachePrewarm();
    std::ostringstream ss;
    ss << "=== Shader Cache ===\nPrewarmed: " << sc.prewarmedShadersCount
       << "\nStutterFree: " << (sc.stutteringPrevented ? "SIM" : "NAO");
    r = ss.str();
    return r.c_str();
}

const char* runSecurityGuardTest(void) {
    static thread_local std::string r;
    auto sg = Optimizer::verifyEngineSecurityGuard();
    std::ostringstream ss;
    ss << "=== Security Guard ===\nEncrypted: " << (sg.isEncrypted ? "SIM" : "NAO")
       << "\nChecksum: " << (sg.memoryChecksumValid ? "OK" : "FAIL");
    r = ss.str();
    return r.c_str();
}

const char* runAiNeuralPredictorTest(void) {
    static thread_local std::string r;
    auto ai = Optimizer::runAiNeuralPredictorEngine();
    std::ostringstream ss;
    ss << "=== AI Neural Predictor ===\nRAM: " << ai.predictedRamAllocationMb
       << "MB\nTouch: " << ai.predictedOptimalTouchHz
       << "Hz\nKalman: " << ai.predictedKalmanGain;
    r = ss.str();
    return r.c_str();
}

const char* runAiQuantumEngineTest(void) {
    static thread_local std::string r;
    auto qe = Optimizer::runAiQuantumNeuralEngine();
    std::ostringstream ss;
    ss << "=== AI Quantum v3.0 ===\nInference: "
       << qe.subMicrosecondInferenceTimeUs << "us"
       << "\nConfidence: " << qe.predictionConfidence
       << "\nEpochs: " << qe.neuralNetEpochs
       << "\nNEON: " << (qe.armNeonTensorCoreActive ? "ON" : "OFF");
    r = ss.str();
    return r.c_str();
}

const char* runScopeStabilizerTest(float magnification) {
    static thread_local std::string r;
    auto ss2 = Optimizer::runDynamicScopeStabilizer(magnification);
    std::ostringstream ss;
    ss << "=== Scope Stabilizer ===\nMag: " << ss2.scopeMagnification
       << "x\nLock: " << (ss2.scopeLockActive ? "ON" : "OFF");
    r = ss.str();
    return r.c_str();
}

const char* runBulletVelocityTest(float speed, float dist) {
    static thread_local std::string r;
    auto bv = Optimizer::runBulletVelocityCompensation(speed, dist);
    std::ostringstream ss;
    ss << "=== Bullet Velocity ===\nLead: " << bv.calculatedLeadDistance
       << "m\nFlight: " << bv.bulletTimeOfFlightMs << "ms";
    r = ss.str();
    return r.c_str();
}

const char* runMagnetSnapTest(void) {
    static thread_local std::string r;
    auto ms = Optimizer::runMagnetSnapVector();
    std::ostringstream ss;
    ss << "=== Magnet Snap ===\nDelta: (" << ms.snapDeltaX << ", " << ms.snapDeltaY
       << ")\nPower: " << ms.magnetPowerPercentage << "%";
    r = ss.str();
    return r.c_str();
}

const char* runFpsStabilizerTest(int targetFps) {
    static thread_local std::string r;
    auto fs = Optimizer::runFpsFrameStabilizer(targetFps);
    std::ostringstream ss;
    ss << "=== FPS Stabilizer ===\nTarget: " << fs.targetFps
       << "\nVariance: " << fs.frameVarianceMs << "ms"
       << "\nStutterFree: " << (fs.stutterEliminated ? "SIM" : "NAO");
    r = ss.str();
    return r.c_str();
}

const char* runTouchJitterTest(void) {
    static thread_local std::string r;
    auto tj = Optimizer::runTouchJitterSmoothingFilter();
    std::ostringstream ss;
    ss << "=== Touch Jitter ===\nSmoothed: (" << tj.smoothedX << ", " << tj.smoothedY
       << ")\nNoise-: " << tj.noiseReductionPercent << "%";
    r = ss.str();
    return r.c_str();
}

const char* runVulkanPipelineTest(void) {
    static thread_local std::string r;
    auto vp = Optimizer::runVulkanPipelinePreloader();
    std::ostringstream ss;
    ss << "=== Vulkan Pipeline ===\nPipelines: " << vp.loadedPipelinesCount
       << "\nSwapReady: " << (vp.instantSwapReady ? "SIM" : "NAO");
    r = ss.str();
    return r.c_str();
}

const char* runDualArenaDefragTest(void) {
    static thread_local std::string r;
    auto da = Optimizer::runDualArenaMemoryDefrag();
    std::ostringstream ss;
    ss << "=== Dual Arena ===\nFreed: " << da.freedMemoryBytes
       << " bytes\nL1: " << da.l1CacheBufferBytes << "B";
    r = ss.str();
    return r.c_str();
}

const char* runSplineTouchTest(void) {
    static thread_local std::string r;
    auto st = Optimizer::runFutureTouchPredictionSpline();
    std::ostringstream ss;
    ss << "=== Spline Touch ===\nFuture: (" << st.futureTouchX << ", " << st.futureTouchY
       << ")\nLead: " << st.leadTimeMs << "ms";
    r = ss.str();
    return r.c_str();
}

const char* runAudioSpectrogramTest(void) {
    static thread_local std::string r;
    auto as = Optimizer::runFootstepAudioFFTSpectrogram();
    std::ostringstream ss;
    ss << "=== Audio Spectrogram ===\nPeak: " << as.footstepFrequencyPeakHz
       << "Hz\nDir: " << as.directionAngleDegrees << "deg"
       << "\nDetected: " << (as.footstepDetected ? "SIM" : "NAO");
    r = ss.str();
    return r.c_str();
}

const char* runRapidFireTapTest(void) {
    static thread_local std::string r;
    auto rf = Optimizer::runRapidFireTapOptimizer();
    std::ostringstream ss;
    ss << "=== Rapid Fire ===\nInterval: " << rf.optimalTapIntervalMs
       << "ms\nRPS: " << rf.maxAchievableRps;
    r = ss.str();
    return r.c_str();
}

const char* runTargetLockPersistenceTest(void) {
    static thread_local std::string r;
    auto tl = Optimizer::runTargetLockPersistenceFilter();
    std::ostringstream ss;
    ss << "=== Target Lock ===\nTarget: " << tl.primaryTargetId
       << "\nStrength: " << tl.lockStrengthPercent << "%";
    r = ss.str();
    return r.c_str();
}

const char* runGyroSensitivityScaleTest(float mag) {
    static thread_local std::string r;
    auto gs = Optimizer::runDynamicGyroSensitivityScaler(mag);
    std::ostringstream ss;
    ss << "=== Gyro Scale ===\nMag: " << gs.scopeMagnification
       << "x\nScaleX: " << gs.scaledGyroSensX
       << "\nScaleY: " << gs.scaledGyroSensY;
    r = ss.str();
    return r.c_str();
}

const char* runGamePresetTest(int gameId) {
    static thread_local std::string r;
    auto gp = Optimizer::runGamePresetAutoSelector(gameId);
    std::ostringstream ss;
    ss << "=== Game Preset ===\nGame: " << gp.gameName
       << "\nTouch: " << gp.activeTouchHz << "Hz";
    r = ss.str();
    return r.c_str();
}

const char* runThermalAnticipatorTest(void) {
    static thread_local std::string r;
    auto ta = Optimizer::runHardwareThermalAnticipator();
    std::ostringstream ss;
    ss << "=== Thermal ===\nTemp: " << ta.currentTempC
       << "C\nForecast: " << ta.forecastedTemp10MinC << "C"
       << "\nThrottleFree: " << (ta.throttlingPrevented ? "SIM" : "NAO");
    r = ss.str();
    return r.c_str();
}

const char* runInputBufferFlushTest(void) {
    static thread_local std::string r;
    auto ib = Optimizer::runZeroLatencyInputBufferFlush();
    std::ostringstream ss;
    ss << "=== Input Buffer ===\nFlushed: " << ib.flushedEventsCount
       << "\nDelay: " << ib.inputDelayMs << "ms";
    r = ss.str();
    return r.c_str();
}

const char* runQuantumHealthCheckTest(void) {
    static thread_local std::string r;
    auto qh = Optimizer::runQuantumEngineHealthCheck();
    std::ostringstream ss;
    ss << "=== Quantum Health ===\nFunctions: " << qh.totalNativeFunctionsActive
       << "\nNEON: " << (qh.armNeonSimdTensorOk ? "OK" : "FAIL")
       << "\nHealth: " << qh.overallEngineHealthPercent << "%";
    r = ss.str();
    return r.c_str();
}

const char* runCachelineAllocatorTest(void) {
    static thread_local std::string r;
    auto ca = Optimizer::runCachelineAligned64BAllocator();
    std::ostringstream ss;
    ss << "=== Cacheline ===\nAlloc: " << ca.allocatedBytes
       << "B\nAlign: " << ca.alignmentBytes << "B"
       << "\nThroughput: " << ca.throughputGbSec << "GB/s";
    r = ss.str();
    return r.c_str();
}

const char* runHapticAudioFusionTest(void) {
    static thread_local std::string r;
    auto ha = Optimizer::runHapticAudioFusionSync();
    std::ostringstream ss;
    ss << "=== Haptic Audio ===\nDir: " << ha.footstepDirectionDegrees
       << "deg\nIntensity: " << ha.hapticIntensityPercent << "%";
    r = ss.str();
    return r.c_str();
}

const char* runPolymorphicProtectorTest(void) {
    static thread_local std::string r;
    auto pp = Optimizer::runPolymorphicBinaryProtector();
    std::ostringstream ss;
    ss << "=== Polymorphic ===\nHash: " << pp.currentKeyHash
       << "\nBlocks: " << pp.obfuscatedMemoryBlocks
       << "\nValid: " << (pp.signatureIntegrityValid ? "SIM" : "NAO");
    r = ss.str();
    return r.c_str();
}

const char* runAdaptiveGyroKalmanTest(void) {
    static thread_local std::string r;
    auto ag = Optimizer::runAdaptiveGyroKalmanFusion();
    std::ostringstream ss;
    ss << "=== Adaptive Gyro ===\nPitch: " << ag.fusedPitch
       << "\nYaw: " << ag.fusedYaw
       << "\nJitter: " << ag.jitterVariance;
    r = ss.str();
    return r.c_str();
}

const char* runZeroStutterShaderMatrixTest(void) {
    static thread_local std::string r;
    auto zs = Optimizer::runZeroStutterShaderMatrix();
    std::ostringstream ss;
    ss << "=== Zero Stutter ===\nShaders: " << zs.precompiledShadersCount
       << "\nWarmup: " << zs.warmupDurationMs << "ms";
    r = ss.str();
    return r.c_str();
}

const char* runDynamicResolutionScalerTest(void) {
    static thread_local std::string r;
    auto dr = Optimizer::runDynamicResolutionScalerAssist();
    std::ostringstream ss;
    ss << "=== Dynamic Res ===\nScale: " << dr.currentScaleRatio
       << "\nFPS: " << dr.stabilizedFps;
    r = ss.str();
    return r.c_str();
}

const char* runBatteryThrottleShieldTest(void) {
    static thread_local std::string r;
    auto bt = Optimizer::runLowPowerBatteryThrottleShield();
    std::ostringstream ss;
    ss << "=== Battery Shield ===\nLevel: " << bt.batteryLevel
       << "%\nCPU: " << bt.cpuClockPreservedGhz << "GHz";
    r = ss.str();
    return r.c_str();
}

const char* runAntiAliasingFxaaTest(void) {
    static thread_local std::string r;
    auto fx = Optimizer::runAntiAliasingFxaaEdgeFilter();
    std::ostringstream ss;
    ss << "=== FXAA ===\nPixels: " << fx.processedPixels
       << "\nTime: " << fx.executionTimeUs << "us";
    r = ss.str();
    return r.c_str();
}

const char* runSubMillisecondInputBufferTest(void) {
    static thread_local std::string r;
    auto sb = Optimizer::runSubMillisecondInputLatencyBuffer();
    std::ostringstream ss;
    ss << "=== Sub-ms Input ===\nLatency: " << sb.inputLatencyMs
       << "ms\nCapacity: " << sb.queueCapacity;
    r = ss.str();
    return r.c_str();
}

const char* runMultiThreadedPhysicsTest(void) {
    static thread_local std::string r;
    auto mp = Optimizer::runMultiThreadedPhysicsSolver();
    std::ostringstream ss;
    ss << "=== Physics MT ===\nProjectiles: " << mp.simulatedProjectiles
       << "\nAvgTime: " << mp.averageTravelTimeMs << "ms";
    r = ss.str();
    return r.c_str();
}

const char* runVerticalFlickCurveTest(float velY) {
    static thread_local std::string r;
    auto vf = Optimizer::runVerticalFlickHeadshotCurve(velY);
    std::ostringstream ss;
    ss << "=== Vertical Flick ===\nMult: " << vf.dynamicSensitivityMultiplier
       << "\nHeadLock: " << vf.headzoneLockStrengthPercent << "%";
    r = ss.str();
    return r.c_str();
}

const char* runWeaponRecoilProfileTest(int categoryInt, int shotNumber) {
    static thread_local std::string r;
    auto wr = Optimizer::runWeaponCategoryRecoilProfile(categoryInt, shotNumber);
    std::ostringstream ss;
    ss << "=== Weapon Recoil ===\nCategory: " << wr.weaponCategoryName
       << "\nVert: " << wr.verticalCompensationPixels << "px"
       << "\nBurst: " << wr.optimalBurstCadenceMs << "ms";
    r = ss.str();
    return r.c_str();
}

const char* runBinauralAudioTrackingTest(float leftDb, float rightDb) {
    static thread_local std::string r;
    auto ba = Optimizer::runBinauralStereoAudioFootstepTracking(leftDb, rightDb);
    std::ostringstream ss;
    ss << "=== Binaural ===\nAzimuth: " << ba.estimatedAzimuthAngleDeg
       << "deg\nDir: " << ba.dominantFlankDirection
       << "\nConf: " << ba.confidenceScorePercent << "%";
    r = ss.str();
    return r.c_str();
}

} // extern "C"
