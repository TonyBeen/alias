/*************************************************************************
    > File Name: singlelist.h
    > Author: hsz
    > Mail:
    > Created Time: 2021年03月28日 星期日 13时38分04秒
 ************************************************************************/

#ifndef __SINGLE_LIST_H__
#define __SINGLE_LIST_H__

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

namespace eular {

/* 1、提供拷贝构造；2、重载 ==，<，> */
template <typename T>
struct Node {
    T *data;
    Node *next;

    Node() : data(nullptr), next(nullptr){}
    Node(const Node& v)
    {
        if (v.data) {
            data = new T(*(v.data)); // 无所谓空或不空,用时再检查
            if (data == nullptr) {
                // log
            }
        }
        next = nullptr;
    }

    Node &operator= (const Node &) = delete;
    ~Node()
    {
        if (data != nullptr) {
            delete data;
            data = nullptr;
        }
    }
};

template <typename T>
class SingleList {
public:
    typedef struct Node<T>  node_t;
    typedef struct Node<T>* nodeptr_t;
    
    SingleList()
    {
        mHeader = new node_t();
        if (mHeader == nullptr) {
            return;
        }
        
        mSize = 0;
        mListBack = mHeader;
    }
    SingleList(const SingleList& sl)
    {
        mHeader = new node_t();
        if (mHeader == nullptr) {
            printf("new error\n");
            return;
        }
        nodeptr_t pCurrent = sl.mHeader;
        nodeptr_t mpCurrent = mHeader;
        while (hasNext(pCurrent)) {
            nodeptr_t pNext = pCurrent->next;

            // Node 拷贝构造
            nodeptr_t node = new node_t(*pNext);
            mpCurrent->next = node;

            mpCurrent = mpCurrent->next;
            pCurrent = pNext;
        }
        mSize = sl.mSize;
        mListBack = mpCurrent;
    }

    SingleList &operator=(const SingleList& sl)
    {
        if(mHeader == nullptr) {
            mHeader = new node_t();
            if (mHeader == nullptr) {
                return *this;
            }
        }

        nodeptr_t pCurrent = sl.mHeader;
        nodeptr_t mpCurrent = mHeader;
        while (hasNext(pCurrent)) {
            nodeptr_t pNext = pCurrent->next;

            // Node 拷贝构造
            nodeptr_t node = new node_t(*pNext);
            mpCurrent->next = node;

            mpCurrent = mpCurrent->next;
            pCurrent = pNext;
        }
        mSize = sl.mSize;
        mListBack = mpCurrent;
    }

    ~SingleList()
    {
        printf("%s()\n", __func__);
        DestroyList();
    }

    // 头插
    void InsertFront(const T& t)
    {
        if (mHeader == nullptr) {
            return;
        }
        nodeptr_t node = ConstructNode(t);
        if (node == nullptr) {
            return;
        }

        nodeptr_t pCurrent = mHeader->next;
        mHeader->next = node;
        node->next = pCurrent;

        ++mSize;
    }

    // 尾插
    void InsertBack(const T& t)
    {
        if (mHeader == nullptr) {
            return;
        }

        nodeptr_t node = ConstructNode(t);
        if (node == nullptr) {
            return;
        }

        mListBack->next = node;
        mListBack = node;

        ++mSize;
    }

    // 从第一个有效节点开始算为0
    void InsertByPos(const T& t, uint32_t pos = 0)
    {
        if (mHeader == nullptr) {
            return;
        }
        if (pos >= mSize) {
            InsertBack(t);
            return;
        }

        nodeptr_t node = ConstructNode(t);
        if (node == nullptr) {
            return;
        }

        nodeptr_t pCurrent = mHeader;
        for (size_t i = 0; (i < pos) && hasNext(pCurrent); ++i) {
            pCurrent = pCurrent->next;
        }
        nodeptr_t pNext = pCurrent->next;
        pCurrent->next = node;
        node->next = pNext;

        ++mSize;
    }

    void DeleteFront()
    {
        if (mHeader == nullptr) {
            return;
        }
        nodeptr_t pCurrent = mHeader->next;
        nodeptr_t pNext = pCurrent->next;
        delete pCurrent;
        mHeader->next = pNext;
        --mSize;
    }

    void DeleteBack()
    {
        if (mHeader == nullptr) {
            return;
        }
        nodeptr_t pCurrent = mHeader;
        nodeptr_t pDel = mHeader->next;
        while (hasNext(pDel)) {
            pCurrent = pCurrent->next;
            pDel = pDel->next;
        }
        pCurrent->next = pDel->next;
        delete pDel;
        pDel = nullptr;
        mListBack = pCurrent;
        --mSize;
    }

    void DeleteByData(const T &t)
    {
        if (mHeader == nullptr) {
            return;
        }
        nodeptr_t pCurrent = mHeader;
        nodeptr_t pDel = mHeader->next;
        while (pDel) {
            if (*(pDel->data) == t) {
                pCurrent->next = pDel->next;
                if (pDel == mListBack) {
                    mListBack = pCurrent;
                }
                delete pDel;
                pDel = nullptr;
                --mSize;
                return;
            }
            pCurrent = pCurrent->next;
            pDel = pDel->next;
        }
    }

    /**
     * @param forword del前一个节点
     * @param del     你想删除的节点
     */
    void DeleteByNode(const nodeptr_t forword, const nodeptr_t del)
    {
        if (mHeader == nullptr || !forword || !del) {
            return;
        }
        if (forword->next != del) {
            return;
        }
        forword->next = del->next;
        if (del == mListBack) {     // 删除的正好是最后一个节点
            mListBack = forword;
        }
        delete del;
        --mSize;
    }

    void ClearList()
    {
        if (mHeader == nullptr) {
            return;
        }
        nodeptr_t pCurrent = mHeader->next;
        while (pCurrent) {
            nodeptr_t pNext = pCurrent->next;
            delete pCurrent;

            pCurrent = pNext;
        }
        mHeader->next = nullptr;
        mSize = 0;
        mListBack = mHeader;
    }

    void DestroyList()
    {
        if (mHeader == nullptr) {
            return;
        }
        nodeptr_t pCurrent = mHeader;
        while (pCurrent) {
            nodeptr_t pNext = pCurrent->next;
            delete pCurrent;

            pCurrent = pNext;
        }
        mHeader = nullptr;
        mSize = 0;
        mListBack = nullptr;
    }

    void ForeachList(void (*CallBack)(void *))
    {
        if (mHeader == nullptr) {
            return;
        }
        nodeptr_t pCurrent = mHeader;
        while (hasNext(pCurrent)) {
            nodeptr_t pNext = pCurrent->next;
            if (pNext != nullptr) {
                CallBack(static_cast<void *>(pNext->data));
                pCurrent = pNext;
            }
        }
    }

    bool isValidList() const
    {
        return !!mHeader;
    }

    uint32_t getSize() const
    {
        return mSize;
    }
    nodeptr_t getLastOne() const
    {
        return mListBack;
    }
    nodeptr_t getHeader() const
    {
        return mHeader;
    }

    bool hasNext(const nodeptr_t n) const
    {
        if (n == nullptr) {
            return false;
        }
        return !!(n->next);
    }

private:
    nodeptr_t ConstructNode(const T &t) const
    {
        T *temp = new T(t);
        if (temp == nullptr) {
            return nullptr;
        }
        nodeptr_t node = new node_t();
        if (node == nullptr) {
            delete temp;
            return nullptr;
        }
        node->data = temp;
        return node;
    }
private:
    nodeptr_t mHeader;
    uint32_t  mSize;
    nodeptr_t mListBack;
};

} // namespace eular
#endif
