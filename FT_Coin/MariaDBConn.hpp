#ifndef MARIADB_CONN_HPP
#define MARIADB_CONN_HPP

#include <mariadb/conncpp.hpp>
#include <memory>
#include <string>
#include <stdexcept>

/**
 * @brief Classe respons�vel por gerenciar a conex�o com o MariaDB usando Connector/C++.
 *
 * Basta instanciar passando host, porta, database, usu�rio e senha.
 * Se houver algum erro, o construtor lan�ar� std::runtime_error com a mensagem de erro do driver.
 */
class MariaDBConn {
public:
    /**
     * @brief Construtor: abre a conex�o com o banco.
     *
     * @param host       Endere�o do servidor (ex: "localhost")
     * @param port       Porta do MariaDB (padr�o: 3306)
     * @param database   Nome do banco de dados (ex: "minha_base")
     * @param user       Usu�rio de acesso (ex: "root")
     * @param password   Senha do usu�rio
     *
     * @throw std::runtime_error Se n�o conseguir conectar ou se ocorrer erro no driver
     */
    MariaDBConn(const std::string& host,
        unsigned int port,
        const std::string& database,
        const std::string& user,
        const std::string& password);

    /**
     * @brief Destrutor: fecha a conex�o ao final do escopo.
     */
    ~MariaDBConn();

    /**
     * @brief Retorna um ponteiro para sql::Connection (POINTER) ativo.
     *
     * Use este m�todo dentro dos DAOs para executar queries, preparar statements, etc.
     * @return sql::Connection*   Ponteiro para a conex�o aberta. Nunca ser� nullptr (ou lan�a antes).
     */
    sql::Connection* getConnection();

    // Para evitar c�pias acidentais:
    MariaDBConn(const MariaDBConn&) = delete;
    MariaDBConn& operator=(const MariaDBConn&) = delete;

private:
    sql::Driver* driver_;                                      ///< Ponteiro para o driver (gerenciado pelo Connector).
    std::unique_ptr<sql::Connection> conn_;                     ///< Conex�o propriamente dita (smart pointer).
};

#endif // MARIADB_CONN_HPP
