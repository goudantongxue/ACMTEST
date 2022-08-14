﻿#include <bits/stdc++.h>
using namespace std;

namespace MonoStack
{
	void FindFirstHigh(vector<int>& vec)
	{
		vec.push_back(INT_MAX);

		stack<int> stk;
		vector<int> result(vec.size());

		for (int i = 0; i < vec.size(); i++)
		{
			while (!stk.empty() && vec[i] > vec[stk.top()])
			{
				result[stk.top()] = i;
				stk.pop();
			}

			stk.push(i);
		}

		vec.pop_back();


		for (auto num : result)
		{
			cout << num << ' ';
		}
	}

	int main()
	{
		vector<int> vec{ 3, 1, 2, 3, 5, 6, 8, 4, 5, 2 };

		FindFirstHigh(vec);

		return 0;
	}
}

namespace BFS
{
	// 是否为有向图
	bool isDirected = false;

	// 根据题目要求建立的一些全局变量
	int rows, cols, pointy, pointx, threshold, result = 0;
	const vector<pair<int, int>> Options{
		{1, 0}, {-1, 0}, {0, 1}, {0, -1}
	};

	// 在进行与节点相关的边被处理前调用它
	void ProcessVertexEarly()
	{
		result++;
	}

	// 处理完与此节点有关的所有出边后调用它
	void ProcessVertexLate()
	{

	}

	// 处理树边
	void ProcessTreeEdge()
	{

	}

	// 处理其他边
	void ProcessOtherEdge()
	{

	}

	// 寻找通往根节点的路径
	void FindPath(pair<int, int> end, vector<vector<pair<int, int>>>& ParentTable)
	{
		// 如果遇到根节点或者死路，输出路径并返回。
		if (end == make_pair(pointx, pointy) || end == make_pair(-1, -1))
		{
			cout << "(" << end.first << "," << end.second << ")" << endl;
			return;
		}

		int x = ParentTable[end.first][end.second].first;
		int y = ParentTable[end.first][end.second].second;

		// 递归
		FindPath(make_pair(x, y), ParentTable);

		// 输出路径
		cout << "(" << end.first << "," << end.second << ")" << endl;
	}

	// BFS 实现
	// Graph | IsDiscovered | IsProcessed | ParentTable
	void _BFSUtil(vector<vector<int>>& image, vector<vector<bool>>& IsDiscovered, vector<vector<bool>>& IsProcessed, vector<vector<pair<int, int>>>& ParentTable)
	{
		// 以节点的数据类型建立适当的队列结构
		queue<pair<int, int>> q;

		// 推入首节点
		q.push(make_pair(pointx, pointy));

		// 并将首节点对应的 IsDiscovered变量设置为 true
		IsDiscovered[pointx][pointy] = true;

		// 建立当队列不为空时的循环
		while (!q.empty())
		{
			// 取一个节点（队列首部元素）
			pair<int, int> tmp = q.front();
			q.pop();

			// 将节点对应的 IsProcessed 标志置为 true
			IsProcessed[tmp.first][tmp.second] = true;

			// 处理节点的时机
			ProcessVertexEarly();

			// 对当前节点所有可能的扩展进行遍历
			for (int i = 0; i < Options.size(); i++)
			{
				// 获取可能的节点，并防止索引溢出
				int x = tmp.first + Options[i].first;
				int y = tmp.second + Options[i].second;

				if (x < 0 || x >= rows || y < 0 || y >= cols)
				{
					continue;
				}
				// 将边分为两种，一种是树边，一种是其他边。其中，对于树边，我们不仅处理此边，而且会将新节点插入队列；对于其他边，根据有向图与无向图，我们会分别进行分类处理。
				// 如果节点尚未发现，则扩展节点
				if (IsDiscovered[x][y] == false && abs(image[x][y] - image[tmp.first][tmp.second]) < threshold)
				{
					q.push(make_pair(x, y));

					// 处理树边
					ProcessTreeEdge();

					IsDiscovered[x][y] = true;
					ParentTable[x][y] = make_pair(tmp.first, tmp.second);
				}
				else if (!IsProcessed[x][y] || isDirected)
				{
					// 处理树边之外的其他边
					ProcessOtherEdge();
				}
			}

			// 处理节点的时机
			ProcessVertexLate();
		}

	}

