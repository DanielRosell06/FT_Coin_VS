#include "MenuBase.hpp"
#include <iostream>
#include <limits>
#include <cstdlib> // Para system()

// Implementação dos métodos protegidos

void MenuBase::limparTela() const {
    // Comando universal para limpar tela
    system("cls || clear");
}

void MenuBase::aguardarEnter() const {
    std::cout << "\nPressione Enter para continuar...";
    std::cin.get();
}

void MenuBase::limparBuffer() const {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void MenuBase::exibirCabecalho(const std::string& titulo) const {
    limparTela();
    std::cout << "=== " << titulo << " ===\n\n";
}