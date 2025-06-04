#pragma once
#include "MenuBase.hpp"
#include "MenuCarteira.hpp"
#include "MenuMovimentacao.hpp"
#include "MenuRelatorios.hpp"
#include "CarteiraDAO.hpp"
#include "MovimentacaoDAO.hpp"
#include "OracleDAO.hpp"

class InterfaceUsuario : public MenuBase {
private:
    InMemoryCarteiraDAO& carteiraDAO;
    InMemoryMovimentacaoDAO& movimentacaoDAO;
    InMemoryOracleDAO& oracleDAO;

    MenuCarteira menuCarteira;
    MenuMovimentacao menuMovimentacao;
    MenuRelatorios menuRelatorios;

    void exibirMenuPrincipal();
    void exibirAjuda();
    void exibirCreditos();

public:
    InterfaceUsuario(InMemoryCarteiraDAO& cDao,
        InMemoryMovimentacaoDAO& mDao,
        InMemoryOracleDAO& oDao);
    void iniciar();
};