#ifndef PERFORMANCE_OPTIMIZER_HPP
#define PERFORMANCE_OPTIMIZER_HPP

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>
#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>

#if defined(__ARM_NEON) || defined(__ARM_NEON__)
#include <arm_neon.h>
#endif

namespace Optimizer {

    // Process Memory Metrics (VSS & RSS)
    struct ProcessMemoryStats {
        long vssKb; // Virtual Set Size
        long rssKb; // Resident Set Size (Physical RAM used by this process)
    };

    ProcessMemoryStats getProcessMemoryStats();

    // Contiguous Memory Arena Allocator for Deep RAM Optimization
    class MemoryArena {
    public:
        MemoryArena(size_t sizeBytes = 2 * 1024 * 1024); // 2 MB Arena
        ~MemoryArena();

        void* allocate(size_t bytes, size_t alignment = 8);
        void reset();

        size_t totalSize() const { return m_size; }
        size_t usedSize() const { return m_offset; }

    private:
        char* m_buffer;
        size_t m_size;
        size_t m_offset;
    };

    // Cache Benchmark Result (AoS vs SoA in RAM)
    struct CacheBenchmarkResult {
        double aosDurationMs;
        double soaDurationMs;
        double speedupFactor;
    };

    CacheBenchmarkResult runCacheOptimizationBenchmark(size_t elements = 1000000);

    // 3D Vector Math
    struct Vector3D {
        float x, y, z;

        Vector3D(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) : x(_x), y(_y), z(_z) {}

        Vector3D operator+(const Vector3D& other) const { return Vector3D(x + other.x, y + other.y, z + other.z); }
        Vector3D operator-(const Vector3D& other) const { return Vector3D(x - other.x, y - other.y, z - other.z); }
        Vector3D operator*(float scalar) const { return Vector3D(x * scalar, y * scalar, z * scalar); }

        float length() const { return std::sqrt(x * x + y * y + z * z); }
        Vector3D normalized() const {
            float len = length();
            return (len > 0.0f) ? Vector3D(x / len, y / len, z / len) : Vector3D(0, 0, 0);
        }
    };

    template <typename T, size_t PoolSize = 1024>
    class MemoryPool {
    public:
        MemoryPool() : m_nextIndex(0) {}

        T* allocate() {
            if (m_nextIndex < PoolSize) {
                return &m_pool[m_nextIndex++];
            }
            return nullptr;
        }

        void reset() { m_nextIndex = 0; }
        size_t capacity() const { return PoolSize; }
        size_t used() const { return m_nextIndex; }

    private:
        T m_pool[PoolSize];
        size_t m_nextIndex;
    };

    // Game Mechanics
    enum class MovementState { Idle = 0, Walking = 1, Running = 2, Jumping = 3, Sprinting = 4 };
    float calculateCrosshairSpread(MovementState state, float baseRadius = 10.0f);

    struct RecoilResult {
        float pitchKick;
        float yawKick;
        float recoveredPitch;
        float recoveredYaw;
    };
    RecoilResult calculateCameraRecoil(int shotNumber, float currentPitch, float currentYaw, float deltaTime);

    struct ScreenPoint {
        bool visibleOnScreen;
        float screenX;
        float screenY;
    };
    ScreenPoint projectWorldToScreen(const Vector3D& worldPos, const Vector3D& cameraPos, float fovDegrees, float screenWidth, float screenHeight);

    float calculateFOVSensitivityScale(float currentFOV, float baseFOV = 90.0f, float baseSensitivity = 1.0f);

    // 3D Collision Detection Structures
    struct BoundingBox3D {
        Vector3D min;
        Vector3D max;
        BoundingBox3D(const Vector3D& _min, const Vector3D& _max) : min(_min), max(_max) {}
    };

    struct BoundingSphere3D {
        Vector3D center;
        float radius;
        BoundingSphere3D(const Vector3D& _center, float _radius) : center(_center), radius(_radius) {}
    };

    bool checkAABBCollision(const BoundingBox3D& boxA, const BoundingBox3D& boxB);
    bool checkSphereCollision(const BoundingSphere3D& sphereA, const BoundingSphere3D& sphereB);

