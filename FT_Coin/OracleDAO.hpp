#ifndef ORACLE_DAO_HPP
#define ORACLE_DAO_HPP

#include "Oracle.hpp"
#include <vector>

class OracleDAO {
public:
    virtual ~OracleDAO() {}
    virtual void adicionarCotacao(const Oracle& oracle) = 0;
    virtual double buscarCotacaoPorData(const std::string& data) = 0;
    virtual std::vector<Oracle> listarTodasCotacoes() = 0;
};

class InMemoryOracleDAO : public OracleDAO {
private:
    std::vector<Oracle> cotacoes;
public:
    void adicionarCotacao(const Oracle& oracle) override;
    double buscarCotacaoPorData(const std::string& data) override;
    std::vector<Oracle> listarTodasCotacoes() override;
};

#endif