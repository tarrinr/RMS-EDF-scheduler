#ifndef __T_WIN__
#define __T_WIN__

#include <windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

typedef std::vector<int> vec;
typedef std::vector<vec> mat;

class Twin {
public:
	Twin(const std::string &);
	template<typename T>
	void println(const T &);
	void println();
	template<typename T>
	void print(const T &);
	template<typename T>
	void printmulti(const T &);
	template<typename T>
	void getInput(T &);
	void getInput(std::string &);
	void getInput();
	void loading(int, bool);
	~Twin();
	bool toFile();
	bool toFile(const std::string &);
	vec gvec();
	void dvec(const vec&);
	mat gmat();
	void dmat(const mat&);
private:
	void size();
	void line();
	int rows();
	int columns();
	void display();
	void clear();
	void wincheck();
	int x, y;
	std::vector<std::string> buffer;
	std::string pname;

};

template<typename T>
void Twin::print(const T &in) {

	if (buffer.size() == 0) buffer.push_back("");

	std::ostringstream oss;
	oss << in;

	std::string temp = buffer.back() + oss.str();

	buffer.pop_back();
	buffer.push_back(temp);

}

template<typename T>
void Twin::println(const T &in) {

	std::ostringstream oss;
	oss << in;

	buffer.push_back(oss.str());

}

template<typename T>
void Twin::printmulti(const T &in) {

	for (auto &i : in)
		println(i);

}

template<typename T>
void Twin::getInput(T &var) {
	std::vector<std::string> backup = buffer;

	while (true) {
		display();

		if (std::cin >> var) {
			std::cin.ignore(1000, '\n');
			std::cin.clear();

			buffer.push_back("Input:");
			buffer.push_back("");
			println(var);
			buffer.push_back("");
			buffer.push_back("Enter 'x' to save and continue.");
			display();

			char temp;
			std::cin >> temp;
			std::cin.ignore(1000, '\n');
			std::cin.clear();

			if (temp == 'x' || temp == 'X') break;
			else buffer = backup;
		}
		else {

			std::cin.clear();
			std::cin.ignore(1000, '\n');

			buffer.push_back("Invalid entry.");
			buffer.push_back("");
			buffer.push_back("Press enter to continue.");
			display();
			buffer = backup;

			std::cin.ignore(1000, '\n');
			std::cin.clear();
		}
	}
}

#endif