	int main()
	{
		cin >> rows >> cols >> pointy >> pointx >> threshold;

		// 根据题意，建立适当的数据结构。
		vector<vector<int> > image(rows, vector<int>(cols));
		vector<vector<bool> > IsDiscovered(rows, vector<bool>(cols, false));
		vector<vector<bool> > IsProcessed(rows, vector<bool>(cols, false));
		vector<vector<pair<int, int>>> ParentTable(rows, vector<pair<int, int>>(cols, make_pair(-1, -1)));

		// 读入数据
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < cols; j++)
			{
				cin >> image[i][j];
			}
		}

		// BFS Search
		_BFSUtil(image, IsDiscovered, IsProcessed, ParentTable);

		cout << "Point Number : " << result << endl;

		FindPath(make_pair(5, 2), ParentTable);

		return 0;
	}
}

namespace DFS
{
	// 是否为有向图
	bool isDirected = false;

	// 根据题目要求建立的一些全局变量
	int rows, cols, pointy, pointx, threshold, result = 0;

	const vector<pair<int, int>> Options{
		{1, 0}, {-1, 0}, {0, 1}, {0, -1}
	};

	// 在进行与节点相关的边被处理前调用它
	void ProcessVertexEarly()
	{
		result++;
	}

	// 处理完与此节点有关的所有出边后调用它
	void ProcessVertexLate()
	{

	}

	// 处理树边
	void ProcessTreeEdge()
	{

	}

	// 处理树边以外的其他边
	void ProcessOtherEdge()
	{

	}

	// 寻找通往根节点的路径
	void FindPath(pair<int, int> end, vector<vector<pair<int, int>>>& ParentTable)
	{
		// 如果遇到根节点或者死路，输出路径并返回。
		if (end == make_pair(pointx, pointy) || end == make_pair(-1, -1))
		{
			cout << "(" << end.first << "," << end.second << ")" << endl;
			return;
		}

		int x = ParentTable[end.first][end.second].first;
		int y = ParentTable[end.first][end.second].second;

		// 递归
		FindPath(make_pair(x, y), ParentTable);

		// 输出路径
		cout << "(" << end.first << "," << end.second << ")" << endl;
	}

	// DFS 实现
	// Graph | Current Node | IsDiscovered | IsProcessed
	void _DFSUtil(vector<vector<int>>& image, int x, int y, vector<vector<bool>>& IsDiscovered, vector<vector<bool>>& IsProcessed, vector<vector<pair<int, int>>>& ParentTable)
	{
		// 将当前节点设置为已发现
		IsDiscovered[x][y] = true;

		// 在遍历出边之前遍历所有节点
		ProcessVertexEarly();

		// 扩展所有可能的节点
		for (int i = 0; i < Options.size(); i++)
		{
			int nextx = x + Options[i].first;
			int nexty = y + Options[i].second;

			if (nextx < 0 || nexty < 0 || nextx >= rows || nexty >= cols)
			{
				continue;
			}

			// 将边分为两种，一种是树边，一种是其他边。其中，对于树边，我们不仅处理此边，而且会将新节点进行递归；对于其他边，根据有向图与无向图，我们会分别进行分类处理。
			if (IsDiscovered[nextx][nexty] == false && abs(image[pointx][pointy] - image[nextx][nexty]) < threshold)
			{
				ParentTable[nextx][nexty] = make_pair(x, y);

				ProcessTreeEdge();

				_DFSUtil(image, nextx, nexty, IsDiscovered, IsProcessed, ParentTable);
			}
			else if (IsProcessed[nextx][nexty] == false || isDirected)
			{
				ProcessOtherEdge();
			}
		}

		// 在遍历完所有出边之后遍历节点
		ProcessVertexLate();

		// 将节点标志“已处理完”置true
		IsProcessed[x][y] = true;

	}

	int main()
	{
		cin >> rows >> cols >> pointy >> pointx >> threshold;

		// 根据题意，建立适当的数据结构。
		vector<vector<int> > image(rows, vector<int>(cols));
		vector<vector<bool> > IsDiscovered(rows, vector<bool>(cols, false));
		vector<vector<bool> > IsProcessed(rows, vector<bool>(cols, false));
		vector<vector<pair<int, int>>> ParentTable(rows, vector<pair<int, int>>(cols, make_pair(-1, -1)));

		// 读入数据
		for (size_t i = 0; i < rows; i++)
		{
			for (size_t j = 0; j < cols; j++)
			{
				cin >> image[i][j];
			}
		}

		_DFSUtil(image, pointx, pointy, IsDiscovered, IsProcessed, ParentTable);

		cout << "Point Number: " << result << endl;

		return 0;
	}
}

