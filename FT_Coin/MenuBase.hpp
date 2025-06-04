// include/View/MenuBase.hpp
#pragma once
#include <iostream>
#include <limits>

class MenuBase {
protected:
    void limparTela() const;

    void aguardarEnter() const;

    void limparBuffer() const;

    void exibirCabecalho(const std::string& titulo) const;
};