    // Vector Math & Interpolation
    float dotProduct(const Vector3D& a, const Vector3D& b);
    Vector3D crossProduct(const Vector3D& a, const Vector3D& b);
    float angleBetweenDegrees(const Vector3D& a, const Vector3D& b);
    bool isObjectInFOV(const Vector3D& cameraDir, const Vector3D& targetOffset, float fovDegrees);

    Vector3D lerpVector(const Vector3D& start, const Vector3D& end, float t);
    Vector3D smoothExponential(const Vector3D& current, const Vector3D& target, float smoothingFactor, float deltaTime);

    struct HardwareStats {
        int cpuCores;
        long totalMemoryMb;
        long freeMemoryMb;
        std::string cpuArchitecture;
        bool supportsNeon;
    };

    HardwareStats getHardwareStats();

    struct SystemDiagnosticReport {
        bool memoryPoolOk;
        bool threadPoolOk;
        bool gyroEngineOk;
        bool vectorEngineOk;
        bool collisionEngineOk;
        double diagnosticDurationMs;
    };

    SystemDiagnosticReport runSystemDiagnostic();

    class GyroscopeCamera360 {
    public:
        GyroscopeCamera360();
        void updateGyro(float gyroX, float gyroY, float gyroZ, float deltaTime);

        float getYaw360() const { return m_yawDegrees; }
        float getPitch() const { return m_pitchDegrees; }
        float getRoll() const { return m_rollDegrees; }

        void reset();

    private:
        float m_yawDegrees;
        float m_pitchDegrees;
        float m_rollDegrees;
    };

    class FramePacer {
    public:
        FramePacer(double targetFps = 60.0);
        void startFrame();
        double endFrameAndSleep();

    private:
        double m_targetFrameDurationMs;
        std::chrono::high_resolution_clock::time_point m_frameStart;
    };

    class ThreadPool {
    public:
        ThreadPool(size_t threads = std::thread::hardware_concurrency());
        ~ThreadPool();

        void enqueue(std::function<void()> task);

    private:
        std::vector<std::thread> m_workers;
        std::queue<std::function<void()>> m_tasks;
        std::mutex m_queueMutex;
        std::condition_variable m_cv;
        bool m_stop;
    };

    struct VectorMathResult {
        double scalarDurationMs;
        double simdDurationMs;
        double speedupRatio;
    };

    VectorMathResult runVectorMathBenchmark(size_t elements = 1000000);

    // 1. Target Trajectory & Lead Shot Prediction (Enemy position forecasting)
    struct LeadPredictionResult {
        Vector3D predictedPosition;
        float timeToImpactSeconds;
        float elevationCorrectionAngle;
    };
    LeadPredictionResult predictTargetLeadPosition(
        const Vector3D& targetPos,
        const Vector3D& targetVelocity,
        const Vector3D& shooterPos,
        float bulletVelocity = 800.0f,
        float gravity = 9.8f
    );

    // 2. Touch Response & Input Lag Prediction
    struct TouchPrediction {
        float predictedRawX;
        float predictedRawY;
        float latencyReductionMs;
    };
    TouchPrediction predictTouchPosition(float currentX, float currentY, float deltaX, float deltaY, float samplingIntervalMs = 8.0f);

    // 3. Bezier Aim Trajectory Smoothing (S-Curve Point Generation)
    struct BezierPoint {
        float x;
        float y;
    };
    std::vector<BezierPoint> generateBezierAimPath(float startX, float startY, float targetX, float targetY, float controlOffsetFactor = 0.25f, int steps = 10);

    // 4. Advanced CPU Thermal Throttling & Priority Governor
    struct CpuGovernorReport {
        std::string governorPolicy;
        int activeThreadsMaxPriority;
        bool thermalThrottlingPrevented;
        float cpuFrequencyGhz;
        float cpuTempCelsius;
    };
    CpuGovernorReport optimizeCpuGovernorSettings();

    // 5. Enhanced Anti-Recoil Compensation (0-100%)
    struct RecoilCompensationResult {
        float originalPitch;
        float originalYaw;
        float compensatedPitch;
        float compensatedYaw;
        float reductionPercentage;
    };
    RecoilCompensationResult calculateAdvancedRecoilCompensation(int shotNumber, float recoilStrengthPercent);

    // 6. Enhanced Dynamic Spread Reduction (0-100%)
    struct SpreadReductionResult {
        float originalSpreadRadius;
        float reducedSpreadRadius;
        float accuracyBoostPercent;
    };
    SpreadReductionResult calculateAdvancedSpreadReduction(MovementState state, float spreadReductionPercent);

