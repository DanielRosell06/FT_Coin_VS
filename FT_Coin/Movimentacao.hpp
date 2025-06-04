#ifndef MOVIMENTACAO_HPP
#define MOVIMENTACAO_HPP

#include <string>

class Movimentacao {
private:
    int id;
    int carteiraId;
    std::string dataOperacao;
    char tipoOperacao; // 'C' (Compra) ou 'V' (Venda)
    double quantidade;
public:
    Movimentacao();
    Movimentacao(int carteiraId, const std::string& data, char tipo, double quantidade);
    void setId(int id);
    int getId() const;
    int getCarteiraId() const;
    void setCarteiraId(int id);
    std::string getDataOperacao() const;
    void setDataOperacao(const std::string& data);
    char getTipoOperacao() const;
    void setTipoOperacao(char tipo);
    double getQuantidade() const;
    void setQuantidade(double quantidade);
};

#endif