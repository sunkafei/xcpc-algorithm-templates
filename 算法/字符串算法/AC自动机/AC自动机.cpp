/*
通过insert将字符串插入Trie之后不要忘记调用getfail建立AC自动机！
对于AC自动机中的状态u，不能仅仅通过val[u] != 0来判断u是否能匹配输入字符串，还要结合last。
对于AC自动机中的状态u，其不能匹配任何一个字符串的条件为val[u] == 0 && last[u] == 0。
*/
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <algorithm> 
#include <queue>
using namespace std;
const int maxnode = 100000;
const int sigma_size = 26;
struct trie
{
	int ch[maxnode][sigma_size];
	int f[maxnode];		// fail函数
	int val[maxnode];	// 每个字符串的结尾结点都有一个非0的val
	int last[maxnode]; // 输出链表的下一个结点
	int sz;
	void init()
	{
		sz = 1;
		memset(ch[0], 0, sizeof(ch[0]));
	}
	// 字符c的编号
	int idx(char c)
	{
		return c - 'a';
	}
	// 插入字符串。v必须非0
	void insert(const char *s, int v)
	{
		int u = 0, n = strlen(s);
		for (int i = 0; i < n; i++)
		{
			int c = idx(s[i]);
			if (!ch[u][c])
			{
				memset(ch[sz], 0, sizeof(ch[sz]));
				val[sz] = 0;
				ch[u][c] = sz++;
			}
			u = ch[u][c];
		}
		val[u] = v;
	}
	// 计算fail函数
	void getfail()
	{
		queue<int> Q;
		f[0] = 0;
		// 初始化队列
		for (int c = 0; c < sigma_size; c++)
		{
			int u = ch[0][c];
			if (u) { f[u] = 0; Q.push(u); last[u] = 0; }
		}
		// 按BFS顺序计算fail
		while (!Q.empty())
		{
			int r = Q.front(); Q.pop();
			for (int c = 0; c < sigma_size; c++)
			{
				int u = ch[r][c];
				if (!u)
				{
					ch[r][c] = ch[f[r]][c];
					continue;
				}
				Q.push(u);
				int v = f[r];
				while (v && !ch[v][c]) v = f[v];
				f[u] = ch[v][c];
				last[u] = val[f[u]] ? f[u] : last[f[u]];
			}
		}
	}
	// 在T中找模板
	void find(const char *T)
	{
		int n = strlen(T);
		int j = 0; // 当前结点编号，初始为根结点
		for (int i = 0; i < n; i++)
		{
			// 文本串当前指针
			int c = idx(T[i]);
			j = ch[j][c];
			if (val[j]) print(j);
			else if (last[j]) print(last[j]); // 找到了！
		}
	}
	// 递归打印以结点j结尾的所有字符串
	void print(int j)
	{
		if (j)
		{
			//对查找到的编号为val[j]的字符串进行操作
			printf("%d\n", val[j]);
			print(last[j]);
		}
	}
}ac;
int main()
{
	ac.init();
	ac.insert("aabbc", 1);
	ac.insert("bbcff", 2);
	ac.insert("bb", 3);
	ac.getfail();
	ac.find("aabbcffbbbop");
	return 0;
}

