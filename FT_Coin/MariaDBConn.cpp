#include "MariaDBConn.hpp"

MariaDBConn::MariaDBConn(
    const std::string& host,
    unsigned int port,
    const std::string& database,
    const std::string& user,
    const std::string& password)
{
    try {
        // Obtém instância única do Driver MariaDB
        driver_ = sql::mariadb::get_driver_instance();
        if (!driver_) {
            throw std::runtime_error("Não foi possível obter instância do driver MariaDB.");
        }

        // Tenta estabelecer a conexão. Se falhar, lançará SQLException.
        conn_.reset(driver_->connect(host, user, password));
        if (!conn_ || !conn_->isValid()) {
            throw std::runtime_error("Conexão retornou ponteiro inválido ou inválida após connect().");
        }
		conn_->setSchema(database); // Define o banco de dados a ser usado

    }
    catch (const sql::SQLException& e) {
        // Se ocorrer qualquer erro durante a conexão, relança como runtime_error
        throw std::runtime_error(std::string("Erro ao conectar no MariaDB: ") + e.what());
    }
}

MariaDBConn::~MariaDBConn() {
    // Quando o unique_ptr<sql::Connection> for destruído, a conexão será fechada automaticamente.
    // Nada mais precisa ser feito manualmente.
}

sql::Connection* MariaDBConn::getConnection() {
    return conn_.get();
}
