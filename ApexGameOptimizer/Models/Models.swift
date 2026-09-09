import Foundation

// MARK: - Module Category

enum ModuleCategory: String, CaseIterable, Identifiable {
    case systemRam = "RAM & Sistema"
    case aimPhysics = "Mira & Física"
    case displayGraphics = "Display & GPU"
    case audioHaptics = "Áudio & Háptico"
    case aiSecurity = "IA & Segurança"
    
    var id: String { rawValue }
    var displayName: String { rawValue }
    
    var accentColor: String {
        switch self {
        case .systemRam: return "borderOrange"
        case .aimPhysics: return "borderGreen"
        case .displayGraphics: return "borderCyan"
        case .audioHaptics: return "borderPurple"
        case .aiSecurity: return "borderRed"
        }
    }
}

// MARK: - Optimizer Module

struct OptimizerModule: Identifiable {
    let id: Int
    let title: String
    let description: String
    let category: ModuleCategory
    let iconEmoji: String
}

// MARK: - Game Info

struct GameInfo: Identifiable {
    let id = UUID()
    let name: String
    let packageName: String
    let storeUrl: String
    let iconEmoji: String
}

// MARK: - Module Catalog

enum ModuleCatalog {
    static let allModules: [OptimizerModule] = [
        // RAM & Sistema
        OptimizerModule(id: 0, title: "Otimização RAM L1/L2", description: "Benchmark de cache AoS vs SoA e Arena Contígua", category: .systemRam, iconEmoji: "🧠"),
        OptimizerModule(id: 8, title: "CPU Governor & Frequência", description: "Evita throttling térmico e bloqueia frequências máximas", category: .systemRam, iconEmoji: "⚡"),
        OptimizerModule(id: 9, title: "CPU Core Affinity", description: "Aloca threads pesadas nos big cores", category: .systemRam, iconEmoji: "🚀"),
        OptimizerModule(id: 38, title: "Dual-Arena Defragmenter", description: "Purga de memória heap e compactação L1/L2", category: .systemRam, iconEmoji: "🧹"),
        OptimizerModule(id: 36, title: "Cacheline 64B Allocator", description: "Alocação alinhada em 64 bytes para zero miss", category: .systemRam, iconEmoji: "💾"),
        
        // Mira & Física
        OptimizerModule(id: 1, title: "Lead Shot Prediction", description: "Cálculo de trajetória balística do alvo", category: .aimPhysics, iconEmoji: "🎯"),
        OptimizerModule(id: 2, title: "Filtro Kalman de Mira", description: "Rastreamento estocástico preditivo", category: .aimPhysics, iconEmoji: "📐"),
        OptimizerModule(id: 6, title: "Curva Bezier S-Curve", description: "Suavização do trajeto de arraste", category: .aimPhysics, iconEmoji: "〰️"),
        OptimizerModule(id: 17, title: "Estabilizador de Mira", description: "Ajuste dinâmico para mira 2x/4x/8x", category: .aimPhysics, iconEmoji: "🔭"),
        OptimizerModule(id: 19, title: "Magnet Snap Vector", description: "Alinhamento magnético suave no alvo", category: .aimPhysics, iconEmoji: "🧲"),
        OptimizerModule(id: 26, title: "Física Balística Multi-thread", description: "Simulação de 30 projéteis em paralelo", category: .aimPhysics, iconEmoji: "💥"),
        OptimizerModule(id: 27, title: "Curva Exponencial de Subida", description: "Multiplicador dinâmico para puxada vertical", category: .aimPhysics, iconEmoji: "⬆️"),
        OptimizerModule(id: 45, title: "Compensador de Recuo", description: "Curva de amortecimento do coice", category: .aimPhysics, iconEmoji: "🔫"),
        
        // Display & GPU
        OptimizerModule(id: 4, title: "Touch Prediction Latency", description: "Antecipação de toque para reduzir input lag", category: .displayGraphics, iconEmoji: "👆"),
        OptimizerModule(id: 5, title: "Display VRR Sync", description: "Sincronização 120/144Hz e redução de jitter", category: .displayGraphics, iconEmoji: "🖥️"),
        OptimizerModule(id: 14, title: "GPU Devfreq Lock", description: "Fixação de clock GPU Adreno/Mali", category: .displayGraphics, iconEmoji: "🎮"),
        OptimizerModule(id: 16, title: "Shader Cache Pre-Warm", description: "Pré-compilação de shaders Vulkan", category: .displayGraphics, iconEmoji: "🧊"),
        OptimizerModule(id: 20, title: "Filtro de Jitter Touch", description: "Eliminação de ruídos do digitalizador", category: .displayGraphics, iconEmoji: "✨"),
        OptimizerModule(id: 23, title: "Dynamic Resolution Scaler", description: "Ajuste dinâmico para manter 120 FPS", category: .displayGraphics, iconEmoji: "📊"),
        OptimizerModule(id: 24, title: "Anti-Aliasing FXAA", description: "Realce de bordas para inimigos distantes", category: .displayGraphics, iconEmoji: "🔍"),
        OptimizerModule(id: 25, title: "Fila de Toque Sub-ms", description: "Buffer circular com latência < 0.7ms", category: .displayGraphics, iconEmoji: "⏱️"),
        OptimizerModule(id: 31, title: "Matriz Vulkan 128 Pipelines", description: "Prevenção de travamentos", category: .displayGraphics, iconEmoji: "⚡"),
        OptimizerModule(id: 47, title: "Touch Sampling 360Hz", description: "Aceleração da taxa de amostragem", category: .displayGraphics, iconEmoji: "📲"),
        
        // Áudio & Háptico
        OptimizerModule(id: 15, title: "Áudio Sub-5ms Minimizer", description: "Redução do buffer de áudio", category: .audioHaptics, iconEmoji: "🔊"),
        OptimizerModule(id: 21, title: "Sincronização Háptica", description: "Pulso vibratório direcional", category: .audioHaptics, iconEmoji: "📳"),
        OptimizerModule(id: 28, title: "Radar de Áudio Binaural", description: "Isolamento azimutal esquerda/direita", category: .audioHaptics, iconEmoji: "🎧"),
        OptimizerModule(id: 40, title: "Espectrograma FFT", description: "Filtragem 1kHz–3kHz para passos", category: .audioHaptics, iconEmoji: "👣"),
        
        // IA & Segurança
        OptimizerModule(id: 22, title: "Fusão Giroscópio + Kalman", description: "Precisão de mira laser com sensor", category: .aiSecurity, iconEmoji: "🧭"),
        OptimizerModule(id: 29, title: "Security Checksum Guard", description: "Blindagem AES/XOR de memória", category: .aiSecurity, iconEmoji: "🛡️"),
        OptimizerModule(id: 30, title: "Proteção Polimórfica", description: "Rotação de chaves a cada 25ms", category: .aiSecurity, iconEmoji: "🔐"),
        OptimizerModule(id: 32, title: "Escudo de Bateria", description: "Estabilidade com bateria baixa", category: .aiSecurity, iconEmoji: "🔋"),
        OptimizerModule(id: 33, title: "Antecipador Térmico", description: "Previne aquecimento antes do throttling", category: .aiSecurity, iconEmoji: "🌡️"),
        OptimizerModule(id: 35, title: "Auditor de Saúde AI", description: "Validação de tensores ARM NEON", category: .aiSecurity, iconEmoji: "🩺"),
        OptimizerModule(id: 53, title: "Preditor Neural Unificado", description: "Auto-calibração cruzada por IA", category: .aiSecurity, iconEmoji: "🤖"),
    ]
    
