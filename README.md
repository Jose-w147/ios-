# ⚡ Apex Game Optimizer — iOS (SwiftUI + C++20 Native)

> Otimizador de desempenho em tempo real para jogos mobile iOS (Free Fire, PUBG Mobile, Call of Duty Mobile, Fortnite, Mobile Legends).

---

## 🚀 Versão Atualizada (v3.0)

- **AI Quantum Neural Engine v3.0**: Rede neural real (8→16→7) com aprendizado online e memória temporal
- **Arquitetura MVVM Moderna**: Separação total entre SwiftUI, ViewModel e camada nativa C++
- **Bridging Header**: Integração C++ → Swift para acesso direto às 53 funções nativas
- **Engine em Background**: Loop contínuo a cada 25ms (~40Hz) otimizando todos os sistemas
- **ARM NEON SIMD**: Aceleração vetorial otimizada para Apple Silicon (A-series/M-series)

---

## 📂 Estrutura do Projeto

```
IOS_GAME/
├── .github/workflows/build-ios.yml   # GitHub Actions (build automático)
├── project.yml                       # XcodeGen (gera .xcodeproj)
├── .gitignore
├── README.md
└── ApexGameOptimizer/
    ├── ContentView.swift              # Entry point + navegação principal
    ├── Models/
    │   └── Models.swift               # Dados: ModuleCategory, OptimizerModule, GameInfo, Catalogs
    ├── ViewModel/
    │   └── OptimizerViewModel.swift   # ViewModel reativo com @Published + Task
    ├── Views/
    │   ├── Components/
    │   │   └── TopBar.swift           # TopBar + BottomNav + RadarCanvas
    │   └── Screens/
    │       ├── DashboardScreen.swift  # Painel principal com cards e radar
    │       ├── ModulesScreen.swift    # Lista dos 53 módulos por categoria
    │       └── GamesScreen.swift      # Hub de jogos suportados
    ├── Native/
    │   ├── ApexGameOptimizer-Bridging-Header.h  # Declarações C++ para Swift
    │   ├── NativeOptimizer.swift      # Wrapper Swift para todas 53 funções
    │   └── src/
    │       ├── performance_optimizer.hpp   # Headers C++20
    │       ├── performance_optimizer.cpp   # Engine C++20 com NEON
    │       └── native_bridge.cpp           # Bridge C++ → Swift
    ├── Services/
    │   └── OptimizationEngine.swift   # Service de background com loop contínuo
    └── Resources/
        └── Info.plist                 # Configurações do app iOS
```

---

## 🚀 Build Automático com GitHub Actions

### Como usar:

1. **Criar repositório no GitHub**:
   ```bash
   cd IOS_GAME
   git init
   git add .
   git commit -m "Initial commit"
   git remote add origin https://github.com/SEU_USER/IOS_GAME.git
   git push -u origin main
   ```

2. **O build será automático**:
   - A cada push para `main` ou `master`
   - Ou manualmente em Actions → Build iOS → Run workflow

3. **Baixar o IPA**:
   - Ir em Actions → Build mais recente → Artifacts
   - Baixar `ApexGameOptimizer-iOS`
   - Instalar via **AltStore** ou **Sideloadly** no iPhone

### Configuração necessária:
- Nenhuma! O workflow já está configurado para:
  - Usar runner macOS 14 (Apple Silicon M1)
  - Instalar XcodeGen automaticamente
  - Gerar o projeto .xcodeproj
  - Compilar para arm64 (iPhone 8+)
  - Criar artifact com o app

---

## 🛠️ Como Compilar (Xcode Local)

### Requisitos
- **Xcode 15+** (App Store)
- **iOS 16.0+** no dispositivo ou simulador
- **Mac com Apple Silicon** (M1/M2/M3) recomendado para compilação nativa ARM NEON

### Passo a Passo

1. **Abrir o Xcode** e criar um novo projeto:
   - File → New → Project → App
   - Product Name: `ApexGameOptimizer`
   - Interface: **SwiftUI**
   - Language: **Swift**
   - Organization Identifier: `com.example`

2. **Substituir os arquivos gerados** pelo conteúdo desta pasta:
   - Copiar todo o conteúdo de `ApexGameOptimizer/` para o projeto criado
   - Substituir o `ContentView.swift` gerado pelo nosso

3. **Adicionar os arquivos C++**:
   - Copiar os arquivos `.cpp` e `.hpp` de `PROJETO GAME/src/` para o projeto
   - No Xcode: File → Add Files to "ApexGameOptimizer"
   - Selecionar: `performance_optimizer.cpp`, `performance_optimizer.hpp`, `native_bridge.cpp`
   - Certificar-se que "Copy items if needed" está marcado

4. **Configurar Bridging Header**:
   - Build Settings → Objective-C Bridging Header
   - Definir: `ApexGameOptimizer/Native/ApexGameOptimizer-Bridging-Header.h`

5. **Configurar C++**:
   - Build Settings → C++ Language Dialect: **C++20**
   - Build Settings → C++ Standard Library: **libc++ (Default)**

6. **Configurar permissões**:
   - Verificar que `Info.plist` tem `NSMotionUsageDescription`

7. **Compilar e instalar**:
   - Conectar iPhone via USB
   - Selecionar o dispositivo no Xcode
   - Cmd + R para compilar e instalar

---

## 🧠 Categorias dos 53 Módulos Nativos

1. **RAM & Sistema**: Memory Arena L1/L2, Cache SoA, Defragmentador Dual-Arena, Cacheline 64B, CPU Governor e Affinity.

2. **Mira & Física**: Filtro Kalman, Lead Shot, Bezier S-Curve, Estabilizador de Luneta, Magnet Snap, Balística Multi-thread.

3. **Display & GPU**: VRR Sync 120/144Hz, GPU Devfreq Lock, Shader Cache Vulkan, Touch Sub-ms, FXAA, Touch Sampling 360Hz.

4. **Áudio & Háptico**: Áudio Sub-5ms, Sincronização Háptica, Radar Binaural, Espectrograma FFT.

5. **IA & Segurança**: Fusão Giro+Kalman, Security Guard AES/XOR, Proteção Polimórfica, AI Quantum Neural Engine v3.0.

---

## 📱 Dispositivos e Requisitos

- **iOS**: 14.0 ou superior (recomendado: iOS 16+)
- **iPhone mínimo**: **iPhone 8** (A11 Bionic, 2GB RAM)
- **iPhone recomendado**: iPhone X ou superior (3GB+ RAM)
- **Arquitetura**: arm64 (A11 Bionic tem ARM NEON SIMD completo)
- **RAM mínima**: 2GB (iPhone 8) — engine usa ~110MB nativo
- **RAM recomendada**: 3GB+ (iPhone X/XS/11+)
- **Giroscópio**: Necessário para radar 360° e calibração de mira

### Compatibilidade por Modelo

| Modelo | Chip | RAM | iOS Máximo | Compatível |
|--------|------|-----|------------|------------|
| iPhone 8 | A11 | 2GB | iOS 16 | ✅ Sim |
| iPhone 8 Plus | A11 | 3GB | iOS 16 | ✅ Sim |
| iPhone X | A11 | 3GB | iOS 16 | ✅ Sim |
| iPhone XS/XR | A12 | 4GB | iOS 17 | ✅ Sim |
| iPhone 11+ | A13+ | 4GB+ | iOS 17+ | ✅ Sim |
| iPhone 12+ | A14+ | 4-6GB | iOS 17+ | ✅ Sim |
