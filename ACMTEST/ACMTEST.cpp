#include <bits/stdc++.h>
using namespace std;

namespace LRVALUE
{
	void overloaded(const int&);
	void overloaded(int&&);

	// 下面三个函数展示了左值右值引用设计的常用方法
	// 第一个函数通常是中间函数，用于完美转发
	// 转发的目的地就是第二个或者第三个函数，分别对用于左值引用的函数和右值引用的函数
	template <class T> void fn(T&& x) {
		overloaded(x);                   // always an lvalue
		overloaded(std::forward<T>(x));  // rvalue if argument is rvalue
	}

	void overloaded(const int& x) { std::cout << "[lvalue]"; }
	void overloaded(int&& x) { std::cout << "[rvalue]"; }

	int main() {
		// Effective Modern C++ 给出的判断一个表达式是否为左值的方法是：
		// 看能否取得此表达式的地址：如果可以取得，则该表达式是左值；如果不可以，该表达式是右值。
		int a;

		std::cout << "calling fn with lvalue: ";
		fn(a);
		std::cout << '\n';

		std::cout << "calling fn with rvalue: ";
		fn(0);
		std::cout << '\n';

		return 0;
	}
}

namespace AUTOTEST
{
	// More Effective C++ 提供的用编译器报错来推导类型的方法
	//template<typename T>
	//class TD;

	//void TypeTest()
	//{
	//	const int theAnswer = 42;
	//	auto x = theAnswer;
	//	auto y = &theAnswer;

	//	TD<decltype(x)> xType;
	//	TD<decltype(y)> yType;
	//}

	/*
	* 理解模板型别推导
	* 在 <Effective Modern C++> 一书中，作者以如下模板形式作为示例
	* 
	* template<typename T>
	* void f(ParamType param);
	* f(expr);
	* 
	* 根据ParamType的类型，将模板型别推导分成了三种情况，分别是：
	* 1. ParamType 具有指针或引用类型，但不是万能引用
	* 2. ParamType 是一个万能引用
	* 3. ParamType 既非指针也非引用
	* 对于上述三种情况，规则分别如下：
	* 1. 若 expr 具有引用类型，则先将其引用部分忽略；尔后， 对 expr 的型别和ParamType的型别执行模式匹配，来决定
	*    T的型别；在此类情况下，expr的常量性将会得以保持
	* 2. 如果 expr 是一个左值，则 T 和 ParamType 会被推导为左值引用； 如果 expr 是一个右值，则应用常规推导规则
	* 3. 这种情况下ParamType既非指针，也非引用，也就是按值传递，此时将应用如下规则：A. 若 expr 具有引用类型，则
	*    忽略其引用部分；忽略 expr 的顶层的 const 和 volatile， 保留其底层的 const 和 volatile
	* 
	* 对于数组和函数，还需要关注其指针退化问题：
	* const char name[] = "J. P. Briggs";
	* 
	* 对于按值传递的模板来说，T 的推导类型为 const char *
	* template <typename T> void f(T param); f(name);
	* 
	* 对于按引用传递的模板来说，T 的推导类型为 const char (&)[13]
	* template <typename T> void f(T& param); f(name);
	* 
	* 所以有了下面的一个求数组长度的模板函数
	*/

	template <typename T, std::size_t N>
	constexpr std::size_t arraySize(T(&)[N]) noexcept
	{
		return N;
	}

