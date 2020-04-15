#include <iostream>
#include <string>
#include <queue>
#include <memory>
#include <utility>
#include <unordered_map>
#include <vector>



//Реализация бора
class pattern_tree {
public:
	pattern_tree();

	friend void search_pattern_name(pattern_tree& bor, const std::string& input);
	friend void search_pattern_phone(pattern_tree& bor, const std::string& input);

private:
	struct node; // forward-декларация для IntelliSense
	struct node {
		std::string get_value();
		void get_next_values(std::vector<std::string>& plus_data);
		int index = -1; //Индекс телефона, за который отвечает узел/-1, если такого нету
		pattern_tree* owner; //Бор, которому принадлежит узел
		char last_char = '\0'; //Буква, по которой перешли в данный узел
		node* parent; //Указатель на родительский узел
		std::unordered_map<char, std::unique_ptr<node>> edges; //Хеш-таблица ребёр из узла (буква ребра, указатель на узёл)
		node* name_phone_number; //Ссылка на соответствующий узел в боре имён/номеров
	};

	std::pair<std::unique_ptr<node>, std::unique_ptr<node>> root; //Корневой узел

	int size_str = 0; //Общий размер строки
	std::vector<std::string> data_base_numbers; //База данных номеров телефонов
	std::vector<std::string> data_base_names; //База данных имён, из которых состоит бор

	void search_pattern(node* root_, std::vector<std::string>& data_base, const std::string& input, int start_index, std::vector<std::string>& next_data);
	void get_data_base(); //Считывает базу данных телефонов
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
	std::string input = "";
	std::cin >> input;
	while (input != "*") {
		data_base_numbers.push_back(input);
		std::cin >> input;
		std::cin >> input;
		data_base_names.push_back(input);
		std::cin >> input;
	}
}

//Строит стандартное дерево бора
void pattern_tree::build_one_tree(node* root_, std::vector<std::string> &data_base){
	//Объявление переменных
	char pattern_char = ' '; //Сюда посимвольно считывается номера/имена
	node* now = root_; //Узел, в котором сейчас находимся
	//Считывание
	std::string input; //Строка для ввода
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
					now->edges[pattern_char]->owner = this;
					now->edges[pattern_char]->last_char = pattern_char;
				}
				now = now->edges[pattern_char].get();
				if (i == input.size() - 1) {
					now->index = j;
				}
			}
		}
	}
}

//Строит дерево бора для телефонов и фамилий
void pattern_tree::build_tree() {
	get_data_base();
	build_one_tree(root.first.get(), data_base_numbers);
	build_one_tree(root.second.get(), data_base_names);
}

//Для получения полного значения, в данном узле
std::string pattern_tree::node::get_value()
{
	return (index == -1? "": owner->data_base_numbers[index] + " - " + owner->data_base_names[index]);
}

//Для получения всех последующих значений
void pattern_tree::node::get_next_values(std::vector<std::string> &plus_data)
{
	if (index != -1) {
		plus_data.push_back(get_value());	
	}

	for (const auto& next_node : edges) {
		next_node.second->get_next_values(plus_data);
	}
}

//Основная функция поиска
void pattern_tree::search_pattern(node* root_, std::vector<std::string>& data_base, const std::string& input, int start_index, std::vector<std::string>& next_data) {
	char pattern_char = ' '; //Сюда посимвольно считывается номера/имена
	node* now = root_; //Узел, в котором сейчас находимся
	for (int i = start_index; i <= input.size(); i++) {
		if (i == input.size()) {
			now->get_next_values(next_data);
			break;
		}
		else {
			pattern_char = input[i];
			if (pattern_char == '*') {
				for (const auto& next_node : now->edges) {
					search_pattern(next_node.second.get(), data_base, input, i + 1, next_data);
				}
			}
			if (now->edges.find(pattern_char) == now->edges.end()) {
				break;
			}
			now = now->edges[pattern_char].get();
		}
	}
}

//Функция поиска телефона по номеру (с пропусками и без)
void search_pattern_name(pattern_tree& bor, const std::string& input) {
	std::vector<std::string> next_data; //Вывод
	bor.search_pattern(bor.root.second.get(), bor.data_base_names, input, 0, next_data);
	for (int j = 0; j < next_data.size(); j++) {
		std::cout << next_data[j];
	}
}

//Функция поиска телефона по имени
void search_pattern_phone(pattern_tree& bor, const std::string& input) {
	std::vector<std::string> next_data; //Вывод
	bor.search_pattern(bor.root.first.get(), bor.data_base_numbers, input, 0, next_data);
	for (int j = 0; j < next_data.size(); j++) {
		std::cout << next_data[j];
	}
}

int main() {
	//Ускорение ввода
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
	//Построение дерева по введённым номерам и фамилиям
	pattern_tree bor;
	//Ввод
	std::string input;
	while (std::cin >> input) {
		if (input == "1") {
			//Поиск телефона по номеру
			std::cin >> input;
			search_pattern_phone(bor, input);
		}
		else if (input == "2") {
			//Поиск телефона по имени
			std::cin >> input;
			search_pattern_name(bor, input);
		}
		else if (input == "3") {
			//Выход
			break;
		}
	}
	return 0;
}
