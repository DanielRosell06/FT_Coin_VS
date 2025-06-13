
#include <iostream>
#include <cstdlib>

#include "InterfaceUsuario.hpp"
#include "MariaDBConn.hpp"

int main() {
    try {
        MariaDBConn dbConn(
            "143.106.243.64",      // host
            3306,             // porta
            "PooI_25_B02",  // database
            "PooI_25_B02",        // usuário
            "02HecSGHFE"           // senha
        );

        InMemoryCarteiraDAO carteiraDAO;
        InMemoryMovimentacaoDAO movimentacaoDAO;
        InMemoryOracleDAO oracleDAO;

        InterfaceUsuario interface(carteiraDAO, movimentacaoDAO, oracleDAO);
        interface.iniciar();

    }
    catch (const std::exception& e) {
        std::cerr << "Erro ao inicializar a aplicação: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