namespace MinimumSpanningTree
{
	struct GraphNode {
		int _node;
		int _weight;

		GraphNode(int node, int weight) : _node(node), _weight(weight) { }
	};

	struct Graph {
		vector<vector<GraphNode>> _graphData;
		int _nodeCount;
		bool _isDirected;

		Graph(int nodeCount, bool isDirected) : _nodeCount(nodeCount), _isDirected(isDirected) {
			_graphData.resize(nodeCount);
		}
	};

	Graph g(7, false);

	void ReadGraph() {
		int x, y, weight;

		// Construct Graph
		while (cin >> x >> y >> weight)
		{
			g._graphData[x].push_back(GraphNode(y, weight));

			// If the graph is directed, we need add x node to y
			if (!g._isDirected)
			{
				g._graphData[y].push_back(GraphNode(x, weight));
			}
		}

		// Print Graph
		cout << "Current Graph Data Structure : " << endl;

		int i = 0;
		for (auto& vec : g._graphData)
		{
			cout << i << " : ";
			for (auto node : vec)
			{
				cout << node._node << ' ';
			}
			cout << endl;
			i++;
		}
	}


	// Minimum Spanning Tree Algorithm
	int MinimumSpanningTreeAlgorithm()
	{
		vector<bool> intree(g._nodeCount, false);
		vector<int> distance(g._nodeCount, INT_MAX);

		// 首先，我们选择一个起始点，并将其对应的distance变量置为0
		int v = 0;
		distance[v] = 0;

		// 最小生成树的总重量
		int totalWeight = 0;

		// 循环进行的条件是我们选出的节点没在最小生成树中
		while (intree[v] == false) {
			// 将节点放入最小生成树中
			intree[v] = true;

			// 遍历当前节点的所有节点
			for (int i = 0; i < g._graphData[v].size(); i++)
			{
				// y : nodeNumber
				// weight : edge weight
				int y = g._graphData[v][i]._node;
				int weight = g._graphData[v][i]._weight;

				// 如果当前节点没有在最小生成树中
				// 且 y 节点入树的代价小于之前的代价，则更新他
				if (intree[y] == false && weight < distance[y]) {
					distance[y] = weight;
				}
			}

			// 将待选节点的标识设置为0
			// 将当前可选的最小距离设置为 INT_MAX
			v = 0;
			int dist = INT_MAX;

			// 遍历，找出当前入树的最小代价
			// 并将这一节点设置为下一个将要扩展的节点
			for (int i = 0; i < distance.size(); i++)
			{
				if (intree[i] == false && distance[i] < dist)
				{
					dist = distance[i];
					v = i;
				}
			}

			// If dist is not INT_MAX, sum it!
			if (dist != INT_MAX)
			{
				totalWeight += dist;
			}
		}

		return totalWeight;
	}

	int main()
	{
		ReadGraph();
		cout << "Minimum Spanning Tree Weight is " << MinimumSpanningTreeAlgorithm() << endl;
		return 0;
	}
}

namespace ShortestPath
{
	struct GraphNode {
		int _node;
		int _weight;

		GraphNode(int node, int weight) : _node(node), _weight(weight) { }
	};

	struct Graph {
		vector<vector<GraphNode>> _graphData;
		int _nodeCount;
		bool _isDirected;

		Graph(int nodeCount, bool isDirected) : _nodeCount(nodeCount), _isDirected(isDirected) {
			_graphData.resize(nodeCount);
		}
	};

	Graph g(7, false);

	void ReadGraph() {
		int x, y, weight;

		// Construct Graph
		while (cin >> x >> y >> weight)
		{
			g._graphData[x].push_back(GraphNode(y, weight));

			// If the graph is directed, we need add x node to y
			if (!g._isDirected)
			{
				g._graphData[y].push_back(GraphNode(x, weight));
			}
		}

		// Print Graph
		cout << "Current Graph Data Structure : " << endl;

		int i = 0;
		for (auto& vec : g._graphData)
		{
			cout << i << " : ";
			for (auto node : vec)
			{
				cout << node._node << ' ';
			}
			cout << endl;
			i++;
		}
	}

