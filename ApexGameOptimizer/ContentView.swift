import SwiftUI

// MARK: - Main Content View

struct ContentView: View {
    @StateObject private var viewModel = OptimizerViewModel()
    @State private var selectedTab = 0
    
    var body: some View {
        ZStack {
            Color.bgPrimary.ignoresSafeArea()
            
            VStack(spacing: 0) {
                TopBar(isLoaded: NativeOptimizer.shared.isLoaded, cpuCores: viewModel.cpuCores)
                
                ZStack {
                    switch selectedTab {
                    case 0:
                        DashboardScreen(viewModel: viewModel)
                    case 1:
                        ModulesScreen(viewModel: viewModel)
                    case 2:
                        GamesScreen { game in
                            launchGame(game)
                        }
                    default:
                        DashboardScreen(viewModel: viewModel)
                    }
                }
                .frame(maxWidth: .infinity, maxHeight: .infinity)
                
                BottomNav(selectedTab: $selectedTab)
            }
        }
        .preferredColorScheme(.dark)
        .onAppear {
            startEngineService()
        }
    }
    
    private func launchGame(_ game: GameInfo) {
        if let url = URL(string: game.storeUrl) {
            UIApplication.shared.open(url)
        }
    }
    
    private func startEngineService() {
        // On iOS, the optimization engine runs in the app process
        // Background optimization is handled via the ViewModel
        viewModel.isServiceActive = true
    }
}

// MARK: - App Entry Point

@main
struct ApexGameOptimizerApp: App {
    var body: some Scene {
        WindowGroup {
            ContentView()
                .edgesIgnoringSafeArea(.bottom)
        }
    }
}
