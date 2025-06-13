#include "InterfaceUsuario.hpp"
#include <iostream>
#include <fstream>

InterfaceUsuario::InterfaceUsuario(
    InMemoryCarteiraDAO& cDao,
    InMemoryMovimentacaoDAO& mDao,
    InMemoryOracleDAO& oDao
) : carteiraDAO(cDao),
movimentacaoDAO(mDao),
oracleDAO(oDao),
menuCarteira(cDao),
menuMovimentacao(mDao, cDao, oDao),
menuRelatorios(cDao, mDao, oDao) {
}

void InterfaceUsuario::exibirMenuPrincipal() {
    limparTela();
    std::cout << "=== FT COIN - Sistema de Gerenciamento ===\n"
        << "1. Carteira\n"
        << "2. Movimentação\n"
        << "3. Relatórios\n"
        << "4. Ajuda\n"
        << "5. Créditos\n"
        << "6. Sair\n"
        << "Escolha uma opção: ";
}

void InterfaceUsuario::exibirAjuda() {
    exibirCabecalho("AJUDA");

    std::ifstream arquivo("ajuda.txt");  // Abre o arquivo de texto

    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de ajuda!\n";
        aguardarEnter();
        return;
    }

    std::string linha;
    while (std::getline(arquivo, linha)) {  // Lê cada linha do arquivo
        std::cout << linha << "\n";         // Imprime a linha
    }

    arquivo.close();  // Fecha o arquivo
    aguardarEnter();
}

void InterfaceUsuario::exibirCreditos() {
    exibirCabecalho("CRÉDITOS");
    std::cout << "Sistema FT Coin\n"
        << "Versão 1.0\n"
        << "Desenvolvido por: Daniel Rosell; Gabriel Sorensen; João Pedro Mamede; Lucas do Amaral; Rafael Ozório\n"
        << "Copyright © 2023\n"
        << "Disciplina: SI300 - Programação Orientada a Objetos I\n";
    aguardarEnter();
}

void InterfaceUsuario::iniciar() {
    int opcao;
    do {
        exibirMenuPrincipal();

        if (!(std::cin >> opcao)) {
            limparBuffer();
            std::cout << "\nEntrada inválida! Tente novamente.\n";
            aguardarEnter();
            continue;
        }
        limparBuffer();

        switch (opcao) {
        case 1:
            menuCarteira.exibir();
            break;
        case 2:
            menuMovimentacao.exibir();
            break;
        case 3:
            menuRelatorios.exibir();
            break;
        case 4:
            exibirAjuda();
            break;
        case 5:
            exibirCreditos();
            break;
        case 6:
            std::cout << "\nEncerrando o sistema...\n";
            break;
        default:
            std::cout << "\nOpção inválida! Tente novamente.\n";
            aguardarEnter();
        }
    } while (opcao != 6);
}