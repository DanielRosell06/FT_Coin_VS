// main.cpp
#include <iostream>
#include <cstdlib>

#include "InterfaceUsuario.hpp"
#include "MariaDBConn.hpp"

// Se você criar DAOs baseados em MariaDB (substituindo os InMemory), 
// inclua aqui os respectivos cabeçalhos. Exemplo:
// #include "MySQLCarteiraDAO.hpp"
// #include "MySQLMovimentacaoDAO.hpp"
// #include "MySQLOracleDAO.hpp"

int main() {
    try {
        // 1) Cria a conexão com o MariaDB.
        //    Ajuste os parâmetros conforme seu ambiente:
        //      host: endereço do servidor (ex: "localhost")
        //      porta: 3306 (padrão do MariaDB)
        //      nome_do_banco: nome do schema que você já criou no MariaDB
        //      usuario e senha: credenciais válidas no MariaDB
        MariaDBConn dbConn(
            "143.106.243.64",      // host
            3306,             // porta
            "PooI_25_B02",  // database
            "PooI_25_B02",        // usuário
            "02HecSGHFE"           // senha
        );

        // 2) Se você já tiver DAOs que usam MariaDBConn, instancie-os assim:
        //    (essas classes devem receber um MariaDBConn& no construtor)
        //
        // MySQLCarteiraDAO carteiraDAO(dbConn);
        // MySQLMovimentacaoDAO movimentacaoDAO(dbConn);
        // MySQLOracleDAO oracleDAO(dbConn);

        // 3) Enquanto não tiver implementado esses DAOs “de banco”, 
        //    mantenha os InMemoryDAO para testes locais:
        InMemoryCarteiraDAO carteiraDAO;
        InMemoryMovimentacaoDAO movimentacaoDAO;
        InMemoryOracleDAO oracleDAO;

        // 4) Instancia a interface passando os DAOs (sejam eles InMemory ou MariaDB)
        InterfaceUsuario interface(carteiraDAO, movimentacaoDAO, oracleDAO);
        interface.iniciar();

    }
    catch (const std::exception& e) {
        std::cerr << "Erro ao inicializar a aplicação: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
