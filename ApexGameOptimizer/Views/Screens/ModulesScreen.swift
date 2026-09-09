import SwiftUI

// MARK: - Modules Screen

struct ModulesScreen: View {
    @ObservedObject var viewModel: OptimizerViewModel
    
    var body: some View {
        VStack(spacing: 0) {
            // Header
            Text("MÓDULOS DE OTIMIZAÇÃO (53 ALGORITMOS)")
                .font(.system(size: 14, weight: .bold))
                .foregroundColor(.borderOrange)
                .frame(maxWidth: .infinity, alignment: .leading)
                .padding(.horizontal, 16)
                .padding(.top, 12)
                .padding(.bottom, 8)
            
            // Category Picker
            ScrollView(.horizontal, showsIndicators: false) {
                HStack(spacing: 8) {
                    ForEach(ModuleCategory.allCases) { category in
                        CategoryChip(
                            title: category.displayName,
                            isSelected: viewModel.selectedCategory == category,
                            color: colorForCategory(category)
                        ) {
                            viewModel.selectCategory(category)
                        }
                    }
                }
                .padding(.horizontal, 16)
                .padding(.bottom, 12)
            }
            
            // Module List
            let filteredModules = viewModel.getModulesForCategory(viewModel.selectedCategory)
            let categoryColor = colorForCategory(viewModel.selectedCategory)
            
            ScrollView {
                LazyVStack(spacing: 8) {
                    ForEach(filteredModules) { module in
                        ModuleCard(
                            module: module,
                            result: viewModel.moduleResults[module.id],
                            isExecuting: viewModel.isExecuting,
                            accentColor: categoryColor
                        ) {
                            viewModel.runModule(module.id)
                        }
                    }
                }
                .padding(.horizontal, 16)
                .padding(.bottom, 16)
            }
        }
    }
    
    private func colorForCategory(_ category: ModuleCategory) -> Color {
        switch category {
        case .systemRam: return .borderOrange
        case .aimPhysics: return .borderGreen
        case .displayGraphics: return .borderCyan
        case .audioHaptics: return .borderPurple
        case .aiSecurity: return .borderRed
        }
    }
}

// MARK: - Category Chip

struct CategoryChip: View {
    let title: String
    let isSelected: Bool
    let color: Color
    let action: () -> Void
    
    var body: some View {
        Button(action: action) {
            Text(title)
                .font(.system(size: 12, weight: isSelected ? .bold : .regular))
                .foregroundColor(isSelected ? color : .textSecond)
                .padding(.horizontal, 12)
                .padding(.vertical, 6)
                .background(isSelected ? color.opacity(0.2) : Color.bgCard)
                .cornerRadius(8)
                .overlay(
                    RoundedRectangle(cornerRadius: 8)
                        .stroke(isSelected ? color : Color(red: 0.13, green: 0.15, blue: 0.2), lineWidth: 1)
                )
        }
    }
}

// MARK: - Module Card

struct ModuleCard: View {
    let module: OptimizerModule
    let result: String?
    let isExecuting: Bool
    let accentColor: Color
    let onExecute: () -> Void
    
    var body: some View {
        VStack(alignment: .leading, spacing: 8) {
            HStack(alignment: .top, spacing: 12) {
                Text(module.iconEmoji)
                    .font(.title2)
                    .frame(width: 36, height: 36)
                
                VStack(alignment: .leading, spacing: 4) {
                    Text(module.title)
                        .font(.system(size: 14, weight: .bold))
                        .foregroundColor(.textPrimary)
                    
                    Text(module.description)
                        .font(.system(size: 12))
                        .foregroundColor(.textSecond)
                        .lineLimit(2)
                }
                
                Spacer()
                
                Button(action: onExecute) {
                    Text("RODAR")
                        .font(.system(size: 11, weight: .bold))
                        .foregroundColor(.white)
                        .padding(.horizontal, 14)
                        .padding(.vertical, 8)
                        .background(accentColor)
                        .cornerRadius(8)
                }
                .disabled(isExecuting)
            }
            
            if let result = result {
                Divider()
                    .background(accentColor.opacity(0.3))
                
                Text(result)
                    .font(.system(size: 11, design: .monospaced))
                    .foregroundColor(.textPrimary)
                    .lineLimit(4)
                    .padding(.top, 4)
            }
        }
        .padding(14)
        .background(Color.bgCard)
        .cornerRadius(14)
        .overlay(
            RoundedRectangle(cornerRadius: 14)
                .stroke(accentColor.opacity(0.3), lineWidth: 1)
        )
    }
}
