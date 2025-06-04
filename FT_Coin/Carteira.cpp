#include "Carteira.hpp"

Carteira::Carteira() : id(0) {}
Carteira::Carteira(const std::string& nome, const std::string& corretora)
    : id(0), nomeTitular(nome), corretora(corretora) {
}

void Carteira::setId(int id) { this->id = id; }
int Carteira::getId() const { return id; }
std::string Carteira::getNomeTitular() const { return nomeTitular; }
void Carteira::setNomeTitular(const std::string& nome) { nomeTitular = nome; }
std::string Carteira::getCorretora() const { return corretora; }
void Carteira::setCorretora(const std::string& corretora) { this->corretora = corretora; }