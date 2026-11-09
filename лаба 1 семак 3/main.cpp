#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <cstdio>
#include "array.h"
#include "one_linked_list.h"
#include "linked_list.h"
#include "stack.h"
#include "queue.h"
#include "tree.h"
using namespace std;
map<string, Array> arrays;
map<string, OneLinkedList> flists;
map<string, LinkedList> lists;
map<string, Stack> stacks;
map<string, Queue> queues;
map<string, Tree> trees;

void cleanup() {
    for (auto& x : arrays) Array_destroy(x.second);
    for (auto& x : flists) OneLinked_destroy(x.second);
    for (auto& x : lists) Linked_destroy(x.second);
    for (auto& x : stacks) Stack_destroy(x.second);
    for (auto& x : queues) Queue_destroy(x.second);
    for (auto& x : trees) Tree_destroy(x.second);
}
bool exists(const string& name) {
    return arrays.count(name)||flists.count(name)||lists.count(name)||stacks.count(name)||queues.count(name)||trees.count(name);
}
int number(const string& value) {
    size_t end = 0;
    int result = stoi(value, &end);
    if (end != value.size()) throw invalid_argument("Ожидается целое число: " + value);
    return result;
}
void load(const string& file) {
    if (!filesystem::exists(file)) return;
    ifstream in(file);
    if (!in) throw runtime_error("Не удалось прочитать файл");
    string line;
    while (getline(in, line)) {
        istringstream row(line);
        string type, name, value;
        if (!(row >> type)) continue;
        if (!(row >> name) || exists(name)) throw runtime_error("Некорректное или повторное имя в файле");
        if (type == "Array") {
            auto& a = arrays.emplace(name, Array_create(16)).first->second;
            while (row >> value) Array_push_back(a, value);
        } else if (type == "OneLinkedList") {
            auto& a = flists.emplace(name, OneLinked_create()).first->second;
            while (row >> value) OneLinked_push_back(a, value);
        } else if (type == "LinkedList") {
            auto& a = lists.emplace(name, Linked_create()).first->second;
            while (row >> value) Linked_push_back(a, value);
        } else if (type == "Stack") {
            auto& a = stacks.emplace(name, Stack_create()).first->second;
            // The file stores bottom-to-top, so pushing restores the original top.
            while (row >> value) Stack_push_back(a, value);
        } else if (type == "Queue") {
            auto& a = queues.emplace(name, Queue_create()).first->second;
            while (row >> value) Queue_push_back(a, value);
        } else if (type == "Tree") {
            auto& a = trees.emplace(name, Tree_create()).first->second;
            while (row >> value) Tree_insert(a, number(value));
        } else throw runtime_error("Неизвестная структура в файле: " + type);
    }
    if (in.bad()) throw runtime_error("Ошибка чтения файла");
}
void save(const string& file) {
    string temp = file + ".tmp";
    ofstream out(temp);
    if (!out) throw runtime_error("Не удалось открыть файл для записи");
    for (auto& x : arrays) out << "Array " << x.first << Array_read(x.second) << "\n";
    for (auto& x : flists) out << "OneLinkedList " << x.first << OneLinked_read(x.second) << "\n";
    for (auto& x : lists) out << "LinkedList " << x.first << Linked_read(x.second) << "\n";
    for (auto& x : stacks) out << "Stack " << x.first << OneLinked_read_reverse(x.second.list) << "\n";
    for (auto& x : queues) out << "Queue " << x.first << Queue_read(x.second) << "\n";
    for (auto& x : trees) out << "Tree " << x.first << Tree_read(x.second) << "\n";
    out.close();
    if (!out) { remove(temp.c_str()); throw runtime_error("Ошибка записи файла"); }
    if (rename(temp.c_str(), file.c_str()) != 0) { remove(temp.c_str()); throw runtime_error("Не удалось заменить файл"); }
}
// min/max number of arguments after the structure name; -1 means unlimited.
struct Spec { int min, max; bool change, create; char type; };
const map<string, Spec> specs = {
    {"PRINT", {0, 0, false, false, 'P'}},
    {"MCREATE", {0, 0, true, true, 'M'}},
    {"FCREATE", {0, 0, true, true, 'F'}},
    {"LCREATE", {0, 0, true, true, 'L'}},
    {"SCREATE", {0, 0, true, true, 'S'}},
    {"QCREATE", {0, 0, true, true, 'Q'}},
    {"TCREATE", {0, 0, true, true, 'T'}},
    {"MPUSH", {1, -1, true, true, 'M'}},
    {"FPUSH", {1, -1, true, true, 'F'}},
    {"FPUSHHEAD", {1, -1, true, true, 'F'}},
    {"LPUSH", {1, -1, true, true, 'L'}},
    {"LPUSHHEAD", {1, -1, true, true, 'L'}},
    {"SPUSH", {1, -1, true, true, 'S'}},
    {"QPUSH", {1, -1, true, true, 'Q'}},
    {"TINSERT", {1, -1, true, true, 'T'}},
    {"MPUSHIDX", {2, 2, true, false, 'M'}},
    {"MSETIDX", {2, 2, true, false, 'M'}},
    {"FPUSHBEFORE", {2, 2, true, false, 'F'}},
    {"FPUSHAFTER", {2, 2, true, false, 'F'}},
    {"LPUSHBEFORE", {2, 2, true, false, 'L'}},
    {"LPUSHAFTER", {2, 2, true, false, 'L'}},
    {"MDELIDX", {1, 1, true, false, 'M'}},
    {"FPOPBEFORE", {1, 1, true, false, 'F'}},
    {"FPOPAFTER", {1, 1, true, false, 'F'}},
    {"FDELVAL", {1, 1, true, false, 'F'}},
    {"LPOPBEFORE", {1, 1, true, false, 'L'}},
    {"LPOPAFTER", {1, 1, true, false, 'L'}},
    {"LDELVAL", {1, 1, true, false, 'L'}},
    {"FPOP", {0, 0, true, false, 'F'}},
    {"FPOPHEAD", {0, 0, true, false, 'F'}},
    {"LPOP", {0, 0, true, false, 'L'}},
    {"LPOPHEAD", {0, 0, true, false, 'L'}},
    {"SPOP", {0, 0, true, false, 'S'}},
    {"QPOP", {0, 0, true, false, 'Q'}},
    {"MGET", {1, 1, false, false, 'M'}},
    {"FFINDVAL", {1, 1, false, false, 'F'}},
    {"LFINDVAL", {1, 1, false, false, 'L'}},
    {"TFIND", {1, 1, false, false, 'T'}},
    {"MLENGTH", {0, 0, false, false, 'M'}},
    {"MREAD", {0, 0, false, false, 'M'}},
    {"FREADHT", {0, 0, false, false, 'F'}},
    {"FREADTH", {0, 0, false, false, 'F'}},
    {"LREADHT", {0, 0, false, false, 'L'}},
    {"LREADTH", {0, 0, false, false, 'L'}},
    {"SREAD", {0, 0, false, false, 'S'}},
    {"QREAD", {0, 0, false, false, 'Q'}},
    {"TREAD", {0, 0, false, false, 'T'}},
    {"TCHECKCOMPLETE", {0, 0, false, false, 'T'}},
};
string canonical(string cmd) {
    const map<string,string> aliases = {{"MDEL","MDELIDX"},{"MSET","MSETIDX"},
        {"FGET","FREADHT"},{"LGET","LREADHT"},{"TGET","TFIND"},{"ISMEMBER","TFIND"},
        {"TCHECK","TCHECKCOMPLETE"}};
    auto it=aliases.find(cmd); return it==aliases.end()?cmd:it->second;
}
void ensure_type(const string& name, char type, bool create) {
    bool correct = (type=='M'&&arrays.count(name))||(type=='F'&&flists.count(name))||
        (type=='L'&&lists.count(name))||(type=='S'&&stacks.count(name))||
        (type=='Q'&&queues.count(name))||(type=='T'&&trees.count(name));
    if (exists(name)) {
        if (!correct) throw runtime_error("Имя занято другой структурой");
        return;
    }
    if (!create) throw runtime_error("Структура не найдена: " + name);
    switch(type) {
        case 'M': arrays.emplace(name,Array_create(16)); break;
        case 'F': flists.emplace(name,OneLinked_create()); break;
        case 'L': lists.emplace(name,Linked_create()); break;
        case 'S': stacks.emplace(name,Stack_create()); break;
        case 'Q': queues.emplace(name,Queue_create()); break;
        case 'T': trees.emplace(name,Tree_create()); break;
    }
}
bool execute(const string& query) {
    istringstream input(query);
    string cmd,name,value;
    if (!(input>>cmd>>name)) throw runtime_error("Ожидается КОМАНДА имя [аргументы]");
    cmd=canonical(cmd);
    auto it=specs.find(cmd);
    if (it==specs.end()) throw runtime_error("Неизвестная команда: "+cmd);
    vector<string> args;
    while (input>>value) args.push_back(value);
    const Spec& op=it->second;
    if (args.size()<static_cast<size_t>(op.min)||(op.max>=0&&args.size()>static_cast<size_t>(op.max)))
        throw runtime_error("Неверное число аргументов: "+cmd);
    vector<int> numbers;
    if (cmd=="TINSERT"||cmd=="TFIND") for(auto& a:args) numbers.push_back(number(a));
    int idx=0;
    if (cmd=="MPUSHIDX"||cmd=="MGET"||cmd=="MDELIDX"||cmd=="MSETIDX") {
        idx=number(args[0]);
        if(idx<0) throw runtime_error("Индекс не может быть отрицательным");
    }
    if (cmd=="PRINT") {
        if (!exists(name)) throw runtime_error("Структура не найдена: "+name);
        cout << "=== PRINT " << name << " ===\n";
        if(arrays.count(name)) cout<<Array_read(arrays.at(name))<<"\n";
        if(flists.count(name)) cout<<OneLinked_read(flists.at(name))<<"\n";
        if(lists.count(name)) cout<<Linked_read(lists.at(name))<<"\n";
        if(stacks.count(name)) cout<<Stack_read(stacks.at(name))<<"\n";
        if(queues.count(name)) cout<<Queue_read(queues.at(name))<<"\n";
        if(trees.count(name)) Tree_print(trees.at(name));
        return false;
    }
    if(cmd.size()>=6&&cmd.substr(cmd.size()-6)=="CREATE"&&exists(name))
        throw runtime_error("Структура уже существует");
    ensure_type(name,op.type,op.create);
    if(op.type=='M'&&!args.empty()&&cmd!="MPUSH") {
        size_t n=arrays.at(name).size;
        if(static_cast<size_t>(idx)>n||(static_cast<size_t>(idx)==n&&cmd!="MPUSHIDX"))
            throw runtime_error("Индекс за пределами массива");
    }
    if(cmd=="MPUSH") { for(auto& a:args) Array_push_back(arrays.at(name),a); }
    if(cmd=="MPUSHIDX") { Array_insert(arrays.at(name),idx,args[1]); }
    if(cmd=="MGET") { cout<<Array_get(arrays.at(name),idx)<<"\n"; }
    if(cmd=="MDELIDX") { Array_erase(arrays.at(name),idx); }
    if(cmd=="MSETIDX") { Array_set(arrays.at(name),idx,args[1]); }
    if(cmd=="MLENGTH") { cout<<Array_length(arrays.at(name))<<"\n"; }
    if(cmd=="MREAD") { cout<<Array_read(arrays.at(name))<<"\n"; }
    if(cmd=="SPUSH") { for(auto& a:args) Stack_push_back(stacks.at(name),a); }
    if(cmd=="SPOP") { cout<<Stack_pop(stacks.at(name))<<"\n"; }
    if(cmd=="SREAD") { cout<<Stack_read(stacks.at(name))<<"\n"; }
    if(cmd=="QPUSH") { for(auto& a:args) Queue_push_back(queues.at(name),a); }
    if(cmd=="QPOP") { cout<<Queue_pop(queues.at(name))<<"\n"; }
    if(cmd=="QREAD") { cout<<Queue_read(queues.at(name))<<"\n"; }
    if(cmd=="TINSERT") { for(int a:numbers) Tree_insert(trees.at(name),a); }
    if(cmd=="TFIND") { cout<<(Tree_find(trees.at(name),numbers[0])?"TRUE":"FALSE")<<"\n"; }
    if(cmd=="TREAD") { cout<<Tree_read(trees.at(name))<<"\n"; }
    if(cmd=="TCHECKCOMPLETE") { cout<<(Tree_is_complete(trees.at(name))?"TRUE":"FALSE")<<"\n"; }
    if(cmd=="FPUSH") { for(auto& a:args) OneLinked_push_back(flists.at(name),a); }
    if(cmd=="FPUSHHEAD") { for(auto& a:args) OneLinked_push_front(flists.at(name),a); }
    if(cmd=="FPUSHBEFORE") { OneLinked_push_before(flists.at(name),args[0],args[1]); }
    if(cmd=="FPUSHAFTER") { OneLinked_push_after(flists.at(name),args[0],args[1]); }
    if(cmd=="FPOP") { OneLinked_pop_back(flists.at(name)); }
    if(cmd=="FPOPHEAD") { OneLinked_pop_front(flists.at(name)); }
    if(cmd=="FPOPBEFORE") { OneLinked_pop_before(flists.at(name),args[0]); }
    if(cmd=="FPOPAFTER") { OneLinked_pop_after(flists.at(name),args[0]); }
    if(cmd=="FDELVAL") { OneLinked_del_by_value(flists.at(name),args[0]); }
    if(cmd=="FFINDVAL") { cout<<OneLinked_find_by_value(flists.at(name),args[0])<<"\n"; }
    if(cmd=="FREADHT") { cout<<OneLinked_read(flists.at(name))<<"\n"; }
    if(cmd=="FREADTH") { cout<<OneLinked_read_reverse(flists.at(name))<<"\n"; }
    if(cmd=="LPUSH") { for(auto& a:args) Linked_push_back(lists.at(name),a); }
    if(cmd=="LPUSHHEAD") { for(auto& a:args) Linked_push_front(lists.at(name),a); }
    if(cmd=="LPUSHBEFORE") { Linked_push_before(lists.at(name),args[0],args[1]); }
    if(cmd=="LPUSHAFTER") { Linked_push_after(lists.at(name),args[0],args[1]); }
    if(cmd=="LPOP") { Linked_pop_back(lists.at(name)); }
    if(cmd=="LPOPHEAD") { Linked_pop_front(lists.at(name)); }
    if(cmd=="LPOPBEFORE") { Linked_pop_before(lists.at(name),args[0]); }
    if(cmd=="LPOPAFTER") { Linked_pop_after(lists.at(name),args[0]); }
    if(cmd=="LDELVAL") { Linked_del_by_value(lists.at(name),args[0]); }
    if(cmd=="LFINDVAL") { cout<<Linked_find(lists.at(name),args[0])<<"\n"; }
    if(cmd=="LREADHT") { cout<<Linked_read(lists.at(name))<<"\n"; }
    if(cmd=="LREADTH") { cout<<Linked_read_reverse(lists.at(name))<<"\n"; }
    return op.change;
}
int main(int argc,char* argv[]) {
    int status=0;
    try {
        string file,query;
        if(argc!=5) throw runtime_error("Использование: ./dbms --file file.data --query 'КОМАНДА имя [аргументы]'");
        for(int i=1;i<argc;i+=2) {
            string flag=argv[i];
            if(flag=="--file"&&file.empty()) file=argv[i+1];
            else if(flag=="--query"&&query.empty()) query=argv[i+1];
            else throw runtime_error("Неизвестный или повторный параметр");
        }
        if(file.empty()||query.empty()) throw runtime_error("Нужны --file и --query");
        load(file);
        if(execute(query)) save(file);
    } catch(const exception& e) { cerr<<"Ошибка: "<<e.what()<<"\n"; status=1; }
    cleanup();
    return status;
}
