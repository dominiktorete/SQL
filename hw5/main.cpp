#include <iostream>
#include <pqxx/pqxx>
#include <string>
class Client_Func{
public:
    void create_table(pqxx::work& tx){
        tx.exec("CREATE TABLE IF NOT EXISTS Clients("
                "id SERIAL PRIMARY KEY,"
                "name VARCHAR(60) NOT NULL,"
                "last_name VARCHAR(60) NOT NULL,"
                "email VARCHAR(60) UNIQUE NOT NULL); ");
        tx.exec("CREATE TABLE IF NOT EXISTS Phones"
                "(id SERIAL PRIMARY KEY,"
                "number VARCHAR(60) UNIQUE,"
                "id_clients INT,"
                "FOREIGN KEY (id_clients) REFERENCES Clients(id)); ");
        tx.commit();
    }
    void add_new_client(pqxx::connection& c){
        std::cout << "Add client" << std::endl;
        std::string name, last_name, email, number;
        std::cout << "Enter to name: ";
        std::cin >> name;
        std::cout << "Enter to last_name: ";
        std::cin >> last_name;
        std::cout << "Enter to email: ";
        std::cin >> email;
        std::cout << "Enter to number: ";
        std::cin >> number;

        pqxx::work tx{c};
        c.prepare("insert_client", "INSERT INTO clients(name, last_name, email) VALUES($1, $2, $3)");
        c.prepare("select_id","SELECT id from clients WHERE email = $1");
        c.prepare("insert_phone","INSERT INTO phones(number, id_clients) VALUES($1, $2)");

        tx.exec_prepared("insert_client", name, last_name, email);
        auto id = tx.exec_prepared("select_id", email);
        tx.exec_prepared("insert_phone", number, id.begin()["id"].as<int>());
        tx.commit();
    }

    void add_number(pqxx::work& tx){
        std::cout << "Add number" << std::endl;
        std::string email, number;
        std::cout << "Enter to email: ";
        std::cin >> email;
        auto id = tx.exec_prepared("select_id", email);
        if(id.empty()){
            std::cout << "Client not found" << std::endl;
            return;
        }
        std::cout << "Enter to number: ";
        std::cin >> number;
        tx.exec_prepared("insert_phone", number, id.begin()["id"].as<int>());
        tx.commit();
    }

    void delete_number(pqxx::work& tx){
        std::cout << "Delete number" << std::endl;
        std::string number;
        std::cout << "Enter to number: ";
        std::cin >> number;
        tx.conn().prepare("find_number", "SELECT id from phones WHERE number = $1");
        if(tx.exec_prepared("find_number", number).empty()){
            std::cout << "Number not found" << std::endl;
            return;
        }
        tx.conn().prepare("delete_phone", "DELETE FROM phones WHERE number = $1");
        tx.exec_prepared("delete_phone", number);
        tx.commit();
    }

    void find_client(pqxx::work& tx){
        std::cout << "Find client" << std::endl;
        int command{};
        std::cout << "Enter to command(1 - phone, other numbers - email): ";
        std::cin >> command;
        std::cin.ignore();
        std::string name, last_name;
        std::cout << "Enter to name: ";
        std::cin >> name;
        std::cout << "Enter to last_name: ";
        std::cin >> last_name;
        pqxx::result result;
        if(command == 1){
            std::string number{};
            std::cout << "Enter to number: ";
            std::cin >> number;
            tx.conn().prepare("find_phone", "SELECT id_clients from phones where number = $1");
            auto id = tx.exec_prepared("find_phone", number).begin();
            tx.conn().prepare("find_client_by_email", "SELECT name, last_name, email from clients where name = $1 AND last_name = $2 AND id = $3");
            result = tx.exec_prepared("find_client_by_email", name, last_name, id.begin().as<int>());
        }
        else{
            std::string email{};
            std::cout << "Enter to email: ";
            std::cin >> email;
            tx.conn().prepare("find_client_by_email", "SELECT name, last_name, email from clients where name = $1 AND last_name = $2 AND email = $3");
            result = tx.exec_prepared("find_client_by_email", name, last_name, email);
        }
        if(!result.empty()){
            std::cout << "CLient: " << result.back()["name"] << " " << result.back()["last_name"] << " " << result.back()["email"] << std::endl;
        }
        else {
            std::cout << "Client not exists\n";
        }
    }

    void delete_client(pqxx::work& tx){
        std::cout << "Delete client" << std::endl;
        std::string email;
        std::cout << "Enter to email: ";
        std::cin >> email;

        auto id = tx.exec_prepared("select_id", email);
        if(id.empty()){
            std::cout << "Client not found" << std::endl;
            return;
        }
        tx.conn().prepare("Delete_phone_on_id", "DELETE FROM phones WHERE id_clients = $1");
        tx.exec_prepared("Delete_phone_on_id", id.begin()["id"].as<int>());
        tx.conn().prepare("Delete_client", "DELETE FROM clients WHERE email = $1");
        tx.exec_prepared("Delete_client", email);
    }
    void edit_data_client(pqxx::work& tx){
        std::cout << "Method Edit" << std::endl;
        std::string email;
        std::cout << "Enter to email: ";
        std::cin >> email;
        auto id = tx.exec_prepared("select_id", email);
        if(id.empty()){
            std::cout << "Client not found" << std::endl;
            return;
        }
        std::string new_name, new_last_name, new_email;
        std::cout << "Enter to name: ";
        std::cin >> new_name;
        std::cout << "Enter to last_name: ";
        std::cin >> new_last_name;
        std::cout << "Enter to email: ";
        std::cin >> new_email;
        tx.conn().prepare("Edit_data", "UPDATE clients SET name = $1, last_name = $2, email = $3 WHERE id = $4");
        tx.exec_prepared("Edit_data", new_name, new_last_name, new_email, id.begin()["id"].as<int>());
    }

};




int main()
{
    try{
        pqxx::connection c(
            "host=localhost "
            "port=5432 "
            "dbname=Clients "
            "user=postgres "
            "password=IPhone@2019");
        pqxx::work tx{c};

        Client_Func funcs;
        funcs.create_table(tx);
        funcs.add_new_client(c);
        funcs.add_number(tx);
        funcs.edit_data_client(tx);
        funcs.find_client(tx);
        funcs.delete_number(tx);
        funcs.delete_client(tx);

    }
    catch (pqxx::sql_error& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
