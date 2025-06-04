#ifndef CARTEIRA_HPP
#define CARTEIRA_HPP

#include <string>

class Carteira {
private:
    int id;
    std::string nomeTitular;
    std::string corretora;
public:
    Carteira();
    Carteira(const std::string& nome, const std::string& corretora);
    void setId(int id);
    int getId() const;
    std::string getNomeTitular() const;
    void setNomeTitular(const std::string& nome);
    std::string getCorretora() const;
    void setCorretora(const std::string& corretora);
};

#endif