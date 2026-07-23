#include <iostream>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <string>

using namespace std;

namespace dbo = Wt::Dbo;
class Book;
class Publisher;
class Stock;
class Shop;
class Sale;

class Publisher {
public:
    std::string name = "";
    Wt::Dbo::collection< dbo::ptr<Book> > Books;
    template<class Action>
    void persist(Action& a)
    {
        Wt::Dbo::field(a, name, "name");
        Wt::Dbo::hasMany(a, Books, dbo::ManyToOne, "publisher");
    }
};

class Book {
public:
    std::string title = "";
    dbo::ptr<Publisher> publisher;
    Wt::Dbo::collection< dbo::ptr<Stock> > Stocks;
    template<class Action>
    void persist(Action& a)
    {
        Wt::Dbo::field(a, title, "title");
        dbo::belongsTo(a, publisher, "publisher");
        Wt::Dbo::hasMany(a, Stocks, dbo::ManyToOne, "book");
    }
};

class Shop {
public:
    std::string name = "";
    Wt::Dbo::collection< dbo::ptr<Stock> > Stocks;
    template<class Action>
    void persist(Action& a)
    {
        Wt::Dbo::field(a, name, "name");
        Wt::Dbo::hasMany(a, Stocks, dbo::ManyToOne, "shop");
    }
};

class Stock {
public:
    int count{};
    dbo::ptr<Book> Book;
    dbo::ptr<Shop> shop;
    Wt::Dbo::collection< dbo::ptr<Sale> > Sale;
    template<class Action>
    void persist(Action& a)
    {
        Wt::Dbo::field(a, count, "count");
        dbo::belongsTo(a, Book, "book");
        dbo::belongsTo(a, shop, "shop");
        Wt::Dbo::hasMany(a, Sale, dbo::ManyToOne, "stock");
    }
};

class Sale {
public:
    int price{};
    int count{};
    std::string data_sale;
    dbo::ptr<Stock> stock;
    template<class Action>
    void persist(Action& a)
    {
        Wt::Dbo::field(a, price, "price");
        Wt::Dbo::field(a, data_sale, "data_sale");
        dbo::belongsTo(a, stock, "stock");
        Wt::Dbo::field(a, count, "count");
    }
};

int main() {
    try {
        string connectionString =
            "host=localhost "
            "port=5432 "
            "dbname=lessons "
            "user=postgres "
            "password=IPhone@2019";

        auto postgres = make_unique<Wt::Dbo::backend::Postgres>(connectionString);

        Wt::Dbo::Session session;
        session.setConnection(move(postgres));

        session.mapClass<Publisher>("publsher");

        session.mapClass<Book>("book");
        session.mapClass<Stock>("stock");

        session.mapClass<Shop>("shop");
        session.mapClass<Sale>("sale");

        Wt::Dbo::Transaction t(session);
        session.createTables();
        t.commit();

    }
    catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
    }

    return 0;
}
