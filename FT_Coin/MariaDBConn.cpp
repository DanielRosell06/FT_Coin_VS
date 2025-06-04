#include "MariaDBConn.hpp"

MariaDBConn::MariaDBConn(
    const std::string& host,
    unsigned int port,
    const std::string& database,
    const std::string& user,
    const std::string& password)
{
    try {
        // Obt�m inst�ncia �nica do Driver MariaDB
        driver_ = sql::mariadb::get_driver_instance();
        if (!driver_) {
            throw std::runtime_error("N�o foi poss�vel obter inst�ncia do driver MariaDB.");
        }

        // Tenta estabelecer a conex�o. Se falhar, lan�ar� SQLException.
        conn_.reset(driver_->connect(host, user, password));
        if (!conn_ || !conn_->isValid()) {
            throw std::runtime_error("Conex�o retornou ponteiro inv�lido ou inv�lida ap�s connect().");
        }
		conn_->setSchema(database); // Define o banco de dados a ser usado

    }
    catch (const sql::SQLException& e) {
        // Se ocorrer qualquer erro durante a conex�o, relan�a como runtime_error
        throw std::runtime_error(std::string("Erro ao conectar no MariaDB: ") + e.what());
    }
}

MariaDBConn::~MariaDBConn() {
    // Quando o unique_ptr<sql::Connection> for destru�do, a conex�o ser� fechada automaticamente.
    // Nada mais precisa ser feito manualmente.
}

sql::Connection* MariaDBConn::getConnection() {
    return conn_.get();
}
