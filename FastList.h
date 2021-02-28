#ifndef __FASTLIST_H
#define __FASTLIST_H
#include<iostream>
#include<string.h>
#include<assert.h>

using namespace std;

typedef struct Node
{
    char *key;
    int value;
    struct Node *pre;
    struct Node *next;
} Node;


class C_Hash
{
public:
    C_Hash(int len);
    size_t BKDRHash(const char *str);
    void Insert(size_t hash, Node *Lru);
    bool KeyExist(const char *key, size_t *hash_bak);
    bool SearchNodeHash(const char *key, size_t *h);
    void Print(void);

public:
    Node ** HashTable;

private:
    const int LEN;

};


class C_LRU
{
public:
    C_LRU(int L_len, int S_len);
    bool set(const char *key, int value);
    int get(const char *key);
    void Print(void);

	C_Hash LRU_Hash;	// �ؼ��ֹ�ϣ��
private:
	Node *Lru;			// ʼ��ָ��ͷ�ڵ� 
	Node *L;			// ��ָ̬��ĩβ�ڵ�

	const int L_LEN;	// LRU���� 
	const int S_LEN;	//��Ч�ֽ��� 
    
};



#endif


