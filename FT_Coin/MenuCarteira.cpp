#include "MenuCarteira.hpp"
#include <iostream>
#include <string>
#include <limits>

MenuCarteira::MenuCarteira(InMemoryCarteiraDAO& dao) : dao(dao) {}

void MenuCarteira::exibirOpcoes() const {
    std::cout << "1. Nova Carteira\n"
        << "2. Editar Carteira\n"
        << "3. Excluir Carteira\n"
        << "4. Listar Todas\n"
        << "5. Voltar\n"
        << "Escolha: ";
}

void MenuCarteira::criarCarteira() {
    try {
        std::string nome, corretora;

        std::cout << "\nNovo Cadastro:\nNome do titular: ";
        std::getline(std::cin, nome);

        if (nome.empty()) {
            throw std::runtime_error("Nome do titular não pode ser vazio!");
        }

        std::cout << "Corretora: ";
        std::getline(std::cin, corretora);

        Carteira nova(nome, corretora);
        dao.criar(nova);
        std::cout << "\nCarteira criada com sucesso! ID: " << nova.getId() << std::endl;

    }
    catch (const std::exception& e) {
        std::cerr << "\nErro ao criar carteira: " << e.what() << std::endl;
    }
    aguardarEnter();
}

void MenuCarteira::editarCarteira() {
    try {
        int id;
        std::cout << "\nEditar Carteira\nID da carteira: ";
        if (!(std::cin >> id)) {
            limparBuffer();
            throw std::runtime_error("ID inválido!");
        }
        limparBuffer();

        Carteira* c = dao.buscarPorId(id);
        if (c) {
            std::string novoNome;
            std::cout << "Novo nome (" << c->getNomeTitular() << "): ";
            std::getline(std::cin, novoNome);

            if (!novoNome.empty()) {
                c->setNomeTitular(novoNome);
                dao.atualizar(*c);
                std::cout << "Carteira atualizada com sucesso!\n";
            }
        }
        else {
            throw std::runtime_error("Carteira não encontrada!");
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
    }
    aguardarEnter();
}

void MenuCarteira::excluirCarteira() {
    try {
        int id;
        std::cout << "\nExcluir Carteira\nID: ";
        if (!(std::cin >> id)) {
            limparBuffer();
            throw std::runtime_error("ID inválido!");
        }
        limparBuffer();

        dao.remover(id);
        std::cout << "Carteira removida com sucesso!\n";
    }
    catch (const std::exception& e) {
        std::cerr << "Erro: " << e.what() << std::endl;
    }
    aguardarEnter();
}

void MenuCarteira::listarCarteiras() const {
    try {
        auto lista = dao.listarTodos();
        if (lista.empty()) {
            std::cout << "Nenhuma carteira cadastrada.\n";
            aguardarEnter();
            return;
        }

        std::cout << "\nLista de Carteiras:\n";
        for (const auto& c : lista) {
            std::cout << "ID: " << c.getId()
                << " | Titular: " << c.getNomeTitular()
                << " | Corretora: " << c.getCorretora() << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Erro ao listar carteiras: " << e.what() << std::endl;
    }
    aguardarEnter();
}

void MenuCarteira::exibir() {
    int opcao;
    do {
        exibirCabecalho("MENU CARTEIRA");
        exibirOpcoes();

        if (!(std::cin >> opcao)) {
            limparBuffer();
            std::cout << "\nEntrada inválida!\n";
            aguardarEnter();
            continue;
        }
        limparBuffer();

        switch (opcao) {
        case 1: criarCarteira(); break;
        case 2: editarCarteira(); break;
        case 3: excluirCarteira(); break;
        case 4: listarCarteiras(); break;
        case 5: break;
        default:
            std::cout << "\nOpção inválida!\n";
            aguardarEnter();
        }
    } while (opcao != 5);
}