	int main()
	{
		int keyVals[] = { 1, 3, 7, 9, 11, 22, 35 };

		cout << "The size of keyVals is " << arraySize(keyVals);

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

namespace CopyTest
{
	class Foo
	{
	public:
		Foo() { cout << "Constructor was called!" << endl; }

		Foo(const Foo&) { cout << "Copy Constructor was called!" << endl; }
		Foo& operator==(const Foo&) { cout << "Assignment operator was called!" << endl; }
		
		Foo(Foo&&) { cout << "Move Constructor was called!" << endl; }
		Foo& operator==(Foo&&) { cout << "Move assignment operator!" << endl; }
	
		// 统计构造函数调用的次数是统计对象构造总次数的有效方法（因为对象构造不止一种途径）
		~Foo() { cout << "Destructor was called!" << endl; } 
	};

	Foo copyTest1()
	{
		Foo f1;

		return f1;
	}

	Foo copyTest2()
	{
		Foo f2;

		return std::move(f2);
	}

	int main()
	{
		// 编译器已经进行优化
		{
			cout << endl << "**********情况1**************" << endl;
			Foo f1 = copyTest1(); // 进行 一次构造 和 一次移动构造
		}

		{
			cout << endl << "**********情况2**************" << endl;
			Foo f2 = copyTest2(); // 进行 一次构造 和 一次移动构造
		}


		// 编译器反而不能对下述语句进行优化
		{
			cout << endl << "**********情况3**************" << endl;
			Foo f3 = std::move(copyTest1()); // 进行 一次构造 和 两次移动构造
		}

		{
			cout << endl << "**********情况4**************" << endl;
			Foo f4 = std::move(copyTest2()); // 进行一次构造 和 两次移动构造
		}

		return 0;
	}
}

namespace RandomCPP
{
	void CRandomTest()
	{
		// Initialize the random seed
		srand(time(nullptr));

		// range of rand()
		cout << "rand() function generate number from 0" << " to " << RAND_MAX << endl;

		// generate number between a and b
		int a = 4;
		int b = 10;
		int c = a + rand() % (b - a + 1);

		cout << "We generate " << c << endl;
	}


	void CPPRandomTest(){
		using my_engine = default_random_engine; // type of engine
		//即使所有的模板参数都有默认值，我们仍然需要显式的加上<>，来表示这是一个类模板
		using my_distribution = uniform_int_distribution<>; // type of distribution

		constexpr int min = 0;
		constexpr int max = 99;

		// time_point -> duration -> count
		unsigned seed = chrono::system_clock::now().time_since_epoch().count(); // Initialization Seed

		my_engine re(seed); // instantiate engine with seed
		my_distribution zero_to_five(min, max); // distribution that map to the ints 1..6

		auto die = [&]() { return zero_to_five(re); }; // make a generator

		// Display the result
		vector<int> result(max-min+1, 0);

		for (int i = 0; i < 1000; i++) {
			result[die()]++;
		}

		auto printStar = [](int num) {
			for (int i = 0; i < num; i++)
			{
				cout << '*';
			}
		};

		for (int i = 0; i < result.size(); i++) {
			printStar(result[i]);
			cout << endl;
		}
	}



	int main()
	{
		//CPPRandomTest();
		//CRandomTest();

		return 0;
	}
}

namespace ChronoCPP
{
	// 对于任何的时间库，都有两个类型：time_point类型用于标识某一时刻的时间；duration类型，用于表示一段时间，两者之间可以进行转换
	using namespace chrono;

	void prog1() {

		// time_point的类型是和 clock 捆绑的，而 duration 的类型由 rep 和 period 决定

		// clock to time_point
		time_point<high_resolution_clock> t0 = high_resolution_clock::now();

		this_thread::sleep_for(seconds(3));

		// clock to time_point
		time_point<high_resolution_clock> t1 = high_resolution_clock::now();

		// time_point to duration
		duration<high_resolution_clock::rep, high_resolution_clock::period> d = t1 - t0;

		// cast to seconds
		seconds s = duration_cast<seconds>(t1 - t0);

		cout << "We spend " << s.count() << " seconds" << endl;
	}

	void prog2() {
		// the number of seconds since 00:00 hours, Jan 1, 1970 UTC
		cout << time(nullptr) << endl;

		// the number of nano seconds since 00:00 hours, Jan 1, 1970 UTC
		duration<system_clock::rep, system_clock::period> d = chrono::system_clock::now().time_since_epoch();
		
		// cast the result to seconds
		seconds dinSecs = duration_cast<seconds>(d);

		cout << dinSecs.count() << endl;
	}

	int main()
	{
		//prog1();
		prog2();
		return 0;
	}
}

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
	/*
	* 6 6 2 3 3
	* 37 37 39 41 13 205
	* 37 41 41 203 39 243
	* 37 41 40 131 40 41
	* 91 41 39 198 41 9
	* 189 41 39 40 40 38
	* 37 124 38 167 41 41
	*/


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

				// 凡是涉及到数组的索引一定要注意不要超出数组的范围
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

namespace BFSEXAMPLE
{
	// 定义我们所需的数据类型，一般包含节点的所有信息。
	struct state_t
	{
		string word;
		int level;

		// 最好定义默认构造函数，尽管用不到。
		state_t() = default;
		state_t(string _word, int _level) : word(_word), level(_level) { }

		// 一定要定义 operator== 运算符，hashtable 取得相应元素需要 operator== 验证是否相等
		bool operator==(const state_t& other) const
		{
			return word == other.word;
		}
	};

	// 为我们自定义的类型创建 hash 函数
	struct stateHash
	{
	private:
		std::hash<string> str_hash;

	public:
		size_t operator()(const state_t& state) const
		{
			return str_hash(state.word);
		}
	};

	unordered_set<state_t, stateHash> ExtendChoice(const state_t& seed, const string& end, const unordered_set<string>& dict, unordered_set<state_t, stateHash>& visited, unordered_map<string, string>& mapParent)
	{
		unordered_set<state_t, stateHash> result;

		string strSeed = seed.word;

		for (size_t i = 0; i < strSeed.length(); i++)
		{
			for (char c = 'a'; c <= 'z'; c++)
			{
				if (c == strSeed[i])
				{
					continue;
				}

				swap(strSeed[i], c);

				state_t tmpState(strSeed, seed.level + 1);

				if ((dict.find(strSeed) != dict.end() || strSeed == end) && visited.find(tmpState) == visited.end())
				{
					result.insert(tmpState);
					mapParent[strSeed] = seed.word;
				}

				swap(strSeed[i], c);
			}
		}

		return result;
	}

	// 通过父子表（通常可以用数组进行记录，但在非数字节点中，需要通过 map 进行记录）， 寻找回去的路。
	void FindPath(unordered_map<string, string>& mapParent, const string& strToFind, const string& begin)
	{
		if (strToFind != begin)
		{
			string strParent = mapParent[strToFind];
			FindPath(mapParent, strParent, begin);
		}

		cout << strToFind << ' ';
	}

	// BFS
	int WordLadder(const string& start, const string& end, const unordered_set<string>& dict)
	{
		queue<state_t> queueSearch;
		unordered_set<state_t, stateHash> visited; // 第一个参数为`自定义数据类型`，第二个参数定义为`自定义数据类型的 hash function`
		unordered_map<string, string> mapParent;

		queueSearch.emplace(start, 0);
		visited.emplace(start, 0);

		auto state_is_target = [&](const state_t& state) { return state.word == end; };

		while (!queueSearch.empty())
		{
			// 取节点
			const state_t tmp = queueSearch.front();
			queueSearch.pop();

			// 如果遍历到了终点，记录并返回。
			if (state_is_target(tmp))
			{
				FindPath(mapParent, tmp.word, start);
				return tmp.level + 1;
			}

			// 扩展结点
			unordered_set<state_t, stateHash> setPathChoices = ExtendChoice(tmp, end, dict, visited, mapParent);

			for (const auto& s : setPathChoices)
			{
				queueSearch.push(s);
				visited.insert(s);
			}
		}

		return 0;
	}

	void UnitTestWordLadder()
	{
		string start = "hit";
		string end = "cog";
		unordered_set<string> dict{ "hot", "dot", "dog", "lot", "log" };

		int ret = WordLadder(start, end, dict);

		cout << endl << "From " << start << " to " << end << " needs " << ret << " steps" << endl;
	}

	int main()
	{
		UnitTestWordLadder();
		return 0;
	}
}

namespace RECURSIVEWAY
{
	// 二叉树的构建与递归遍历
	struct TreeNode
	{
		int val;
		TreeNode* left;
		TreeNode* right;

		TreeNode(int v):val(v), left(nullptr), right(nullptr){}
		TreeNode(int v, TreeNode* l, TreeNode* r) : val(v), left(l), right(r) {}
	};

	
	TreeNode* ReadTree()
	{
		string strRoot;
		cin >> strRoot;

		if (strRoot == "!")
		{
			return nullptr;
		}

		int num = stoi(strRoot);
		TreeNode* root = new TreeNode(num);

		queue<TreeNode*> q;
		q.push(root);

		while (!q.empty())
		{
			TreeNode* tmp = q.front();
			q.pop();

			string l;
			cin >> l;
			if (l == "!")
			{
				break;
			}

			string r;
			cin >> r;
			if (r == "!")
			{
				break;
			}

			if (l != "None")
			{
				int lnum = stoi(l);
				tmp->left = new TreeNode(lnum);
				q.push(tmp->left);
			}

			if (r != "None")
			{
				int rnum = stoi(r);
				tmp->right = new TreeNode(rnum);
				q.push(tmp->right);
			}
		}
		
		return root;
	}

	void PrintTree(TreeNode* root)
	{
		if (root == nullptr)
		{
			cout << "空树！" << endl;
		}

		queue<TreeNode*> qcurrent, qnext;
		qcurrent.push(root);

		while (!qcurrent.empty())
		{
			while (!qcurrent.empty())
			{
				TreeNode* tmp = qcurrent.front();
				qcurrent.pop();

				if (tmp != nullptr)
				{
					cout << tmp->val << ' ';
					qnext.push(tmp->left);
					qnext.push(tmp->right);
				}
				else
				{
					cout << "None" << ' ';
				}				
			}
			
			cout << endl;

			qnext.swap(qcurrent);	
		}
	}

	// 这是一道典型的用递归解决问题的题目：根据参数决定终止条件，根据返回值的判断以及加工决定最终的返回结果。
	// 二叉树是回溯算法的一种简单情况，因为在任何时刻，其只有两种情况：左子节点或者右子节点，然后对这两种情况综合分析
	// 最终决定当前递归深度的返回值。
	int MinimumDepth(TreeNode* root)
	{
		if (root == nullptr)
		{
			return 0;
		}
		if (root->left == nullptr && root->right == nullptr)
		{
			return 1;
		}

		int minDep = INT_MAX;

		if (root->left)
		{
			int tmp = MinimumDepth(root->left);
			minDep = minDep < tmp ? minDep : tmp;
		}

		if (root->right)
		{
			int tmp = MinimumDepth(root->right);
			minDep = minDep < tmp ? minDep : tmp;
		}

		return minDep + 1;
	}


	void TestTree()
	{
		TreeNode* root = ReadTree();
		PrintTree(root);

		cout << "The minimum depth of tree is : " << MinimumDepth(root) << endl;

	}

	// 本质：多叉树的递归问题，在递归不断嵌套的时候可以不断的通过参数传递一些信息
	//       在递归不断被解开的时候通过返回值的加工不断向上传递结果。
	//       但是，在树的扩展过程中，我们有可能遇到一模一样的节点，这是，继续进行重复的扩展与递归
	//       会极大的增加复杂度，这时我们可以用记忆化的方法进行优化。
	//       二叉树中并不存在此类问题，因为其每个节点都不重复(尽管值有可能一样，但是结构也会不同，至少我们不能
	//        有节点相同的预设，即使他们节点真的相同)。


	// 题目一：
	// 小红拿到两个正整数a和b，他每次操作可以选择其中一个正整数，删除一个数位。
	// 例如，对于 "1, 2, 3, 4" 而言，进行一次操作可以生成 "124"|"123"|"143"或"243"
	// 小红希望最终a是b的倍数或者b是a的倍数，他想知道自己最少的操作
	// 次数是多少？

	// 如果将字符串当成对象，将每次减少一个字符作为操作的话，我们可以得到如下的递归树：
	//                                         (A, B)
	//                     (A-1, B)                                  (A, B-1)
	//               (A-2, B)    **(A-1, B-1)**            **(A-1, B-1)**   (A, B-2)
	// 标记**的地方出现了重复递归，造成复杂度过大，因此我们需要记忆化来减少递归的复杂度。
	struct pairHash
	{
	private:
		std::hash<string> str_hash;

	public:
		size_t operator()(const pair<string, string>& param) const
		{
			return str_hash(param.first + param.second);
		}
	};

	unordered_map<pair<string, string>, int, pairHash> RecordMap;

	int MinOps(string str1, string str2)
	{
		if (str1.empty() || str2.empty())
		{
			return INT_MAX;
		}

		pair<string, string> tmpPair1(str1, str2);
		pair<string, string> tmpPair2(str2, str1);
		if (RecordMap.find(tmpPair1) != RecordMap.end() || RecordMap.find(tmpPair2) != RecordMap.end())
		{
			return RecordMap[tmpPair1];
		}

		int num1 = stoi(str1);
		int num2 = stoi(str2);

		if (num1 % num2 == 0 || num2 % num1 == 0)
		{
			return 0;
		}

		int minOfstr = INT_MAX;

		for (int i = 0; i < str1.length(); i++)
		{
			string tmp = str1;
			tmp.erase(tmp.begin() + i);
			int ret = MinOps(tmp, str2);
			minOfstr = minOfstr < ret ? minOfstr : ret;
		}

		for (int i = 0; i < str2.length(); i++)
		{
			string tmp = str2;
			tmp.erase(tmp.begin() + i);
			int ret = MinOps(str1, tmp);
			minOfstr = minOfstr < ret ? minOfstr : ret;
		}

		int result = minOfstr == INT_MAX ? INT_MAX : minOfstr + 1;

		RecordMap[make_pair(str1, str2)] = result;
		RecordMap[make_pair(str2, str1)] = result;

		return result;
	}

	void testMinOps()
	{
		string str1 = "3712";
		string str2 = "8";

		cout << "We at least need " << MinOps(str1, str2) << " steps!";
	}


	// 题目二：
	// 给你一个字符串和一个字符串银行（包含可用字符串），问是否可以用可用字符串构造出字符串。

	// 如果我们将最终需要构建的字符串对象记为A， 将字符串银行记为(W1, W2, W3, W4, ...)
	// 我们递归的过程可以表示成如下形式：
	//                                                 A
	//                     A-Wi1                     A-Wj1                    A-Wk1
	//             A-Wi11       A-Wi12              A-Wj11           A-Wk11   A-Wk12     A-Wk13
	// 在有些情况下，不同的节点是有可能重复的，这就有可能造成重复和不必要的递归，因此我们也需要使用记忆化的方式改善之。

	unordered_map<string, bool> canConstructMap;

	bool CanConstruct(string targetStr, vector<string>& wordBank)
	{
		if (targetStr.empty())
		{
			return true;
		}

		if (canConstructMap.find(targetStr) != canConstructMap.end())
		{
			return canConstructMap[targetStr];
		}

		for (auto& s : wordBank)
		{
			// 注意，下面其实涉及了字符串的较为高级的操作：如何寻找某个字符串并将其分割出去？
			string::size_type found = targetStr.find(s);

			if (found == 0)
			{
				string tmp = targetStr.substr(s.size());
				bool ret = CanConstruct(tmp, wordBank);

				if (ret)
				{
					canConstructMap[targetStr] = true;
					return true;
				}
			}
		}

		canConstructMap[targetStr] = false;

		return false;
	}

	void TestCanConstruct()
	{
		string str = "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeef";
		vector<string> strVec{ "e", "ee", "eee", "eeee" };

		int ret = CanConstruct(str, strVec);

		cout << (ret ? "True" : "False") << endl;

	}

	// Coin Change Ⅰ : Recursive Way




	// Coin Change Ⅱ : Recursive Way



	int main()
	{
		//testMinOps();
		//TestCanConstruct();
		TestTree();
		return 0;
	}
}

namespace DynamicProgramming
{
	// 最长递增子序列
	pair<int, int> lengthOfLIS(vector<int>& nums, vector<int>& parentTable)
	{
		vector<int> dp(nums.size(), 1);

		for (int i = 1; i < nums.size(); i++)
		{
			int maxval = INT_MIN;
			for (int j = i - 1; j >= 0; j--)
			{
				if (nums[i] > nums[j])
				{
					if (maxval < dp[j] + 1)
					{
						maxval = dp[j] + 1;
						dp[i] = maxval;
						parentTable[i] = j;
					}
				}
			}
		}

		int maxval = INT_MIN;
		int index = -1;
		for (int i = 0; i < nums.size(); i++)
		{
			if (dp[i] > maxval)
			{
				maxval = dp[i];
				index = i;
			}
		}

		return make_pair(maxval, index);
	}

	void FindPath(vector<int>& nums, vector<int>& parentTable, int index)
	{
		if (index == -1)
		{
			return;
		}
		else
		{
			FindPath(nums, parentTable, parentTable[index]);
			cout << nums[index] << ' ';
		}
	}

	void lengthOfLISTestCase()
	{
		vector<int> nums{ 10, 9, 2, 5, 3, 7, 101, 18 };
		vector<int> parentTable(nums.size(), -1);

		pair<int, int> result = lengthOfLIS(nums, parentTable);

		cout << "The longest regid increment sequence length is : " << result.first << endl;
		FindPath(nums, parentTable, result.second);
	}

	// 零钱兑换[排列]
	// 给定某一面值的纸币和可供兑换的零钱面值，求最少兑换的硬币数。
	int coinChange(vector<int>& coins, int amount)
	{
		vector<int> dp(amount + 1, -1);

		dp[0] = 0;
		for (int i = 1; i <= amount; i++)
		{
			int minval = INT_MAX;
			for (int j = 0; j < coins.size(); j++)
			{
				int tmp = i - coins[j];

				if (tmp >= 0 && dp[tmp] != -1 && minval > dp[tmp] + 1)
				{
					minval = dp[tmp] + 1;
					dp[i] = minval;
				}
			}
		}

		return dp[amount];
	}


	void coinChangeTest()
	{
		vector<int> coins{ 1, 2, 5 };
		int amount = 11;

		cout << "To change " << amount << " $, " << "we at least get " << coinChange(coins, amount) << " coins" << endl;
	}

	// 零钱兑换2[组合问题]
	// 给定一定面值的纸币和可供兑换的硬币面值，给出兑换方式的种类数
	// 上述做法不会重复计算不同的排列。因为外层循环是遍历数组 coins 的值，内层循环是遍历不同的金额之和，在计算 dp[i] 的值时，
	// 可以确保金额之和等于 i 的硬币面额的顺序，**由于顺序确定，因此不会重复计算不同的排列**。
	//	3 = 1 + 1 + 1
	//	3 = 1 + 2
	// 硬币面额 2 不可能出现在硬币面额 1 之前，即不会重复计算 3 = 2 + 1 和 3 = 1 + 2 的情况。
	int coinChange2(vector<int>& coins, int amount)
	{
		vector<int> dp(amount + 1);
		dp[0] = 1;

		// 先遍历球
		for (int coin : coins) {
			for (int i = coin; i <= amount; i++) {
				dp[i] += dp[i - coin];
			}
		}
		return dp[amount];
	}


	void coinChange2Test()
	{
		vector<int> coins{ 1, 2, 5 };
		int amount = 11;

		cout << "To change " << amount << " $, " << "we have " << coinChange2(coins, amount) << " different ways" << endl;
	}


	int main()
	{
		//lengthOfLISTestCase();
		//coinChangeTest();
		coinChange2Test();
		return 0;
	}
}

namespace MinimumSpanningTree
{
	// Graph Data 
	//  0 1 5
	//	0 2 9
	//	0 3 7
	//	1 2 7
	//	1 6 12
	//	2 3 4
	//	2 5 3
	//	2 6 4
	//	3 4 5
	//	3 5 2
	//	4 5 2
	//	5 6 7

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

namespace BitsStream
{
	// 
	/*
	* 100101000001001010000100101
	* 100101 00000100101 0000100101
	*/

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
			int mod = num & 1;
			str.push_back('0' + mod);
			num = num >> 1;
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
			return '/';
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

namespace SortAlgorithm
{
	// QuickSort Algorithm
	int Partition(vector<int>& vec, int l, int r, int randomPoint)
	{
		int cmpVal = vec[randomPoint];
		swap(vec[r], vec[randomPoint]);

		// firstHigh 应该从 l 开始，而非从 0 开始
		int firstHigh = l;

		for (int i = l; i < r; i++)
		{
			if (vec[i] < cmpVal)
			{
				swap(vec[i], vec[firstHigh++]);
			}
		}

		swap(vec[firstHigh], vec[r]);

		return firstHigh;
	}

	void _quicksort(vector<int>& vec, int l, int r){
		if (l >= r )
		{
			return;
		}

		int randomPoint = l + rand() % (r - l + 1);

		int partitionPoint = Partition(vec, l, r, randomPoint);

		_quicksort(vec, l, partitionPoint - 1);
		_quicksort(vec, partitionPoint + 1, r);
	}

	void QuickSort(vector<int>& vec, int l, int r) {
		srand(time(nullptr));

		_quicksort(vec, l, r-1);
	}

	void TestQuickSort()
	{
		ofstream ofs("quicksorttest.txt", ofstream::out);

		for (int i = 0; i <= 1000000; i = i + 10000)
		{
			vector<int> vec(i);

			for (int j = 0; j < vec.size(); j++)
			{
				vec[j] = rand();
			}

			auto start = chrono::high_resolution_clock::now();

			QuickSort(vec, 0, vec.size());

			auto interval = chrono::high_resolution_clock::now() - start;

			auto secs = chrono::duration_cast<chrono::milliseconds>(interval);

			ofs << secs.count() << ',';
		}

		ofs.close();
	}

	int main()
	{
		//TestQuickSort();


		return 0;
	}
}

namespace MySingleton {

	class Singleton {
	private:
		static std::atomic<Singleton*> _instance;
		static std::mutex _mutex;
		Singleton(string objflag) :objectIndicator_(objflag) {};

	public:
		string objectIndicator_;

	public:
		Singleton(const Singleton&) = delete;
		Singleton& operator==(const Singleton&) = delete;

		static Singleton* GetInstance(string val);
	};

	std::atomic<Singleton*> Singleton::_instance;
	std::mutex Singleton::_mutex;

	Singleton* Singleton::GetInstance(string val) {
		Singleton* tmp = _instance.load(memory_order::memory_order_acquire);

		if (tmp == nullptr)
		{
			std::lock_guard<std::mutex> lk(_mutex);
			
			tmp = _instance;

			if (tmp == nullptr)
			{
				tmp = new Singleton(val);
				_instance.store(tmp, memory_order::memory_order_release);
			}
		}

		return tmp;
	}

	void threadFoo(string value)
	{
		this_thread::sleep_for(chrono::milliseconds(1000));
		Singleton* val = Singleton::GetInstance(value);
		cout << val->objectIndicator_ << endl;
	}


	int main()
	{
		std::thread t1(threadFoo, "ABC");
		std::thread t2(threadFoo, "DEF");
		std::thread t3(threadFoo, "GHI");

		t1.join();
		t2.join();
		t3.join();

		return 0;
	}
}

namespace PrintLetterSequentially
{
	std::mutex lock;
	std::condition_variable cond;
	volatile int curid = 0;
	const int gcount = 100;
	const int range = 10;
	void PrintLetter(int id)
	{
		for (int i = 0; i < gcount; i++ )
		{
			std::unique_lock<std::mutex> lck(lock);
			while (id != curid)
			{
				cond.wait(lck);
			}
			cout << char('A' + id);

			curid = (curid + 1) % range;

			cond.notify_all();
		}
	}

	int main()
	{
		std::vector<std::thread> threads;

		for (int i = 0; i < range; i++)
		{
			threads.push_back(std::thread(PrintLetter, i));
		}

		for (auto& t : threads)
		{
			t.join();
		}

		return 0;
	}
}

namespace PascalTriangle
{
	vector<vector<int>> DynamicWay(int numRows) {
		// 为向量分配内存
		vector<vector<int>> result(numRows);

		for (int i = 0; i < numRows; i++)
		{
			result[i].resize(i + 1);
		}

		// 初始化向量
		for (int i = 0; i < numRows; i++)
		{
			result[i][0] = 1;
			result[i][i] = 1;
		}

		//递推
		for (int i = 1; i < numRows; i++)
		{
			for (int j = 1; j < i; j++)
			{
				result[i][j] = result[i - 1][j - 1] + result[i - 1][j];
			}
		}
		// 打印结果
		for (auto& v : result)
		{
			for (auto i : v)
			{
				cout << i << ' ';
			}
			cout << endl;
		}


		return result;
	}

	int _RecursiveWay(vector<vector<int>>& result,int i, int j) {
		if (j <= 0 || i <= 0 || i <= j)
		{
			return 1;
		}
		else if (result[i][j] != -1)
		{
			return result[i][j];
		}
		else
		{
			return result[i - 1][j - 1] + result[i - 1][j];
		}

	}

	vector<vector<int>> RecursiveWay(int numRows) {
		vector<vector<int>> result(numRows);

		for (int i = 0; i < numRows; i++)
		{
			result[i].resize(i + 1, -1);
		}

		for (int i = 0; i < numRows; i++)
		{
			for (int j = 0; j <= i; j++)
			{
				if (result[i][j] != -1)
				{
					continue;
				}
				else
				{
					result[i][j] = _RecursiveWay(result, i, j);
				}
			}
		}

		for (auto& v : result)
		{
			for (auto i : v)
			{
				cout << i << ' ';
			}
			cout << endl;
		}

		return result;
	}

	int main()
	{
		int numRows = 5;

		RecursiveWay(numRows);

		return 0;
	}
}

namespace FindMaxValEveryK
{
	vector<int> FindMaxEveryK(vector<int>& nums, int k)
	{
		priority_queue<pair<int, int> > pq;
		vector<int> result;

		for (int i = 0; i < k; i++)
		{
			pq.emplace(nums[i], i);
		}

		result.push_back(pq.top().first);

		for (int i = k; i < nums.size(); i++)
		{
			pq.emplace(nums[i], i);

			while (i - pq.top().second >= k)
			{
				pq.pop();
			}

			result.push_back(pq.top().first);
		}

		return result;
	}

	void testcase()
	{
		const int count = 20;
		vector<int> nums(count);

		uniform_int_distribution<> distri(0, 1000);
		default_random_engine generator;

		auto f = [&]() { return distri(generator); };

		generate(nums.begin(), nums.end(), f);
		
		for (auto i : nums)
		{
			cout << i << ' ';
		}
		cout << endl;

		random_shuffle(nums.begin(), nums.end());

		for (auto i : nums)
		{
			cout << i << ' ';
		}
		cout << endl;

		vector<int> ret = FindMaxEveryK(nums, 5);

		for (auto i : ret)
		{
			cout << i << ' ';
		}
	}

	int main()
	{
		testcase();
		return 0;
	}
}

namespace FORPRATICE
{
	// Practice makes perfect!!!
	void TestCase()
	{
		
	}

	int main()
	{
		TestCase();

		return 0;
	}
	
}

namespace ArrayTest
{
	void arraytest1(int arr[])
	{
		cout << "Array Test 1 (int arr[]) : " << sizeof(arr) << endl;
	}

	void arraytest2(int* arr)
	{
		cout << "Array Test 2 (int* arr) : " << sizeof(arr) << endl;
	}

	int main()
	{
		int a[] = { 1, 2, 3, 4, 5 };
		
		char c[] = "Hello";
		const char* d = "World";
		
		arraytest1(a);
		arraytest2(a);

		cout << sizeof(a) << endl;
		cout << sizeof(c) << endl;
		cout << sizeof(d) << endl;

		return 0;
	}
}

int main()
{
	//Permutations::main();
	//FixedLenSet::main();
	//BFS::main();
	//DFS::main();
	//MonoStack::main();
	//stringManipulation::main();
	//AllSubSet::main();
	//DynamicProgramming::main();
	//BitsStream::main();
	//MinimumSpanningTree::main();
	//ShortestPath::main();
	//IteratorTraits::main();
	//Hex2Base64::main();
	//RandomCPP::main();
	//ChronoCPP::main();
	//BFSEXAMPLE::main();
	//SortAlgorithm::main();
	//MySingleton::main();
	//PrintLetterSequentially::main();
	//PascalTriangle::main();
	//LRVALUE::main();
	//AUTOTEST::main();
	//CopyTest::main();
	//FindMaxValEveryK::main();
	//FORPRATICE::main();
	//ArrayTest::main();
	RECURSIVEWAY::main();
	
	return 0;

}

