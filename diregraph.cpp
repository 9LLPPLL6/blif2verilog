// 将.blif文件转换为有向图
#include "blif2verilog.h"
#include "ml_rcs.h"
#include <iostream>
#include <string>

using namespace std;
int main()
{
    std::cout << "--------task4-1.v-----------" << std::endl;
    std::cout<<std::endl;

    string blif_file = "../vAndBlif/task4-1.v";
    // cell * hnop = vtog(blif_file);
    // map<string, int> m;
    // // m.insert(pair<string, int>("!", 1));
    // // m.insert(pair<string, int>("|", 2));
    // // m.insert(pair<string, int>("&", 3));

    // //ML_RCS_enumerate(fnop, m);
    // // ML_RCS(hnop);
    // // vector<string> a = {"../test.blif", "../test2.blif"};
    // // for (auto it = a.begin(); it < a.end(); it++)
    // //     cout << *it << endl;
    // vector<cell*>& v = hnop->getPrev();
    // for(cell* c:v)
    // {
    //     print_tree(c,0);
    //     cout << endl;
    // }
    // destroy(hnop);
    huV(blif_file, 3);


    std::cout << "--------task4-2.v-----------" << std::endl;
    std::cout<<std::endl;
    string blif_file2 = "../vAndBlif/task4-2.v";
    huV(blif_file2, 3);

    std::cout << "--------task4-3.v-----------" << std::endl;
    std::cout<<std::endl;
    string blif_file3 = "../vAndBlif/task4-3.v";
    huV(blif_file3, 3);


    return 0;
}