#include "blif2verilog.h"
#include <iostream>
#include <fstream>

void hu(string bfile, int s)
{
    ifstream file(bfile);
    if (!file.is_open())
    {
        std::cout << "Error: File not found" << endl;
        return;
    }
    vector<string> lines;
    string line;
    while (getline(file, line))
    {
        if (!line.empty())
            lines.push_back(line);
    }
    file.close();

    model m;
    input inputs;
    output outputs;
    vector<names> ns;

    // 解析blif文件转化为model、input、output、ns
    for (auto it = lines.begin(); it < lines.end(); ++it)
    {
        vector<string> tokens = split_blank(*it);
        if (tokens[0] == ".model")
        {
            m = model(tokens[1]);
        }
        else if (tokens[0] == ".inputs")
        {
            tokens.erase(tokens.begin());
            inputs = input(tokens);
        }
        else if (tokens[0] == ".outputs")
        {
            tokens.erase(tokens.begin());
            outputs = output(tokens);
        }
        else if (tokens[0] == ".names")
        {
            vector<string> outputs = {tokens[tokens.size() - 1]};
            output o(outputs);
            vector<string> inputs(tokens.begin() + 1, tokens.end() - 1);
            input input(inputs);
            auto next_it = it + 1;
            vector<string> next_tokens = split_blank(*next_it);
            vector<vector<int>> table;

            while (next_tokens[0] != ".names" && next_tokens[0] != ".end")
            {
                string str = next_tokens[0];
                vector<int> v;
                for (char c : str)
                {
                    if (c == '0')
                        v.push_back(0);
                    else if (c == '1')
                        v.push_back(1);
                    else
                        v.push_back(-1);
                }
                table.push_back(v);
                next_tokens = split_blank(*(++next_it));
            }
            ns.push_back(names(input, o, table));
        }
    }
    std::cout << "Input :";
    for (auto i : inputs.getInputs())
        std::cout << i << " ";
    std::cout << "Output :";
    for (auto o : outputs.getOutputs())
        std::cout << o << " ";
    std::cout << endl;
    int l = 1;
    int i = 0;
    int j = 0;
    vector<string> pri;
    int count = s;
    bool flag = false;
    while (true)
    {
        flag = false;
        string out = " ";
        for (j = 0, i; i < count && i < ns.size(); i++, j++)
        {
            if (j > 0)
            {
                vector<string> current = ns[i].getInputs().getInputs();
                for (j; j > 0; j--)
                {
                    string outp = ns[i - j].getOutputs().getOutputs()[0];
                    if (find(current.begin(), current.end(), outp) != current.end())
                    {
                        flag = true;
                        break;
                    }
                }
            }
            if (flag)
            {
                count += j;
                break;
            }
            out.append(ns[i].getOutputs().getOutputs()[0]);
            out.append(" ");
        }
        pri.push_back(out);
        if (!flag)
            count += s;
        if (i == ns.size())
            break;
        l++;
    }
    std::cout << "Total " << l << " Cycles" << endl;
    for (int i = 0; i < pri.size(); i++)
    {
        std::cout << "Cycle " << i << ":" << "{ " << pri[i] << " },{},{}" << endl;
    }
}