    // 7. Enhanced Touch Sampling Rate Optimizer (240Hz / 360Hz / 480Hz)
    struct TouchSamplingOptimizerResult {
        int targetHz;
        float inputLagMs;
        float smoothnessScore;
    };
    TouchSamplingOptimizerResult optimizeTouchSamplingRate(int targetHzMode);

    // 8. Kalman Filter Aim Tracking (Predictive Target Smoothing)
    struct KalmanAimState {
        float estimatedX;
        float estimatedY;
        float predictedNextX;
        float predictedNextY;
        float accuracyScorePercent;
    };
    KalmanAimState runKalmanFilterAimTracking(float currentX, float currentY, float velocityX, float velocityY, float dt = 0.016f);

    // 9. Adaptive Gyroscope Deadzone & Tremor Filter
    struct GyroFilterResult {
        float filteredYaw;
        float filteredPitch;
        float filteredRoll;
        bool tremorFiltered;
    };
    GyroFilterResult runAdaptiveGyroFilter(float rawYaw, float rawPitch, float rawRoll, float deadzoneThreshold = 0.05f);

    // 10. Display VRR Sync (120Hz / 144Hz Frame Pacing)
    struct DisplaySyncResult {
        int targetRefreshRateHz;
        float frameTimeTargetMs;
        bool vrrSyncActive;
        float jitterReductionPercent;
    };
    DisplaySyncResult runDisplayVrrSync(int targetHz = 120);

    // 11. Native Linux RAM Defragmenter & Cache Trimmer
    struct RamDefragResult {
        size_t memoryTrimmedBytes;
        float freeRamMb;
        bool fragmentationReduced;
    };
    RamDefragResult runNativeRamDefragmenter();

    // 12. CPU Core Affinity & Big-Core Locking
    struct CpuAffinityResult {
        int pinnedCoreId;
        int totalBigCores;
        bool affinityLockSuccess;
    };
    CpuAffinityResult runCpuCoreAffinityLocking();

    // 13. Dynamic Headshot Elevation Angle Calculator
    struct ElevationAngleResult {
        float distanceMeters;
        float elevationAngleDegrees;
        float targetHeightOffsetMeters;
        float bulletFlightTimeMs;
    };
    ElevationAngleResult calculateHeadshotElevationAngle(float distanceMeters, float targetHeightMeters = 1.75f, float bulletSpeed = 800.0f);

    // 14. Real-Time C++ Native Engine Telemetry Verification
    struct EngineVerificationResult {
        double loopExecutionTimeMicroseconds;
        int activeCoresCount;
        float totalRamMb;
        float freeRamMb;
        float processRssMb;
        double simdSpeedupRatio;
        std::string coreFrequenciesText;
        bool serviceRunningOK;
    };
    EngineVerificationResult verifyRealtimeEngineMetrics();

    // 15. Multi-Target Directional Vector Tracking Array
    struct MultiTargetVectorResult {
        int trackedTargetsCount;
        float snapTargetAngleDegrees;
        bool multiTargetLockActive;
    };
    MultiTargetVectorResult runMultiTargetVectorTracking(int enemyCount = 4);

    // 16. Recoil Pattern Reset Forecast & Timing
    struct RecoilResetResult {
        float recoilResetTimeMs;
        bool readyForBurstFire;
    };
    RecoilResetResult predictRecoilResetTime(float currentRecoilPitch);

    // 17. Sub-Pixel Precision Touch Interpolator
    struct SubPixelTouchResult {
        float subPixelX;
        float subPixelY;
        bool jitterEliminated;
    };
    SubPixelTouchResult runSubPixelTouchInterpolator(float rawTouchX, float rawTouchY);

    // 18. Touch Contact Pressure Curve Normalizer
    struct TouchPressureResult {
        float normalizedPressure;
        bool pressureStabilized;
    };
    TouchPressureResult runTouchPressureNormalizer(float rawPressure);

    // 19. GPU Devfreq Adreno/Mali Frequency Locking
    struct GpuDevfreqResult {
        int gpuFrequencyMhz;
        bool gpuLockActive;
    };
    GpuDevfreqResult runGpuDevfreqLock();

