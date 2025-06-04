#include "OracleDAO.hpp"
#include "MariaDBConn.hpp"
#include <mariadb/conncpp.hpp>
#include <algorithm>
#include <iostream>
#include <cstdlib>
#include <ctime>

void InMemoryOracleDAO::adicionarCotacao(const Oracle& oracle) {
    try {
        MariaDBConn dbConn(
            "143.106.243.64",   // host
            3306,               // porta
            "PooI_25_B02",      // database
            "PooI_25_B02",      // usuário
            "02HecSGHFE"        // senha
        );

        sql::Connection* conn = dbConn.getConnection();

        // Query para inserir ou atualizar cotação
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "INSERT INTO ORACULO (Data, Cotacao) VALUES (?, ?) "
                "ON DUPLICATE KEY UPDATE Cotacao = VALUES(Cotacao)"
            )
        );

        pstmt->setString(1, oracle.getData());
        pstmt->setDouble(2, oracle.getCotacao());

        pstmt->executeUpdate();

        // Atualiza memória local
        cotacoes.erase(std::remove_if(cotacoes.begin(), cotacoes.end(),
            [&oracle](const Oracle& o) { return o.getData() == oracle.getData(); }),
            cotacoes.end());
        cotacoes.push_back(oracle);

        std::cout << "Cotação para " << oracle.getData() << " adicionada/atualizada" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[ERRO-AdicionarCotacao] " << e.what() << std::endl;
    }
}

double InMemoryOracleDAO::buscarCotacaoPorData(const std::string& data) {
    // Tenta buscar na memória
    for (const auto& o : cotacoes) {
        if (o.getData() == data) {
            return o.getCotacao();
        }
    }

    // Se não encontrou na memória, busca no banco
    try {
        MariaDBConn dbConn(
            "143.106.243.64",   // host
            3306,               // porta
            "PooI_25_B02",      // database
            "PooI_25_B02",      // usuário
            "02HecSGHFE"        // senha
        );

        sql::Connection* conn = dbConn.getConnection();

        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "SELECT Cotacao FROM ORACULO WHERE Data = ?"
            )
        );
        pstmt->setString(1, data);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            double cotacao = res->getDouble("Cotacao");
            // Atualiza memória local
            cotacoes.push_back(Oracle(data, cotacao));
            return cotacao;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ERRO-BuscarCotacao] " << e.what() << std::endl;
    }

    // Se não encontrou em lugar nenhum, gera aleatório
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    double cotacaoAleatoria = 150.0 + (std::rand() % 1001) / 100.0; // Entre 150.00 e 160.00

    // Cria e salva a nova cotação
    Oracle novaOracle(data, cotacaoAleatoria);
    adicionarCotacao(novaOracle);

    return cotacaoAleatoria;
}

std::vector<Oracle> InMemoryOracleDAO::listarTodasCotacoes() {
    // Primeiro tenta atualizar da base de dados
    try {
        MariaDBConn dbConn(
            "143.106.243.64",   // host
            3306,               // porta
            "PooI_25_B02",      // database
            "PooI_25_B02",      // usuário
            "02HecSGHFE"        // senha
        );

        sql::Connection* conn = dbConn.getConnection();

        std::unique_ptr<sql::Statement> stmt(conn->createStatement());
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT Data, Cotacao FROM ORACULO"));

        // Limpa cotações locais
        cotacoes.clear();

        while (res->next()) {
            std::string data = res->getString("Data").c_str();
            double valor = res->getDouble("Cotacao");
            cotacoes.push_back(Oracle(data, valor));
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ERRO-ListarCotacoes] " << e.what() << std::endl;
    }

    return cotacoes;
}