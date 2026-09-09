import SwiftUI

// MARK: - Games Screen

struct GamesScreen: View {
    let onLaunchGame: (GameInfo) -> Void
    
    var body: some View {
        ScrollView {
            VStack(spacing: 16) {
                Text("JOGOS SUPORTADOS")
                    .font(.system(size: 14, weight: .bold))
                    .foregroundColor(.borderCyan)
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .padding(.horizontal, 16)
                    .padding(.top, 12)
                
                Text("Selecione um jogo para iniciar a otimização automática com todos os 53 módulos ativos.")
                    .font(.system(size: 12))
                    .foregroundColor(.textSecond)
                    .frame(maxWidth: .infinity, alignment: .leading)
                    .padding(.horizontal, 16)
                
                ForEach(GameCatalog.supportedGames) { game in
                    GameCard(game: game) {
                        onLaunchGame(game)
                    }
                }
            }
            .padding(.bottom, 16)
        }
    }
}

// MARK: - Game Card

struct GameCard: View {
    let game: GameInfo
    let onLaunch: () -> Void
    
    var body: some View {
        HStack(spacing: 14) {
            Text(game.iconEmoji)
                .font(.system(size: 36))
                .frame(width: 56, height: 56)
                .background(Color.bgCard)
                .cornerRadius(12)
            
            VStack(alignment: .leading, spacing: 4) {
                Text(game.name)
                    .font(.system(size: 15, weight: .bold))
                    .foregroundColor(.textPrimary)
                
                Text(game.packageName)
                    .font(.system(size: 11, design: .monospaced))
                    .foregroundColor(.textSecond)
                
                HStack(spacing: 4) {
                    Text("⚡")
                    Text("Otimização automática com 53 módulos")
                        .font(.system(size: 10))
                        .foregroundColor(.borderGreen)
                }
            }
            
            Spacer()
            
            Button(action: onLaunch) {
                Text("INICIAR")
                    .font(.system(size: 12, weight: .bold))
                    .foregroundColor(.black)
                    .padding(.horizontal, 16)
                    .padding(.vertical, 8)
                    .background(Color.borderCyan)
                    .cornerRadius(8)
            }
        }
        .padding(14)
        .background(Color.bgCard)
        .cornerRadius(14)
        .overlay(
            RoundedRectangle(cornerRadius: 14)
                .stroke(Color.borderCyan.opacity(0.3), lineWidth: 1)
        )
        .padding(.horizontal, 16)
    }
}