	// Dijkstra算法：在边加权的图中寻找一条最短路径
	// 给定一个起点s，Dijkstra算法会找到图中从s到剩下每个节点的最短距离，包括终点t
	// 思路：
	// 假设图G中从s到t的最短路径途径某个顶点x，显然这条路径肯定包含从s到x的最短路径作为前缀，因为不这样的
	// 话，我们可以使用较短的s到x的前缀来缩短s到t的路径，因此，再找到从s到t的路径前，我们必须计算从s到x的最短路径
	int ShortestPathAlgorithm(int start, int end)
	{
		vector<bool> intree(g._nodeCount, false);
		vector<int> distance(g._nodeCount, INT_MAX);

		int v = start;
		distance[v] = 0;

		while (intree[v] == false)
		{
			intree[v] = true;

			for (int i = 0; i < g._graphData[v].size(); i++)
			{
				int nextNode = g._graphData[v][i]._node;
				int weight = distance[v] + g._graphData[v][i]._weight;

				if (intree[nextNode] == false && weight < distance[nextNode])
				{
					distance[nextNode] = weight;
				}
			}

			v = 0;
			int dist = INT_MAX;

			for (int i = 0; i < g._nodeCount; i++)
			{
				if (intree[i] == false && dist > distance[i])
				{
					dist = distance[i];
					v = i;
				}
			}

		}


		return distance[end];
	}

	int main()
	{
		ReadGraph();
		cout << "Shortest Path From 0 to 4 is " << ShortestPathAlgorithm(0, 4) << endl;
		return 0;
	}

}

namespace Permutations
{
	int count = 0;

	void ProcessLogic(vector<int>& vec)
	{
		for (auto& num : vec)
		{
			cout << num << ' ';
		}
		cout << endl;
		count++;
	}

	void AllPer(vector<int>& vec)
	{
		sort(vec.begin(), vec.end());

		do
		{
			ProcessLogic(vec);
		} while (next_permutation(vec.begin(), vec.end()));

		cout << count;
	}

	int main()
	{
		vector<int> vec{ 3, 5, 6, 2, 4 };

		AllPer(vec);

		return 0;
	}
}

namespace FixedLenSet
{
	int _Len = 0;
	vector<vector<int>> result;
	vector<int> path;

	void FixedLenSet(vector<int>& vec, int beginIndex)
	{
		if (path.size() + vec.size() - beginIndex < _Len)
		{
			return;
		}

		if (path.size() == _Len)
		{
			result.push_back(path);
			return;
		}

		for (int i = beginIndex; i < vec.size(); i++)
		{
			path.push_back(vec[i]);
			FixedLenSet(vec, i + 1);
			path.pop_back();
		}
	}

	int main()
	{
		vector<int> vec{ 3, 1, 2, 4, 5, 6, 8 };
		_Len = 3;


		FixedLenSet(vec, 0);

		for (auto& v : result)
		{
			for (auto i : v)
			{
				cout << i << ' ';
			}
			cout << endl;
		}


		cout << result.size();
		return 0;
	}
}

namespace AllSubSet
{
	void AllSubsets(vector<int>& vec, vector<int>& path, int index, vector<vector<int>>& result)
	{
		if (index >= vec.size())
		{
			result.push_back(path);
			return;
		}

		AllSubsets(vec, path, index + 1, result);

		path.push_back(vec[index]);
		AllSubsets(vec, path, index + 1, result);
		path.pop_back();

	}


	int main()
	{
		vector<int> vec{ 1, 2, 3, 4, 5 };
		vector<int> path;
		vector<vector<int>> result;

		AllSubsets(vec, path, 0, result);


		for (auto& v : result)
		{
			for (auto i : v)
			{
				cout << i << ' ';
			}
			cout << endl;
		}

		cout << result.size() << endl;
		return 0;
	}
}

namespace stringManipulation
{
	// 通过某种标志行字符将字符串分成两个部分
	void StringPartition1(string& str, string& str1, string& str2)
	{
		string::size_type found = str.find_first_of('-');

		// 真实情况下还有考虑 found 是否等于 string::npos
		str1 = str.substr(0, found);
		str2 = str.substr(found + 1);
	}

