import Foundation
import SwiftUI
import Combine

// MARK: - UI State

@MainActor
class OptimizerViewModel: ObservableObject {
    
    @Published var systemMetrics: String = "Carregando telemetria..."
    @Published var cpuCores: Int = 1
    @Published var isGyroAvailable: Bool = true
    @Published var currentYaw: Float = 0.0
    @Published var selectedTab: Int = 0  // 0=Dashboard, 1=Modules, 2=Games
    @Published var selectedCategory: ModuleCategory = .systemRam
    @Published var moduleResults: [Int: String] = [:]
    @Published var isExecuting: Bool = false
    @Published var isServiceActive: Bool = false
    
    private let nativeOptimizer = NativeOptimizer.shared
    private var gyroTimer: Timer?
    
    init() {
        refreshMetrics()
    }
    
    // MARK: - Metrics
    
    func refreshMetrics() {
        Task.detached { [weak self] in
            guard let self else { return }
            let metrics = await self.nativeOptimizer.getSystemMetrics()
            let cores = await self.nativeOptimizer.getSystemCores()
            await MainActor.run {
                self.systemMetrics = metrics
                self.cpuCores = cores
            }
        }
    }
    
    // MARK: - Navigation
    
    func selectTab(_ index: Int) {
        selectedTab = index
    }
    
    func selectCategory(_ category: ModuleCategory) {
        selectedCategory = category
    }
    
    // MARK: - Gyroscope
    
    func updateGyro(gx: Float, gy: Float, gz: Float, dt: Float) {
        nativeOptimizer.updateGyro(gx: gx, gy: gy, gz: gz, dt: dt)
        currentYaw = nativeOptimizer.getYaw()
    }
    
    func setGyroAvailable(_ available: Bool) {
        isGyroAvailable = available
    }
    
    func resetGyro() {
        nativeOptimizer.resetGyro()
        currentYaw = 0.0
    }
    
    // MARK: - Module Execution
    
    func runModule(_ moduleId: Int) {
        isExecuting = true
        Task.detached { [weak self] in
            guard let self else { return }
            let result = await self.nativeOptimizer.executeModule(id: moduleId)
            await MainActor.run {
                self.moduleResults[moduleId] = result
                self.isExecuting = false
            }
        }
    }
    
    func runRamDefrag() {
        isExecuting = true
        Task.detached { [weak self] in
            guard let self else { return }
            let res = await self.nativeOptimizer.runRamDefrag()
            await MainActor.run {
                self.moduleResults[38] = res
                self.isExecuting = false
                self.refreshMetrics()
            }
        }
    }
    
    func runQuantumAi() {
        isExecuting = true
        Task.detached { [weak self] in
            guard let self else { return }
            let res = await self.nativeOptimizer.runQuantumAi()
            await MainActor.run {
                self.moduleResults[53] = res
                self.isExecuting = false
            }
        }
    }
    
    // MARK: - Module Lists
    
    func getModulesForCategory(_ category: ModuleCategory) -> [OptimizerModule] {
        ModuleCatalog.modules(for: category)
    }
    
    func getAllModules() -> [OptimizerModule] {
        ModuleCatalog.allModules
    }
}
