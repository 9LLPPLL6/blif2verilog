//
// Created by Pepephant on 2024/11/1.
//

#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <map>
#include <queue>
#include <algorithm>

using namespace std;

enum GateType
{
    G_AND,
    G_OR,
    G_NOT
};

class Gate
{
public:
    string name;           // Gate name
    vector<string> inputs; // Input signals
    string output;         // Output signal
    vector<GateType> ops;
};

class NetList
{
public:
    NetList(string n) : name(n) {}
    string name;
    map<string, Gate *> Gates;
    vector<string> BlifInputs;
    vector<string> BlifOutputs;
};

class Schedule
{
    int time;
    map<string, int> timetable;
    map<string, Gate *> Gates;
};

Schedule *ASAP(NetList *netlist)
{
    Schedule *schedule = new Schedule();
    map<string, int> timetable;
    queue<string> readyQueue;

    // Schedule the initial node at t0 = 0
    for (auto &input : netlist->BlifInputs)
    {
        timetable.emplace(input, 0);
    }

    for (auto &gt : netlist->Gates)
    {
        bool is_first_cycle = true;
        for (auto &input : gt.second->inputs)
        {
            if (std::count(netlist->BlifInputs.begin(), netlist->BlifInputs.end(), input) == 0)
            {
                is_first_cycle = false;
            }
        }
        if (is_first_cycle)
        {
            timetable.emplace(gt.first, 1);
            readyQueue.push(gt.first);
            cout << "Scheduled " << gt.first << endl;
        }
    }

    while (!readyQueue.empty())
    {
        auto current = readyQueue.front();
        readyQueue.pop();

        for (auto &gt : netlist->Gates)
        {
            auto gate = gt.second;
            if (std::find(gate->inputs.begin(), gate->inputs.end(), current) != gate->inputs.end())
            {
                bool allPredecessorsScheduled = true;
                int maxTime = 0;
                auto next = gt.first;
                cout << "Try to schedule " << next << endl;

                for (auto &prev : gate->inputs)
                {
                    if (timetable.find(prev) == timetable.end())
                    {
                        cout << "Can't schedule " << next << endl;
                        allPredecessorsScheduled = false;
                        break;
                    }
                    maxTime = max(maxTime, timetable[prev] + 1); // Assuming unit delay (dj = 1)
                }

                if (allPredecessorsScheduled && timetable.find(next) == timetable.end())
                {
                    cout << "Scheduled " << next << endl;
                    timetable[next] = maxTime;
                    readyQueue.push(next);
                }
            }
        }
    }

    for (auto &it : timetable)
    {
        cout << it.first << " at cycle " << it.second << endl;
    }
}

vector<string> split_blank(const string &str)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(str);
    while (tokenStream >> token)
        tokens.push_back(token);
    return tokens;
}

NetList *ReadBlif(string filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: File not found" << endl;
        return nullptr;
    }
    vector<string> lines;
    string line;
    while (getline(file, line))
    {
        if (!line.empty())
            lines.push_back(line);
    }
    file.close();

    map<string, Gate *> Gates;
    NetList *netlist;
    vector<string> BlifInputs;
    vector<string> BlifOutputs;

    for (auto it = lines.begin(); it < lines.end(); ++it)
    {
        vector<string> tokens = split_blank(*it);
        if (tokens[0] == ".model")
        {
            netlist = new NetList(tokens[1]);
        }
        else if (tokens[0] == ".inputs")
        {
            tokens.erase(tokens.begin());
            BlifInputs = tokens;
        }
        else if (tokens[0] == ".outputs")
        {
            tokens.erase(tokens.begin());
            BlifOutputs = tokens;
        }
        else if (tokens[0] == ".names")
        {
            tokens.erase(tokens.begin());
            Gate *gate = new Gate();
            gate->name = tokens.back();
            gate->output = tokens.back();
            tokens.pop_back();
            gate->inputs = tokens;
            // TODO: Gates
            it++;
            string token = *it;
            if (std::count(token.begin(), token.end(), '-') != 0)
            {
                gate->ops.resize(tokens.size() - 1, G_OR);
                for (int i = 0; i < tokens.size() - 2; i++)
                {
                    it++;
                }
            }
            else if (tokens.size() == 1)
            {
                gate->ops.resize(1, G_NOT);
            }
            else
            {
                gate->ops.resize(tokens.size() - 1, G_AND);
            }
            Gates.emplace(gate->output, gate);
        }
    }

    netlist->BlifInputs = BlifInputs;
    netlist->BlifOutputs = BlifOutputs;
    netlist->Gates = Gates;

    cout << "Inputs: ";
    for (auto &input : netlist->BlifInputs)
    {
        cout << input << " ";
    }
    cout << endl;

    cout << "Outputs: ";
    for (auto &output : netlist->BlifOutputs)
    {
        cout << output << " ";
    }
    cout << endl;

    cout << "Gates: ";
    for (auto &gate : netlist->Gates)
    {
        cout << "(";
        for (auto &in : gate.second->inputs)
        {
            cout << in << " ";
        }
        cout << "-> " << gate.first << ") ";
    }
    cout << endl;

    return netlist;
}

int main()
{
    string filename;
    cout << "Input file: " << endl;
    cin >> filename;
    auto netlist = ReadBlif(filename);
    ASAP(netlist);
}
