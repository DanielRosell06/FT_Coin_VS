#ifndef MARIADB_CONN_HPP
#define MARIADB_CONN_HPP

#include <mariadb/conncpp.hpp>
#include <memory>
#include <string>
#include <stdexcept>

/**
 * @brief Classe responsável por gerenciar a conexão com o MariaDB usando Connector/C++.
 *
 * Basta instanciar passando host, porta, database, usuário e senha.
 * Se houver algum erro, o construtor lançará std::runtime_error com a mensagem de erro do driver.
 */
class MariaDBConn {
public:
    /**
     * @brief Construtor: abre a conexão com o banco.
     *
     * @param host       Endereço do servidor (ex: "localhost")
     * @param port       Porta do MariaDB (padrão: 3306)
     * @param database   Nome do banco de dados (ex: "minha_base")
     * @param user       Usuário de acesso (ex: "root")
     * @param password   Senha do usuário
     *
     * @throw std::runtime_error Se não conseguir conectar ou se ocorrer erro no driver
     */
    MariaDBConn(const std::string& host,
        unsigned int port,
        const std::string& database,
        const std::string& user,
        const std::string& password);

    /**
     * @brief Destrutor: fecha a conexão ao final do escopo.
     */
    ~MariaDBConn();

    /**
     * @brief Retorna um ponteiro para sql::Connection (POINTER) ativo.
     *
     * Use este método dentro dos DAOs para executar queries, preparar statements, etc.
     * @return sql::Connection*   Ponteiro para a conexão aberta. Nunca será nullptr (ou lança antes).
     */
    sql::Connection* getConnection();

    // Para evitar cópias acidentais:
    MariaDBConn(const MariaDBConn&) = delete;
    MariaDBConn& operator=(const MariaDBConn&) = delete;

private:
    sql::Driver* driver_;                                      ///< Ponteiro para o driver (gerenciado pelo Connector).
    std::unique_ptr<sql::Connection> conn_;                     ///< Conexão propriamente dita (smart pointer).
};

#endif // MARIADB_CONN_HPP
