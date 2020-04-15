#include <iostream>
#include <string>
#include <queue>
#include <memory>
#include <utility>
#include <unordered_map>
#include <vector>



//���������� ����
class pattern_tree {
public:
	pattern_tree();

	friend void search_pattern_name(pattern_tree& bor, const std::string& input);
	friend void search_pattern_phone(pattern_tree& bor, const std::string& input);

private:
	struct node; // forward-���������� ��� IntelliSense
	struct node {
		std::string get_value();
		void get_next_values(std::vector<std::string>& plus_data);
		int index = -1; //������ ��������, �� ������� �������� ����/-1, ���� ������ ����
		pattern_tree* owner; //���, �������� ����������� ����
		char last_char = '\0'; //�����, �� ������� ������� � ������ ����
		node* parent; //��������� �� ������������ ����
		std::unordered_map<char, std::unique_ptr<node>> edges; //���-������� ���� �� ���� (����� �����, ��������� �� ���)
		node* name_phone_number; //������ �� ��������������� ���� � ���� ���/�������
	};

	std::pair<std::unique_ptr<node>, std::unique_ptr<node>> root; //�������� ����

	int size_str = 0; //����� ������ ������
	std::vector<std::string> data_base_numbers; //���� ������ ������� ���������
	std::vector<std::string> data_base_names; //���� ������ ���, �� ������� ������� ���

	void search_pattern(node* root_, std::vector<std::string>& data_base, const std::string& input, int start_index, std::vector<std::string>& next_data);
	void get_data_base(); //��������� ���� ������ ���������
	void build_tree();
	void build_one_tree(node* root_, std::vector<std::string>& data_base);
};


pattern_tree::pattern_tree() {
	root.first.reset(new node);
	root.second.reset(new node);
	root.first->owner = this;
	root.second->owner = this;
	build_tree();
}

void get_name_phone(std::string &input, std::string &input_name, std::string &input_phone) {
	int i = 0;
	for (; i < input.size(); i++) {
		input_phone += input[i];
		if (input[i] == '-') {
			input_phone.resize(input_phone.size() - 1);
			break;
		}
	}
	i++;
	for (; i < input.size(); i++) {
		input_name += input[i];
	}
}

void pattern_tree::get_data_base() {
	std::string input_name = "";
	std::string input_phone = "";
	std::string input = "";
	std::cin >> input;
	while (input != "*") {
		get_name_phone(input, input_name, input_phone);
		data_base_names.push_back(input_name);
		data_base_numbers.push_back(input_phone);
		input_name = "";
		input_phone = "";
	}
}

//������ ����������� ������ ����
void pattern_tree::build_one_tree(node* root_, std::vector<std::string> &data_base){
	//���������� ����������
	char pattern_char = ' '; //���� ����������� ����������� ������/�����
	node* now = root_; //����, � ������� ������ ���������
	//����������
	std::string input; //������ ��� �����
	for (int j = 0; j < data_base.size(); j++) {
		input = data_base[j];
		for (int i = 0; i <= input.size(); i++) {
			pattern_char = input[i];
			if (i == input.size()) {
				now = root_;
				break;
			}
			else {
				if (now->edges.find(pattern_char) == now->edges.end()) {
					now->edges[pattern_char] = std::move(std::unique_ptr<node>(new node));
					now->edges[pattern_char]->parent = now;
					if (i == input.size() - 1) {
						now->edges[pattern_char]->index = j;
					}
					now->edges[pattern_char]->owner = this;
					now->edges[pattern_char]->last_char = pattern_char;
				}
				now = now->edges[pattern_char].get();
			}
		}
	}
}

//������ ������ ���� ��� ��������� � �������
void pattern_tree::build_tree() {
	get_data_base();
	build_one_tree(root.first.get(), data_base_numbers);
	build_one_tree(root.second.get(), data_base_names);
}

//��� ��������� ������� ��������, � ������ ����
std::string pattern_tree::node::get_value()
{
	return (index == -1? "": owner->data_base_numbers[index] + " - " + owner->data_base_names[index]);
}

//��� ��������� ���� ����������� ��������
void pattern_tree::node::get_next_values(std::vector<std::string> &plus_data)
{
	if (index != -1) {
		plus_data.push_back(get_value());	
	}

	for (auto next_node : now->edges) {
		next_node.second->get_next_values(plus_data);
	}
}

//�������� ������� ������
void pattern_tree::search_pattern(node* root_, std::vector<std::string>& data_base, const std::string& input, int start_index, std::vector<std::string>& next_data) {
	for (int i = start_index; i <= input.size(); i++) {
		char pattern_char = ' '; //���� ����������� ����������� ������/�����
		node* now = root_; //����, � ������� ������ ���������
		if (i == input.size()) {
			now->get_next_values(next_data);
			break;
		}
		else {
			pattern_char = input[i];
			if (pattern_char == '*') {
				for (auto next_node : now->edges) {
					search_pattern(next_node.second.get(), data_base, input, i, next_data);
				}
			}
			if (now->edges.find(pattern_char) == now->edges.end()) {
				next_data = {"�� ������� ����������� ������"};
			}
			now = now->edges[pattern_char].get();
		}
	}
}

//������� ������ �������� �� ������ (� ���������� � ���)
void search_pattern_name(pattern_tree& bor, const std::string& input) {
	std::vector<std::string> next_data; //�����
	bor.search_pattern(bor.root.second.get(), bor.data_base_names, input, 0, next_data);
	for (int j = 0; j < next_data.size(); j++) {
		std::cout << next_data[j];
	}
}

//������� ������ �������� �� �����
void search_pattern_phone(pattern_tree& bor, const std::string& input) {
	std::vector<std::string> next_data; //�����
	bor.search_pattern(bor.root.first.get(), bor.data_base_numbers, input, 0, next_data);
	for (int j = 0; j < next_data.size(); j++) {
		std::cout << next_data[j];
	}
}

int main() {
	//��������� �����
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	//���������� ������ �� �������� ������� � ��������
	pattern_tree bor;
	//����
	std::string input;
	while (std::cin >> input) {
		if (input == "1") {
			//����� �������� �� ������
			std::cin >> input;
			search_pattern_phone(bor, input);
		}
		else if (input == "2") {
			//����� �������� �� �����
			std::cin >> input;
			search_pattern_name(bor, input);
		}
		else if (input == "3") {
			//�����
			break;
		}
	}
	return 0;
}