	void TestStringPartition1()
	{
		string str = "A B C D-E F G H";
		string str1, str2;

		StringPartition1(str, str1, str2);

		cout << "str " << str << endl;
		cout << "str1 " << str1 << endl;
		cout << "str2 " << str2 << endl;
	}

	// 相当于 C++ 版的 Split 函数
	vector<string> StringPartition2(string str, char delim = ',')
	{
		// replace delim with space
		replace_if(str.begin(), str.end(), [&](const char c) { return c == delim; }, ' ');

		// input string stream
		istringstream iss(str);

		// Final Result
		vector<string> result;

		string tmp;
		while (iss >> tmp)
		{
			result.push_back(tmp);
		}

		return result;
	}

	void TestStringPartition2()
	{
		string str = "One,Two,Three,Four,Five";
		vector<string> strVec = std::move(StringPartition2(str, ','));

		for (auto& s : strVec)
		{
			cout << s << ' ';
		}
		cout << endl;
	}


	int main()
	{
		TestStringPartition1();
		TestStringPartition2();

		return 0;
	}
}

namespace PlayCard
{
	unordered_map<string, int> PokerOrderMap{ {"3", 0}, {"4", 1},
		{"5", 2}, {"6", 3}, {"7", 4},{"8", 5}, {"9", 6}, {"10", 7},
		{"J", 8}, {"Q", 9}, {"K", 10},{"A", 11}, {"2", 12} };


	enum PokerType
	{
		SingleCard = 0,
		DoubleCard,
		ThreeCard,
		SequenceCard,
		BombCard,
		JokerBombCard,
		UNKNOWNCARD,
	};


	enum RESULTTYPE
	{
		POKERWIN = 0,
		POKERLOSE,
		POKERERROR,
	};

	void PartitionJoker(const string& str, string& player1, string& player2)
	{
		string::size_type found = str.find('-');
		player1 = str.substr(0, found);
		player2 = str.substr(found + 1);
	}


	PokerType GetPokerType(const string& strPoker, vector<string>& strVecPoker)
	{
		istringstream iStream(strPoker);
		string singlePokerCard;

		while (iStream >> singlePokerCard)
		{
			strVecPoker.push_back(singlePokerCard);
		}

		int n = strVecPoker.size();
		switch (n)
		{
		case 1:
			return SingleCard;
		case 2:
			if (strVecPoker[0] == "joker" || strVecPoker[1] == "joker")
			{
				return JokerBombCard;
			}
			else
			{
				return DoubleCard;
			}
		case 3:
			return ThreeCard;
		case 4:
			return BombCard;
		default:
			return SequenceCard;
		}

		return UNKNOWNCARD;
	}


	RESULTTYPE CompareTwoPlayers(string& player1, string& player2)
	{
		vector<string> player1CardVec;
		vector<string> player2CardVec;

		PokerType typePlayer1 = GetPokerType(player1, player1CardVec);
		PokerType typePlayer2 = GetPokerType(player2, player2CardVec);

		// cout << typePlayer1 << ' ' << typePlayer2 << endl;

		if (typePlayer1 == JokerBombCard)
		{
			return POKERWIN;
		}
		else if (typePlayer2 == JokerBombCard)
		{
			return POKERLOSE;
		}

		if (typePlayer1 == BombCard || typePlayer2 == BombCard)
		{
			if (typePlayer1 == BombCard && typePlayer2 == BombCard)
			{
				return PokerOrderMap[player1CardVec[0]] > PokerOrderMap[player2CardVec[0]] ? POKERWIN : POKERLOSE;
			}
			else if (typePlayer1 == BombCard && typePlayer2 != BombCard)
			{
				return POKERWIN;
			}
			else
			{
				return POKERLOSE;
			}
		}
		else
		{
			if (typePlayer1 != typePlayer2)
			{
				return POKERERROR;
			}
			else
			{
				return PokerOrderMap[player1CardVec[0]] > PokerOrderMap[player2CardVec[0]] ? POKERWIN : POKERLOSE;
			}
		}
	}

