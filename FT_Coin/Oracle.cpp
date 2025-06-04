#include "Oracle.hpp"

Oracle::Oracle(const std::string& data, double cotacao)
    : data(data), cotacao(cotacao) {
}

std::string Oracle::getData() const { return data; }
double Oracle::getCotacao() const { return cotacao; }
void Oracle::setCotacao(double cotacao) { this->cotacao = cotacao; }