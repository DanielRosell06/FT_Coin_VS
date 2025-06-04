#ifndef MOVIMENTACAO_DAO_HPP
#define MOVIMENTACAO_DAO_HPP

#include "Movimentacao.hpp"
#include <vector>

class MovimentacaoDAO {
public:
    virtual ~MovimentacaoDAO() {}
    virtual void criar(Movimentacao& mov) = 0;
    virtual Movimentacao* buscarPorId(int id) = 0;
    virtual void atualizar(Movimentacao& mov) = 0;
    virtual void remover(int id) = 0;
    virtual std::vector<Movimentacao> listarPorCarteira(int carteiraId) = 0;
};

class InMemoryMovimentacaoDAO : public MovimentacaoDAO {
private:
    std::vector<Movimentacao> movimentacoes;
    int proximoId;
public:
    InMemoryMovimentacaoDAO() : proximoId(1) {}
    void criar(Movimentacao& mov) override;
    Movimentacao* buscarPorId(int id) override;
    void atualizar(Movimentacao& mov) override;
    void remover(int id) override;
    std::vector<Movimentacao> listarPorCarteira(int carteiraId) override;
};

#endif