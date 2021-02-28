#include"FastList.h"



//=======================================================================================  HashTable

C_Hash::C_Hash(int len) : LEN(len)
{
    HashTable = new Node* [LEN];    // ָ������ 
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
        hash = hash * 31 + ch;   // Ҳ���Գ���31��131��1313��13131��131313..    
    }
    hash = hash % 5;

    return hash;
}

void C_Hash::Insert(size_t hash, Node *Lru)
{
    while (HashTable[ hash ] != NULL)   	// �����Ϊ�գ����ͻ
    {
        hash = (hash + 1) % LEN;            // ���Ŷ�ַ��������̽�� 
    }   
    HashTable[ hash ] = Lru;
}

bool C_Hash::KeyExist(const char *key, size_t *hash_bak)
{
    size_t hash = 0;
    
    hash = BKDRHash(key);                   // ���ַ���key�Ĺ�ϣֵ 
    (*hash_bak) = hash;

    if(HashTable[hash] == NULL)     		//�Ƿ�Ϊ�ձ� 
    {
        return false; 
    }
    else
    {
        // �����������֤��֮ǰ���ڳ�ͻ�������ַ���ƶ��������ط������ٴ��ƶ�����
        while( strcmp( HashTable[hash]->key, key) )
        {
            hash = (hash + 1) % LEN;            // ���Ŷ�ַ��������̽��
            if ( HashTable[hash] == NULL || hash == (*hash_bak))      // ���û�в�ѯ����ѭ���ص�ԭ�㣬��˵���ؼ��ֲ����� 
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
        	cout<<"||  "<< i <<" ��"<< HashTable[i]->key <<endl;
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
    
    Lru->value = 0; // ͷ�ڵ㴢��� 
}


bool C_LRU::set(const char *key, int value)
{
    assert(key != NULL);
    
    // key�ظ���� 
    size_t hash;
    if ( LRU_Hash.KeyExist(key, &hash) )
    {
        cout<< key <<"�Ѵ��ڣ�����ʧ��!"<< endl;
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
    t->pre = L;         // t��ָ�����һ���ڵ� 
    t->next = NULL;
    
    L->next = t;
    L = L->next;    // Lʼ��ָ�����Ľڵ� 
    
    if(Lru->value == L_LEN)
    {
        size_t firstNode_hash;
        Node *p;
        p = Lru->next->next;    // pָ��ڶ����ڵ�
        
        // ͬʱ�����ڹ�ϣ���еļ�¼ 
        LRU_Hash.SearchNodeHash( Lru->next->key, &firstNode_hash );
        LRU_Hash.HashTable[firstNode_hash] = NULL;                  
        
        delete [] Lru->next->key;   // ���ٵ�һ���ڵ��key 
        Lru->next->pre = NULL;
        Lru->next->next = NULL;
        delete Lru->next;           // ���ٵ�һ���ڵ� 
        
        Lru->next = p;      // ͷ�ڵ���ڶ����ڵ��������ڶ����ڵ�����Ϊ��һ���ڵ㣩 
        p->pre = Lru;
        
        LRU_Hash.Insert(firstNode_hash, t);		// ��ʣ����һ���� 
    }
    else
    {
    	LRU_Hash.Insert(hash, t);
    	Lru->value++;   //ʵʱ���泤��
	}
    
    cout<< key <<"����ɹ�!"<< endl;
    
    return true;
}


int C_LRU::get(const char *key)
{
    size_t hash=0;
    LRU_Hash.SearchNodeHash(key, &hash);
    
    Node *p;    
    p = LRU_Hash.HashTable[hash];   // pָ��ýڵ� 
    
    if(p != L)  //����ýڵ㲻�����һ���ڵ� 
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
        cout<<"||   ����"<<endl;
        cout<<"||  "<< p->key <<"("<< p->value <<")"<<endl;
        p = p->next;
    }
    cout<<"======================"<<endl;
} 




















