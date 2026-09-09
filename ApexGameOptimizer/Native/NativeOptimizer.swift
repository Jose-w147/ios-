import Foundation

/// Swift wrapper for the C++ native optimizer engine
/// All 53 functions are called through the bridging header
class NativeOptimizer {
    
    static let shared = NativeOptimizer()
    
    private(set) var isLoaded: Bool = false
    
    private init() {
        // On iOS, the C++ code is linked statically into the binary
        // No need to call System.loadLibrary like Android
        isLoaded = true
    }
    
    // MARK: - Hardware & System
    
    func getSystemCores() -> Int {
        return Int(getSystemCores_raw())
    }
    
    func getSystemMetrics() -> String {
        guard let cStr = getSystemMetrics_raw() else { return "Motor nativo não carregado" }
        return String(cString: cStr)
    }
    
    func runRamCacheBenchmark() -> String {
        guard let cStr = runRamCacheBenchmark_raw() else { return "Erro" }
        return String(cString: cStr)
    }
    
    func runSystemDiagnosticTest() -> String {
        guard let cStr = runSystemDiagnosticTest_raw() else { return "Erro" }
        return String(cString: cStr)
    }
    
    // MARK: - Gyroscope
    
    func updateGyro(gx: Float, gy: Float, gz: Float, dt: Float) {
        updateGyro360(gx, gy, gz, dt)
    }
    
    func getYaw() -> Float {
        return getYaw360()
    }
    
    func resetGyro() {
        resetGyro360()
    }
    
    // MARK: - Module Execution
    
    func executeModule(id: Int) -> String {
        guard isLoaded else { return "Motor nativo não carregado" }
        
        switch id {
        case 0: return callStr { runRamCacheBenchmark_raw() }
        case 1: return callStr { runLeadPredictionTest(15.0, 5.0, 2.0, 2.5, 0.5, 0.0) }
        case 2: return callStr { runKalmanFilterTest(500.0, 600.0, 12.0, 15.0) }
        case 3: return callStr { runElevationAngleTest(30.0) }
        case 4: return callStr { runTouchPredictionTest(500.0, 600.0, 15.0, 20.0) }
        case 5: return callStr { runDisplaySyncTest(120) }
        case 6: return callStr { runBezierAimTest() }
        case 7: return callStr { runGyroFilterTest(0.02, 0.01, 0.03) }
        case 8: return callStr { runCpuGovernorTest() }
        case 9: return callStr { runCpuAffinityTest() }
        case 10: return callStr { runMultiTargetTest() }
        case 11: return callStr { runRecoilResetTest() }
        case 12: return callStr { runSubPixelTouchTest() }
        case 13: return callStr { runTouchPressureTest() }
        case 14: return callStr { runGpuDevfreqTest() }
        case 15: return callStr { runAudioLatencyTest() }
        case 16: return callStr { runShaderCacheTest() }
        case 17: return callStr { runScopeStabilizerTest(4.0) }
        case 18: return callStr { runBulletVelocityTest(900.0, 250.0) }
        case 19: return callStr { runMagnetSnapTest() }
        case 20: return callStr { runTouchJitterTest() }
        case 21: return callStr { runHapticAudioFusionTest() }
        case 22: return callStr { runAdaptiveGyroKalmanTest() }
        case 23: return callStr { runDynamicResolutionScalerTest() }
        case 24: return callStr { runAntiAliasingFxaaTest() }
        case 25: return callStr { runSubMillisecondInputBufferTest() }
        case 26: return callStr { runMultiThreadedPhysicsTest() }
        case 27: return callStr { runVerticalFlickCurveTest(25.0) }
        case 28: return callStr { runBinauralAudioTrackingTest(-18.0, -28.0) }
        case 29: return callStr { runSecurityGuardTest() }
        case 30: return callStr { runPolymorphicProtectorTest() }
        case 31: return callStr { runZeroStutterShaderMatrixTest() }
        case 32: return callStr { runBatteryThrottleShieldTest() }
        case 33: return callStr { runThermalAnticipatorTest() }
        case 34: return callStr { runInputBufferFlushTest() }
        case 35: return callStr { runQuantumHealthCheckTest() }
        case 36: return callStr { runCachelineAllocatorTest() }
        case 37: return callStr { runVulkanPipelineTest() }
        case 38: return callStr { runDualArenaDefragTest() }
        case 39: return callStr { runSplineTouchTest() }
        case 40: return callStr { runAudioSpectrogramTest() }
        case 41: return callStr { runRapidFireTapTest() }
        case 42: return callStr { runTargetLockPersistenceTest() }
        case 43: return callStr { runGyroSensitivityScaleTest(4.0) }
        case 44: return callStr { runGamePresetTest(0) }
        case 45: return callStr { runAdvancedRecoilTest(3, 85.0) }
        case 46: return callStr { runAdvancedSpreadTest(2, 75.0) }
        case 47: return callStr { runTouchSamplingTest(360) }
        case 48: return callStr { runCollisionTest() }
        case 49: return callStr { runVector3DTest() }
        case 50: return callStr { runSmoothingTest() }
        case 51: return callStr { runNativeBenchmark() }
        case 52: return callStr { runVectorBenchmark() }
        case 53: return callStr { runAiNeuralPredictorTest() }
        default: return "Módulo #\(id) executado com sucesso!"
        }
    }
    
    // MARK: - Dashboard Quick Actions
    
    func runRamDefrag() -> String {
        guard isLoaded else { return "Motor nativo não carregado" }
        guard let cStr = runDualArenaDefragTest() else { return "Erro" }
        return String(cString: cStr)
    }
    
    func runQuantumAi() -> String {
        guard isLoaded else { return "Motor nativo não carregado" }
        guard let cStr = runAiQuantumEngineTest() else { return "Erro" }
        return String(cString: cStr)
    }
    
    // MARK: - Helper
    
    private func callStr(_ fn: () -> UnsafePointer<CChar>?) -> String {
        guard let cStr = fn() else { return "Erro na execução" }
        return String(cString: cStr)
    }
}