    // 20. Sub-5ms Audio Dynamic Latency Minimizer
    struct AudioLatencyResult {
        float audioLatencyMs;
        bool bufferOptimized;
    };
    AudioLatencyResult runAudioLatencyMinimizer();
    AudioLatencyResult runAudioLatencyMinimizer(float sampleRate, int periodFrames);

    // 21. Vulkan/OpenGL Shader Cache Pre-Warm & Pipeline Allocation
    struct ShaderCacheResult {
        int prewarmedShadersCount;
        bool stutteringPrevented;
    };
    ShaderCacheResult runShaderCachePrewarm();

    // 22. Native Security Encryption & Checksum Guard (AES-256 / XOR Memory Shield)
    struct SecurityGuardStatus {
        bool isEncrypted;
        bool memoryChecksumValid;
        std::string securityHashSignature;
    };
    SecurityGuardStatus verifyEngineSecurityGuard();

    // 23. Unified AI Neural Predictor Engine (Cross-System Predictive Tuning)
    struct AiNeuralPredictorResult {
        float predictedRamAllocationMb;
        int predictedOptimalTouchHz;
        float predictedKalmanGain;
        float predictedThermalThrottlingRisk;
        float predictedGyroDeadzone;
        bool neuralOptimizationActive;
        std::string aiPredictorStatusText;
    };
    AiNeuralPredictorResult runAiNeuralPredictorEngine();

    // 24. AI Quantum Neural Engine v2.0 (ARM NEON SIMD Tensor Core & 3D Quaternion Target Focus)
    // Feeds all engine functions with adaptive predicted parameters
    struct Quaternion3D {
        float w, x, y, z;
    };

    // Adaptive parameters computed by AI Quantum Engine v3.0
    // These are injected into all aim functions for cross-system auto-tuning
    struct AiQuantumAdaptiveParams {
        float adaptiveKalmanGain;        // Optimal Kalman gain predicted by AI (0.001–0.05)
        float adaptiveRecoilCurveFactor; // Exponential recoil curve factor for burst duration
        float adaptiveSnapAngleDeg;      // Minimum snap angle for multi-target priority
        float adaptiveSubPixelBias;      // Sub-pixel coordinate forward bias (touch prediction)
        float adaptiveGyroDamping;       // Gyro deadzone damping factor for scope smoothness
        float adaptiveTouchHzBoost;      // Dynamic touch sampling Hz (240–480)
        float adaptiveBulletLeadMult;    // Bullet lead prediction multiplier (0.5–2.0)
        float adaptiveSensitivityScale;  // Global sensitivity scale (0.5–1.5)
        int   combatModeIndex;           // 0=Long, 1=Medium, 2=CQC Rush
    };

    // Neural Network: 8 inputs → 16 hidden (ReLU) → 7 outputs (Sigmoid)
    struct QuantumNeuralNet {
        static constexpr int INPUT_SIZE  = 8;
        static constexpr int HIDDEN_SIZE = 16;
        static constexpr int OUTPUT_SIZE = 7;

        // Weights: input→hidden (8×16=128), hidden→output (16×7=112)
        float weights_ih[INPUT_SIZE * HIDDEN_SIZE];
        float bias_h[HIDDEN_SIZE];
        float weights_ho[HIDDEN_SIZE * OUTPUT_SIZE];
        float bias_o[OUTPUT_SIZE];

        // Temporal memory: last 8 engagement snapshots for online learning
        struct EngagementSnapshot {
            float inputs[INPUT_SIZE];
            float outputs[OUTPUT_SIZE];
            float score;  // 0.0–1.0 success rating
            bool  valid;
        };
        static constexpr int MEMORY_SIZE = 8;
        EngagementSnapshot memory[MEMORY_SIZE];
        int memoryIndex;

        float hiddenBuffer[HIDDEN_SIZE];

        void init();
        void forward(const float inputs[INPUT_SIZE], float outputs[OUTPUT_SIZE]);
        void onlineLearn(const float inputs[INPUT_SIZE], const float targets[OUTPUT_SIZE], float learningRate);
        void recordEngagement(const float inputs[INPUT_SIZE], const float outputs[OUTPUT_SIZE], float score);
    };

