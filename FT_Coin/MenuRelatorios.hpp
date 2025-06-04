#pragma once
#include "MenuBase.hpp"
#include "CarteiraDAO.hpp"
#include "MovimentacaoDAO.hpp"
#include "OracleDAO.hpp"
#include "Carteira.hpp"
#include "Movimentacao.hpp"
#include "Oracle.hpp"

class MenuRelatorios : public MenuBase {
private:
    InMemoryCarteiraDAO& cartDAO;
    InMemoryMovimentacaoDAO& movDAO;
    InMemoryOracleDAO& oraDAO;

    void exibirOpcoes() const;
    void listarPorId() const;
    void listarPorNome() const;
    void exibirSaldo() const;
    void exibirHistorico() const;
    void exibirGanhosPerdas() const;

public:
    MenuRelatorios(InMemoryCarteiraDAO& cDao,
        InMemoryMovimentacaoDAO& mDao,
        InMemoryOracleDAO& oDao);
    void exibir();
};