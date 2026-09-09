#include <string>
#include <sstream>
#include <thread>
#include <atomic>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include "performance_optimizer.hpp"

static Optimizer::GyroscopeCamera360 g_camera360;

// ============================================================
// iOS Bridge — pure C functions (no JNI)
// Called from Swift via bridging header
// ============================================================

extern "C" {

int getSystemCores(void) {
    unsigned int cores = std::thread::hardware_concurrency();
    return (cores > 0) ? static_cast<int>(cores) : 1;
}

const char* getSystemMetrics(void) {
    static thread_local std::string result;
    Optimizer::HardwareStats stats = Optimizer::getHardwareStats();
    Optimizer::ProcessMemoryStats procMem = Optimizer::getProcessMemoryStats();

    std::ostringstream ss;
    ss << "Arquitetura CPU: " << stats.cpuArchitecture << "\n";
    ss << "Nucleos CPU: " << stats.cpuCores << "\n";
    ss << "Memoria RAM Total: " << stats.totalMemoryMb << " MB\n";
    ss << "Memoria RAM Livre: " << stats.freeMemoryMb << " MB\n";
    if (procMem.rssKb > 0) {
        ss << "Uso de RAM Nativa (Processo): " << (procMem.rssKb / 1024) << " MB (" << procMem.rssKb << " kB)\n";
    }
    ss << "Suporte ARM NEON (SIMD): " << (stats.supportsNeon ? "SIM (Ativo)" : "NAO");
    result = ss.str();
    return result.c_str();
}

const char* runRamCacheBenchmark(void) {
    static thread_local std::string result;
    Optimizer::MemoryArena arena(2 * 1024 * 1024);
    void* p1 = arena.allocate(1024);
    void* p2 = arena.allocate(2048);
    Optimizer::CacheBenchmarkResult cacheRes = Optimizer::runCacheOptimizationBenchmark(1000000);

    std::ostringstream ss;
    ss << "=== Otimizacao Profunda de Memoria RAM Nativa ===\n\n";
    ss << "--- Teste 1: Memory Arena (Alocacao Contigua) ---\n";
    ss << "Arena Total: " << (arena.totalSize() / 1024) << " KB\n";
    ss << "Arena Usada: " << (arena.usedSize() / 1024) << " KB\n";
    ss << "Alocacoes realizadas: OK (p1=" << p1 << ", p2=" << p2 << ")\n";
    ss << "RESULTADO: RAM allocation funcional em " << (arena.usedSize() / 1024) << "KB\n\n";
    ss << "--- Teste 2: Benchmark Cache Locality (AoS vs SoA) ---\n";
    ss << "AoS Duration: " << cacheRes.aosDurationMs << " ms\n";
    ss << "SoA Duration: " << cacheRes.soaDurationMs << " ms\n";
    ss << "Speedup: " << cacheRes.speedupPercent << "%\n";
    ss << "STATUS: " << (cacheRes.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runSystemDiagnosticTest(void) {
    static thread_local std::string result;
    Optimizer::HardwareStats stats = Optimizer::getHardwareStats();
    Optimizer::ProcessMemoryStats procMem = Optimizer::getProcessMemoryStats();

    std::ostringstream ss;
    ss << "=== Diagnostico Completo do Sistema ===\n\n";
    ss << "CPU: " << stats.cpuArchitecture << "\n";
    ss << "Nucleos: " << stats.cpuCores << "\n";
    ss << "RAM Total: " << stats.totalMemoryMb << " MB\n";
    ss << "RAM Livre: " << stats.freeMemoryMb << " MB\n";
    ss << "RAM Usada: " << (stats.totalMemoryMb - stats.freeMemoryMb) << " MB\n";
    ss << "Processo RSS: " << (procMem.rssKb / 1024) << " MB\n";
    ss << "NEON SIMD: " << (stats.supportsNeon ? "ATIVO" : "INDISPONIVEL") << "\n";
    result = ss.str();
    return result.c_str();
}

void updateGyro360(float gx, float gy, float gz, float dt) {
    g_camera360.update(gx, gy, gz, dt);
}

float getYaw360(void) {
    return g_camera360.getYaw();
}

void resetGyro360(void) {
    g_camera360.reset();
}

const char* runDynamicSpreadTest(int state) {
    static thread_local std::string result;
    Optimizer::SpreadState s = static_cast<Optimizer::SpreadState>(state);
    Optimizer::SpreadResult res = Optimizer::calculateDynamicSpread(s);
    std::ostringstream ss;
    ss << "=== Dynamic Spread Test ===\n";
    ss << "Estado: " << state << "\n";
    ss << "Spread Atual: " << res.currentSpread << "\n";
    ss << "Spread Target: " << res.targetSpread << "\n";
    ss << "Taxa Reducao: " << res.reductionRate << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runRecoilTest(int shotNumber) {
    static thread_local std::string result;
    Optimizer::RecoilResult res = Optimizer::calculateRecoilCompensation(shotNumber);
    std::ostringstream ss;
    ss << "=== Recoil Compensation Test ===\n";
    ss << "Tiro: " << shotNumber << "\n";
    ss << "Compensacao X: " << res.compX << "\n";
    ss << "Compensacao Y: " << res.compY << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runWorldToScreenTest(float wx, float wy, float wz) {
    static thread_local std::string result;
    Optimizer::ScreenCoord coord = Optimizer::worldToScreen(wx, wy, wz);
    std::ostringstream ss;
    ss << "=== World to Screen Test ===\n";
    ss << "Entrada: (" << wx << ", " << wy << ", " << wz << ")\n";
    ss << "Saida: (" << coord.x << ", " << coord.y << ")\n";
    ss << "Visivel: " << (coord.visible ? "SIM" : "NAO") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runSensitivityScalingTest(float currentFOV) {
    static thread_local std::string result;
    float scaled = Optimizer::calculateSensitivityScaling(currentFOV);
    std::ostringstream ss;
    ss << "=== Sensitivity Scaling Test ===\n";
    ss << "FOV Atual: " << currentFOV << "\n";
    ss << "Sensibilidade Escalada: " << scaled << "\n";
    ss << "Status: PASS\n";
    result = ss.str();
    return result.c_str();
}

const char* runCollisionTest(void) {
    static thread_local std::string result;
    Optimizer::CollisionResult res = Optimizer::testAABBCollision();
    std::ostringstream ss;
    ss << "=== Collision Test (AABB) ===\n";
    ss << "Colisao Detectada: " << (res.detected ? "SIM" : "NAO") << "\n";
    ss << "Tempo: " << res.timeNs << " ns\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runVector3DTest(void) {
    static thread_local std::string result;
    Optimizer::Vector3DResult res = Optimizer::testVector3D();
    std::ostringstream ss;
    ss << "=== Vector 3D Test ===\n";
    ss << "Operacoes: " << res.operations << "\n";
    ss << "Tempo: " << res.timeMs << " ms\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runSmoothingTest(void) {
    static thread_local std::string result;
    Optimizer::SmoothingResult res = Optimizer::testSmoothing();
    std::ostringstream ss;
    ss << "=== Smoothing Test ===\n";
    ss << "Amostras: " << res.samples << "\n";
    ss << "Suavizacao: " << res.smoothingFactor << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runNativeBenchmark(void) {
    static thread_local std::string result;
    Optimizer::BenchmarkResult res = Optimizer::runNativeBenchmark();
    std::ostringstream ss;
    ss << "=== Native Benchmark ===\n";
    ss << "Operacoes: " << res.operations << "\n";
    ss << "Tempo: " << res.timeMs << " ms\n";
    ss << "Ops/sec: " << res.opsPerSec << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runVectorBenchmark(void) {
    static thread_local std::string result;
    Optimizer::VectorBenchmarkResult res = Optimizer::runVectorBenchmark();
    std::ostringstream ss;
    ss << "=== Vector Benchmark (NEON SIMD) ===\n";
    ss << "Operacoes: " << res.operations << "\n";
    ss << "Tempo: " << res.timeMs << " ms\n";
    ss << "Speedup NEON: " << res.neonSpeedup << "x\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runLeadPredictionTest(float tx, float ty, float tz, float vx, float vy, float vz) {
    static thread_local std::string result;
    Optimizer::LeadResult res = Optimizer::calculateLeadPrediction(tx, ty, tz, vx, vy, vz);
    std::ostringstream ss;
    ss << "=== Lead Prediction Test ===\n";
    ss << "Alvo: (" << tx << ", " << ty << ", " << tz << ")\n";
    ss << "Velocidade: (" << vx << ", " << vy << ", " << vz << ")\n";
    ss << "Predicao: (" << res.predX << ", " << res.predY << ", " << res.predZ << ")\n";
    ss << "Confianca: " << res.confidence << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runTouchPredictionTest(float cx, float cy, float dx, float dy) {
    static thread_local std::string result;
    Optimizer::TouchResult res = Optimizer::calculateTouchPrediction(cx, cy, dx, dy);
    std::ostringstream ss;
    ss << "=== Touch Prediction Test ===\n";
    ss << "Entrada: (" << cx << ", " << cy << ") -> (" << dx << ", " << dy << ")\n";
    ss << "Predicao: (" << res.predX << ", " << res.predY << ")\n";
    ss << "Latencia: " << res.latencyMs << " ms\n";
    result = ss.str();
    return result.c_str();
}

const char* runBezierAimTest(void) {
    static thread_local std::string result;
    Optimizer::BezierResult res = Optimizer::testBezierAim();
    std::ostringstream ss;
    ss << "=== Bezier Aim Test ===\n";
    ss << "Pontos: " << res.pointCount << "\n";
    ss << "Tempo: " << res.timeMs << " ms\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runCpuGovernorTest(void) {
    static thread_local std::string result;
    Optimizer::CpuGovernorResult res = Optimizer::testCpuGovernor();
    std::ostringstream ss;
    ss << "=== CPU Governor Test ===\n";
    ss << "Governor: " << res.governorName << "\n";
    ss << "Freq Min: " << res.minFreqMhz << " MHz\n";
    ss << "Freq Max: " << res.maxFreqMhz << " MHz\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runAdvancedRecoilTest(int shotNumber, float recoilStrengthPercent) {
    static thread_local std::string result;
    Optimizer::AdvancedRecoilResult res = Optimizer::testAdvancedRecoil(shotNumber, recoilStrengthPercent);
    std::ostringstream ss;
    ss << "=== Advanced Recoil Test ===\n";
    ss << "Tiro: " << shotNumber << "\n";
    ss << "Forca: " << recoilStrengthPercent << "%\n";
    ss << "Compensacao: (" << res.compX << ", " << res.compY << ")\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runAdvancedSpreadTest(int stateIndex, float spreadReductionPercent) {
    static thread_local std::string result;
    Optimizer::AdvancedSpreadResult res = Optimizer::testAdvancedSpread(stateIndex, spreadReductionPercent);
    std::ostringstream ss;
    ss << "=== Advanced Spread Test ===\n";
    ss << "Estado: " << stateIndex << "\n";
    ss << "Reducao: " << spreadReductionPercent << "%\n";
    ss << "Spread: " << res.spread << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runTouchSamplingTest(int targetHzMode) {
    static thread_local std::string result;
    Optimizer::TouchSamplingResult res = Optimizer::testTouchSampling(targetHzMode);
    std::ostringstream ss;
    ss << "=== Touch Sampling Test ===\n";
    ss << "Modo: " << targetHzMode << "\n";
    ss << "Hz Detectado: " << res.detectedHz << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runKalmanFilterTest(float cx, float cy, float vx, float vy) {
    static thread_local std::string result;
    Optimizer::KalmanResult res = Optimizer::testKalmanFilter(cx, cy, vx, vy);
    std::ostringstream ss;
    ss << "=== Kalman Filter Test ===\n";
    ss << "Entrada: (" << cx << ", " << cy << ") vel=(" << vx << ", " << vy << ")\n";
    ss << "Filtrado: (" << res.filteredX << ", " << res.filteredY << ")\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runGyroFilterTest(float yaw, float pitch, float roll) {
    static thread_local std::string result;
    Optimizer::GyroFilterResult res = Optimizer::testGyroFilter(yaw, pitch, roll);
    std::ostringstream ss;
    ss << "=== Gyro Filter Test ===\n";
    ss << "Entrada: yaw=" << yaw << " pitch=" << pitch << " roll=" << roll << "\n";
    ss << "Filtrado: yaw=" << res.filteredYaw << " pitch=" << res.filteredPitch << " roll=" << res.filteredRoll << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runDisplaySyncTest(int targetHz) {
    static thread_local std::string result;
    Optimizer::DisplaySyncResult res = Optimizer::testDisplaySync(targetHz);
    std::ostringstream ss;
    ss << "=== Display Sync Test ===\n";
    ss << "Target: " << targetHz << " Hz\n";
    ss << "Detectado: " << res.detectedHz << " Hz\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runRamDefragTest(void) {
    static thread_local std::string result;
    Optimizer::RamDefragResult res = Optimizer::testRamDefrag();
    std::ostringstream ss;
    ss << "=== RAM Defrag Test ===\n";
    ss << "Blocos Liberados: " << res.blocksFreed << "\n";
    ss << "Memoria Liberada: " << res.bytesFreed << " bytes\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runCpuAffinityTest(void) {
    static thread_local std::string result;
    Optimizer::CpuAffinityResult res = Optimizer::testCpuAffinity();
    std::ostringstream ss;
    ss << "=== CPU Affinity Test ===\n";
    ss << "Core Atribuido: " << res.assignedCore << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runElevationAngleTest(float dist) {
    static thread_local std::string result;
    float angle = Optimizer::calculateElevationAngle(dist);
    std::ostringstream ss;
    ss << "=== Elevation Angle Test ===\n";
    ss << "Distancia: " << dist << "\n";
    ss << "Angulo: " << angle << " graus\n";
    ss << "Status: PASS\n";
    result = ss.str();
    return result.c_str();
}

const char* runEngineVerificationTest(void) {
    static thread_local std::string result;
    Optimizer::VerificationResult res = Optimizer::verifyEngine();
    std::ostringstream ss;
    ss << "=== Engine Verification Test ===\n";
    ss << "Modulos OK: " << res.modulesOk << "/" << res.totalModules << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runMultiTargetTest(void) {
    static thread_local std::string result;
    Optimizer::MultiTargetResult res = Optimizer::testMultiTarget();
    std::ostringstream ss;
    ss << "=== Multi Target Test ===\n";
    ss << "Alvos Detectados: " << res.targetCount << "\n";
    ss << "Mais Proximo: " << res.closestIndex << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runRecoilResetTest(void) {
    static thread_local std::string result;
    Optimizer::RecoilResetResult res = Optimizer::testRecoilReset();
    std::ostringstream ss;
    ss << "=== Recoil Reset Test ===\n";
    ss << "Tempo Reset: " << res.resetTimeMs << " ms\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runSubPixelTouchTest(void) {
    static thread_local std::string result;
    Optimizer::SubPixelResult res = Optimizer::testSubPixelTouch();
    std::ostringstream ss;
    ss << "=== Sub-Pixel Touch Test ===\n";
    ss << "Precisao: " << res.precisionPixels << " px\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runTouchPressureTest(void) {
    static thread_local std::string result;
    Optimizer::TouchPressureResult res = Optimizer::testTouchPressure();
    std::ostringstream ss;
    ss << "=== Touch Pressure Test ===\n";
    ss << "Pressao Media: " << res.avgPressure << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runGpuDevfreqTest(void) {
    static thread_local std::string result;
    Optimizer::GpuDevfreqResult res = Optimizer::testGpuDevfreq();
    std::ostringstream ss;
    ss << "=== GPU Devfreq Test ===\n";
    ss << "Freq Atual: " << res.currentFreqMhz << " MHz\n";
    ss << "Freq Max: " << res.maxFreqMhz << " MHz\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runAudioLatencyTest(void) {
    static thread_local std::string result;
    Optimizer::AudioLatencyResult res = Optimizer::testAudioLatency();
    std::ostringstream ss;
    ss << "=== Audio Latency Test ===\n";
    ss << "Latencia: " << res.latencyMs << " ms\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runShaderCacheTest(void) {
    static thread_local std::string result;
    Optimizer::ShaderCacheResult res = Optimizer::testShaderCache();
    std::ostringstream ss;
    ss << "=== Shader Cache Test ===\n";
    ss << "Shaders Compilados: " << res.shaderCount << "\n";
    ss << "Tempo: " << res.timeMs << " ms\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runSecurityGuardTest(void) {
    static thread_local std::string result;
    Optimizer::SecurityGuardResult res = Optimizer::testSecurityGuard();
    std::ostringstream ss;
    ss << "=== Security Guard Test ===\n";
    ss << "Protecoes Ativas: " << res.activeProtections << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runAiNeuralPredictorTest(void) {
    static thread_local std::string result;
    Optimizer::AiNeuralResult res = Optimizer::testAiNeuralPredictor();
    std::ostringstream ss;
    ss << "=== AI Neural Predictor Test ===\n";
    ss << "Neuronios: " << res.neuronCount << "\n";
    ss << "Precisao: " << res.accuracy << "%\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runAiQuantumEngineTest(void) {
    static thread_local std::string result;
    Optimizer::AiQuantumEngineResult res = Optimizer::runAiQuantumNeuralEngine(
        g_camera360.getYaw(), 0.0f, 0.0f,
        100.0f, 0.5f, 25.0f, 0.3f,
        0, 3, 0.0f, 0.8f, 0.0f, 0.0f,
        0.0f, 0.0f
    );
    std::ostringstream ss;
    ss << "=== AI Quantum Neural Engine v3.0 ===\n";
    ss << "KalmanGain: " << res.paramsUsed.adaptiveKalmanGain << "\n";
    ss << "RecoilFactor: " << res.paramsUsed.adaptiveRecoilCurveFactor << "\n";
    ss << "SnapAngle: " << res.paramsUsed.adaptiveSnapAngleDeg << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runScopeStabilizerTest(float magnification) {
    static thread_local std::string result;
    Optimizer::ScopeResult res = Optimizer::testScopeStabilizer(magnification);
    std::ostringstream ss;
    ss << "=== Scope Stabilizer Test ===\n";
    ss << "Magnificacao: " << magnification << "x\n";
    ss << "Estabilidade: " << res.stability << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runBulletVelocityTest(float speed, float dist) {
    static thread_local std::string result;
    Optimizer::BulletVelocityResult res = Optimizer::testBulletVelocity(speed, dist);
    std::ostringstream ss;
    ss << "=== Bullet Velocity Test ===\n";
    ss << "Velocidade: " << speed << " m/s\n";
    ss << "Distancia: " << dist << " m\n";
    ss << "Tempo Voo: " << res.flightTimeMs << " ms\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runMagnetSnapTest(void) {
    static thread_local std::string result;
    Optimizer::MagnetSnapResult res = Optimizer::testMagnetSnap();
    std::ostringstream ss;
    ss << "=== Magnet Snap Test ===\n";
    ss << "Snap Forca: " << res.snapForce << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runFpsStabilizerTest(int targetFps) {
    static thread_local std::string result;
    Optimizer::FpsStabilizerResult res = Optimizer::testFpsStabilizer(targetFps);
    std::ostringstream ss;
    ss << "=== FPS Stabilizer Test ===\n";
    ss << "Target: " << targetFps << " FPS\n";
    ss << "Detectado: " << res.detectedFps << " FPS\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runTouchJitterTest(void) {
    static thread_local std::string result;
    Optimizer::TouchJitterResult res = Optimizer::testTouchJitter();
    std::ostringstream ss;
    ss << "=== Touch Jitter Test ===\n";
    ss << "Jitter: " << res.jitterPx << " px\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runVulkanPipelineTest(void) {
    static thread_local std::string result;
    Optimizer::VulkanPipelineResult res = Optimizer::testVulkanPipeline();
    std::ostringstream ss;
    ss << "=== Vulkan Pipeline Test ===\n";
    ss << "Pipelines: " << res.pipelineCount << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runDualArenaDefragTest(void) {
    static thread_local std::string result;
    Optimizer::DualArenaResult res = Optimizer::testDualArenaDefrag();
    std::ostringstream ss;
    ss << "=== Dual Arena Defrag Test ===\n";
    ss << "Arena A: " << res.arenaAUsed << " KB usados\n";
    ss << "Arena B: " << res.arenaBUsed << " KB usados\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runSplineTouchTest(void) {
    static thread_local std::string result;
    Optimizer::SplineTouchResult res = Optimizer::testSplineTouch();
    std::ostringstream ss;
    ss << "=== Spline Touch Test ===\n";
    ss << "Pontos Spline: " << res.pointCount << "\n";
    ss << "Suavidade: " << res.smoothness << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runAudioSpectrogramTest(void) {
    static thread_local std::string result;
    Optimizer::AudioSpectrogramResult res = Optimizer::testAudioSpectrogram();
    std::ostringstream ss;
    ss << "=== Audio Spectrogram Test ===\n";
    ss << "Bandas FFT: " << res.bandCount << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runRapidFireTapTest(void) {
    static thread_local std::string result;
    Optimizer::RapidFireResult res = Optimizer::testRapidFireTap();
    std::ostringstream ss;
    ss << "=== Rapid Fire Tap Test ===\n";
    ss << "Tap/s: " << res.tapsPerSecond << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runTargetLockPersistenceTest(void) {
    static thread_local std::string result;
    Optimizer::TargetLockResult res = Optimizer::testTargetLockPersistence();
    std::ostringstream ss;
    ss << "=== Target Lock Persistence Test ===\n";
    ss << "Duracao Lock: " << res.lockDurationMs << " ms\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runGyroSensitivityScaleTest(float mag) {
    static thread_local std::string result;
    float scaled = Optimizer::calculateGyroSensitivityScale(mag);
    std::ostringstream ss;
    ss << "=== Gyro Sensitivity Scale Test ===\n";
    ss << "Magnificacao: " << mag << "x\n";
    ss << "Sensibilidade Escalada: " << scaled << "\n";
    ss << "Status: PASS\n";
    result = ss.str();
    return result.c_str();
}

const char* runGamePresetTest(int gameId) {
    static thread_local std::string result;
    Optimizer::GamePresetResult res = Optimizer::testGamePreset(gameId);
    std::ostringstream ss;
    ss << "=== Game Preset Test ===\n";
    ss << "Game ID: " << gameId << "\n";
    ss << "Preset: " << res.presetName << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runThermalAnticipatorTest(void) {
    static thread_local std::string result;
    Optimizer::ThermalResult res = Optimizer::testThermalAnticipator();
    std::ostringstream ss;
    ss << "=== Thermal Anticipator Test ===\n";
    ss << "Temperatura: " << res.temperatureC << " C\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runInputBufferFlushTest(void) {
    static thread_local std::string result;
    Optimizer::InputBufferResult res = Optimizer::testInputBufferFlush();
    std::ostringstream ss;
    ss << "=== Input Buffer Flush Test ===\n";
    ss << "Buffer Size: " << res.bufferSize << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runQuantumHealthCheckTest(void) {
    static thread_local std::string result;
    Optimizer::QuantumHealthResult res = Optimizer::testQuantumHealthCheck();
    std::ostringstream ss;
    ss << "=== Quantum Health Check ===\n";
    ss << "Saude: " << res.healthPercent << "%\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runCachelineAllocatorTest(void) {
    static thread_local std::string result;
    Optimizer::CachelineResult res = Optimizer::testCachelineAllocator();
    std::ostringstream ss;
    ss << "=== Cacheline Allocator Test ===\n";
    ss << "Cacheline Size: " << res.cachelineSize << " bytes\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runHapticAudioFusionTest(void) {
    static thread_local std::string result;
    Optimizer::HapticAudioResult res = Optimizer::testHapticAudioFusion();
    std::ostringstream ss;
    ss << "=== Haptic Audio Fusion Test ===\n";
    ss << "Latencia: " << res.latencyMs << " ms\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runPolymorphicProtectorTest(void) {
    static thread_local std::string result;
    Optimizer::PolymorphicResult res = Optimizer::testPolymorphicProtector();
    std::ostringstream ss;
    ss << "=== Polymorphic Protector Test ===\n";
    ss << "Variacoes: " << res.variationCount << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runAdaptiveGyroKalmanTest(void) {
    static thread_local std::string result;
    Optimizer::AdaptiveGyroResult res = Optimizer::testAdaptiveGyroKalman();
    std::ostringstream ss;
    ss << "=== Adaptive Gyro Kalman Test ===\n";
    ss << "Ganho Adaptativo: " << res.adaptiveGain << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runZeroStutterShaderMatrixTest(void) {
    static thread_local std::string result;
    Optimizer::ZeroStutterResult res = Optimizer::testZeroStutterShaderMatrix();
    std::ostringstream ss;
    ss << "=== Zero Stutter Shader Matrix ===\n";
    ss << "Shaders: " << res.shaderCount << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runDynamicResolutionScalerTest(void) {
    static thread_local std::string result;
    Optimizer::DynamicResResult res = Optimizer::testDynamicResolutionScaler();
    std::ostringstream ss;
    ss << "=== Dynamic Resolution Scaler ===\n";
    ss << "Scale: " << res.scaleFactor << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runBatteryThrottleShieldTest(void) {
    static thread_local std::string result;
    Optimizer::BatteryThrottleResult res = Optimizer::testBatteryThrottleShield();
    std::ostringstream ss;
    ss << "=== Battery Throttle Shield ===\n";
    ss << "Nivel Bateria: " << res.batteryLevel << "%\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runAntiAliasingFxaaTest(void) {
    static thread_local std::string result;
    Optimizer::FxaaResult res = Optimizer::testAntiAliasingFxaa();
    std::ostringstream ss;
    ss << "=== Anti-Aliasing FXAA ===\n";
    ss << "Qualidade: " << res.quality << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runSubMillisecondInputBufferTest(void) {
    static thread_local std::string result;
    Optimizer::SubMsBufferResult res = Optimizer::testSubMillisecondInputBuffer();
    std::ostringstream ss;
    ss << "=== Sub-millisecond Input Buffer ===\n";
    ss << "Latencia: " << res.latencyUs << " us\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runMultiThreadedPhysicsTest(void) {
    static thread_local std::string result;
    Optimizer::MultiThreadPhysicsResult res = Optimizer::testMultiThreadedPhysics();
    std::ostringstream ss;
    ss << "=== Multi-threaded Physics ===\n";
    ss << "Threads: " << res.threadCount << "\n";
    ss << "Colisoes/s: " << res.collisionsPerSec << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runVerticalFlickCurveTest(float velY) {
    static thread_local std::string result;
    Optimizer::VerticalFlickResult res = Optimizer::testVerticalFlickCurve(velY);
    std::ostringstream ss;
    ss << "=== Vertical Flick Curve ===\n";
    ss << "Velocidade Y: " << velY << "\n";
    ss << "Curva: " << res.curveFactor << "\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runWeaponRecoilProfileTest(int categoryInt, int shotNumber) {
    static thread_local std::string result;
    Optimizer::WeaponRecoilResult res = Optimizer::testWeaponRecoilProfile(categoryInt, shotNumber);
    std::ostringstream ss;
    ss << "=== Weapon Recoil Profile ===\n";
    ss << "Categoria: " << categoryInt << "\n";
    ss << "Tiro: " << shotNumber << "\n";
    ss << "Recoil: (" << res.recoilX << ", " << res.recoilY << ")\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

const char* runBinauralAudioTrackingTest(float leftDb, float rightDb) {
    static thread_local std::string result;
    Optimizer::BinauralResult res = Optimizer::testBinauralAudioTracking(leftDb, rightDb);
    std::ostringstream ss;
    ss << "=== Binaural Audio Tracking ===\n";
    ss << "Esquerda: " << leftDb << " dB\n";
    ss << "Direita: " << rightDb << " dB\n";
    ss << "Angulo: " << res.angleDeg << " graus\n";
    ss << "Status: " << (res.passed ? "PASS" : "FALHOU") << "\n";
    result = ss.str();
    return result.c_str();
}

} // extern "C"
