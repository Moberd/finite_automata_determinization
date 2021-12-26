#pragma once
#include <vector>
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <initializer_list>
#include <set>
#include <queue>
#include <algorithm>
#include <cstring>

class finite_state_automata {
	std::map<std::string, std::map<char, std::set<std::string>>> transitions;
	std::vector<std::string> finite_states;
	int states_cnt;
	std::string start_state;
	std::vector<char> alphabet;

public:
	finite_state_automata() {
        states_cnt = 0;
        start_state = "";
	}

	finite_state_automata(std::string file_path) : finite_state_automata() {
        create_from_file(file_path);
	}

	// создаём автомат из файла
	void create_from_file(std::string file_path) {
		std::ifstream in(file_path.c_str());
		std::string line;
		if (!in) {
            throw std::invalid_argument("Неправильное имя файла");
		}

		std::getline(in, line);
		for (auto s : split(line)) {
			if (s.size() > 1) {
				throw std::invalid_argument("Алфавит должен состоять из символов длины 1");
			}
			alphabet.push_back(s[0]);
		}

		while (std::getline(in, line)) {
			if (line.empty()) {
				return;
			}
			auto state_info = split(line);
			if (!(state_info[0] == "+" || state_info[0] == "-")) {
				throw std::invalid_argument("Переход должен начинаться с - или +");
			}
			if (state_info[0] == "+") {
				finite_states.push_back(state_info[1]);
			}

			auto state = state_info[1];
			states_cnt++;
			if (start_state.empty()) {
                start_state = state;
			}

            transitions[state] = {};
			for (size_t i = 2; i < state_info.size(); i++) {
				auto pair = split(state_info[i], ':');
				if (pair.size() != 2 || pair[0].size() != 1) {
					throw std::invalid_argument("Ошибка: неверно описанный переход");
				}
				transitions[state][pair[0][0]].insert(pair[1]);
			}
		}
	}

	bool recognize(const std::string& input) {
		return recognize_word(input, start_state);
	}

    //детерминизация
	void determinization() {
		std::map<std::string, std::map<char, std::set<std::string>>> new_transitions;
		std::queue<std::set<std::string>> queue;
		for (const auto& tr : transitions[start_state]) {
			auto new_state = transitions[start_state][tr.first];
			new_transitions["{" + start_state + "}"][tr.first].insert(get_state_name_from_set(new_state));
			queue.push(new_state);
		}
		while (!queue.empty()) {
			auto states_set = queue.front();
			auto state_name = get_state_name_from_set(states_set);
			if (new_transitions.find(state_name) == new_transitions.end()) {
				for (auto sym: alphabet) {
					std::set<std::string> new_state;
					for (const auto & it : states_set) {
						auto old_state = transitions[it][sym];
                        new_state = set_union(old_state, new_state);
						queue.push(new_state);
					}
					if (new_state.empty()) {
						continue;
					}
					new_transitions[state_name][sym].insert(get_state_name_from_set(new_state));
				}
			}
			queue.pop();
		}
		
		std::vector<std::string> new_finite_states;
		for (const auto& pair : new_transitions) {
			for (const auto& old : finite_states) {
				if (pair.first.find(old) != std::string::npos) {
					new_finite_states.push_back(pair.first);
					continue;
				}
			}
		}

        this->finite_states = new_finite_states;
        this->start_state = "{" + start_state + "}";
        this->states_cnt = transitions.size();
        this->transitions = new_transitions;
	}

    void transition_table() {
        int length = calc_size_of_box();
        std::string head = string_helper(' ', length + 7);
        std::string body = string_helper(' ', length + 6) + "|";
        std::string down = string_helper('-', length + 6) + "|";
        for (auto s : alphabet) {
            head += string_helper('_', length * 2 + 6);
            body += string_helper(' ', (length * 2 + 5) / 2) + std::string(1, s) + string_helper(' ', (length * 2 + 6) / 2) + "|";
            down += string_helper('-', length * 2 + 6) + "|";
        }
        head += "_";
        std::cout << head << std::endl << body << std::endl << down << std::endl;

        for (auto st : transitions) {
            if (std::find(finite_states.begin(), finite_states.end(), st.first) != finite_states.end()) {
                std::cout << "F";
            }
            else {
                std::cout << " ";
            }
            if (st.first == start_state) {
                std::cout << "->";
            }
            else {
                std::cout << "  ";
            }
            centered_line(st.first.c_str(), length);
            std::cout << "   |   ";
            for (auto s : alphabet) {
                std::string full_transition_string;
                if (!st.second[s].empty()) {
                    for (const auto& state : st.second[s]) {
                        full_transition_string += state + " ";
                    }

                    full_transition_string.replace(--full_transition_string.end(), full_transition_string.end(), "");
                }
                else {
                    full_transition_string = " ";
                }

                centered_line(full_transition_string.c_str(), length * 2);
                std::cout << "   |   ";
            }
            std::cout << "\n";
            std::cout << down << std::endl;
        }
        std::cout << std::endl;
    }

private:

	bool recognize_word(const std::string& input, std::string state) {
		for (auto p = input.begin(); p < input.end(); p++) {
			auto stateSet = transitions[state][*p];
			if (stateSet.size() == 1) {
				state = *stateSet.begin();
			}
			else {
				for (const auto & q : stateSet) {
					if (recognize_word(std::string(p + 1, input.end()), q)) {
						return true;
					}
				}
				return false;
			}
		}
		return std::find(finite_states.begin(), finite_states.end(), state) != finite_states.end();
	}

    //жесть оказывается в цпп нет сплита строки
	std::vector<std::string> split(const std::string& text, char sep = ' ') {
		std::vector<std::string> tokens;
		std::size_t start = 0, end = 0;
		while ((end = text.find(sep, start)) != std::string::npos) {
			tokens.push_back(text.substr(start, end - start));
			start = end + 1;
		}
		tokens.push_back(text.substr(start));
		return tokens;
	}

	void centered_line(const char* text, int fieldWidth) {
		int padlen = (fieldWidth - strlen(text)) / 2;
		printf("%*s%s%*s", padlen, "", text, padlen, "");
		if ((fieldWidth - strlen(text)) % 2 == 1) {
			printf(" ");
		}
	}

	std::string get_state_name_from_set(std::set<std::string> in) {
		if (in.empty()) {
			return " ";
		}
		std::string res = "{";
		for (auto p = in.begin(); p != --in.end(); p++)
		{
			res += *p + ", ";
		}
		res += *(--in.end()) + "}";
		return res;
	}

	std::set<std::string> set_union(const std::set<std::string>& a, const std::set<std::string>& b)
	{
		std::set<std::string> result = a;
		result.insert(b.begin(), b.end());
		return result;
	}

	int calc_size_of_box() {
		int max = 0;
		for (const auto& pair : transitions) {
			if (pair.first.size() > max) {
				max = pair.first.size();
			}
		}
		if (max < 2) {
			return 5;
		}
		return max;
	}

	std::string string_helper(char elem, int count) {
		std::string res = "";
		for (int i = 0; i < count; i++)
		{
			res += elem;
		}
		return res;
	}

};