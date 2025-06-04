#include "MenuMovimentacao.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <iomanip>

// Construtor
MenuMovimentacao::MenuMovimentacao(
    InMemoryMovimentacaoDAO& mDao,
    InMemoryCarteiraDAO& cDao,
    InMemoryOracleDAO& oDao
) : movDAO(mDao), cartDAO(cDao), oraDAO(oDao) {
}

// Exibe as opções do menu
void MenuMovimentacao::exibirOpcoes() const {
    std::cout << "1. Registrar Compra\n"
        << "2. Registrar Venda\n"
        << "3. Voltar\n"
        << "Escolha: ";
}

// Registra uma operação (compra/venda)
void MenuMovimentacao::registrarOperacao(char tipo) {
    try {
        int idCarteira;
        std::string data;
        double quantidade;

        std::cout << "ID da Carteira: ";
        if (!(std::cin >> idCarteira)) {
            limparBuffer();
            throw std::runtime_error("Entrada inválida para ID!");
        }

        if (!cartDAO.buscarPorId(idCarteira)) {
            throw std::runtime_error("Carteira não encontrada!");
        }

        std::cout << "Data (AAAA-MM-DD): ";
        std::cin >> data;

        std::cout << "Quantidade: ";
        if (!(std::cin >> quantidade) || quantidade <= 0) {
            limparBuffer();
            throw std::runtime_error("Quantidade inválida!");
        }

        // Cria e registra a movimentação
        Movimentacao mov(idCarteira, data, tipo, quantidade);
        movDAO.criar(mov);

        // Gera cotação aleatória entre 150 e 160
        std::srand(std::time(nullptr));
        double cotacao = 150.0 + (static_cast<double>(std::rand()) / RAND_MAX * 10.0);
        oraDAO.adicionarCotacao(Oracle(data, cotacao));

        std::cout << "\nOperação registrada com sucesso!\n";
        std::cout << "Tipo: " << (tipo == 'C' ? "Compra" : "Venda") << "\n";
        std::cout << "Quantidade: " << quantidade << "\n";
        std::cout << "Cotação registrada: R$ " << std::fixed << std::setprecision(2) << cotacao << "\n";

    }
    catch (const std::exception& e) {
        std::cerr << "\nErro: " << e.what() << "\n";
    }
    limparBuffer();
    aguardarEnter();
}

// Exibe o menu principal de movimentações
void MenuMovimentacao::exibir() {
    int opcao;
    do {
        exibirCabecalho("MOVIMENTAÇÃO");
        exibirOpcoes();

        if (!(std::cin >> opcao)) {
            limparBuffer();
            std::cout << "\nOpção inválida!\n";
            aguardarEnter();
            continue;
        }
        limparBuffer();

        switch (opcao) {
        case 1:
        case 2:
            registrarOperacao(opcao == 1 ? 'C' : 'V');
            break;
        case 3:
            break;
        default:
            std::cout << "\nOpção inválida!\n";
            aguardarEnter();
        }
    } while (opcao != 3);
}