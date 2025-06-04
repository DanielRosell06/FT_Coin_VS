#include "Movimentacao.hpp"

Movimentacao::Movimentacao()
    : id(0), carteiraId(0), dataOperacao(""), tipoOperacao('C'), quantidade(0) {
}

Movimentacao::Movimentacao(int carteiraId, const std::string& data, char tipo, double quantidade)
    : id(0), carteiraId(carteiraId), dataOperacao(data), tipoOperacao(tipo), quantidade(quantidade) {
}

// Setters e Getters
void Movimentacao::setId(int id) { this->id = id; }
int Movimentacao::getId() const { return id; }
int Movimentacao::getCarteiraId() const { return carteiraId; }
void Movimentacao::setCarteiraId(int id) { carteiraId = id; }
std::string Movimentacao::getDataOperacao() const { return dataOperacao; }
void Movimentacao::setDataOperacao(const std::string& data) { dataOperacao = data; }
char Movimentacao::getTipoOperacao() const { return tipoOperacao; }
void Movimentacao::setTipoOperacao(char tipo) { tipoOperacao = tipo; }
double Movimentacao::getQuantidade() const { return quantidade; }
void Movimentacao::setQuantidade(double qtd) { quantidade = qtd; }