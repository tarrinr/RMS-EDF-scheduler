#include "twin.h"

void Twin::size() {

	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	x = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	y = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

}

int Twin::rows() {
	size();
	return y;
}

int Twin::columns() {
	size();
	return x;
}

void Twin::println() {
	buffer.push_back("");
}

void Twin::display() {

	wincheck();

	system("clear");

	line();
	std::cout << "  " << pname << std::endl;
	line();
	std::cout << std::endl;

	for (auto&& i : buffer) {
		if (i != "") std::cout << "  > " << i << std::endl;
		else std::cout << std::endl;
	}

	for (int i = 0; i < y - 8 - int(buffer.size()); i++) std::cout << std::endl;
	line();
	std::cout << "  " << "Input:" << std::endl << "  > ";
	buffer.clear();

}

void Twin::wincheck() {

	size();

	int b = 0;

	for (auto &i : buffer) {

		if (int(i.length()) > b) b = int(i.length());

	}

	while (y - 8 - int(buffer.size()) < 1 || int(pname.length()) > x - 4 || int(b) > x - 8 || x < 120 || y < 30) {

		std::cout << std::endl << "Window too small.";
		std::cout << std::endl << "Enter 'x' to output to file, or resize window and enter 'r': ";

		char temp;
		std::cin >> temp;
		std::cin.ignore(1000, '\n');
		std::cin.clear();

		if (temp == 'x' || temp == 'X') {
			toFile();
			buffer.push_back("[See " + pname + ".txt for output.]");
		}

		size();

	}

}

Twin::Twin(const std::string &name) {

	pname = name;

	wincheck();

	system("clear");

	std::string ws;

	for (int i = 0; i < x - 49; i++)
		ws += " ";

	line();
	std::cout << "  " << pname << std::endl;
	line();

    std::cout << std::endl << std::endl;
	for (int i = 0; i < (y - 36)/2; i++)
        std::cout << std::endl;

std::cout << ws << "                 .@@@@@        " << std::endl;
std::cout << ws << "                .@@@@@@        " << std::endl;
std::cout << ws << "           .#@@@@@@@@@@        " << std::endl;
std::cout << ws << "       &@@@@@@@@@@@@@@@(       " << std::endl;
std::cout << ws << "    *@@@@     @@@@/@@@@@@@,    " << std::endl;
std::cout << ws << "   @@@,      @@@@*/@@@@ *@@@   " << std::endl;
std::cout << ws << " /@@/       @@@@( /@@@@   (@@* " << std::endl;
std::cout << ws << "*@@.       @@@@&  /@@@@    (@@," << std::endl;
std::cout << ws << "@@&       @@@@@   /@@@@     @@@" << std::endl;
std::cout << ws << "@@       @@@@@     @@@@      @@" << std::endl;
std::cout << ws << "@@      @@@@@     /@ #@      @@" << std::endl;
std::cout << ws << "@@     @@@@@@@@@@@@@@@       @@" << std::endl;
std::cout << ws << "@@@   @@@@@@@@@@@@@@@@@     @@@" << std::endl;
std::cout << ws << ",@@, @@@@@@       &@/ @    %@@." << std::endl;
std::cout << ws << " .@ @@@@@@         /@@@   @@@  " << std::endl;
std::cout << ws << "   @@@@@@               (@@@   " << std::endl;
std::cout << ws << "  @@@@@@ /           /@@@@     " << std::endl;
std::cout << ws << " @@@@@@ @@@@@@@@@@@@@@@,       " << std::endl;

	std::cout << std::endl << std::endl;
	std::cout << "             Welcome to " << pname << std::endl;

	std::cout << std::endl << std::endl << "  > Press enter to continue...";
	std::cin.ignore(1000, '\n');
	std::cin.clear();


}

void Twin::line() {

	size();

	std::cout << " ";
	for (int i = 0; i < x - 2; i++) std::cout << "=";
	std::cout << " ";

}

void Twin::clear() {
	buffer.clear();
}


