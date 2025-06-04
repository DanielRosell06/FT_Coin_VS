#include "MenuRelatorios.hpp"
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <ctime>
#include <limits>
#include <sstream>
#include <memory>  // Para std::unique_ptr

// Fun��o auxiliar port�vel para obter data atual formatada
std::string getDataAtual() {
    std::time_t t = std::time(nullptr);
    std::tm now;
#ifdef _WIN32
    localtime_s(&now, &t);
#else
    localtime_r(&t, &now);
#endif
    char buffer[11];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d", &now);
    return std::string(buffer);
}

MenuRelatorios::MenuRelatorios(
    InMemoryCarteiraDAO& cDao,
    InMemoryMovimentacaoDAO& mDao,
    InMemoryOracleDAO& oDao
) : cartDAO(cDao), movDAO(mDao), oraDAO(oDao) {
}

void MenuRelatorios::exibirOpcoes() const {
    std::cout << "\n1. Listar Carteiras por ID\n"
        << "2. Listar Carteiras por Nome\n"
        << "3. Saldo de uma Carteira\n"
        << "4. Hist�rico de Movimenta��es\n"
        << "5. Ganhos/Perdas Totais\n"
        << "6. Voltar\n"
        << "Escolha: ";
}

void MenuRelatorios::listarPorId() const {
    try {
        exibirCabecalho("CARTEIRAS (ORDEM DE ID)");
        auto carteiras = cartDAO.listarTodos();

        if (carteiras.empty()) {
            std::cout << "Nenhuma carteira cadastrada!\n";
            return;
        }

        std::cout << std::left
            << std::setw(6) << "ID"
            << std::setw(25) << "Titular"
            << "Corretora\n"
            << std::string(60, '-') << "\n";

        for (const auto& c : carteiras) {
            std::cout << std::setw(6) << c.getId()
                << std::setw(25) << c.getNomeTitular()
                << c.getCorretora() << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "\nErro: " << e.what() << "\n";
    }
    aguardarEnter();
}

void MenuRelatorios::listarPorNome() const {
    try {
        exibirCabecalho("CARTEIRAS (ORDEM ALFAB�TICA)");
        auto carteiras = cartDAO.listarTodos();

        if (carteiras.empty()) {
            std::cout << "Nenhuma carteira cadastrada!\n";
            return;
        }

        std::sort(carteiras.begin(), carteiras.end(),
            [](const Carteira& a, const Carteira& b) {
                return a.getNomeTitular() < b.getNomeTitular();
            });

        std::cout << std::left
            << std::setw(25) << "Titular"
            << std::setw(6) << "ID"
            << "Corretora\n"
            << std::string(60, '-') << "\n";

        for (const auto& c : carteiras) {
            std::cout << std::setw(25) << c.getNomeTitular()
                << std::setw(6) << c.getId()
                << c.getCorretora() << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "\nErro: " << e.what() << "\n";
    }
    aguardarEnter();
}

void MenuRelatorios::exibirSaldo() const {
    try {
        exibirCabecalho("SALDO DA CARTEIRA");
        int id;
        std::cout << "ID da Carteira: ";

        if (!(std::cin >> id)) {
            limparBuffer();
            throw std::runtime_error("ID inv�lido!");
        }
        limparBuffer();

        // Usar smart pointer para gerenciar mem�ria
        std::unique_ptr<Carteira> carteira(cartDAO.buscarPorId(id));
        if (!carteira) {
            throw std::runtime_error("Carteira n�o encontrada!");
        }

        auto movimentacoes = movDAO.listarPorCarteira(id);
        double saldoQuantidade = 0.0;
        for (const auto& m : movimentacoes) {
            saldoQuantidade += (m.getTipoOperacao() == 'C') ?
                m.getQuantidade() : -m.getQuantidade();
        }

        std::string dataAtual = getDataAtual();
        double cotacao = oraDAO.buscarCotacaoPorData(dataAtual);

        std::cout << "\nRESUMO DA CARTEIRA ID: " << carteira->getId()
            << "\nTitular: " << carteira->getNomeTitular()
            << "\nCorretora: " << carteira->getCorretora()
            << "\n\nSaldo em Quantidade: " << std::fixed << std::setprecision(4)
            << saldoQuantidade << " FT"
            << "\nCota��o Atual (" << dataAtual << "): R$ "
            << std::fixed << std::setprecision(2) << cotacao
            << "\nValor Total: R$ " << std::fixed << std::setprecision(2)
            << (saldoQuantidade * cotacao) << "\n";

    }
    catch (const std::exception& e) {
        std::cerr << "\nErro: " << e.what() << "\n";
    }
    aguardarEnter();
}

void MenuRelatorios::exibirHistorico() const {
    try {
        exibirCabecalho("HIST�RICO DE MOVIMENTA��ES");
        int id;
        std::cout << "ID da Carteira: ";

        if (!(std::cin >> id)) {
            limparBuffer();
            throw std::runtime_error("ID inv�lido!");
        }
        limparBuffer();

        // Usar smart pointer para gerenciar mem�ria
        std::unique_ptr<Carteira> carteira(cartDAO.buscarPorId(id));
        if (!carteira) {
            throw std::runtime_error("Carteira n�o encontrada!");
        }

        auto movimentacoes = movDAO.listarPorCarteira(id);
        if (movimentacoes.empty()) {
            std::cout << "Nenhuma movimenta��o registrada!\n";
            return;
        }

        // Ordenar por data (mais recente primeiro)
        std::sort(movimentacoes.begin(), movimentacoes.end(),
            [](const Movimentacao& a, const Movimentacao& b) {
                return a.getDataOperacao() > b.getDataOperacao();
            });

        std::cout << "\nCarteira ID: " << carteira->getId()
            << " | Titular: " << carteira->getNomeTitular() << "\n\n"
            << std::left << std::setw(12) << "Data"
            << std::setw(10) << "Opera��o"
            << std::setw(15) << "Quantidade (FT)"
            << "Valor (R$)\n"
            << std::string(55, '-') << "\n";

        double totalInvestido = 0.0;

        for (const auto& m : movimentacoes) {
            double cotacao = oraDAO.buscarCotacaoPorData(m.getDataOperacao());
            double valorOperacao = m.getQuantidade() * cotacao;

            if (m.getTipoOperacao() == 'C') {
                totalInvestido += valorOperacao;
            }
            else {
                totalInvestido -= valorOperacao;
            }

            std::cout << std::setw(12) << m.getDataOperacao()
                << std::setw(10) << (m.getTipoOperacao() == 'C' ? "Compra" : "Venda")
                << std::fixed << std::setprecision(4) << std::setw(15)
                << m.getQuantidade()
                << "R$ " << std::fixed << std::setprecision(2)
                << valorOperacao << "\n";
        }

        std::cout << "\nTotal Investido: R$ " << std::fixed << std::setprecision(2)
            << totalInvestido << "\n";

    }
    catch (const std::exception& e) {
        std::cerr << "\nErro: " << e.what() << "\n";
    }
    aguardarEnter();
}

void MenuRelatorios::exibirGanhosPerdas() const {
    try {
        exibirCabecalho("GANHOS/PERDAS TOTAIS");
        auto carteiras = cartDAO.listarTodos();
        auto cotacoes = oraDAO.listarTodasCotacoes();

        if (cotacoes.empty()) {
            throw std::runtime_error("Nenhuma cota��o registrada!");
        }

        // Ordenar cota��es por data (mais recente primeiro)
        std::sort(cotacoes.begin(), cotacoes.end(),
            [](const Oracle& a, const Oracle& b) {
                return a.getData() > b.getData();
            });

        double ultimaCotacao = cotacoes[0].getCotacao();
        std::string dataUltimaCotacao = cotacoes[0].getData();

        std::cout << "�ltima cota��o dispon�vel: " << dataUltimaCotacao
            << " | Valor: R$ " << std::fixed << std::setprecision(2)
            << ultimaCotacao << "\n\n";

        for (const auto& c : carteiras) {
            auto movimentacoes = movDAO.listarPorCarteira(c.getId());
            if (movimentacoes.empty()) continue;

            double totalInvestido = 0.0;
            double saldoQuantidade = 0.0;

            for (const auto& m : movimentacoes) {
                double cotacao = oraDAO.buscarCotacaoPorData(m.getDataOperacao());
                double valorOperacao = m.getQuantidade() * cotacao;

                if (m.getTipoOperacao() == 'C') {
                    totalInvestido += valorOperacao;
                    saldoQuantidade += m.getQuantidade();
                }
                else {
                    totalInvestido -= valorOperacao;
                    saldoQuantidade -= m.getQuantidade();
                }
            }

            double valorAtual = saldoQuantidade * ultimaCotacao;
            double resultado = valorAtual - totalInvestido;

            std::cout << "Carteira ID: " << c.getId()
                << " | Titular: " << c.getNomeTitular()
                << "\n  Investimento Total: R$ " << std::fixed << std::setprecision(2)
                << totalInvestido
                << "\n  Valor Atual: R$ " << valorAtual
                << "\n  Resultado: " << (resultado >= 0 ? "+" : "")
                << "R$ " << std::fixed << std::setprecision(2) << resultado
                << " (" << (resultado >= 0 ? "Lucro" : "Preju�zo") << ")"
                << "\n" << std::string(60, '-') << "\n";
        }

    }
    catch (const std::exception& e) {
        std::cerr << "\nErro: " << e.what() << "\n";
    }
    aguardarEnter();
}

void MenuRelatorios::exibir() {
    int opcao;
    do {
        exibirCabecalho("RELAT�RIOS");
        exibirOpcoes();

        if (!(std::cin >> opcao)) {
            limparBuffer();
            std::cout << "\nOp��o inv�lida!\n";
            aguardarEnter();
            continue;
        }
        limparBuffer();

        switch (opcao) {
        case 1: listarPorId(); break;
        case 2: listarPorNome(); break;
        case 3: exibirSaldo(); break;
        case 4: exibirHistorico(); break;
        case 5: exibirGanhosPerdas(); break;
        case 6: break;
        default:
            std::cout << "\nOp��o inv�lida!\n";
            aguardarEnter();
        }
    } while (opcao != 6);
}