    struct AiQuantumEngineResult {
        double subMicrosecondInferenceTimeUs;
        float targetFocusAlignmentScore;
        float predictionConfidence;       // Neural net output confidence (0–100%)
        int   neuralNetEpochs;            // How many online learning steps performed
        Quaternion3D predictedQuaternionRotation;
        AiQuantumAdaptiveParams adaptiveParams;
        bool armNeonTensorCoreActive;
        bool fftAudioFootstepDetectorActive;
        bool allFunctionsFed;             // True when all engine functions received adaptive params
        std::string quantumEngineStatusText;
    };
    AiQuantumEngineResult runAiQuantumNeuralEngine(
        float targetX  = 500.0f, float targetY  = 600.0f, float targetZ  = 10.0f,
        float velX = 0.0f, float velY = 0.0f, float velZ = 0.0f,
        float gyroNoise = 0.0f, float thermalPercent = 0.0f);

    // Global adaptive params — updated every cycle by AI Quantum Engine v3.0
    // All engine functions read from this to get AI-tuned parameters
    extern AiQuantumAdaptiveParams g_quantumAdaptiveParams;

    // 25. Dynamic Scope Stabilizer (2x / 4x / 8x Scope Deadzone Auto-Tuning)
    struct ScopeStabilizerResult {
        float scopeMagnification;
        float stabilizedPitch;
        float stabilizedYaw;
        bool scopeLockActive;
    };
    ScopeStabilizerResult runDynamicScopeStabilizer(float scopeMagnification = 4.0f);

    // 26. Bullet Velocity & Drop Compensator (Lead Distance Calculation)
    struct BulletVelocityResult {
        float calculatedLeadDistance;
        float bulletTimeOfFlightMs;
        float verticalDropCompensation;
    };
    BulletVelocityResult runBulletVelocityCompensation(float bulletSpeed = 900.0f, float distance = 250.0f);

    // 27. Magnet Snap Vector (Magnetic Target Alignment)
    struct MagnetSnapResult {
        float snapDeltaX;
        float snapDeltaY;
        float magnetPowerPercentage;
        bool snapLockAcquired;
    };
    MagnetSnapResult runMagnetSnapVector(float crosshairX = 500.0f, float crosshairY = 600.0f, float targetX = 505.0f, float targetY = 602.0f);

    // 28. FPS Frame Stabilizer & Micro-Stutter Eliminator
    struct FpsStabilizerResult {
        int targetFps;
        float frameVarianceMs;
        bool stutterEliminated;
    };
    FpsStabilizerResult runFpsFrameStabilizer(int targetFps = 120);

    // 29. Touch Jitter Smoothing Filter (High-Frequency Touch Noise Cancellation)
    struct TouchJitterResult {
        float smoothedX;
        float smoothedY;
        float noiseReductionPercent;
    };
    TouchJitterResult runTouchJitterSmoothingFilter(float touchRawX = 500.0f, float touchRawY = 600.0f);

    // 30. Vulkan Fast Pipeline Preloader (Instant Weapon Swap & Fire Render)
    struct VulkanPipelineResult {
        int loadedPipelinesCount;
        bool instantSwapReady;
    };
    VulkanPipelineResult runVulkanPipelinePreloader();

    // 31. Dual-Arena L1/L2 RAM Defragmenter & Purger
    struct DualArenaMemoryResult {
        size_t freedMemoryBytes;
        size_t l1CacheBufferBytes;
        size_t kernelHeapPurgedBytes;
        bool dualPurgeSuccessful;
    };
    DualArenaMemoryResult runDualArenaMemoryDefrag();

    // 32. Cubic Spline Future Touch Position Predictor (1-2 Frames Ahead)
    struct SplineTouchPredictionResult {
        float futureTouchX;
        float futureTouchY;
        float leadTimeMs;
        bool predictionValid;
    };
    SplineTouchPredictionResult runFutureTouchPredictionSpline(float currentX = 500.0f, float currentY = 600.0f, float velocityX = 15.0f, float velocityY = 20.0f);

    // 33. FFT Audio Footstep Frequency Spectrogram Isolator (1kHz-3kHz)
    struct AudioSpectrogramResult {
        float footstepFrequencyPeakHz;
        float directionAngleDegrees;
        float isolatedSignalGainDb;
        bool footstepDetected;
        std::string analysisStatusText;
    };
    AudioSpectrogramResult runFootstepAudioFFTSpectrogram();
    // Overload that performs a REAL DFT over a captured audio buffer (sampleRate in Hz)
    AudioSpectrogramResult runFootstepAudioFFTSpectrogram(const float* samples, size_t sampleCount, float sampleRate = 44100.0f);

