#include"FastList.h"



//=======================================================================================  HashTable

C_Hash::C_Hash(int len) : LEN(len)
{
    HashTable = new Node* [LEN];    // 指针数组 
    for(int i=0; i < LEN; i++)
    {
        HashTable[i] = NULL;
    }
}

size_t C_Hash::BKDRHash(const char *str)  
{  
    size_t hash = 0;  
    while (size_t  ch = (size_t )*str++)  
    {       
        hash = hash * 31 + ch;   // 也可以乘以31、131、1313、13131、131313..    
    }
    hash = hash % 5;

    return hash;
}

void C_Hash::Insert(size_t hash, Node *Lru)
{
    while (HashTable[ hash ] != NULL)   	// 如果不为空，则冲突
    {
        hash = (hash + 1) % LEN;            // 开放定址法的线性探测 
    }   
    HashTable[ hash ] = Lru;
}

bool C_Hash::KeyExist(const char *key, size_t *hash_bak)
{
    size_t hash = 0;
    
    hash = BKDRHash(key);                   // 求字符串key的哈希值 
    (*hash_bak) = hash;

    if(HashTable[hash] == NULL)     		//是否为空表 
    {
        return false; 
    }
    else
    {
        // 如果不符，则证明之前存在冲突情况，地址已移动到其他地方，需再次移动查找
        while( strcmp( HashTable[hash]->key, key) )
        {
            hash = (hash + 1) % LEN;            // 开放定址法的线性探测
            if ( HashTable[hash] == NULL || hash == (*hash_bak))      // 如果没有查询到或循环回到原点，则说明关键字不存在 
                return false;
        }
    }
    
    return true;
}

bool C_Hash::SearchNodeHash(const char *key, size_t *h)
{
    size_t hash=0;
    size_t hash_bak=0;
    hash = BKDRHash(key);  
	hash_bak = hash;            

    while( strcmp( HashTable[hash]->key, key) )
    {
        hash = (hash + 1) % LEN;           
        if ( HashTable[hash] == NULL || hash == hash_bak)
        {
            (*h) = 0;
            return false;
        }
    }
    (*h) = hash;
    
    return true;
}

void C_Hash::Print(void)
{
    cout<<"---- HashTable ----"<<endl;
    for(int i=0; i < LEN; i++)
    {
        if(HashTable[i] != NULL)
        	cout<<"||  "<< i <<" →"<< HashTable[i]->key <<endl;
    }
    cout<<"-------------------"<<endl;
} 


//=======================================================================================  List

C_LRU::C_LRU(int L_len, int S_len) : L_LEN(L_len), S_LEN(S_len), LRU_Hash(L_len)
{
    Lru = new Node;
    if(Lru == NULL) exit(1);
    
    Lru->key = new char[strlen("head") + 1];
    strcpy(Lru->key, "head");
    
    Lru->value = 0;
    Lru->pre = NULL;
    Lru->next = NULL;
    
    L = Lru;
    
    Lru->value = 0; // 头节点储存表长 
}


bool C_LRU::set(const char *key, int value)
{
    assert(key != NULL);
    
    // key重复检查 
    size_t hash;
    if ( LRU_Hash.KeyExist(key, &hash) )
    {
        cout<< key <<"已存在，插入失败!"<< endl;
        return false;
    }
    
    Node * t;
    t = new Node;
    if(t == NULL) exit(1);
    
    int k_len = strlen(key);
    assert(k_len <= S_LEN);
    t->key = new char[k_len+1];
    if(t->key == NULL) exit(1);
    strcpy(t->key, key);
    
    t->value = value;
    t->pre = L;         // t回指向最后一个节点 
    t->next = NULL;
    
    L->next = t;
    L = L->next;    // L始终指向最后的节点 
    
    if(Lru->value == L_LEN)
    {
        size_t firstNode_hash;
        Node *p;
        p = Lru->next->next;    // p指向第二个节点
        
        // 同时销毁在哈希表中的记录 
        LRU_Hash.SearchNodeHash( Lru->next->key, &firstNode_hash );
        LRU_Hash.HashTable[firstNode_hash] = NULL;                  
        
        delete [] Lru->next->key;   // 销毁第一个节点的key 
        Lru->next->pre = NULL;
        Lru->next->next = NULL;
        delete Lru->next;           // 销毁第一个节点 
        
        Lru->next = p;      // 头节点与第二个节点相连（第二个节点重置为第一个节点） 
        p->pre = Lru;
        
        LRU_Hash.Insert(firstNode_hash, t);		// 就剩了这一个空 
    }
    else
    {
    	LRU_Hash.Insert(hash, t);
    	Lru->value++;   //实时保存长度
	}
    
    cout<< key <<"插入成功!"<< endl;
    
    return true;
}


int C_LRU::get(const char *key)
{
    size_t hash=0;
    LRU_Hash.SearchNodeHash(key, &hash);
    
    Node *p;    
    p = LRU_Hash.HashTable[hash];   // p指向该节点 
    
    if(p != L)  //如果该节点不是最后一个节点 
    {
        p->pre->next = p->next;
        p->next->pre = p->pre;
        
        p->pre = L;
        p->next = NULL;
        L->next = p;
        
        L = L->next;
        
        return p->value;
    }
    else
        return L->value;
}



void C_LRU::Print(void)
{
    Node *p;
    p = Lru;
    cout<<"======================"<<endl;
    cout<<"||  "<< p->key <<"(len: "<< p->value <<")"<<endl;
    p = p->next;
    while(p != NULL)
    {
        cout<<"||   ↓↑"<<endl;
        cout<<"||  "<< p->key <<"("<< p->value <<")"<<endl;
        p = p->next;
    }
    cout<<"======================"<<endl;
} 




















