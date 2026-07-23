#include <iostream>
#include <Wt/Dbo/Dbo.h>
#include <Wt/Dbo/backend/Postgres.h>
#include <string>
#include <memory>
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
       
        session.mapClass<Publisher>("publisher");
        
        session.mapClass<Book>("book");
        session.mapClass<Stock>("stock");

        session.mapClass<Shop>("shop");
        session.mapClass<Sale>("sale");
        session.createTables();

        Wt::Dbo::Transaction t(session);
        unique_ptr<Publisher> pub1{new Publisher{}};
        pub1->name = "Maxim";
        unique_ptr<Publisher> pub2{ new Publisher{} };
        pub2->name = "Egor";
        unique_ptr<Publisher> pub3{ new Publisher{} };
        pub3->name = "Kirill";
        unique_ptr<Publisher> pub4{ new Publisher{} };
        pub4->name = "Evgeniy";
        Wt::Dbo::ptr<Publisher> pub = session.add(std::move(pub1));
        Wt::Dbo::ptr<Book> book1 = session.add(std::unique_ptr<Book>{new Book()});
        Wt::Dbo::ptr<Book> book2 = session.add(std::unique_ptr<Book>{new Book()});
        Wt::Dbo::ptr<Book> book3 = session.add(std::unique_ptr<Book>{new Book()});
        Wt::Dbo::ptr<Book> book4 = session.add(std::unique_ptr<Book>{new Book()});
        Wt::Dbo::ptr<Book> book5 = session.add(std::unique_ptr<Book>{new Book()});

        book1.modify()->publisher = pub;
        book1.modify()->title = "Book1";
        book2.modify()->publisher = pub;
        book2.modify()->title = "Book2";
        pub.modify()->Books.insert(book1);
        pub.modify()->Books.insert(book2);

        pub = session.add(std::move(pub2));
        book3.modify()->publisher = pub;
        book3.modify()->title = "Book3";
        pub.modify()->Books.insert(book3);
        
        pub = session.add(std::move(pub3));
        book4.modify()->publisher = pub;
        book4.modify()->title = "Book4";
        pub.modify()->Books.insert(book4);

        pub = session.add(std::move(pub4));
        book5.modify()->publisher = pub;
        book5.modify()->title = "Book5";
        pub.modify()->Books.insert(book5);
      

        Wt::Dbo::ptr<Shop> shop1 = session.add(std::unique_ptr<Shop>{new Shop()});
        shop1.modify()->name = "Shop1";
        Wt::Dbo::ptr<Shop> shop2 = session.add(std::unique_ptr<Shop>{new Shop()});
        shop2.modify()->name = "Shop2";
        Wt::Dbo::ptr<Shop> shop3 = session.add(std::unique_ptr<Shop>{new Shop()});
        shop3.modify()->name = "Shop3";
        Wt::Dbo::ptr<Shop> shop4 = session.add(std::unique_ptr<Shop>{new Shop()});
        shop4.modify()->name = "Shop4";




        Wt::Dbo::ptr<Stock> stock = session.add(std::unique_ptr<Stock>{new Stock()});
        stock.modify()->shop = shop1;
        stock.modify()->Book = book1;
        stock.modify()->count = 10;

        Wt::Dbo::ptr<Stock> stock2 = session.add(std::unique_ptr<Stock>{new Stock()});
        stock2.modify()->shop = shop2;
        stock2.modify()->Book = book2;
        stock2.modify()->count = 5;

        Wt::Dbo::ptr<Stock> stock3 = session.add(std::unique_ptr<Stock>{new Stock()});
        stock3.modify()->shop = shop2;
        stock3.modify()->Book = book3;
        stock3.modify()->count = 1;

        Wt::Dbo::ptr<Stock> stock4 = session.add(std::unique_ptr<Stock>{new Stock()});
        stock4.modify()->shop = shop3;
        stock4.modify()->Book = book4;
        stock4.modify()->count = 7;

        Wt::Dbo::ptr<Stock> stock5 = session.add(std::unique_ptr<Stock>{new Stock()});
        stock5.modify()->shop = shop4;
        stock5.modify()->Book = book5;
        stock5.modify()->count = 5;

        Wt::Dbo::ptr<Sale> sale = session.add(std::unique_ptr<Sale>{new Sale()});
        sale.modify()->stock = stock;
        sale.modify()->count = 5;
        sale.modify()->data_sale = "21-05-22";
        sale.modify()->price = 2000;
   
        sale = session.add(std::unique_ptr<Sale>{new Sale()});
        sale.modify()->stock = stock;
        sale.modify()->count = 5;
        sale.modify()->data_sale = "14-03-21";
        sale.modify()->price = 500;

        sale = session.add(std::unique_ptr<Sale>{new Sale()});
        sale.modify()->stock = stock2;
        sale.modify()->count = 5;
        sale.modify()->data_sale = "31-12-22";
        sale.modify()->price = 500;

        sale = session.add(std::unique_ptr<Sale>{new Sale()});
        sale.modify()->stock = stock3;
        sale.modify()->count = 5;
        sale.modify()->data_sale = "12-11-24";
        sale.modify()->price = 4000;

        sale = session.add(std::unique_ptr<Sale>{new Sale()});
        sale.modify()->stock = stock4;
        sale.modify()->count = 5;
        sale.modify()->data_sale = "11-06-12";
        sale.modify()->price = 300;

        sale = session.add(std::unique_ptr<Sale>{new Sale()});
        sale.modify()->stock = stock5;
        sale.modify()->count = 5;
        sale.modify()->data_sale = "22-03-21";
        sale.modify()->price = 1000;

        std::string publisher;
        std::cin >> publisher;
        auto q = session.query<string>("select shop.name from shop").leftJoin<Stock>("s", "s.shop_id = shop.id").leftJoin<Book>("b", "b.id = s.book_id").leftJoin<Publisher>("p", "p.id = b.publisher_id").where("p.name = ?").bind(publisher);
    
        auto results = q.resultList();

       if (results.empty()) {
            std::cout << "Магазины не найдены." << std::endl;
       }
       else {
           for(auto res : results)
           std::cout << "Magazin:" << res << std::endl;
            
        }
        t.commit();
    }
    catch (const exception& ex) {
        cerr << "Error: " << ex.what() << endl;
    }

    return 0;
}
