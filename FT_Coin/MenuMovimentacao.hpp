#pragma once
#include "MenuBase.hpp"
#include "MovimentacaoDAO.hpp"
#include "CarteiraDAO.hpp"
#include "OracleDAO.hpp"
#include "Movimentacao.hpp"
#include "Oracle.hpp"

class MenuMovimentacao : public MenuBase {
private:
    InMemoryMovimentacaoDAO& movDAO;
    InMemoryCarteiraDAO& cartDAO;
    InMemoryOracleDAO& oraDAO;

    void exibirOpcoes() const;
    void registrarOperacao(char tipo);

public:
    MenuMovimentacao(InMemoryMovimentacaoDAO& mDao,
        InMemoryCarteiraDAO& cDao,
        InMemoryOracleDAO& oDao);
    void exibir();
};