#include"FastList.h"


int main()
{
	
	C_LRU LRU(5, 10);	// LRU最大长度为10，字符串key最大有效长度为10
	
	LRU.Print();
	LRU.LRU_Hash.Print();
	
	LRU.set("jack", 1);
	LRU.Print();
	LRU.LRU_Hash.Print();
	
	LRU.set("tom", 2);
	LRU.set("bob", 3);
	LRU.set("alice", 4);
	LRU.set("rose", 5);
	
	LRU.Print();
	LRU.LRU_Hash.Print();

	LRU.set("jack", 6);

	LRU.Print();
	LRU.LRU_Hash.Print();
	
	LRU.set("lola", 6);
	
	LRU.Print();
	LRU.LRU_Hash.Print();
	
	cout<< "tom" << "的值为：" << LRU.get("tom") <<endl;
	LRU.Print();
	LRU.LRU_Hash.Print();
	
	cout<< "lola" << "的值为：" << LRU.get("lola") <<endl;
	LRU.Print();
	LRU.LRU_Hash.Print();

	cout<< "lola" << "的值为：" << LRU.get("lola") <<endl;
	LRU.Print();
	LRU.LRU_Hash.Print();
	
	cout<< "bob" << "的值为：" << LRU.get("bob") <<endl;
	LRU.Print();
	LRU.LRU_Hash.Print();
	
	
	return 0;
} 