void Twin::getInput(std::string &var) {
	std::vector<std::string> backup = buffer;

	while (true) {
		display();

		if (std::getline(std::cin, var)) {

			std::cin.clear();

			if (var[0] == var[var.length() - 1] && var[0] == '"')
				var = var.substr(1,var.length() - 2);

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

void Twin::getInput() {

	buffer.push_back("Press enter to continue.");
	display();

	std::cin.ignore(1000, '\n');
	std::cin.clear();

}

Twin::~Twin() {

	//buffer.push_back("Press enter to exit. . .");
	//display();

	//std::cin.ignore(1000, '\n');
	//std::cin.clear();
    system("clear");

}

bool Twin::toFile() {

	std::ofstream fout(pname + ".txt");
	if (fout.good()) {
		for (auto &i : buffer)
			fout << i << std::endl;

		fout.close();
		buffer.clear();
		buffer.push_back("Saved output to '" + pname + ".txt'.");
		buffer.push_back("");
		getInput();
		return 0;
	}
	else {
		std::vector<std::string> backup = buffer;
		buffer.clear();
		buffer.push_back("Unable to open '" + pname + ".txt'.");
		buffer.push_back("");
		getInput();
		buffer = backup;
		return 1;
	}
}

bool Twin::toFile(const std::string &name) {

	std::ofstream fout(name);
	if (fout.good()) {
		for (auto &i : buffer)
			fout << i << std::endl;

		fout.close();
		buffer.clear();
		buffer.push_back("Saved output to '" + name + "'.");
		buffer.push_back("");
		getInput();
		return 0;
	}
	else {
		std::vector<std::string> backup = buffer;
		buffer.clear();
		buffer.push_back("Unable to open '" + name + "'.");
		buffer.push_back("");
		getInput();
		buffer = backup;
		return 1;
	}
}

vec Twin::gvec() {

	display();
	vec b;
	double in;
	std::string input;

	std::getline(std::cin, input);
	std::stringstream iss(input);
	iss >> in;
	b.push_back(in);

	while (true) {

		println("Vector:");
		println();
		dvec(b);
		println("Enter the next element.");
		println("Enter 'x' to save and continue.");
		display();

		std::getline(std::cin, input);
		std::stringstream iss(input);
		iss >> in;

		if (input == "x" || input == "X") return b;
		else b.push_back(in);

	}
}

void Twin::dvec(const vec& vec) {

	for (auto&& i : vec) {
		println("[ ");
		print(i);
		print(" ]");
	}

	println();
}

mat Twin::gmat() {

	display();
	mat a;
	double in;
	std::string input;

	std::getline(std::cin, input);
	std::stringstream iss(input);
	a.push_back(vec());
	while (iss >> in) a[0].push_back(in);

	for (int i = 1; i > 0; i++) {

		println("Matrix:");
		println();
		dmat(a);
		println("Enter the next row.");
		println("Enter 'x' to save and continue.");
		display();

		std::getline(std::cin, input);
		std::stringstream iss(input);
		if (input == "x" || input == "X") i = -1;

		else {
			a.push_back(vec());
			while (iss >> in) a[i].push_back(in);

			if (a[i].size() != a[0].size()) {
				println("Invalid row.");
				a.pop_back();
				i--;
			}
		}
	}
	return a;
}

void Twin::dmat(const mat& mat) {

	println();

	for (int i = 0; i < int(mat.size()); i++) {

		print("[ ");

		for (int j = 0; j < int(mat[0].size()); j++) {
			print(mat[i][j]);
			if (j < int(mat[0].size()) - 1) print(",");
			print(" ");
		}

		print("]");
		println();

	}
}

void Twin::loading(int progress, bool done) {

	wincheck();

	system("clear");

	int w = x - 27;
	int p = w * progress / 100;
	
	std::string bar;
	std::string ws;

	for (int i = 0; i < w; i++)
		if (i < p) bar += "#";
		else if (i == p) bar += ">";
		else bar += " ";

	for (int i = 0; i < x - 49; i++)
		ws += " ";

	line();
	std::cout << "  " << pname << std::endl;
	line();
	
    std::cout << std::endl << std::endl;
	for (int i = 0; i < (y - 36)/2; i++)
        std::cout << std::endl;

std::cout << ws << "                 .@@@@@        " << std::endl;
std::cout << ws << "                .@@@@@@        " << std::endl;
std::cout << ws << "           .#@@@@@@@@@@        " << std::endl;
std::cout << ws << "       &@@@@@@@@@@@@@@@(       " << std::endl;
std::cout << ws << "    *@@@@     @@@@/@@@@@@@,    " << std::endl;
std::cout << ws << "   @@@,      @@@@*/@@@@ *@@@   " << std::endl;
std::cout << ws << " /@@/       @@@@( /@@@@   (@@* " << std::endl;
std::cout << ws << "*@@.       @@@@&  /@@@@    (@@," << std::endl;
std::cout << ws << "@@&       @@@@@   /@@@@     @@@" << std::endl;
std::cout << ws << "@@       @@@@@     @@@@      @@" << std::endl;
std::cout << ws << "@@      @@@@@     /@ #@      @@" << std::endl;
std::cout << ws << "@@     @@@@@@@@@@@@@@@       @@" << std::endl;
std::cout << ws << "@@@   @@@@@@@@@@@@@@@@@     @@@" << std::endl;
std::cout << ws << ",@@, @@@@@@       &@/ @    %@@." << std::endl;
std::cout << ws << " .@ @@@@@@         /@@@   @@@  " << std::endl;
std::cout << ws << "   @@@@@@               (@@@   " << std::endl;
std::cout << ws << "  @@@@@@ /           /@@@@     " << std::endl;
std::cout << ws << " @@@@@@ @@@@@@@@@@@@@@@,       " << std::endl;

	std::cout << std::endl << std::endl;
	std::cout << "             Loading. . ." << std::endl;
	std::cout << "          [" << bar << "] " << progress << "%" << std::endl;

	if (done) {
		std::cout << std::endl << "  > Press enter to continue...";
		std::cin.ignore(1000, '\n');
		std::cin.clear();
	}

}