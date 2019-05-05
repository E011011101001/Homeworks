#ifndef __BTREE_HPP_EOL
#define __BTREE_HPP_EOL

#include <iostream>
#include <cstdlib>
#include <map>

namespace ett
{
    template <typename T>
    struct HuffLeaf
    {
        T obj;
        size_t cnt;
        std::string code;
    };

    template <typename T>
    HuffLeaf<T>* HuffLeaf_create()
    {
        HuffLeaf<T>* pLeaf(nullptr);
        try
        {
            pLeaf = new HuffLeaf<T>;
            pLeaf->cnt = 0;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
        return pLeaf;
    }

    template <typename T>
    void HuffLeaf_destroy(HuffLeaf<T>* pLeaf)
    {
        delete pLeaf;
    }

    template <typename T>
    struct HNode
    {
        HuffLeaf<T>* pLeaf;
        size_t cntSum;
        HNode<T>* left, * right;
    };

    template <typename T>
    HNode<T>* HNode_create()
    {
        HNode<T>* pNode(nullptr);
        try
        {
            pNode = new HNode<T>;
            pNode->left = pNode->right = nullptr;
            pNode->pLeaf = nullptr;
            pNode->cntSum = 0;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
        return pNode;
    }

    template <typename T>
    bool HNode_isLeaf(const HNode<T>* pNode)
    {
        return (pNode->pLeaf != nullptr);
    }

    template <typename T>
    void HNode_destroy(HNode<T>* pNode)
    {
        if (HNode_isLeaf(pNode))
            HuffLeaf_destroy(pNode->pLeaf);
        delete pNode;
    }

    template <typename T>
    struct HTree
    {
        HNode<T>* root;
    };

    template <typename T>
    HTree<T>* HTree_create()
    {
        HTree<T>* pTree(nullptr);
        try
        {
            pTree = new HTree<T>;
            pTree->root = nullptr;
        }
        catch (const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            exit(EXIT_FAILURE);
        }
        return pTree;
    }

    template <typename T>
    void HTree_assign_codes_recursively(HNode<T>* pNode, std::string&& presentCode)
    {
        if (presentCode.length() && HNode_isLeaf(pNode))
            pNode->pLeaf->code = presentCode;
        else
        {
            HTree_assign_codes_recursively(pNode->left, presentCode + "1");
            HTree_assign_codes_recursively(pNode->right, presentCode + "0");
        }
    }

    template <typename T>
    void HTree_generate_coding(HTree<T>* pTree)
    {
        HTree_assign_codes_recursively(pTree->root, std::string());
    }

    template <typename T>
    HTree<T>* HTree_create(const HNode<T>* rtNode)
    {
        HTree<T>* pTree(HTree_create<T>());
        pTree->root = const_cast<ett::HNode<T>*>(rtNode);

        HTree_generate_coding(pTree);

        return pTree;
    }

    template <typename T>
    void HTree_add_pairs_recursively(const HNode<T>* pNode, std::map<T, std::string>& codeMap)
    {
        if (HNode_isLeaf(pNode))
            codeMap.insert(std::pair<T, std::string>(pNode->pLeaf->obj, pNode->pLeaf->code));
        else
        {
            HTree_add_pairs_recursively(pNode->left, codeMap);
            HTree_add_pairs_recursively(pNode->right, codeMap);
        }
    }

    template <typename T>
    std::map<T, std::string> HTree_parse_to_map(const HTree<T>* pTree)
    {
        std::map<T, std::string> codeMap;
        HTree_add_pairs_recursively(pTree->root, codeMap);
        return codeMap;
    }

    template <typename T>
    void HTree_destroy_hNodes_recursively(HNode<T>* pNode)
    {
        if (!HNode_isLeaf(pNode))
        {
            HTree_destroy_hNodes_recursively(pNode->left);
            HTree_destroy_hNodes_recursively(pNode->right);
        }
        HNode_destroy(pNode);
    }

    template <typename T>
    void HTree_destroy(HTree<T>* pTree)
    {
        HTree_destroy_hNodes_recursively(pTree->root);
        delete pTree;
    }
}

#endif // __BTREE_HPP_EOL