	int main()
	{
		string strLineInput;
		while (getline(cin, strLineInput))
		{
			string player1;
			string player2;
			PartitionJoker(strLineInput, player1, player2);

			RESULTTYPE result = CompareTwoPlayers(player1, player2);

			// cout << result << endl;

			switch (result)
			{
			case POKERWIN:
				cout << player1 << endl;
				break;
			case POKERLOSE:
				cout << player2 << endl;
				break;
			case POKERERROR:
				cout << "ERROR" << endl;
				break;
			}
		}

		return 0;
	}
}

// 
/*
* 100101000001001010000100101
* 100101 00000100101 0000100101
*/
namespace BitsStream
{
	bool CompareBits(string& bitsString1, string& bitsString2)
	{
		int i = 0, j = 0;
		bool flag = true;

		while (i < bitsString1.size() && bitsString1[i] == '0')
		{
			i++;
		}

		while (j < bitsString2.size() && bitsString2[j] == '0')
		{
			j++;
		}

		if ((bitsString1.size() - i) != (bitsString2.size() - j))
		{
			return false;
		}

		while (i < bitsString1.size() && j < bitsString2.size())
		{
			if (bitsString1[i] != bitsString2[j])
			{
				flag = false;
				break;
			}
			i++;
			j++;
		}

		return flag;

	}

	int _BitsStream(string& str)
	{

		return 0;
	}

	int main()
	{
		string str1 = "00001111010001";
		string str2 = "111101000";

		cout << (CompareBits(str1, str2) ? "Equal" : "Unequal") << endl;

		return 0;
	}
}

namespace IteratorTraits
{
	// For every iterator type, a corresponding specialization of iterator_traits class template shall be 
	// defined, with at least the following member types defined :
	// difference_type | value_type | pointer | reference | iterator_category 
	// iterator_category includes follows : input_iterator_tag | output_iterator_tag | forward_iterator_tag | bidirectional_iterator_tag | random_access_iterator
	int main()
	{
		using traits = std::iterator_traits<int*>;

		if (typeid(traits::iterator_category) == typeid(std::random_access_iterator_tag))
		{
			cout << "int * is a random-access iterator";
		}

		return 0;
	}
}

namespace Hex2Base64
{
	string _hex2bits(char s)
	{
		int num;

		if ('0' <= s && s <= '9'){
			num = s - '0';
		} else{
			num = s - 'A' + 10;
		}

		string str;

		for (int i = 0; i < 8; i++)
		{
			int mod = num % 2;
			str.push_back('0' + mod);
			num = num / 2;
		}

		reverse(str.begin(), str.end());

		return str;
	}

	int _bits2num(string& str)
	{
		int result = 0;
		int base = 1;

		for (int i = str.length()-1; i >= 0; i--) {
			if (str[i] == '1'){
				result += base;
			}
			base *= 2;
		}

		return result;
	}

	char _num2char(int num)
	{
		assert(num >= 0 && num <= 63);

		if (0 <= num && num <= 25)
		{
			return 'A' + (num - 0);
		}
		else if (26 <= num && num <= 51)
		{
			return 'a' + (num - 26);
		}
		else if (52 <= num && num <= 61)
		{
			return '0' + (num - 52);
		}
		else if (num == 62)
		{
			return '+';
		}
		else
		{
			return '//';
		}
	}

	void hex2base64(string& str)
	{
		transform(str.begin(), str.end(), str.begin(), ::toupper);

		int mod = str.length() % 3;

		mod = mod == 0 ? -1 : 3 - mod;

		string tmp;

		for (int i = 0; i < str.length(); i++){
			tmp += _hex2bits(str[i]);
		}

		for (int i = 0; i+6 <= tmp.length(); i = i+6)
		{
			string base64bits = tmp.substr(i, 6);
			int num = _bits2num(base64bits);
			cout << _num2char(num);
		}

		for (int i = 0; i < mod+1; i++)
		{
			cout << '=';
		}
	}

	int main()
	{
		string test1 = "1A";
		hex2base64(test1);
		return 0;
	}
}

int main()
{
	//Permutations::main();
	//FixedLenSet::main();
	// BFS::main();
	//DFS::main();
	//MonoStack::main();
	//stringManipulation::main();
	//AllSubSet::main();
	//BitsStream::main();
	//MinimumSpanningTree::main();
	//ShortestPath::main();
	//IteratorTraits::main();
	Hex2Base64::main();
	return 0;

}
