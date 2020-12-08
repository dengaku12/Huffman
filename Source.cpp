#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <fstream>
#include <bitset>
using namespace std;

#define EMPTY_STRING ""

struct Node
{
	char ch;
	int freq;
	Node *left, *right;
};

Node* getNode(char ch, int freq, Node* left, Node* right)
{
	Node* node = new Node();
	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;
	return node;
}
// Comparison object used to order the heap
struct comp
{
	bool operator()(Node* l, Node* r)
	{
		// highest priority item has lowest frequency
		return l->freq > r->freq;
	}
};

//check if Huffman Tree contains a single node
bool isLeaf(Node* root) {
	return root->left == nullptr && root->right == nullptr;
}

// Traverse the Huffman Tree and store Huffman Codes in a map.
void encode(Node* root, string str, unordered_map<char, string> &huffmanCode)
{
	if (root == nullptr) {
		return;
	}
	// leaf node is found
	if (isLeaf(root)) {
		huffmanCode[root->ch] = (str != EMPTY_STRING) ? str : "1";
	}
	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}
// Traverse the Huffman Tree and decode the encoded string
void decode(Node* root, int &index, string str)
{
	
	if (root == nullptr) {
		return;
	}
	//leaf node is found
	if (isLeaf(root)) {
		cout << root->ch;
		return;
	}
	index++;
	if (str[index] == '0') {
		decode(root->left, index, str);
	}
	else {
		decode(root->right, index, str);
	}
	
}

// Builds Huffman Tree and decode given input text
void buildHuffmanTree(string text, bool isFile)
{
	// base case: empty string
	if (text == EMPTY_STRING) {
		return;
	}
	// count frequency of each character
	// store it in a map
	unordered_map<char, int> freq;
	for (char ch : text) {
		freq[ch]++;
	}
	// Create a priority queue to store live nodes of
	// Huffman tree;
	priority_queue<Node*, vector<Node*>, comp> pq;
	// Create a leaf node for each character and add it
	// to the priority queue.
	for (auto pair : freq) {
		pq.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}
	// do till there is more than one node in the queue
	while (pq.size() != 1)
	{
		Node *left = pq.top(); pq.pop();
		Node *right = pq.top();    pq.pop();

		int sum = left->freq + right->freq;
		pq.push(getNode('\0', sum, left, right));
	}
	// root stores pointer to root of Huffman Tree
	Node* root = pq.top();
	// Traverse the Huffman Tree and store Huffman Codes
	// in a map. Also print them
	unordered_map<char, string> huffmanCode;
	encode(root, EMPTY_STRING, huffmanCode);

	if (!isFile)
	{
		cout << "Codes:" << "\t\tFrequency:"<< endl;
		for (auto pair : huffmanCode) 
		{
			cout << pair.first << " " << pair.second << "\t\t" << freq[pair.first]<<endl;
		}
		cout << "\nOriginal string: " << text << endl;
		// Print encoded string
		string str;
		for (char ch : text)
		{
			str += huffmanCode[ch];
		}
		cout << "\nEncoded string: " << str << endl;
		cout << "\nDecoded string: ";
		if (isLeaf(root))
		{
			// Special case: For input like a, aa, aaa, etc
			while (root->freq--) 
			{
				cout << root->ch;
			}
		}
		else
		{
			// Traverse the Huffman Tree again and this time
			// decode the encoded string
			int index = -1;
			while (index < (int)str.size() - 1) 
			{
				decode(root, index, str);
			}
		}
		cout << endl;
	}
	else
	{
		//open file to write to
		ofstream enc("encode.txt");
		string str;
		for (char ch : text) 
		{
			str += huffmanCode[ch];
		}
		//split Huffman code output into substrings of 8 bits (bitstream)
		//convert 8 bit substrings into ASCII characters
		int i = 0;
		while (i < str.length())
		{
			bitset<8>set(str.substr(i, i + 7));
			enc << char(set.to_ulong());
			i = i + 8;
		}
		enc.close();
	}
	
}

int main()
{
	string text;
	cout << "Enter a string:" << endl;
	getline(cin, text);
	buildHuffmanTree(text,false);

	ifstream input("input.txt");
	string out((istreambuf_iterator<char>(input)), (istreambuf_iterator<char>()));
	buildHuffmanTree(out,true);
	
	
	input.close();
	system("pause");
	return 0;
}