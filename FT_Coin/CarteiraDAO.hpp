#ifndef CARTEIRA_DAO_HPP
#define CARTEIRA_DAO_HPP

#include "Carteira.hpp"
#include "MariaDBConn.hpp"
#include <vector>

class CarteiraDAO {
public:
    virtual ~CarteiraDAO() {}
    virtual void criar(Carteira& carteira) = 0;
    virtual Carteira* buscarPorId(int id) = 0;
    virtual void atualizar(Carteira& carteira) = 0;
    virtual void remover(int id) = 0;
    virtual std::vector<Carteira> listarTodos() = 0;
};

class InMemoryCarteiraDAO : public CarteiraDAO {
private:
    std::vector<Carteira> carteiras;
    int proximoId;
public:
    InMemoryCarteiraDAO() : proximoId(1) {}
    void criar(Carteira& carteira) override;
    Carteira* buscarPorId(int id) override;
    void atualizar(Carteira& carteira) override;
    void remover(int id) override;
    std::vector<Carteira> listarTodos() override;
};

#endif