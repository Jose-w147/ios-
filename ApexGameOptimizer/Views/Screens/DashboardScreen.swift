import SwiftUI

// MARK: - Color Theme (Cyberpunk)

extension Color {
    static let bgPrimary = Color(red: 0.027, green: 0.031, blue: 0.039)
    static let bgCard = Color(red: 0.075, green: 0.082, blue: 0.102)
    static let borderOrange = Color(red: 1.0, green: 0.6, blue: 0.0)
    static let borderGreen = Color(red: 0.0, green: 1.0, blue: 0.5)
    static let borderCyan = Color(red: 0.0, green: 0.85, blue: 1.0)
    static let borderPurple = Color(red: 0.7, green: 0.3, blue: 1.0)
    static let borderRed = Color(red: 1.0, green: 0.2, blue: 0.3)
    static let textPrimary = Color.white
    static let textSecond = Color(red: 0.55, green: 0.57, blue: 0.65)
}

// MARK: - Dashboard Screen

struct DashboardScreen: View {
    @ObservedObject var viewModel: OptimizerViewModel
    
    var body: some View {
        ScrollView {
            VStack(spacing: 16) {
                // Motor Nativo Card
                NativeMotorCard(metrics: viewModel.systemMetrics, cpuCores: viewModel.cpuCores) {
                    viewModel.refreshMetrics()
                }
                
                // Radar Giroscópio
                GyroRadarCard(yaw: viewModel.currentYaw, isAvailable: viewModel.isGyroAvailable) {
                    viewModel.resetGyro()
                }
                
                // Quick Actions
                HStack(spacing: 12) {
                    QuickActionButton(
                        emoji: "🧹", title: "PURGAR RAM",
                        color: .borderGreen, bgColor: Color.borderGreen.opacity(0.15)
                    ) {
                        viewModel.runRamDefrag()
                    }
                    
                    QuickActionButton(
                        emoji: "⚛️", title: "AI QUANTUM",
                        color: .borderPurple, bgColor: Color.borderPurple.opacity(0.15)
                    ) {
                        viewModel.runQuantumAi()
                    }
                }
                .padding(.horizontal, 16)
                
                // Module Results
                ForEach(Array(viewModel.moduleResults.keys.sorted()), id: \.self) { key in
                    if let result = viewModel.moduleResults[key] {
                        ResultCard(title: key == 38 ? "RESULTADO DEPURADOR DUAL-ARENA" : "CALIBRAÇÃO PREVENTIVA AI QUANTUM",
                                   result: result,
                                   borderColor: key == 38 ? .borderGreen : .borderPurple)
                    }
                }
            }
            .padding(.vertical, 16)
        }
    }
}

// MARK: - Native Motor Card

struct NativeMotorCard: View {
    let metrics: String
    let cpuCores: Int
    let onRefresh: () -> Void
    
    var body: some View {
        VStack(alignment: .leading, spacing: 8) {
            HStack {
                Text("⚡ MOTOR NATIVO C++20 (NEON SIMD)")
                    .font(.system(size: 14, weight: .bold))
                    .foregroundColor(.borderOrange)
                
                Spacer()
                
                Button(action: onRefresh) {
                    Text("ATUALIZAR")
                        .font(.system(size: 11, weight: .bold))
                        .foregroundColor(.borderOrange)
                        .padding(.horizontal, 12)
                        .padding(.vertical, 6)
                        .background(Color.borderOrange.opacity(0.15))
                        .cornerRadius(8)
                        .overlay(
                            RoundedRectangle(cornerRadius: 8)
                                .stroke(Color.borderOrange, lineWidth: 1)
                        )
                }
            }
            
            Text(metrics)
                .font(.system(size: 13, design: .monospaced))
                .foregroundColor(.textPrimary)
                .lineSpacing(2)
        }
        .padding(16)
        .background(Color.bgCard)
        .cornerRadius(14)
        .overlay(
            RoundedRectangle(cornerRadius: 14)
                .stroke(Color.borderOrange.opacity(0.5), lineWidth: 1)
        )
        .padding(.horizontal, 16)
    }
}

// MARK: - Gyro Radar Card

struct GyroRadarCard: View {
    let yaw: Float
    let isAvailable: Bool
    let onReset: () -> Void
    
