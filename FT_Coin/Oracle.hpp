#ifndef ORACLE_HPP
#define ORACLE_HPP

#include <string>

class Oracle {
private:
    std::string data;
    double cotacao;
public:
    Oracle(const std::string& data, double cotacao);
    std::string getData() const;
    double getCotacao() const;
    void setCotacao(double cotacao);
};

#endif