    // 34. Rapid-Fire Tap Cadence Optimizer (Single-Tap Rate Maximizer)
    struct RapidFireTapResult {
        float optimalTapIntervalMs;
        float maxAchievableRps;
        bool spreadBloomPrevented;
    };
    RapidFireTapResult runRapidFireTapOptimizer();

    // 35. Target Lock Persistence Filter (Cross-Target Lock Protection)
    struct TargetLockPersistenceResult {
        int primaryTargetId;
        float lockStrengthPercent;
        bool lockSwitchPrevented;
    };
    TargetLockPersistenceResult runTargetLockPersistenceFilter(int targetId = 1, float deltaX = 2.0f, float deltaY = 1.5f);

    // 36. Dynamic Scope Gyroscope Sensitivity Scaler
    struct GyroSensitivityScaleResult {
        float scopeMagnification;
        float scaledGyroSensX;
        float scaledGyroSensY;
        bool autoScaled;
    };
    GyroSensitivityScaleResult runDynamicGyroSensitivityScaler(float scopeMagnification = 4.0f, float baseSens = 1.0f);

    // 37. Multi-Game Optimization Profile Auto-Selector
    struct GamePresetProfileResult {
        std::string gameName;
        int activeTouchHz;
        float activeKalmanGain;
        std::string presetStatusText;
    };
    GamePresetProfileResult runGamePresetAutoSelector(int gameId = 0); // 0=FreeFire, 1=PUBG, 2=CoD

    // 38. Hardware Thermal Anticipator (dTemp/dt Temperature Derivative Forecast)
    struct ThermalAnticipatorResult {
        float currentTempC;
        float temperatureDerivativeCPerMin;
        float forecastedTemp10MinC;
        bool throttlingPrevented;
    };
    ThermalAnticipatorResult runHardwareThermalAnticipator();

    // 39. Zero-Latency Android Input Queue Buffer Flusher
    struct InputBufferFlushResult {
        int flushedEventsCount;
        float inputDelayMs;
        bool zeroLagActive;
    };
    InputBufferFlushResult runZeroLatencyInputBufferFlush();

    // 40. Quantum AI Engine v4.0 Deep Health Auditor
    struct QuantumHealthCheckResult {
        int totalNativeFunctionsActive;
        bool armNeonSimdTensorOk;
        bool quaternionMathOk;
        double overallEngineHealthPercent;
        std::string systemStatusSignature;
    };
    QuantumHealthCheckResult runQuantumEngineHealthCheck();

    // 41. Cacheline-Aligned 64-Byte Allocator (Zero L1/L2 Cache-Miss SIMD Arena)
    struct CachelineAllocatorResult {
        size_t allocatedBytes;
        size_t alignmentBytes;
        bool zeroCacheMissVerified;
        double throughputGbSec;
    };
    CachelineAllocatorResult runCachelineAligned64BAllocator(size_t allocSize = 64 * 1024);

    // 42. Haptic & Audio FFT Directional Fusion Sync (Millimeter Haptic Pulse)
    struct HapticAudioFusionResult {
        float footstepDirectionDegrees;
        int hapticIntensityPercent;
        int pulsePatternDurationMs;
        bool hapticSyncActive;
    };
    HapticAudioFusionResult runHapticAudioFusionSync(float audioFootstepFreqHz = 2400.0f, float directionDegrees = 45.0f);

    // 43. Polymorphic Dynamic Binary Protector v2.0 (25ms Rotating Security Keys)
    struct PolymorphicProtectorResult {
        uint32_t currentKeyHash;
        int obfuscatedMemoryBlocks;
        bool signatureIntegrityValid;
        std::string securityShieldStatus;
    };
    PolymorphicProtectorResult runPolymorphicBinaryProtector(uint32_t cycleNonce = 100);

    // 44. Adaptive Gyroscope + Kalman Stochastic Fusion (8x Scope Laser Lock)
    struct AdaptiveGyroKalmanResult {
        float fusedPitch;
        float fusedYaw;
        float jitterVariance;
        bool gyroKalmanLocked;
    };
    AdaptiveGyroKalmanResult runAdaptiveGyroKalmanFusion(float gyroRoll = 0.5f, float gyroPitch = 1.2f, float gyroYaw = 0.8f);

