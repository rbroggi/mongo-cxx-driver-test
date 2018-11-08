#include <iostream>
#include <unordered_map>
#include <memory>
#include <thread>

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/document/view.hpp>
#include <bsoncxx/builder/basic/kvp.hpp>
#include <bsoncxx/json.hpp>

#include <mongocxx/client.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/instance.hpp>
#include <iostream>

using bsoncxx::builder::basic::kvp;
using bsoncxx::builder::stream::document;
using bsoncxx::document::view;
using bsoncxx::builder::stream::finalize;

typedef std::string db_name;
typedef std::string collection_name;

int main(int, char**) {
    mongocxx::instance inst{};

    //Create connection pool
    mongocxx::pool pool{mongocxx::uri{}};

    //Define generator lambda generator - this lambda insert documents into the db and colection specified
    auto generator = [&pool](const db_name db, const collection_name col, view view) {
        auto client = pool.acquire();
        auto collection = (*client)[db][col];
        collection.insert_one(view);
        std::cout << "Inserted into " << db << ":" << col << std::endl;
    };

    //Define lambda reader method
    auto retrieve = [&pool](const db_name db, const collection_name col, const std::int64_t id, std::vector<view> & doc_views) {
        auto client = pool.acquire();
        auto collection = (*client)[db][col];
        for (auto&& doc : collection.find(document{} << "id" << id << finalize) ) {
            doc_views.push_back(doc);
        }
    };

    //Define lambda delete method
    auto remove = [&pool](const db_name db, const collection_name col, const std::int64_t id) {
        auto client = pool.acquire();
        auto collection = (*client)[db][col];
        collection.delete_many(document{} << "id" << id << finalize);
    };

    //Using variables to simulate procedure
    std::vector<int> ids = {1, 2, 3, 4, 5, 6};
    std::vector<std::string> names  = {"a", "b", "c", "d", "e", "f"};
    std::unordered_map<int, db_name> ids_to_db = {
            {1, "dbOne"},
            {2, "dbTwo"},
            {3, "dbOne"},
            {4, "dbTwo"},
            {5, "dbOne"},
            {6, "dbTwo"},
    };

    std::unordered_map<int, collection_name> ids_to_collections = {
            {1, "collectionOne"},
            {2, "collectionTwo"},
            {3, "collectionThree"},
            {4, "collectionOne"},
            {5, "collectionTwo"},
            {6, "collectionThree"},
    };

    //Delete all enties before begining
    std::cout << "Delete all data before begining..." << std::endl;
    for (auto i : ids) {
        remove(ids_to_db[i], ids_to_collections[i], i);
    }
    std::cout << "Data deleted." << std::endl;


    //Generation loop using separate threads
    std::vector<std::shared_ptr<std::thread>> threads{};
    for (auto i : ids) {
        document doc{};
        doc << "id" << bsoncxx::types::b_int32{i}
            << "name" << names[i-1];
        std::cout << "View to insert into db: " << bsoncxx::to_json(doc.view()) << std::endl;
        std::shared_ptr<std::thread> runner =
                std::make_shared<std::thread>(generator, ids_to_db[i], ids_to_collections[i], doc.view());
        threads.push_back(runner);
    }

    //wait for threads to finish
    std::cout << "Waiting for threads to finish..." << std::endl;
    for (auto t : threads) (*t).join();
    std::cout << "Threads finished..." << std::endl;

    //Retriving using single thread
    for (auto i : ids) {
        std::vector<view> doc_views{};
        retrieve(ids_to_db[i], ids_to_collections[i], i, doc_views);
        for (auto view : doc_views) std::cout << bsoncxx::to_json(view) << std::endl;
    }
}
