#include "compress.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <algorithm>
#include "htree.hpp"
#include "freqcounter.hpp"
#include "bitIO.hpp"

namespace
{
    template <typename T>
    ett::HuffLeaf<T>* generate_hLeaf_from_FU(const ett::FreqUnit<T>& fu)
    {
        using ett::HuffLeaf;
        HuffLeaf<T>* pLeaf(ett::HuffLeaf_create<T>());

        pLeaf->obj = fu.obj;
        pLeaf->cnt = fu.cnt;

        return pLeaf;
    }

    template <typename T>
    ett::HNode<T>* generate_hNode_from_FU(const ett::FreqUnit<T>& fu)
    {
        auto hLeaf(generate_hLeaf_from_FU(fu));
        ett::HNode<T>* pNode(ett::HNode_create<T>());
        pNode->pLeaf = hLeaf;
        pNode->cntSum = pNode->pLeaf->cnt;
        return pNode;
    }

    template <typename T>
    ett::HTree<T>* make_hTree_from_FC(const ett::FreqCounter<T>& fcnter)
    {
        using ett::HNode;
        std::vector<HNode<T>*> nodes(0);
        const auto& fus = ett::FC_get_fus(fcnter);
        for (const auto& fu : fus)
        {
            auto tmpNode(generate_hNode_from_FU(fu));
            nodes.emplace_back(tmpNode);
        }

        if (nodes.size() <= 1)
            throw "File too small to be compressed.";
        while (nodes.size() > 1)
        {
            // smaller one on the left
            sort(nodes.begin(), nodes.end(), [](const HNode<T>* node1, const HNode<T>* node2) -> bool
            {
                return node1->cntSum > node2->cntSum;
            });
            HNode<T>* tmpNode(ett::HNode_create<T>());
            tmpNode->right = nodes.at(nodes.size() - 1);
            tmpNode->left = nodes.at(nodes.size() - 2);
            tmpNode->cntSum = tmpNode->left->cntSum + tmpNode->right->cntSum;

            nodes.pop_back();
            nodes.pop_back();
            nodes.emplace_back(tmpNode);
        }
        ett::HTree<T>* pTree(ett::HTree_create(nodes.front()));
        nodes.pop_back();

        return pTree;
    }
}

void compress(const std::string& inputFile, const std::string& outputFile)
{
    std::ifstream fin(inputFile);
    if (!fin)
        throw std::ios::failure("Failed to open" + inputFile);
    ett::FreqCounter<char> fcnter;
    char iterCh;
    while (fin)
    {
        fin.get(iterCh);
        fcnter << iterCh;
    }

    ett::HTree<char>* hTree = make_hTree_from_FC(fcnter);
    std::map<char, std::string> codeMap(ett::HTree_parse_to_map(hTree));
    ett::HTree_destroy(hTree);

    // std::ofstream fout(outputFile);
    // if (!fout)
    //     throw std::ios::failure("Failed to open" + outputFile);
    //
    // fout.close();

    fin.clear();
    fin.ignore(1000, '\n');
    fin.seekg(std::ios::beg);

    OBitStream& bout(OBStream_create(outputFile));
    while (fin)
    {
        fin.get(iterCh);
        bout << codeMap[iterCh];
    }
    OBStream_destroy(bout);

    fin.close();
}