    var body: some View {
        VStack(spacing: 12) {
            HStack {
                HStack(spacing: 6) {
                    Text("🧭")
                        .font(.title3)
                    Text("RADAR GIROSCÓPIO 360°")
                        .font(.system(size: 13, weight: .bold))
                        .foregroundColor(.borderCyan)
                }
                
                Spacer()
                
                Button(action: onReset) {
                    Text("ZERAR MIRA")
                        .font(.system(size: 11, weight: .bold))
                        .foregroundColor(.borderCyan)
                        .padding(.horizontal, 10)
                        .padding(.vertical, 4)
                        .background(Color.borderCyan.opacity(0.15))
                        .cornerRadius(8)
                        .overlay(
                            RoundedRectangle(cornerRadius: 8)
                                .stroke(Color.borderCyan, lineWidth: 0.5)
                        )
                }
            }
            
            // Radar Canvas
            RadarCanvas(yaw: CGFloat(yaw))
                .frame(width: 160, height: 160)
            
            HStack {
                Text(String(format: "Ângulo: %.1f°", yaw))
                    .font(.system(size: 12, design: .monospaced).bold())
                    .foregroundColor(.textPrimary)
                
                Spacer()
                
                Text(isAvailable ? "Status: ATIVO ✅" : "Status: SEM SENSOR ⚠️")
                    .font(.system(size: 12, design: .monospaced))
                    .foregroundColor(isAvailable ? .borderGreen : .borderRed)
            }
        }
        .padding(16)
        .background(Color.bgCard)
        .cornerRadius(14)
        .overlay(
            RoundedRectangle(cornerRadius: 14)
                .stroke(Color.borderCyan.opacity(0.4), lineWidth: 1.5)
        )
        .padding(.horizontal, 16)
    }
}

// MARK: - Radar Canvas (SwiftUI)

struct RadarCanvas: View {
    let yaw: CGFloat
    
    var body: some View {
        Canvas { context, size in
            let center = CGPoint(x: size.width / 2, y: size.height / 2)
            let radius = min(size.width, size.height) / 2
            
            // Concentric rings
            for r in [0.75, 0.45, 0.20] {
                var path = Path()
                path.addEllipse(in: CGRect(
                    x: center.x - radius * r,
                    y: center.y - radius * r,
                    width: radius * r * 2,
                    height: radius * r * 2
                ))
                context.stroke(path, with: .color(Color.borderCyan.opacity(0.12)), lineWidth: 1)
            }
            
            // Crosshair lines
            var hLine = Path()
            hLine.move(to: CGPoint(x: center.x - radius, y: center.y))
            hLine.addLine(to: CGPoint(x: center.x + radius, y: center.y))
            context.stroke(hLine, with: .color(Color.borderCyan.opacity(0.25)), lineWidth: 1)
            
            var vLine = Path()
            vLine.move(to: CGPoint(x: center.x, y: center.y - radius))
            vLine.addLine(to: CGPoint(x: center.x, y: center.y + radius))
            context.stroke(vLine, with: .color(Color.borderCyan.opacity(0.25)), lineWidth: 1)
            
            // Yaw vector
            let rad = (yaw - 90) * .pi / 180
            let endX = center.x + radius * 0.78 * cos(rad)
            let endY = center.y + radius * 0.78 * sin(rad)
            
            var yawLine = Path()
            yawLine.move(to: center)
            yawLine.addLine(to: CGPoint(x: endX, y: endY))
            context.stroke(yawLine, with: .color(.borderGreen), lineWidth: 3)
            
            // Arrow tip
            context.fill(Path(ellipseIn: CGRect(x: endX - 4.5, y: endY - 4.5, width: 9, height: 9)), with: .color(.borderGreen))
            
            // Center dot
            context.fill(Path(ellipseIn: CGRect(x: center.x - 3.5, y: center.y - 3.5, width: 7, height: 7)), with: .color(.borderCyan))
        }
    }
}

// MARK: - Quick Action Button

struct QuickActionButton: View {
    let emoji: String
    let title: String
    let color: Color
    let bgColor: Color
    let action: () -> Void
    
    var body: some View {
        Button(action: action) {
            HStack {
                Text(emoji)
                Text(title)
                    .font(.system(size: 13, weight: .bold))
            }
            .foregroundColor(color)
            .frame(maxWidth: .infinity)
            .padding(.vertical, 14)
            .background(bgColor)
            .cornerRadius(12)
            .overlay(
                RoundedRectangle(cornerRadius: 12)
                    .stroke(color, lineWidth: 1)
            )
        }
    }
}

// MARK: - Result Card

struct ResultCard: View {
    let title: String
    let result: String
    let borderColor: Color
    
    var body: some View {
        VStack(alignment: .leading, spacing: 8) {
            HStack {
                Text("✅")
                    .font(.title2)
                Text(title)
                    .font(.system(size: 13, weight: .bold))
                    .foregroundColor(borderColor)
            }
            
            Text(result)
                .font(.system(size: 12, design: .monospaced))
                .foregroundColor(.textPrimary)
                .lineSpacing(2)
        }
        .padding(16)
        .background(Color.bgCard)
        .cornerRadius(14)
        .overlay(
            RoundedRectangle(cornerRadius: 14)
                .stroke(borderColor.opacity(0.5), lineWidth: 1)
        )
        .padding(.horizontal, 16)
    }
}
