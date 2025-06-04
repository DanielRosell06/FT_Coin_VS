#include "CarteiraDAO.hpp"
#include "MariaDBConn.hpp"
#include <mariadb/conncpp.hpp>
#include <algorithm>
#include <iostream>

void InMemoryCarteiraDAO::criar(Carteira& carteira) {
    try {
        MariaDBConn dbConn(
            "143.106.243.64",   // host
            3306,               // porta
            "PooI_25_B02",      // database
            "PooI_25_B02",      // usuário
            "02HecSGHFE"        // senha
        );

        sql::Connection* conn = dbConn.getConnection();

        // Usando RETURN_GENERATED_KEYS para obter o ID gerado
        std::unique_ptr<sql::PreparedStatement> pstmt(
            conn->prepareStatement(
                "INSERT INTO CARTEIRA (Titular, Corretora) VALUES (?, ?)",
                sql::Statement::RETURN_GENERATED_KEYS
            )
        );

        pstmt->setString(1, carteira.getNomeTitular());
        pstmt->setString(2, carteira.getCorretora());

        pstmt->executeUpdate();

        // Obtendo o ID gerado automaticamente
        sql::ResultSet* generatedKeys = pstmt->getGeneratedKeys();
        if (generatedKeys && generatedKeys->next()) {
            int idGerado = generatedKeys->getInt(1);
            carteira.setId(idGerado);
        }
        else {
            throw std::runtime_error("Falha ao obter ID gerado para carteira");
        }

        carteiras.push_back(carteira);
        std::cout << "Carteira criada com ID: " << carteira.getId() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[ERRO-Criar] " << e.what() << std::endl;
    }
}

Carteira* InMemoryCarteiraDAO::buscarPorId(int id) {
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
                "SELECT * FROM CARTEIRA WHERE IdCarteira = ?"
            )
        );
        pstmt->setInt(1, id);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            Carteira* carteira = new Carteira();
            carteira->setId(res->getInt("IdCarteira"));
            carteira->setNomeTitular(res->getString("Titular").c_str());
            carteira->setCorretora(res->getString("Corretora").c_str());
            return carteira;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ERRO-Busca] " << e.what() << std::endl;
    }
    return nullptr;
}

void InMemoryCarteiraDAO::atualizar(Carteira& carteira) {
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
                "UPDATE CARTEIRA SET Titular = ?, Corretora = ? WHERE IdCarteira = ?"
            )
        );

        pstmt->setString(1, carteira.getNomeTitular());
        pstmt->setString(2, carteira.getCorretora());
        pstmt->setInt(3, carteira.getId());

        int linhasAfetadas = pstmt->executeUpdate();
        if (linhasAfetadas > 0) {
            // Atualiza a cópia em memória
            for (auto& c : carteiras) {
                if (c.getId() == carteira.getId()) {
                    c = carteira;
                    break;
                }
            }
            std::cout << "Carteira atualizada com sucesso" << std::endl;
        }
        else {
            std::cerr << "Nenhuma carteira encontrada com ID: " << carteira.getId() << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ERRO-Atualizar] " << e.what() << std::endl;
    }
}

void InMemoryCarteiraDAO::remover(int id) {
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
                "DELETE FROM CARTEIRA WHERE IdCarteira = ?"
            )
        );
        pstmt->setInt(1, id);

        int linhasAfetadas = pstmt->executeUpdate();
        if (linhasAfetadas > 0) {
            // Remove da memória
            carteiras.erase(std::remove_if(carteiras.begin(), carteiras.end(),
                [id](Carteira& c) { return c.getId() == id; }), carteiras.end());
            std::cout << "Carteira removida com sucesso" << std::endl;
        }
        else {
            std::cerr << "Nenhuma carteira encontrada com ID: " << id << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ERRO-Remover] " << e.what() << std::endl;
    }
}

std::vector<Carteira> InMemoryCarteiraDAO::listarTodos() {
    std::vector<Carteira> listaBanco;

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
        std::unique_ptr<sql::ResultSet> res(stmt->executeQuery("SELECT * FROM CARTEIRA"));

        while (res->next()) {
            Carteira c;
            c.setId(res->getInt("IdCarteira"));
            c.setNomeTitular(res->getString("Titular").c_str());
            c.setCorretora(res->getString("Corretora").c_str());
            listaBanco.push_back(c);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ERRO-Listar] " << e.what() << std::endl;
    }

    // Atualiza a memória com os dados do banco
    carteiras = listaBanco;
    return carteiras;
}