#pragma once
#include "MenuBase.hpp"
#include "CarteiraDAO.hpp"
#include "Carteira.hpp"

class MenuCarteira : public MenuBase {
private:
    InMemoryCarteiraDAO& dao;

    void exibirOpcoes() const;
    void criarCarteira();
    void editarCarteira();
    void excluirCarteira();
    void listarCarteiras() const;

public:
    explicit MenuCarteira(InMemoryCarteiraDAO& dao);
    void exibir();
};