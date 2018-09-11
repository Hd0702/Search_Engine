#ifndef INDEXINTERFACE_H
#define INDEXINTERFACE_H
#include <string>
#include <vector>
/**
 * @brief The indexInterface class : this is a pure abstract class that influences the avl/hash index files
 * Each of the index types have ALL of these members, which is explained in depth on their selected class filesv
 */
class indexInterface {
public:
    virtual void getTF() =0;
    virtual void getIDF() =0;
    virtual void join() =0;
    virtual void writeFile() =0;
    virtual void evaluate(std::string) =0;
    virtual std::vector<int> getRests() = 0;
    virtual void adjustStats(int&,int&,std::string&) =0;
};

#endif // INDEXINTERFACE_H
