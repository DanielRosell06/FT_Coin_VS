#include "MovimentacaoDAO.hpp"
#include "MariaDBConn.hpp"
#include <mariadb/conncpp.hpp>
#include <algorithm>
#include <iostream>

void InMemoryMovimentacaoDAO::criar(Movimentacao& mov) {
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
                "INSERT INTO MOVIMENTACAO (IdCarteira, Data, TipoOperacao, Quantidade) VALUES (?, ?, ?, ?)",
                sql::Statement::RETURN_GENERATED_KEYS
            )
        );

        pstmt->setInt(1, mov.getCarteiraId());
        pstmt->setString(2, mov.getDataOperacao());
        pstmt->setString(3, std::string(1, mov.getTipoOperacao())); // Converte char para string
        pstmt->setDouble(4, mov.getQuantidade());

        pstmt->executeUpdate();

        // Obtém o ID gerado
        sql::ResultSet* generatedKeys = pstmt->getGeneratedKeys();
        if (generatedKeys && generatedKeys->next()) {
            int idGerado = generatedKeys->getInt(1);
            mov.setId(idGerado);
        }
        else {
            throw std::runtime_error("Falha ao obter ID gerado para movimentação");
        }

        movimentacoes.push_back(mov);
        std::cout << "Movimentação criada com ID: " << mov.getId() << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "[ERRO-Criar-Movimentacao] " << e.what() << std::endl;
    }
}

Movimentacao* InMemoryMovimentacaoDAO::buscarPorId(int id) {
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
                "SELECT * FROM MOVIMENTACAO WHERE IdMovimento = ?"
            )
        );
        pstmt->setInt(1, id);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        if (res->next()) {
            Movimentacao* mov = new Movimentacao();
            mov->setId(res->getInt("IdMovimento"));
            mov->setCarteiraId(res->getInt("CarteiraId"));
            mov->setDataOperacao(res->getString("DataOperacao").c_str());

            // Converte string para char
            std::string tipoStr = res->getString("TipoOperacao").c_str();
            mov->setTipoOperacao(!tipoStr.empty() ? tipoStr[0] : ' ');

            mov->setQuantidade(res->getDouble("Quantidade"));
            return mov;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ERRO-Busca-Movimentacao] " << e.what() << std::endl;
    }
    return nullptr;
}

void InMemoryMovimentacaoDAO::atualizar(Movimentacao& mov) {
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
                "UPDATE MOVIMENTACAO SET IdCarteira = ?, Data = ?, TipoOperacao = ?, Quantidade = ? WHERE IdMovimento = ?"
            )
        );

        pstmt->setInt(1, mov.getCarteiraId());
        pstmt->setString(2, mov.getDataOperacao());
        pstmt->setString(3, std::string(1, mov.getTipoOperacao()));
        pstmt->setDouble(4, mov.getQuantidade());
        pstmt->setInt(5, mov.getId());

        int linhasAfetadas = pstmt->executeUpdate();
        if (linhasAfetadas > 0) {
            // Atualiza a cópia em memória
            for (auto& m : movimentacoes) {
                if (m.getId() == mov.getId()) {
                    m = mov;
                    break;
                }
            }
            std::cout << "Movimentação atualizada com sucesso" << std::endl;
        }
        else {
            std::cerr << "Nenhuma movimentação encontrada com ID: " << mov.getId() << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ERRO-Atualizar-Movimentacao] " << e.what() << std::endl;
    }
}

void InMemoryMovimentacaoDAO::remover(int id) {
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
                "DELETE FROM MOVIMENTACAO WHERE IdMovimento = ?"
            )
        );
        pstmt->setInt(1, id);

        int linhasAfetadas = pstmt->executeUpdate();
        if (linhasAfetadas > 0) {
            // Remove da memória
            movimentacoes.erase(std::remove_if(movimentacoes.begin(), movimentacoes.end(),
                [id](Movimentacao& m) { return m.getId() == id; }), movimentacoes.end());
            std::cout << "Movimentação removida com sucesso" << std::endl;
        }
        else {
            std::cerr << "Nenhuma movimentação encontrada com ID: " << id << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ERRO-Remover-Movimentacao] " << e.what() << std::endl;
    }
}

std::vector<Movimentacao> InMemoryMovimentacaoDAO::listarPorCarteira(int carteiraId) {
    std::vector<Movimentacao> listaBanco;

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
                "SELECT * FROM MOVIMENTACAO WHERE IdCarteira = ?"
            )
        );
        pstmt->setInt(1, carteiraId);

        std::unique_ptr<sql::ResultSet> res(pstmt->executeQuery());

        while (res->next()) {
            Movimentacao m;
            m.setId(res->getInt("IdMovimento"));
            m.setCarteiraId(res->getInt("IdCarteira"));
            m.setDataOperacao(res->getString("Data").c_str());

            // Converte string para char
            std::string tipoStr = res->getString("TipoOperacao").c_str();
            m.setTipoOperacao(!tipoStr.empty() ? tipoStr[0] : ' ');

            m.setQuantidade(res->getDouble("Quantidade"));
            listaBanco.push_back(m);
        }
    }
    catch (const std::exception& e) {
        std::cerr << "[ERRO-Listar-Movimentacao] " << e.what() << std::endl;
    }

    // Atualiza a lista em memória apenas para esta carteira
    movimentacoes.erase(std::remove_if(movimentacoes.begin(), movimentacoes.end(),
        [carteiraId](Movimentacao& m) { return m.getCarteiraId() == carteiraId; }), movimentacoes.end());

    for (auto& m : listaBanco) {
        movimentacoes.push_back(m);
    }

    return listaBanco;
}