    // 45. Zero-Stutter Shader Matrix & Pipeline Pre-Warming (128 Vulkan Pipelines)
    struct ZeroStutterShaderMatrixResult {
        int precompiledShadersCount;
        float warmupDurationMs;
        bool zeroStutterReady;
    };
    ZeroStutterShaderMatrixResult runZeroStutterShaderMatrix(int pipelineCount = 128);

    // 46. Dynamic Resolution Scaler & 120 FPS Pacing Assist
    struct DynamicResolutionScalerResult {
        float currentScaleRatio;
        int stabilizedFps;
        bool frameDropPrevented;
        float targetGpuLoadPercent;
    };
    DynamicResolutionScalerResult runDynamicResolutionScalerAssist(int currentFps = 118, int targetFps = 120);

    // 47. Low-Power Battery Thermal & Throttle Shield
    struct BatteryThrottleShieldResult {
        int batteryLevel;
        float cpuClockPreservedGhz;
        bool thermalGovernorUnlocked;
        std::string shieldState;
    };
    BatteryThrottleShieldResult runLowPowerBatteryThrottleShield(int batteryLevelPercent = 18, bool isCharging = false);

    // 48. Ultra-Fast FXAA Edge Filter & Distance Target Enhancer
    struct AntiAliasingFxaaResult {
        int processedPixels;
        float executionTimeUs;
        float edgeSharpnessBoost;
        bool fxaaEnabled;
    };
    AntiAliasingFxaaResult runAntiAliasingFxaaEdgeFilter(int screenWidth = 1080, int screenHeight = 2400);

    // 49. Sub-Millisecond Input Latency Buffer (Circular Queue < 0.7ms)
    struct SubMillisecondInputBufferResult {
        float inputLatencyMs;
        int queueCapacity;
        int droppedStaleEvents;
        bool subMillisecondLatencyAchieved;
    };
    SubMillisecondInputBufferResult runSubMillisecondInputLatencyBuffer(int pendingEvents = 4);

    // 50. Multi-Threaded Ballistics Physics Solver (Parallel Trajectory & Wind Drift)
    struct MultiThreadedPhysicsResult {
        int simulatedProjectiles;
        float averageTravelTimeMs;
        float windDriftCorrection;
        bool parallelExecutionOk;
    };
    MultiThreadedPhysicsResult runMultiThreadedPhysicsSolver(int bulletCount = 30, float muzzleVelocity = 920.0f);

    // 51. Vertical Flick Exponential Curve & Headshot Hitbox Lock
    struct VerticalFlickCurveResult {
        float dynamicSensitivityMultiplier;
        float adjustedCrosshairY;
        float headzoneLockStrengthPercent;
        bool headshotLockAcquired;
        std::string flickStatusText;
    };
    VerticalFlickCurveResult runVerticalFlickHeadshotCurve(float dragVelocityY = 25.0f, float currentCrosshairY = 600.0f, float headTargetY = 520.0f);

    // 52. Weapon Category-Specific Recoil & Spread Profile (SMG / AR / Sniper / Shotgun)
    enum class WeaponCategory { SMG = 0, AR = 1, Sniper = 2, Shotgun = 3 };
    struct WeaponCategoryRecoilResult {
        std::string weaponCategoryName;
        float verticalCompensationPixels;
        float horizontalSpreadReductionPercent;
        float optimalBurstCadenceMs;
        bool burstStabilityActive;
        std::string profileSummary;
    };
    WeaponCategoryRecoilResult runWeaponCategoryRecoilProfile(int categoryInt = 0, int shotNumber = 1, float burstDurationMs = 150.0f);

    // 53. Binaural Stereo Audio Footstep Tracking (Left/Right Channel Azimuth)
    struct BinauralAudioResult {
        float leftChannelGainDb;
        float rightChannelGainDb;
        float estimatedAzimuthAngleDeg;
        std::string dominantFlankDirection; // "ESQUERDA", "DIREITA", "CENTRO"
        float confidenceScorePercent;
        bool enemyFootstepLocalized;
    };
    BinauralAudioResult runBinauralStereoAudioFootstepTracking(float leftChannelDb = -18.0f, float rightChannelDb = -28.0f);

} // namespace Optimizer

#endif // PERFORMANCE_OPTIMIZER_HPP



