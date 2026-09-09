import SwiftUI

// MARK: - Top Bar

struct TopBar: View {
    let isLoaded: Bool
    let cpuCores: Int
    
    var body: some View {
        HStack {
            VStack(alignment: .leading, spacing: 2) {
                Text("⚡ APEX GAME OPTIMIZER")
                    .font(.system(size: 16, weight: .black))
                    .foregroundColor(.borderOrange)
                
                Text("C++20 NATIVE • ARM NEON SIMD • 120 FPS")
                    .font(.system(size: 10, weight: .semibold))
                    .foregroundColor(.textSecond)
            }
            
            Spacer()
            
            Text(isLoaded ? "ONLINE (\(cpuCores) CORES)" : "OFFLINE")
                .font(.system(size: 10, weight: .bold))
                .foregroundColor(isLoaded ? .borderGreen : .borderRed)
                .padding(.horizontal, 10)
                .padding(.vertical, 4)
                .background((isLoaded ? Color.borderGreen : .borderRed).opacity(0.1))
                .cornerRadius(20)
                .overlay(
                    RoundedRectangle(cornerRadius: 20)
                        .stroke(isLoaded ? Color.borderGreen : .borderRed, lineWidth: 1)
                )
        }
        .padding(.horizontal, 16)
        .padding(.vertical, 12)
        .background(Color.bgPrimary)
    }
}

// MARK: - Bottom Navigation

struct BottomNav: View {
    @Binding var selectedTab: Int
    
    var body: some View {
        HStack(spacing: 0) {
            TabButton(emoji: "📊", title: "Dashboard", isSelected: selectedTab == 0, color: .borderOrange) {
                selectedTab = 0
            }
            
            TabButton(emoji: "⚡", title: "Módulos (53)", isSelected: selectedTab == 1, color: .borderGreen) {
                selectedTab = 1
            }
            
            TabButton(emoji: "🎮", title: "Jogos", isSelected: selectedTab == 2, color: .borderCyan) {
                selectedTab = 2
            }
        }
        .background(Color(red: 0.055, green: 0.063, blue: 0.082))
    }
}

struct TabButton: View {
    let emoji: String
    let title: String
    let isSelected: Bool
    let color: Color
    let action: () -> Void
    
    var body: some View {
        Button(action: action) {
            VStack(spacing: 4) {
                Text(emoji)
                    .font(.title3)
                Text(title)
                    .font(.system(size: 11))
                    .foregroundColor(isSelected ? color : .textSecond)
            }
            .frame(maxWidth: .infinity)
            .padding(.vertical, 8)
            .background(isSelected ? color.opacity(0.15) : .clear)
        }
    }
}