    static func modules(for category: ModuleCategory) -> [OptimizerModule] {
        allModules.filter { $0.category == category }
    }
}

// MARK: - Supported Games

enum GameCatalog {
    static let supportedGames: [GameInfo] = [
        GameInfo(name: "Free Fire", packageName: "com.dts.freefireth", storeUrl: "https://apps.apple.com/app/free-fire/id1300146617", iconEmoji: "🔥"),
        GameInfo(name: "Free Fire MAX", packageName: "com.dts.freefiremax", storeUrl: "https://apps.apple.com/app/free-fire-max/id1501061782", iconEmoji: "🔥"),
        GameInfo(name: "PUBG Mobile", packageName: "com.tencent.ig", storeUrl: "https://apps.apple.com/app/pubg-mobile/id1330123889", iconEmoji: "🪖"),
        GameInfo(name: "Call of Duty Mobile", packageName: "com.activision.callofduty.shooter", storeUrl: "https://apps.apple.com/app/call-of-duty-mobile/id1287282214", iconEmoji: "🎯"),
        GameInfo(name: "Fortnite", packageName: "com.epicgames.fortnite", storeUrl: "https://apps.apple.com/app/fortnite/id1261357853", iconEmoji: "🏗️"),
        GameInfo(name: "Mobile Legends", packageName: "com.mobile.legends", storeUrl: "https://apps.apple.com/app/mobile-legends/id1131748304", iconEmoji: "⚔️"),
    ]
}
