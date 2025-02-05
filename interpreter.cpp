#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <variant>

using namespace std;
void print_var();
void print_mark();
void print_pol();
void print_structs();
void error() {
	cout << "error" << endl;
	print_var();
	print_mark();
	cout << "pol:" << endl;
	print_pol();
	cout << "structs:" << endl;
	print_structs();
	cout << "________________________________";
	cout << endl;
	throw 1;
	return;}

enum types {
//  0         1            2          3          4
	type_int, type_string, type_bool, type_real, type_mark};

enum type_of_lex {
	LEX_NULL, LEX_PROGRAM, LEX_END, LEX_WHILE, LEX_FOR, LEX_IF, LEX_THEN, LEX_ELSE, LEX_BREAK, LEX_WRITE, LEX_READ, 
	LEX_GOTO, LEX_IDENT, LEX_FALSE, LEX_TRUE, LEX_INT, LEX_BOOL, LEX_STRING, LEX_REAL, LEX_STRUCT, LEX_NOT, LEX_OR, LEX_AND,
	
	LEX_SEMICOLON, LEX_COLON, LEX_COMMA, LEX_DOT, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_LCURLY, LEX_RCURLY, LEX_QUOTE,
	LEX_EQ, LEX_LSS, LEX_GTR, LEX_LEQ, LEX_NEQ, LEX_GEQ,
	LEX_PLUS, LEX_UPLUS, LEX_MINUS, LEX_UMINUS, LEX_TIMES, LEX_DIVIDE, LEX_MOD, LEX_BACKSLASH,
	LEX_SCONST, LEX_CONST,
	POLIZ_LABEL, POLIZ_ADDRESS, POLIZ_GO, POLIZ_FGO};

struct lex {
	type_of_lex t_lex;
	string      n_lex;
	lex(type_of_lex t = LEX_NULL, string n = ""): t_lex(t), n_lex(n) {}
	bool operator==(lex l) {
		return t_lex == l.t_lex && n_lex == l.n_lex;
	}
	bool operator==(string s) {
		return n_lex == s;
	}
	bool operator==(type_of_lex t) {
		return t_lex == t;
	}};
	
ifstream f;
map<string, int> priority = {
		{"=",   1},
		{"or",  2},
		{"and", 3},
		{"<",   4},
		{">",   4},
		{"<=",  4},
		{"==",  4},
		{"!=",  4},
		{">=",  4},
		{"+",   5},
		{"-",   5},
		{"*",   6},
		{"/",   6},
		{"not", 7},
		{"&-",  7},
		{"&+",  7}
	};
map<string, type_of_lex> lex_table = {
		{"NULL", LEX_NULL},
		{"program", LEX_PROGRAM},
		{"end", LEX_END},
		{"while", LEX_WHILE},
		{"for", LEX_FOR},
		{"if", LEX_IF},
		{"then", LEX_THEN},
		{"else", LEX_ELSE},
		{"break", LEX_BREAK},
		{"write", LEX_WRITE},
		{"read", LEX_READ},
		{"goto", LEX_GOTO},
	    {"ident", LEX_IDENT},
		{"false", LEX_FALSE},
		{"true", LEX_TRUE},
		{"int", LEX_INT},
		{"bool", LEX_BOOL},
		{"string", LEX_STRING},
		{"real", LEX_REAL},
		{"struct", LEX_STRUCT},
		{"not", LEX_NOT},
		{"or", LEX_OR},
		{"and", LEX_AND},

		{";", LEX_SEMICOLON},
		{":", LEX_COLON},
		{",", LEX_COMMA},
		{".", LEX_DOT},
		{"=", LEX_ASSIGN},
		{"(", LEX_LPAREN},
		{")", LEX_RPAREN},
		{"{", LEX_LCURLY},
		{"}", LEX_RCURLY},
		{"\"", LEX_QUOTE},
		{"==", LEX_EQ},
		{"<", LEX_LSS},
		{">", LEX_GTR},
		{"<=", LEX_LEQ},
		{"!=", LEX_NEQ},
		{">=", LEX_GEQ},
		{"+", LEX_PLUS},
		{"&+", LEX_UPLUS},
		{"-", LEX_MINUS},
		{"&-", LEX_UMINUS},
		{"*", LEX_TIMES},
		{"/", LEX_DIVIDE},
		{"%", LEX_MOD},
		{"\\", LEX_BACKSLASH},
		{"sconst", LEX_SCONST},
		{"const", LEX_CONST},

		{"pol_label", POLIZ_LABEL},
		{"pol_addr", POLIZ_ADDRESS},
		{"pol_go", POLIZ_GO},
		{"pol_fgo", POLIZ_FGO},
		/*
		{"program", LEX_PROGRAM},
		*/	
	};
vector<char> special_symbols = {
	',', '.', ';', ':', '{', '}', '(', ')', '+', '-', '*',  '/',  '%', '\\', '\'', 
	'|', '&', '!', '@', '#', '$', '^', '<', '>', '=', '~', '\0', '\n', ' ' , '"'};
vector<lex> l;

string get_lex_name(lex x) {
    string s;
	for (auto &it : lex_table) {
	    if (it.second == x.t_lex)
			s = it.first;
	}
	return s;
}

string get_lex_name(type_of_lex x) {
    string s;
	for (auto &it : lex_table) {
	    if (it.second == x)
			s = it.first;
	}
	return s;
}

typedef variant<int, float, string, bool> info;
class ident {
	string 		  name;
	string struct_type;
	type_of_lex   type;
	info   		 value;
public:
	bool   is_init;
	ident(string n, type_of_lex t, info v):name(n),struct_type(""),type(t),value(v),is_init(true) {}
	ident(string n, type_of_lex t):name(n),struct_type(""),type(t),is_init(false) {}
	ident():name(""), struct_type(""),type(LEX_NULL),is_init(false) {}
	void set_name(string s) {
		name = s;
	}

	string get_name() {
		return name;
	}
	void set_struct(string s) {
		struct_type = s;
	}

	string get_struct() {
		return struct_type;
	}

	info get_value() {
		if (is_init){
			return value;
		}
		else {
			error();
			return 0;
		}
	}

	void set_value(info t) {
		value = t;
		is_init = true;
	}

	type_of_lex get_type() {
		return type;
	}

	void set_type(type_of_lex t) {
		type = t;
	}
	
	void print() {
	    cout << left << setw(10) << name << ": ";
	    cout << left << setw(10) << get_lex_name(type) << ": ";
	    if (is_init) {
	        if (holds_alternative<int>(value)) cout << get<int>(value) << endl;
	        else if (holds_alternative<string>(value)) cout << "__" << get<string>(value) << "__" << endl;
	        else if (holds_alternative<bool>(value)) cout << get<bool>(value) << endl;
	        else if (holds_alternative<float>(value)) cout << get<float>(value) << endl;
	        else cout << "strange value error" << endl;
	    }
	    else {
	        cout << "not initialized" << endl;
	    }
	}

	bool operator==(const string& s) {
		return name == s;
	}
	bool operator==(const type_of_lex& t) {
		return type == t;
	}};
	
vector<ident> var;
vector<ident> mark;
vector<ident> unknown_marks;

ident get_ident(string s) {
	for (auto &it : var) {
		if (it == s)
			return it;
	}
	ident x("", LEX_NULL);
	return x;
}
ident get_mark(string s) {
	for (auto &it : mark) {
		if (it == s)
			return it;
	}
	ident x("", LEX_NULL);
	return x;
}

void print_var() {
    int i = 0;
    cout << "var:" << endl;
    for (auto &it : var) {
        cout << i << "): ";
        it.print();
        i++;
    }
}

void print_mark() {
    int i = 0;
    cout << "mark:" << endl;
    for (auto &it : mark) {
        cout << i << "): ";
        it.print();
        i++;
    }
}

struct Struct {
	string name;
	vector<ident> idents;

	Struct(string n = ""):name(n) {}

	bool operator==(string& s) {
		return name == s;
	}
	bool operator==(const string& s) {
		return name == s;
	}
};

vector<Struct> structs;
vector<lex> pol;
stack<lex> help;
stack<Struct> types_struct;
stack<type_of_lex> types;

void clear() {
	l.clear();
	pol.clear();
	while (!help.empty())  {help.pop();}
	while (!types.empty()) {types.pop();}
	while (!types_struct.empty()) {types_struct.pop();}
	var.clear();
	mark.clear();
	unknown_marks.clear();
	structs.clear();}

void print_pol() {
	int i = 0;
	for (auto &it : pol) {
		cout << left << setw(5)  << i << " : ";
		cout << left << setw(10) << get_lex_name(it) << " : ";
		cout << left << setw(10) << it.n_lex << endl;
		i++;
	}
	cout << endl;}

void print_structs() {
	int i = 0;
	for (auto &it : structs) {
		cout << left << setw(5)  << i << " : ";
		cout << left << setw(10) << it.name  << endl;;
		for (auto &iit : it.idents) {
			iit.print();
		}
		i++;
	}
	cout << endl;}

void print_lex(vector<lex> vec) {
	auto iter = vec.begin();
	int i = 0;
	while (iter != vec.end()) {
		string s = get_lex_name(*iter);
		cout << left << setw(5)  << i << " : ";
		cout << left << setw(6)  << iter->t_lex << " : ";
		cout << left << setw(10) << s << " : ";
		cout << left << setw(10) << iter->n_lex << endl;
		iter++;
		i++;
	}}

class lexicalAnalyser {
public:
	bool skip_spaces() {
		char x = 'a';
		while ((f.peek() == ' ' || f.peek() == '\n') && f.peek() != -1) {
			x = f.get();
		}
		if (x == 0) error();
		else if (x == 'a') return false;
		return true;
	}
	bool skip_comment() {
		char x = 'a';
		if (f.peek() == '/') {
			f.get();
			if (f.peek() == '*') {
				f.get();
			}
			else {
				f.unget();
				return false;
			}
		}
		else {
			return false;
		}
		while ((x = f.get()) && x != -1) {
			if (x == '*' && f.peek() == '/') {
				f.get();
				return true;
			}
		}
		return false;
		error();
	}
	void skip() {
		while (skip_spaces() && skip_comment()) {}
	}

	bool is_special(char c) {
		return find(special_symbols.begin(), special_symbols.end(), c) != special_symbols.end();
	}
	bool is_end(char c) {
		return c == '\0' || c == -1 || c == 1;
	}
	
	string read_word() {
		skip();
		string w = "";
		char x = f.peek();
		while (true) {
			if (is_special(x) || is_end(x))
				break;
			w += f.get();
			x = f.peek();
		}
		return w;
	}
	string read_special() {
		skip();
		string w = "";
		if (is_special(f.peek())) {
			w += f.get();
			if ((w == ">" && f.peek() == '=') ||
				(w == "<" && f.peek() == '=') ||
				(w == "!" && f.peek() == '=') ||
				(w == "=" && f.peek() == '=')) {
				w += f.get();
			}
		}
		return w;
	}
	string read_quote() {
		string w = "";
		while(true) {
			if (f.peek() == '\\') {
				f.get();
				switch(f.peek()) {
				case 'n' : 
					w += '\n';
					break;
				case '\\':
					w += '\\';
					break;
				case 't' :
					w += '\t';
					break;
				case '?' :
					w += '\?';
					break;
				case 'v' :
					w += '\v';
					break;
				case '\'':
					w += '\'';
					break;
				case '\"':
					w += '\"';
					break;
				case 'b' :
					w += '\b';
					break;
				case 'r' :
					w += '\r';
					break;
				case '0' :
					w += '\0';
					break;
				case 'f' :
					w += '\f';
					break;
				case 'a' :
					w += '\a';
					break;
				default  :
					cout << "\\ without shielded symbol in a string const " << endl;
					w += '\\';
					break;
				}
				f.get();
			}	
			else {
				if (f.peek() == '\"' || is_end(f.peek()))
					break;
				w += f.get();
			}
		}
		return w;
	}
	
	static bool is_name(string s) {
		return ('a' <= s[0] && s[0] <= 'z') || ('A' <= s[0] && s[0] <= 'Z');
	}
	static bool is_number(string s) {
		for (int i = 0; i < s.size(); i++) {
			if ((s[i] < '0' || s[i] > '9'))
				return false;
		}
		return true;
	}

	vector<lex> analyse() {
		string s;
		while (f.peek() != 0 && f.peek() != -1 && f.peek() != -1) {
			s = read_word();
			//cout << s << endl;
			if (s != "") {
				lex x;
				int a = -1;
				try {
					a = lex_table.at(s);
				} catch (out_of_range) {}
				switch(a) {
				case LEX_PROGRAM :
					x = lex(LEX_PROGRAM, s);
					l.push_back(x); break;
				case LEX_WHILE :
					x = lex(LEX_WHILE, s);
					l.push_back(x); break;
				case LEX_FOR :
					x = lex(LEX_FOR, s);
					l.push_back(x); break;
				case LEX_IF :
					x = lex(LEX_IF, s);
					l.push_back(x); break;
				case LEX_THEN :
					x = lex(LEX_THEN, s);
					l.push_back(x); break;
				case LEX_ELSE :
					x = lex(LEX_ELSE, s);
					l.push_back(x); break;
				case LEX_BREAK :
					x = lex(LEX_BREAK, s);
					l.push_back(x); break;
				case LEX_WRITE :
					x = lex(LEX_WRITE, s);
					l.push_back(x); break;
				case LEX_READ :
					x = lex(LEX_READ, s);
					l.push_back(x); break;
				case LEX_GOTO :
					x = lex(LEX_GOTO, s);
					l.push_back(x); break;
				case LEX_FALSE :
					x = lex(LEX_FALSE, s);
					l.push_back(x); break;
				case LEX_TRUE :
					x = lex(LEX_TRUE, s);
					l.push_back(x); break;
				case LEX_INT :
					x = lex(LEX_INT, s);
					l.push_back(x); break;
				case LEX_BOOL :
					x = lex(LEX_BOOL, s);
					l.push_back(x); break;
				case LEX_STRING :
					x = lex(LEX_STRING, s);
					l.push_back(x); break;
				case LEX_REAL :
					x = lex(LEX_REAL, s);
					l.push_back(x); break;
				case LEX_STRUCT :
					x = lex(LEX_STRUCT, s);
					l.push_back(x); break;
				case LEX_NOT :
					x = lex(LEX_NOT, s);
					l.push_back(x); break;
				case LEX_OR :
					x = lex(LEX_OR, s);
					l.push_back(x); break;
				case LEX_AND :
					x = lex(LEX_AND, s);
					l.push_back(x); break;
				case -1:
					if (is_name(s)) {
						x = lex(LEX_IDENT, s);
						l.push_back(x);
					}
					else if(is_number(s)) {
						x = lex(LEX_CONST, s);
						l.push_back(x);
					}
					else {
						cout << "word lex error1: " << s << endl;
						error();
					}
					break;
				default:
					cout << "word lex error2: " << s << endl;
					error();
				}
			}
			else {
				s = read_special();
				//cout << s << endl;
				if (s != "") {
					lex x;
					int a = -1;
					try {
						a = lex_table.at(s);
					} catch (out_of_range) {}
					switch(a) {
					case LEX_SEMICOLON :
						x = lex(LEX_SEMICOLON, s);
						l.push_back(x); break;
					case LEX_COLON :
						x = lex(LEX_COLON, s);
						l.push_back(x); break;
					case LEX_COMMA :
						x = lex(LEX_COMMA, s);
						l.push_back(x); break;
					case LEX_DOT :
						x = lex(LEX_DOT, s);
						l.push_back(x); break;
					case LEX_ASSIGN :
						x = lex(LEX_ASSIGN, s);
						l.push_back(x); break;
					case LEX_LPAREN :
						x = lex(LEX_LPAREN, s);
						l.push_back(x); break;
					case LEX_RPAREN :
						x = lex(LEX_RPAREN, s);
						l.push_back(x); break;
					case LEX_LCURLY :
						x = lex(LEX_LCURLY, s);
						l.push_back(x); break;
					case LEX_RCURLY :
						x = lex(LEX_RCURLY, s);
						l.push_back(x); break;
					case LEX_QUOTE :
						x = lex(LEX_QUOTE, s);
						l.push_back(x); 
						s = read_quote();
						x = lex(LEX_SCONST, s);
						l.push_back(x);
						s = read_special();
						x = lex(LEX_QUOTE, s);
						l.push_back(x);
						break;
					case LEX_EQ :
						x = lex(LEX_EQ, s);
						l.push_back(x); break;
					case LEX_LSS :
						x = lex(LEX_LSS, s);
						l.push_back(x); break;
					case LEX_GTR :
						x = lex(LEX_GTR, s);
						l.push_back(x); break;
					case LEX_LEQ :
						x = lex(LEX_LEQ, s);
						l.push_back(x); break;
					case LEX_NEQ :
						x = lex(LEX_NEQ, s);
						l.push_back(x); break;
					case LEX_GEQ :
						x = lex(LEX_GEQ, s);
						l.push_back(x); break;
					case LEX_PLUS :
					    if (l.back() == LEX_IDENT || l.back() == LEX_CONST || l.back() == LEX_RPAREN) {
					        x = lex(LEX_PLUS, s);
					    }
					    else {
					        x = lex(LEX_UPLUS, "&" + s);
					    }
					    l.push_back(x); 
						break;
					case LEX_MINUS :
						if (l.back() == LEX_IDENT || l.back() == LEX_CONST || l.back() == LEX_RPAREN) {
					        x = lex(LEX_MINUS, s);
					    }
					    else {
					        x = lex(LEX_UMINUS, "&" + s);
					    } 
					    l.push_back(x); 
					    break;
					case LEX_TIMES :
						x = lex(LEX_TIMES, s);
						l.push_back(x); break;
					case LEX_DIVIDE :
						x = lex(LEX_DIVIDE, s);
						l.push_back(x); break;
					case LEX_MOD :
						x = lex(LEX_MOD, s);
						l.push_back(x); break;
					case LEX_BACKSLASH :
						x = lex(LEX_BACKSLASH, s);
						l.push_back(x); break;
					case -1:
						x = lex(LEX_SCONST, s);
						l.push_back(x);
						break;
					default:
						cout << "special word lex error: " << s << endl;
						error();
					}
				}
			}
		}
		l.push_back(lex(LEX_END, "end"));
		return l;
	}
};

class syntSemAnalyserGenerator {

public:
	int i;
	vector< vector<int> > break_vec;
	syntSemAnalyserGenerator(): i(0) {}
	bool read(string s) {
		if (i < l.size()){
			return get_lex_name(l[i].t_lex) == s;
		}
		else {
			cout << "read error: end of lex vector";
			//error();
			return false;
		}
	}
	lex get_lex() {
		if (i < l.size()){
			return l[i];
		}
		else {
			cout << "get_lex error: end of lex vector";
			//error();
			return lex(LEX_NULL, "");
		}
	}
	type_of_lex get_type() {
		return get_lex().t_lex;
	}
	void next_lex() {
		i++;
		if (i > l.size()) {
			cout << "next_lex error: end of lex vector";
			error();
		}
	}
	void prev_lex() {
		i--;
		if (i < 0) {
			cout << "prev_lex error: end of lex vector";
			error();
		}
	}
	bool name_available(string s) {
		for (int i = 0; i < var.size(); i++) {
			if (var[i] == s)
				return false;
		}
		for (int i = 0; i < mark.size(); i++) {
			if (mark[i] == s)
				return false;
		}
		for (int i = 0; i < structs.size(); i++) {
			if (structs[i] == s)
				return false;
		}
		return true;
	}
	float read_float() {
		lex des = get_lex();
		next_lex();
		next_lex();
		lex frac = get_lex();
		next_lex();
		string num = des.n_lex + "." + frac.n_lex;
		float ans = stof(num);
		return ans;
	}

	void START() {
		i = 0;
		cout << "START" << endl;
		if (!read("program")) {
			cout << "error no program " << i << endl;
			error();
		}
		next_lex();
		if (!read("{")) {
			cout << "error no { after program " << i << endl;
			error();
		}
		next_lex();
		DECLS();
		OPERS();
		if (!read("}")) {
			cout << "error no } after program " << i << endl;
			error();
		}
		next_lex();
		if(!read("end")) {
			cout << "very strange start error " << i << endl;
			error();
		}
		for (ident &it : unknown_marks) {
			ident id = get_mark(it.get_name());
			if (id == LEX_NULL) {
				cout << "mark _" << it.get_name() << "_ not declared" << endl;
				error();
			}
		}
		pol.push_back(lex(LEX_END, ""));
		i = 0;
	}

	void DECLS() {
		cout << "DECLS" << endl;
		while (read("struct")) {
			STRUCT();
			if (!read(";")) {
				cout << "error no ; after struct declaration " << i <<endl;
				error();
			}
			next_lex();
		}
		while (read("int") || read("real") || read("string") || read("bool") || read("struct")) {
			DECL1();
			if (!read(";")) {
				cout << "error no ; after declaration " << i <<endl;
				error();
			}
			next_lex();
		}
	}

	void STRUCT() {
		next_lex();
		cout << "STRUCT" << endl;
		lex name = get_lex();
		next_lex();
		if (name == LEX_IDENT) {
			if (name_available(name.n_lex)) {
				Struct str (name.n_lex);
				if (!read("{")) {
					cout << "no { after struct declaration " << i << endl;
					error();
				}
				next_lex();
				while (read("int") || read("bool") || read("real") || read("string")) {
					type_of_lex type = get_type();
					next_lex();
					while (true) {
						ident x;
						name = get_lex();
						if (name == LEX_IDENT) {
							next_lex();
							bool b = true;
							for (int j = 0; j < str.idents.size(); j++) {
								if (str.idents[j] == name.n_lex) {
									b = false;
								}
							}
							if (!b) {
								cout << "declaring name not available " << i << endl;
								error();
							}
							if (read("=")) {
								next_lex();
								ident c = DECLCONST();
								if (type != c.get_type()) {
									cout << "declaration const not matching types " << i << endl;
									error();
								}
								x.set_name(name.n_lex);
								x.set_type(type);
								x.set_value(c.get_value());
								str.idents.push_back(x);
							}
							else if (read(",")) {
								x.set_name(name.n_lex);
								x.set_type(type);
								str.idents.push_back(x);
							}
							else if (read(";")) {
							    x.set_name(name.n_lex);
								x.set_type(type);
								str.idents.push_back(x);
								break;;
							}
							else {
								cout << "declaration error " << i << endl;
								error();
							}
						}
						else {
							cout << "declaration without identifier " << i << endl;
							error();
						}
						if (read(",")) {next_lex();}
						else if (read(";")) {break;}
						else {
						    cout << "very strange declaration error " << i << endl;
						    error();
						}
					}
					next_lex();
				}
				if (!read("}")) {
					cout << "error no } after struct declaration " << i << endl;
					error();
				}
				next_lex();
				if (!read(";")) {
					cout << "error no ; after struct declaration " << i << endl;
					error();
				}
				structs.push_back(str);
			}
			else {
				ident x;
				x.set_type(LEX_STRUCT);
				x.set_struct(name.n_lex);
				x.is_init = true;
				Struct str;
				for (int j = 0; j < structs.size(); j++) {
					if (structs[j] == name.n_lex) {
						str = structs[j];
					}
				}
				if (str == "") {
					cout << "error unknown type of struct " << i << endl;
					error();
				}
				name = get_lex();
				next_lex();
				if (name == LEX_IDENT) {
					x.set_name(name.n_lex);
					var.push_back(x);
					ident id;
					for (auto &it : str.idents) {
						id = it;
						id.set_name(x.get_name() + "." + it.get_name());
						var.push_back(id);
					}
				}
				else {
					cout << "error no identifier for the struct " << i << endl;
					error();
				}
			}
		}
		else {
			cout << "no name after struct " << i << endl;
			error();
		}
	}

	void DECL1() {
		type_of_lex type = get_type();
		cout << "DECL1" << endl;
		cout << get_lex_name(type) << endl;
		next_lex();
		if (type == LEX_STRUCT) {
			lex type_name = get_lex();
			next_lex();
			bool b = false;
			Struct str;
			for (int j = 0; j < structs.size(); j++) {
				if (structs[j] == type_name.n_lex) {
					b = true;
					str = structs[j];
				}
			}
			if (!b) {
				cout << "unknown struct type " << i << endl;
				error();
			}
			while(true) {
				ident x;
				x.set_type(LEX_STRUCT);
				x.set_struct(str.name);
				x.is_init = true;
				lex name = get_lex();
				next_lex();
				if (!name_available(name.n_lex)) {
					cout << "declaring name not available " << i << endl;
					error();
				}
				x.set_name(name.n_lex);
				var.push_back(x);
				ident id;
				for (auto &it : str.idents) {
					id = it;
					id.set_name(x.get_name() + "." + it.get_name());
					var.push_back(id);
				}
				if (read(",")) {
					next_lex();
					continue;
				}
				else if (read(";")) {
					return;
				}
				else {
					cout << "very strange declaration error " << i << endl;
					error();
				}

			}
		}
		while(true) {
			ident x;
			lex name = get_lex();
			if(name == LEX_IDENT) {
				next_lex();
				if (!name_available(name.n_lex)) {
					cout << "declaring name not available " << i << endl;
					error();
				}
				if (read("=")) {
					next_lex();
					ident c = DECLCONST();
					if (type != c.get_type()) {
						cout << "declaration const not matching types " << i << endl;
						error();
					}
					x.set_name(name.n_lex);
					x.set_type(type);
					x.set_value(c.get_value());
					var.push_back(x);
				}
				else if (read(",")) {
					x.set_name(name.n_lex);
					x.set_type(type);
					var.push_back(x);
				}
				else if (read(";")) {
				    x.set_name(name.n_lex);
					x.set_type(type);
					var.push_back(x);
					return;
				}
				else if (read(".")) {
					next_lex();
					lex name2 = get_lex();
					if (name2 != LEX_IDENT) {
						cout << "wrong field identifier declaration " << i << endl;
						error();
					}
					next_lex();
					name.n_lex = name.n_lex + "." + name2.n_lex;
					if (name_available(name.n_lex)) {
						cout << "unknown struct field identifier " << i << endl;
						error();
					}
					if (read("=")) {
						next_lex();
						ident c = DECLCONST();
						if (type != c.get_type()) {
							cout << "declaration const not matching types " << i << endl;
							error();
						}
						x.set_name(name.n_lex);
						x.set_type(type);
						x.set_value(c.get_value());
						for (int j = 0; j < var.size(); j++) {
							if (var[j] == name.n_lex) {
								var[j] = x;
							}
						}
					}
					else if (read(",")) {
						x.set_name(name.n_lex);
						x.set_type(type);
						for (int j = 0; j < var.size(); j++) {
							if (var[j] == name.n_lex) {
								var[j] = x;
							}
						}
					}
					else if (read(";")) {
					    x.set_name(name.n_lex);
						x.set_type(type);
						for (int j = 0; j < var.size(); j++) {
							if (var[j] == name.n_lex) {
								var[j] = x;
							}
						}
						return;
					}

				}
				else {
					cout << "declaration error " << i << endl;
					error();
				}

			}
			else {
				cout << "declaration without identifier " << i << endl;
				error();
			}
			if (read(",")) {next_lex();}
			else if (read(";")) {return;}
			else {
			    cout << "very strange declaration error " << i << endl;
			    error();
			}
		}
	}

	ident CONST() {
		cout << "CONST" << endl;
		ident c;
		if (read("const") || read("\"") || read("true") || read("false")) {}
		else {
		    cout << "not a const " << i << endl;
			error();
		}
		type_of_lex type = get_type();
		lex x = get_lex();
		next_lex();
		if (read(".")) {
		    //cout << "real" << endl;
			next_lex();
			if (read("const")) {
				prev_lex();
				prev_lex();
				float a = read_float();
				c.set_type(LEX_REAL);
				c.set_value(a);
				return c;
			}
			else {
				cout << "error bad real const " << i << endl;
				error();
			}
		}
		switch(type) {
		case LEX_CONST:
			c.set_type(LEX_INT);
			c.set_value(stoi(x.n_lex));
			break;
		case LEX_QUOTE:
		    if (read("sconst")) {
		        x = get_lex();
		        next_lex();
		        if (read("\"")) {
		            c.set_type(LEX_STRING);
			        c.set_value(x.n_lex);
			        next_lex();
		        }
		        else {
		            cout << "error const string not finished " << i << endl;
		            error();
		        }
		    }
		    else {
		        cout << "error no const string " << i << endl;
                error();
		    }
			
			break;
		case LEX_TRUE:
			c.set_type(LEX_BOOL);
			c.set_value(true);
			break;
		case LEX_FALSE:
			c.set_type(LEX_BOOL);
			c.set_value(false);
			break;
		default:
			cout << "very very strange const error " << i << endl;
			error();
		}
		return c;
	}
	
	bool check_int_int(type_of_lex oper2) {
		if (types.empty()) {
			cout << "missing second operand " << i << endl;
		}
		type_of_lex oper1 = types.top();
		types.pop();
		types.push(LEX_INT);
		return oper1 == oper2 && oper1 == LEX_INT;
	}
	bool check_int_real(type_of_lex oper2) {
		if (types.empty()) {
			cout << "missing second operand " << i << endl;
			error();
		}
		type_of_lex oper1 = types.top();
		types.pop();
		if (oper1 == LEX_INT) {
			if (oper2 == LEX_REAL) {types.push(LEX_REAL);}
			else                   {types.push(LEX_INT);}
			return oper2 == LEX_REAL || oper2 == LEX_INT;
		}
		else if (oper1 == LEX_REAL) {
			types.push(LEX_REAL);
			return oper2 == LEX_REAL || oper2 == LEX_INT;
		}
		else {
			return false;
		}
		
	}
	bool check_bool_bool(type_of_lex oper2) {
		if (types.empty()) {
			cout << "missing second operand " << i << endl;
		}
		type_of_lex oper1 = types.top();
		types.pop();
		types.push(LEX_BOOL);
		return oper1 == oper2 && oper1 == LEX_BOOL;
	}
	bool check_types(lex x) {
		if (types.empty()) {
			cout << "missing operand " << i << endl;
			error();
		}
		type_of_lex t = x.t_lex;
		type_of_lex oper1;
		type_of_lex oper2 = types.top();
		types.pop();
		switch(t) {
		case LEX_UPLUS:
			if (oper2 == LEX_INT || oper2 == LEX_REAL) {
				types.push(oper2);
			}
			else {
				cout << "wrong operand for (int/real) &+ " << i << endl;
				error();
			}
			break;
		case LEX_UMINUS:
			if (oper2 == LEX_INT || oper2 == LEX_REAL) {
				types.push(oper2);
			}
			else {
				cout << "wrong operand for (int/real) &- " << i << endl;
				error();
			}
			break;
		case LEX_PLUS:
			if (oper2 == LEX_STRING) {
				if (types.empty()) {
					cout << "empty second operand for (string) + " << i << endl;
					error();
				}
				oper1 = types.top();
				types.pop();
				if (oper1 == oper2) {
					types.push(LEX_STRING);
					break;
				}
				else {
					cout << "wrong second operand for (string) + " << i << endl;
					error();
				}
			}
			if (!check_int_real(oper2)) {
				cout << "wrong operands for (int/real) + " << i << endl;
				error();
			}
			break;
		case LEX_MINUS:
			if (!check_int_real(oper2)) {
				cout << "wrong operands for (int/real) - " << i << endl;
				error();
			}
			break;
		case LEX_TIMES:
			if (!check_int_real(oper2)) {
				cout << "wrong operands for (int/real) * " << i << endl;
				error();
			}
			break;
		case LEX_DIVIDE:
			if (!check_int_real(oper2)) {
				cout << "wrong operands for (int/real) / " << i << endl;
				error();
			}
			break;
		case LEX_MOD:
			if (!check_int_int(oper2)) {
				cout << "wrong operands for (int) % " << i << endl;
				error();
			}
			break;
		case LEX_NOT:
			if (oper2 == LEX_BOOL) {
				types.push(oper2);
			}
			else {
				cout << "wrong operand for (bool) not " << i << endl;
				error();
			}
			break;
		case LEX_OR:
			if (!check_bool_bool(oper2)) {
				cout << "wrong second operand for (bool) or " << i << endl;
				error();
			}
			break;
		case LEX_AND:
			if (!check_bool_bool(oper2)) {
				cout << "wrong second operand for (bool) and " << i << endl;
				error();
			}
			break;
		case LEX_EQ:
			if (oper2 == LEX_STRING) {
				if (types.empty()) {
					cout << "empty second operand for (string) == " << i << endl;
					error();
				}
				oper1 = types.top();
				types.pop();
				if (oper1 == oper2) {
					types.push(LEX_BOOL);
					break;
				}
				else {
					cout << "wrong second operand for (string) == " << i << endl;
					error();
				}
			}
			if (!check_int_real(oper2)) {
				cout << "wrong operands for (int/real) == " << i << endl;
				error();
			}
			else {
				types.pop();
				types.push(LEX_BOOL);
			}
			break;
		case LEX_LSS:
			if (oper2 == LEX_STRING) {
				if (types.empty()) {
					cout << "empty second operand for (string) < " << i << endl;
					error();
				}
				oper1 = types.top();
				types.pop();
				if (oper1 == oper2) {
					types.push(LEX_BOOL);
					break;
				}
				else {
					cout << "wrong second operand for (string) < " << i << endl;
					error();
				}
			}
			if (!check_int_real(oper2)) {
				cout << "wrong operands for (int/real) < " << i << endl;
				error();
			}
			else {
				types.pop();
				types.push(LEX_BOOL);
			}
			break;
		case LEX_GTR:
			if (oper2 == LEX_STRING) {
				if (types.empty()) {
					cout << "empty second operand for (string) > " << i << endl;
					error();
				}
				oper1 = types.top();
				types.pop();
				if (oper1 == oper2) {
					types.push(LEX_BOOL);
					break;
				}
				else {
					cout << "wrong second operand for (string) >  " << i << endl;
					error();
				}
			}
			if (!check_int_real(oper2)) {
				cout << "wrong operands for (int/real) > " << i << endl;
				error();
			}
			else {
				types.pop();
				types.push(LEX_BOOL);
			}
			break;
		case LEX_NEQ:
			if (oper2 == LEX_STRING) {
				if (types.empty()) {
					cout << "empty second operand for (string) != " << i << endl;
					error();
				}
				oper1 = types.top();
				types.pop();
				if (oper1 == oper2) {
					types.push(LEX_BOOL);
					break;
				}
				else {
					cout << "wrong second operand for (string) != " << i << endl;
					error();
				}
			}
			if (!check_int_real(oper2)) {
				cout << "wrong operands for (int/real) != " << i << endl;
				error();
			}
			else {
				types.pop();
				types.push(LEX_BOOL);
			}
			break;
		case LEX_LEQ:
			if (!check_int_real(oper2)) {
				cout << "wrong operands for (int/real) <= " << i << endl;
				error();
			}
			else {
				types.pop();
				types.push(LEX_BOOL);
			}
			break;
		case LEX_GEQ:
			if (!check_int_real(oper2)) {
				cout << "wrong operands for (int/real) >= " << i << endl;
				error();
			}
			else {
				types.pop();
				types.push(LEX_BOOL);
			}
			break;
		case LEX_ASSIGN:
			if (oper2 == LEX_STRING) {
				if (types.empty()) {
					cout << "empty second operand for (string) = " << i << endl;
					error();
				}
				oper1 = types.top();
				types.pop();
				if (oper1 == oper2) {
					types.push(LEX_STRING);
					break;
				}
				else {
					cout << "wrong second operand for (string) = " << i << endl;
					error();
				}
			}
			else if (oper2 == LEX_BOOL) {
				if (types.empty()) {
					cout << "empty second operand for (bool) = " << i << endl;
					error();
				}
				if (types.top() != LEX_BOOL) {
					cout << "wrong second operand for (bool) = " << i << endl;
					error();
				}
			}
			else if (oper2 == LEX_INT || oper2 == LEX_REAL) {
				if (types.empty()) {
					cout << "empty second operand for (int/real) = " << i << endl;
					error();
				}
				oper1 = types.top();
				types.pop();
				if (oper1 != LEX_INT && oper1 != LEX_REAL) {
					cout << "wrong second operand for (int/real) = " << i << endl;
					error();
				}
				else {
					types.push(oper1);
				}
			}
			else if (oper2 == LEX_STRUCT) {
				oper1 = types.top();
				types.pop();
				if (oper1 != oper2) {
					cout << "wrong second operand for (struct) = " << i << endl;
					error();
				}
				if (types_struct.empty()) {
					cout << "struct = error " << i << endl;
				}
				Struct str1 = types_struct.top();
				types_struct.pop();
				if (types_struct.empty()) {
					cout << "struct = error " << i << endl;
				}
				Struct str2 = types_struct.top();
				types_struct.pop();
				if (str1.name != str2.name) {
					cout << "wrong second operand for (struct) = " << i << endl;
					error();
				}
				else {
					types.push(LEX_STRUCT);
				}
			}
			break;
		default:
			cout << "unknown operation " << i << endl;
			return false;
			break;
		}
		return true;
	}
	void push_oper(lex x) {
		cout << "push_oper: " << get_lex_name(x) << endl;
		if (help.empty()) {
			cout << "help empty, pushed oper " << get_lex_name(x) << endl;
	    	help.push(x);
	   		return;
	    }
	    if (x == LEX_ASSIGN) {
	    	help.push(x);
	    	return;
	    }
	    while (priority[x.n_lex] <= priority[help.top().n_lex]) {
	    	lex y = help.top();
	    	help.pop();
	    	pol.push_back(y);
	    	check_types(y);
	    	cout << "pushed oper " << get_lex_name(y) << endl;
	    	if (help.empty()) {break;}
	    }
	    help.push(x);
	}
	void push_const() {
		cout << "push_const" << endl;
		prev_lex();
		ident id = CONST();
		lex x(id.get_type(), "");	
		bool b;
		switch(x.t_lex) {
		case LEX_INT:
			x.n_lex = to_string(get<int>(id.get_value()));
			break;
		case LEX_REAL:
			x.n_lex = to_string(get<float>(id.get_value()));
			break;
		case LEX_STRING:
			x.n_lex = get<string>(id.get_value());
			break;
		case LEX_BOOL:
			b = get<bool>(id.get_value());
			if (b) {x.n_lex = "true";}
			else   {x.n_lex = "false";};
			break;
		default:
			cout << "push_const error " << i << endl;
			error();
			break;
		}
		pol.push_back(x);
		types.push(x.t_lex);
	}
	lex read_expr() {
	    lex expr_type(LEX_NULL, "");
	    type_of_lex type = get_type();
	    lex x = get_lex();
	    cout << "read_expr: " << get_lex_name(x) << endl;
	    lex y;
	    ident id;
	    int balance = 0;
	    while (!read(";") && !read(",") && (!read(")") || balance != 0)) {
	    	x = get_lex();
	    	type = get_type();
	    	cout << "while read_expr: " << get_lex_name(x) << endl;
	    	next_lex();
	    	switch(type) {
	    	case LEX_IDENT:
	    		id = get_ident(x.n_lex);
	    		if (id == LEX_NULL) {
	    			cout << "unknown identifier _" << x.n_lex << "_ " << i << endl;
	    			error();
	    		}
	    		if (read(".")) {
	    			next_lex();
	    			lex name2 = get_lex();
	    			if (name2 != LEX_IDENT) {
	    				cout << "error accessing struct field " << i << endl;
	    				error();
	    			}
	    			Struct str_type;
	    			for (int j = 0; j < structs.size(); j++) {
	    				if (structs[j] == x.n_lex) 
	    					str_type = structs[j];
	    			}
	    			types_struct.push(str_type);
	    			x.n_lex = x.n_lex + "." + name2.n_lex;
	    			id = get_ident(x.n_lex);
	    			if (id == LEX_NULL) {
	    				cout << "unknown identifier " << i << endl;
	    			}
	    			next_lex();
	    		}
	    		pol.push_back(x);
	    		types.push(id.get_type());
	    		if (id == LEX_STRUCT) {
	    			Struct str_type;
	    			for (int j = 0; j < structs.size(); j++) {
	    				if (structs[j] == x.n_lex) 
	    					str_type = structs[j];
	    			}
	    			types_struct.push(str_type);
	    		}
	    		break;
	    	case LEX_PLUS:
	    		push_oper(x);
	    		break;
	    	case LEX_UPLUS:
	    		push_oper(x);
	    		break;
	    	case LEX_MINUS:
	    		push_oper(x);
	    		break;
	    	case LEX_UMINUS:
	    		push_oper(x);
	    		break;
	    	case LEX_TIMES:
	    		push_oper(x);
	    		break;
	    	case LEX_DIVIDE:
	    		push_oper(x);
	    		break;
	    	case LEX_MOD:
	    		push_oper(x);
	    		break;
	    	case LEX_LPAREN:
	    		balance++;
	    		help.push(lex(LEX_LPAREN, ""));
	    		break;
	    	case LEX_RPAREN:
	    		balance--;
	    		if (help.empty()) {
	    			cout << "unbalanced parentheses " << i << endl;
	    			error();
	    		}
	    		while (help.top() != LEX_LPAREN) {
	    			y = help.top();
	    			help.pop();
	    			check_types(y);
	    			pol.push_back(y);
	    			if (help.empty()) {
	    				cout << "unbalanced parentheses " << i << endl;
	    				error();
	    			}
	    		}
	    		help.pop();
	    		break;
	    	case LEX_EQ:
	    		push_oper(x);
	    		break;
	    	case LEX_LSS:
	    		push_oper(x);
	    		break;
	    	case LEX_GTR:
	    		push_oper(x);
	    		break;
	    	case LEX_LEQ:
	    		push_oper(x);
	    		break;
	    	case LEX_NEQ:
	    		push_oper(x);
	    		break;
	    	case LEX_GEQ:
	    		push_oper(x);
	    		break;
	    	case LEX_QUOTE:
	    		push_const();
	    		break;
	    	case LEX_CONST:
	    		push_const();
	    		break;
	    	case LEX_FALSE:
	    		push_const();
	    		break;
	    	case LEX_TRUE:
	    		push_const();
	    		break;
	    	case LEX_NOT:
	    		push_oper(x);
	    		break;
	    	case LEX_OR:
	    		push_oper(x);
	    		break;
	    	case LEX_AND:
	    		push_oper(x);
	    		break;
	    	case LEX_ASSIGN:
	    		push_oper(x);
	    		break;
	    	default:
	    		cout << "wrong lex in expression " << i << endl;
	    		error();
	    		break;
	    	}
	    }
	    while (!help.empty()) {
	   		y = help.top();
	   		help.pop();
	   		check_types(y);
	   		pol.push_back(y);
	   	}
	    if (types.empty()) {
	    	return LEX_NULL;
	    }
	    expr_type = types.top();
	    types.pop();
	    if (!types.empty()) {
	    	cout << "excessive operand in expression " << i << endl;
	    	error();
	    }
	    //cout << get_lex_name(expr_type) << endl;
	    return expr_type;
	}
	bool read_bool_null_expr() {
		lex ans = read_expr();
		return ans == LEX_BOOL || ans == LEX_NULL;
	}
	bool read_bool_expr() {
		lex ans = read_expr();
		return ans == LEX_BOOL;
	}

	ident DECLCONST() {
		cout << "DECLCONST" << endl;
		type_of_lex type = get_type();
		lex x = get_lex();
		ident c;
		int k = 0;
		if (type == LEX_UPLUS || type == LEX_UMINUS) {
			k += type == LEX_UMINUS;
			next_lex();
			x = get_lex();
			while (x == LEX_UPLUS || x == LEX_UMINUS) {
				k += x.t_lex == LEX_UMINUS;
				next_lex();
				x = get_lex();
			}
		}
		else if (type == LEX_NOT) {
			k++;
			next_lex();
			x = get_lex();
			while (x == LEX_NOT) {
				k ++;
				next_lex();
				lex x = get_lex();
			}
		}
		c = CONST();
		switch(c.get_type()) {
		case LEX_INT:
			if (k % 2 == 1) {
				int x = get<int>(c.get_value());
				x = -x;
				c.set_value(x);
			}
			break;
		case LEX_REAL:
			if (k % 2 == 1) {
				float x = get<float>(c.get_value());
				x = -x;
				c.set_value(x);
			}
			break;
		case LEX_BOOL:
			if (k % 2 == 1) {
				bool x = get<bool>(c.get_value());
				x = !x;
				c.set_value(x);
			}
			break;
		case LEX_STRING:
		    break;
		default:
			cout << "very very strang DECLCONST error " << i << endl;
			error();
		}
		return c;
	}

	void OPERS() {
		cout << "OPERS" << endl;
        while (read("if") || read("for") || read("while") || read("break") ||
               read("goto") || read("read") || read("write") || read("{") || read("ident")) {
            OPER();       
        }
	}
	
	void OPER() {
		cout << "OPER" << endl;
	    type_of_lex x = get_type();
	    switch(x) {
	    case LEX_IF:
	        read_if();
	        break;
	    case LEX_FOR:
	        read_for();
	        break;
	    case LEX_WHILE:
	        read_while();
	        break;
	    case LEX_BREAK:
	        read_break();
	        break;
	    case LEX_GOTO:
	        read_goto();
	        break;
	    case LEX_READ:
	        read_read();
	        break;
	    case LEX_WRITE:
	        read_write();
	        break;
	    case LEX_LCURLY:
	        read_operc();
	        break;
	    case LEX_IDENT:
	        read_ident();
	        break;
	    case LEX_NULL:
	        break;
	    default:
	        cout << "no operator " << i << endl;
	        error();
	    }
	}

	void read_if() {
		cout << "read_if" << endl;
	    next_lex();
	    if (!read("(")) {
	        cout << "no ( after if " << i << endl;
	        error();
	    }
	    next_lex();
	    if (!read_bool_expr()) {
	        cout << "not a bool expression in if condition " << i << endl;
	        error();
	    }
	    if (!read(")")) {
	        cout << "no ) after if condition " << i << endl;
	        error();
	    }
	    next_lex();
	    lex go (POLIZ_FGO, "");
	    pol.push_back(go);
	    int pos = pol.size() - 1;
	    OPER();
	    pol[pos].n_lex = to_string(pol.size());
	    if (read("else")) {
	    	lex skip_else (POLIZ_GO, "");
	    	pol.push_back(skip_else);
	    	int pos_skip_else = pol.size() - 1;
	    	pol[pos].n_lex = to_string(pol.size());
	        next_lex();
	        OPER();
	        pol[pos_skip_else].n_lex = to_string(pol.size());
	    }
	    return;
	}
	void read_for() {
		cout << "read_for" << endl;
	    next_lex();
	    if (!read("(")) {
	        cout << "no ( after for " << i << endl;
	        error();
	    }
	    next_lex();
	    read_expr();
	    if (!read(";")) {
	        cout << "no ; after 1st for expression " << i << endl;
	        error();
	    }
	    int mark_back = pol.size();
	    next_lex();
	    lex t = read_expr();
	    if (t != LEX_NULL && t != LEX_BOOL) {
	        cout << "not a bool expression in for condition " << i << endl;
	        error();
	    }
	    if (t == LEX_BOOL) {
	    	lex go_forward (POLIZ_FGO, "");
	    	pol.push_back(go_forward);
	    }
	    int pos_end = pol.size() - 1;
	    if (!read(";")) {
	        cout << "no ; after 2nd for expression " << i << endl;
	        error();
	    }
	    next_lex();
	    int start3 = i;
	    int poliz3 = pol.size();
	    read_expr();
	    if (!read(")")) {
	        cout << "no ) after 3rd for expression " << i << endl;
	        error();
	    }
	    while (pol.size() != poliz3) {pol.pop_back();}
	    next_lex();
	    break_vec.push_back(vector<int>());
	    OPER();
	    int oper_end = i;
	    i = start3;
	    read_expr();
	    i = oper_end;
	    lex go_back (POLIZ_GO, to_string(mark_back));
	    pol.push_back(go_back);
	    int mark_forward = pol.size();
	    if (t == LEX_BOOL) {
	    	pol[pos_end].n_lex = to_string(mark_forward);
	    }
	    vector<int> breaks = break_vec.back();
	    for (int &k : breaks) {
	    	pol[k].n_lex = to_string(mark_forward);
	    }
	    break_vec.pop_back();
	    return;
	}
	void read_while() {
		cout << "read_while" << endl;
	    next_lex();
	    if (!read("(")) {
	        cout << "no ( after while " << i << endl;
	        error();
	    }
	    next_lex();
	    int mark_back = pol.size();
	    if (!read_bool_expr()) {
	        cout << "not a bool expression in while condition " << i << endl;
	        error();
	    }
	    lex go_forward (POLIZ_FGO, "");
	    pol.push_back(go_forward);
	    int pos_forward = pol.size() - 1;
	    if (!read(")")) {
	        cout << "no ) after while condition " << i << endl;
	        error();
	    }
	    next_lex();
	    break_vec.push_back(vector<int>());
	    OPER();
	    lex go_back (POLIZ_GO, to_string(mark_back));
	    pol.push_back(go_back);
	    int mark_forward = pol.size();
	    pol[pos_forward].n_lex = to_string(mark_forward);
	    vector<int> breaks = break_vec.back();
	    for (int &k : breaks) {
	    	pol[k].n_lex = to_string(mark_forward);
	    }
	    break_vec.pop_back();
	    return;
	}
	void read_break() {
		cout << "read_break" << endl;
		next_lex();
		if (!read(";")) {
			cout << "no ; after break " << i << endl;
			error();
		}
		next_lex();
		if (break_vec.empty()) {
			cout << "break not in a cycle " << i << endl;
			error();
		}
		pol.push_back(lex(POLIZ_GO, ""));
		break_vec.back().push_back(pol.size() - 1);
	}
	void read_goto() {
		cout << "read_goto" << endl;
		next_lex();
		if (!read("ident")) {
			cout << "no ident after goto " << i << endl;
			error();
		}
		lex x = get_lex();
		ident id = get_ident(x.n_lex);
		next_lex();
		if (id != LEX_NULL) {
			cout << "non-mark identifier used in goto " << i << endl;
			error();
		}
		if (!read(";")) {
			cout << "no ; after goto " << i << endl;
			error(); 
		}
		next_lex();
		id = get_mark(x.n_lex);
		if (id == LEX_NULL) {
			id.set_name(x.n_lex);
			id.set_type(x.t_lex);
			unknown_marks.push_back(id);
		}
		lex go (POLIZ_GO, x.n_lex);
		pol.push_back(go);
	}
	void read_read() {
		cout << "read_read" << endl;
		next_lex();
		if (!read("(")) {
			cout << "no ( after read " << i << endl;
			error();
		}
		next_lex();
		lex x = read_expr();
		if (x == LEX_NULL) {
			cout << "empty read " << i << endl;
			error();
		}
		if (!read(")")) {
			cout << "no ) after read " << i << endl;
			error();
		}
		next_lex();
		if (!read(";")) {
			cout << "no ; after read " << i << endl;
			error();
		}
		next_lex();
		pol.push_back(lex(LEX_READ, "read"));
	}
	void read_write() {
		cout << "read_write" << endl;
		next_lex();
		if (!read("(")) {
			cout << "no ( after read " << i << endl;
			error();
		}
		next_lex();
		while (true) {
			lex x = read_expr();
			if (x == LEX_NULL) {
				cout << "empty expression as write argument " << i << endl;
				error();
			}
			if (read(")")) {
				pol.push_back(lex(LEX_WRITE, "write"));
				break;
			}
			if (!read(",")) {
				cout << "error wrond delimeter in write " << i << endl;
				error();
			}
			pol.push_back(lex(LEX_WRITE, "write"));
			next_lex();
		}
		next_lex();
		if (!read(";")) {
			cout << "no ; after write " << i << endl;
			error();
		}
		next_lex();
	}
	void read_operc() {
		cout << "read_operc" << endl;
	    next_lex();
	    OPERS();
	    if (!read("}")) {
	        cout << "no } after complex operator " << i << endl;
	        error();
	    }
	    next_lex();
	    return;
	}
	void read_ident() {
		cout << "read_ident" << endl;
		lex x = get_lex();
		next_lex();
		if (read(":")) {
			ident m = get_ident(x.n_lex);
			if (m != LEX_NULL) {
				cout << "identifier used as mark " << i << endl;
				error();
			}
			m = get_mark(x.n_lex);
			if (m != LEX_NULL) {
				cout << "mark already declared " << i << endl;
				error();
			}
			next_lex();
			m.set_name(x.n_lex);
			m.set_type(LEX_INT);
			m.set_value((int) pol.size());
			mark.push_back(m);
			OPER();
		}
		else if (read("=")) {
			prev_lex();
			read_expr();
			if (!read(";")) {
				cout << "no ; after expression " << i << endl;
				error();
			}
			next_lex();
		}
		else if (read(";")) {
			pol.push_back(x);
			next_lex();
		}
		else if (read(".")) {
			prev_lex();
			read_expr();
			if (!read(";")) {
				cout << "no ; after expression " << i << endl;
				error();
			}
			next_lex();	
		}
		else {
			cout << "oper-ident error " << i << endl;
			error();
		}
	}
	
};

class interpreter {
public:
	stack<lex> st;
	int i;
	interpreter():i(0) {}

	lex get_lex() {
		return pol[i];
	}
	ident get_arg(lex x) {
		ident id;
		id = get_ident(x.n_lex);
		if (id != LEX_NULL)  {
			//id.print();
			return id;
		}
		id.set_type(x.t_lex);
		switch(x.t_lex) {
		case LEX_STRING:
			id.set_value(x.n_lex);
			break;
		case LEX_REAL:
			id.set_value(stof(x.n_lex));
			break;
		case LEX_INT:
			id.set_value(stoi(x.n_lex));
			break;
		case LEX_BOOL:
			if (x.n_lex == "true") id.set_value(true);
			else                   id.set_value(false);
			break;
		default:
			cout << "error get arg " << i << endl;
			error();
			break;
		}
		//id.print();
		return id;
	}
	void check_init(ident id1, ident id2) {
		if (!id1.is_init) {
			cout << "runtime uninitialised identifier " << i << endl;
			id1.print();
			error();
		}
		if (!id2.is_init) {
			cout << "runtime uninitialised identifier " << i << endl;
			id2.print();
			error();
		}
	}
	void check_init(ident id1) {
		if (!id1.is_init) {
			cout << "runtime uninitialised identifier " << i << endl;
			id1.print();
			error();
		}
	}

	void execute() {
		i = 0;
		lex x = get_lex();
		type_of_lex t = x.t_lex;
		lex y;
		lex oper1;
		lex oper2;
		ident id1;
		ident id2;
		string n1;
		string n2;
		Struct str1;
		Struct str2;
		ident m;
		bool b;
		int    i_val;
		float  f_val;
		bool   b_val;
		string s_val;
		info   inf;
		while (x != LEX_END) {
			//cout << i << endl;
			switch(t) {
			case LEX_IDENT:
				st.push(x);
				break;
			case LEX_INT:
				st.push(x);
				break;
			case LEX_REAL:
				st.push(x);
				break;
			case LEX_STRING:
				st.push(x);
				break;
			case LEX_BOOL:
				st.push(x);
				break;
			case POLIZ_GO:
				m = get_mark(x.n_lex);
				if (m == LEX_NULL) {
					i = stoi(x.n_lex) - 1;
				}
				else {
					i = get<int>(m.get_value()) - 1;
				}
				break;
			case POLIZ_FGO:
				oper1 = st.top();
				st.pop();
				id1 = get_ident(oper1.n_lex);
				if (id1 == LEX_NULL) {
					if (oper1.n_lex == "false")
						i = stoi(x.n_lex) - 1;
				}
				else {
					if (!get<bool>(id1.get_value()))
						i = stoi(x.n_lex) - 1;
				}
				break;
			case LEX_UPLUS:
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				check_init(id1);
				if (id1.get_type() == LEX_INT) {
					y = lex(LEX_INT, to_string(get<int>(id1.get_value())));
					st.push(y);
				}
				else {
					y = lex(LEX_REAL, to_string(get<float>(id1.get_value())));
					st.push(y);
				}
				break;
			case LEX_UMINUS:
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				check_init(id1);
				if (id1.get_type() == LEX_INT) {
					y = lex(LEX_INT, to_string(-get<int>(id1.get_value())));
					st.push(y);
				}
				else {
					y = lex(LEX_REAL, to_string(-get<float>(id1.get_value())));
					st.push(y);
				}
				break;
			case LEX_PLUS:
				oper2 = st.top();
				st.pop();
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				id2 = get_arg(oper2);
				check_init(id1, id2);
				if (id1.get_type() == LEX_INT) {
					if (id2.get_type() == LEX_REAL) {
						y.t_lex = LEX_REAL;
						y.n_lex = to_string(get<int>(id1.get_value()) + get<float>(id2.get_value()));
					}
					else {
						y.t_lex = LEX_INT;
						y.n_lex = to_string(get<int>(id1.get_value()) + get<int>(id2.get_value()));
					}
					st.push(y);
				}
				else if (id1.get_type() == LEX_REAL) {
					y.t_lex = LEX_REAL;
					if (id2.get_type() == LEX_REAL) {
						y.n_lex = to_string(get<float>(id1.get_value()) + get<float>(id2.get_value()));
					}
					else {
						y.n_lex = to_string(get<float>(id1.get_value()) + get<int>(id2.get_value()));
					}
					st.push(y);
				}
				else if (id1.get_type() == LEX_STRING) {
					y.t_lex = LEX_STRING;
					y.n_lex = get<string>(id1.get_value()) + get<string>(id2.get_value());
					st.push(y);
				}
				break;
			case LEX_MINUS:
				oper2 = st.top();
				st.pop();
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				id2 = get_arg(oper2);
				check_init(id1, id2);
				if (id1.get_type() == LEX_INT) {
					if (id2.get_type() == LEX_REAL) {
						y.t_lex = LEX_REAL;
						y.n_lex = to_string(get<int>(id1.get_value()) - get<float>(id2.get_value()));
					}
					else {
						y.t_lex = LEX_INT;
						y.n_lex = to_string(get<int>(id1.get_value()) - get<int>(id2.get_value()));
					}
					st.push(y);
				}
				else if (id1.get_type() == LEX_REAL) {
					y.t_lex = LEX_REAL;
					if (id2.get_type() == LEX_REAL) {
						y.n_lex = to_string(get<float>(id1.get_value()) - get<float>(id2.get_value()));
					}
					else {
						y.n_lex = to_string(get<float>(id1.get_value()) - get<int>(id2.get_value()));
					}
					st.push(y);
				}
				break;
			case LEX_TIMES:
				oper2 = st.top();
				st.pop();
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				id2 = get_arg(oper2);
				check_init(id1, id2);
				if (id1.get_type() == LEX_INT) {
					if (id2.get_type() == LEX_REAL) {
						y.t_lex = LEX_REAL;
						y.n_lex = to_string(get<int>(id1.get_value()) * get<float>(id2.get_value()));
					}
					else {
						y.t_lex = LEX_INT;
						y.n_lex = to_string(get<int>(id1.get_value()) * get<int>(id2.get_value()));
					}
					st.push(y);
				}
				else if (id1.get_type() == LEX_REAL) {
					y.t_lex = LEX_REAL;
					if (id2.get_type() == LEX_REAL) {
						y.n_lex = to_string(get<float>(id1.get_value()) * get<float>(id2.get_value()));
					}
					else {
						y.n_lex = to_string(get<float>(id1.get_value()) * get<int>(id2.get_value()));
					}
					st.push(y);
				}
				break;
			case LEX_DIVIDE:
				oper2 = st.top();
				st.pop();
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				id2 = get_arg(oper2);
				check_init(id1, id2);
				if (id1.get_type() == LEX_INT) {
					if (id2.get_type() == LEX_REAL) {
						if (get<float>(id2.get_value()) == 0.0) {
							cout << "division by zero " << i << endl;
							error();
						}
						y.t_lex = LEX_REAL;
						y.n_lex = to_string(get<int>(id1.get_value()) / get<float>(id2.get_value()));
					}
					else {
						if (get<int>(id2.get_value()) == 0) {
							cout << "division by zero " << i << endl;
							error();
						}
						y.t_lex = LEX_INT;
						y.n_lex = to_string(get<int>(id1.get_value()) / get<int>(id2.get_value()));
					}
					st.push(y);
				}
				else if (id1.get_type() == LEX_REAL) {
					y.t_lex = LEX_REAL;
					if (id2.get_type() == LEX_REAL) {
						if (get<float>(id2.get_value()) == 0.0) {
							cout << "division by zero " << i << endl;
							error();
						}
						y.n_lex = to_string(get<float>(id1.get_value()) / get<float>(id2.get_value()));
					}
					else {
						if (get<int>(id2.get_value()) == 0) {
							cout << "division by zero " << i << endl;
							error();
						}
						y.n_lex = to_string(get<float>(id1.get_value()) / get<int>(id2.get_value()));
					}
					st.push(y);
				}
				break;
			case LEX_MOD:
				oper2 = st.top();
				st.pop();
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				id2 = get_arg(oper2);
				check_init(id1, id2);
				y.t_lex = LEX_INT;
				y.n_lex = to_string(get<int>(id1.get_value()) % get<int>(id2.get_value()));
				st.push(y);
				break;
			case LEX_EQ:
				oper2 = st.top();
				st.pop();
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				id2 = get_arg(oper2);
				check_init(id1, id2);
				y.t_lex = LEX_BOOL;
				if (id1.get_type() == LEX_INT) {
					if (id2.get_type() == LEX_REAL) {
						b = get<int>(id1.get_value()) == get<float>(id2.get_value());
					}
					else {
						b = get<int>(id1.get_value()) == get<int>(id2.get_value());
					}
				}
				else if (id1.get_type() == LEX_REAL) {
					if (id2.get_type() == LEX_REAL) {
						b = get<float>(id1.get_value()) == get<float>(id2.get_value());
					}
					else {
						b = get<float>(id1.get_value()) == get<int>(id2.get_value());
					}
				}
				else if (id1.get_type() == LEX_STRING) {
					b = get<string>(id1.get_value()) == get<string>(id1.get_value());
				}
				if (b) y.n_lex = "true";
				else   y.n_lex = "false";
				st.push(y);
				break;
			case LEX_LSS:
				oper2 = st.top();
				st.pop();
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				id2 = get_arg(oper2);
				check_init(id1, id2);
				y.t_lex = LEX_BOOL;
				if (id1.get_type() == LEX_INT) {
					if (id2.get_type() == LEX_REAL) {
						b = get<int>(id1.get_value()) < get<float>(id2.get_value());
					}
					else {
						b = get<int>(id1.get_value()) < get<int>(id2.get_value());
					}
				}
				else if (id1.get_type() == LEX_REAL) {
					if (id2.get_type() == LEX_REAL) {
						b = get<float>(id1.get_value()) < get<float>(id2.get_value());
					}
					else {
						b = get<float>(id1.get_value()) < get<int>(id2.get_value());
					}
				}
				else if (id1.get_type() == LEX_STRING) {
					b = get<string>(id1.get_value()) < get<string>(id1.get_value());
				}
				if (b) y.n_lex = "true";
				else   y.n_lex = "false";
				st.push(y);
				break;
			case LEX_GTR:
				oper2 = st.top();
				st.pop();
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				id2 = get_arg(oper2);
				check_init(id1, id2);
				y.t_lex = LEX_BOOL;
				if (id1.get_type() == LEX_INT) {
					if (id2.get_type() == LEX_REAL) {
						b = get<int>(id1.get_value()) > get<float>(id2.get_value());
					}
					else {
						b = get<int>(id1.get_value()) > get<int>(id2.get_value());
					}
				}
				else if (id1.get_type() == LEX_REAL) {
					if (id2.get_type() == LEX_REAL) {
						b = get<float>(id1.get_value()) > get<float>(id2.get_value());
					}
					else {
						b = get<float>(id1.get_value()) > get<int>(id2.get_value());
					}
				}
				else if (id1.get_type() == LEX_STRING) {
					b = get<string>(id1.get_value()) > get<string>(id1.get_value());
					st.push(y);
				}
				if (b) y.n_lex = "true";
				else   y.n_lex = "false";
				st.push(y);
				break;
			case LEX_NEQ:
				oper2 = st.top();
				st.pop();
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				id2 = get_arg(oper2);
				check_init(id1, id2);
				y.t_lex = LEX_BOOL;
				if (id1.get_type() == LEX_INT) {
					if (id2.get_type() == LEX_REAL) {
						b = get<int>(id1.get_value()) != get<float>(id2.get_value());
					}
					else {
						b = get<int>(id1.get_value()) != get<int>(id2.get_value());
					}
				}
				else if (id1.get_type() == LEX_REAL) {
					if (id2.get_type() == LEX_REAL) {
						b = get<float>(id1.get_value()) != get<float>(id2.get_value());
					}
					else {
						b = get<float>(id1.get_value()) != get<int>(id2.get_value());
					}
				}
				else if (id1.get_type() == LEX_STRING) {
					b = get<string>(id1.get_value()) != get<string>(id1.get_value());
					st.push(y);
				}
				if (b) y.n_lex = "true";
				else   y.n_lex = "false";
				st.push(y);
				break;
			case LEX_LEQ:
				oper2 = st.top();
				st.pop();
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				id2 = get_arg(oper2);
				check_init(id1, id2);
				y.t_lex = LEX_BOOL;
				if (id1.get_type() == LEX_INT) {
					if (id2.get_type() == LEX_REAL) {
						b = get<int>(id1.get_value()) <= get<float>(id2.get_value());
					}
					else {
						b = get<int>(id1.get_value()) <= get<int>(id2.get_value());
					}
				}
				else if (id1.get_type() == LEX_REAL) {
					if (id2.get_type() == LEX_REAL) {
						b = get<float>(id1.get_value()) <= get<float>(id2.get_value());
					}
					else {
						b = get<float>(id1.get_value()) <= get<int>(id2.get_value());
					}
				}
				if (b) y.n_lex = "true";
				else   y.n_lex = "false";
				st.push(y);
				break;
			case LEX_GEQ:
				oper2 = st.top();
				st.pop();
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				id2 = get_arg(oper2);
				check_init(id1, id2);
				y.t_lex = LEX_BOOL;
				if (id1.get_type() == LEX_INT) {
					if (id2.get_type() == LEX_REAL) {
						b = get<int>(id1.get_value()) >= get<float>(id2.get_value());
					}
					else {
						b = get<int>(id1.get_value()) >= get<int>(id2.get_value());
					}
				}
				else if (id1.get_type() == LEX_REAL) {
					if (id2.get_type() == LEX_REAL) {
						b = get<float>(id1.get_value()) >= get<float>(id2.get_value());
					}
					else {
						b = get<float>(id1.get_value()) >= get<int>(id2.get_value());
					}
				}
				if (b) y.n_lex = "true";
				else   y.n_lex = "false";
				st.push(y);
				break;
			case LEX_NOT:
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				check_init(id1);
				y.t_lex = LEX_BOOL;
				b = !get<bool>(id1.get_value());
				if (b) y.n_lex = "true";
				else   y.n_lex = "false";
				st.push(y);
				break;
			case LEX_OR:
				oper2 = st.top();
				st.pop();
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				id2 = get_arg(oper2);
				check_init(id1, id2);
				y.t_lex = LEX_BOOL;
				b = get<bool>(id1.get_value()) || get<bool>(id2.get_value());
				if (b) y.n_lex = "true";
				else   y.n_lex = "false";
				st.push(y);
				break;
			case LEX_AND:
				oper2 = st.top();
				st.pop();
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				id2 = get_arg(oper2);
				check_init(id1, id2);
				y.t_lex = LEX_BOOL;
				b = get<bool>(id1.get_value()) && get<bool>(id2.get_value());
				if (b) y.n_lex = "true";
				else   y.n_lex = "false";
				st.push(y);
				break;
			case LEX_WRITE:
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				check_init(id1);
				switch(id1.get_type()) {
				case LEX_INT:
					cout << get<int>(id1.get_value());
					break;
				case LEX_REAL:
					cout << get<float>(id1.get_value());
					break;
				case LEX_STRING:
					cout << (string) get<string>(id1.get_value());
					break;
				case LEX_BOOL:
					cout << get<bool>(id1.get_value());
					break;
				default:
					cout << "runtime wrong write agrument " << i << endl;
					error();
				}
				break;
			case LEX_READ:
				oper1 = st.top();
				st.pop();
				id1 = get_arg(oper1);
				switch(id1.get_type()) {
				case LEX_INT:
					cin >> i_val;
					if (!cin.fail()) {
						id1.set_value(i_val);
					}
					else {
						cout << "runtime wrong read input, expected int " << i << endl;
						error();
					}
					break;
				case LEX_REAL:
					cin >> f_val;
					if (!cin.fail()) {
						id1.set_value(f_val);
					}
					else {
						cout << "runtime wrong read input, expected float " << i << endl;
						error();
					}

					break;
				case LEX_STRING:
					cin >> s_val;
					if (!cin.fail()) {
						id1.set_value(s_val);
					}
					else {
						cout << "runtime wrong read input, expected string " << i << endl;
						error();
					}

					break;
				case LEX_BOOL:
					cin >> b_val;
					if (!cin.fail()) {
						id1.set_value(b_val);
					}
					else {
						cout << "runtime wrong read input, expected bool " << i << endl;
						error();
					}
					break;
				default:
					cout << "runtime wrong read agrument " << i << endl;
					error();
					break;
				}
				for (int j = 0; j < var.size(); j++) {
					if (var[j] == id1.get_name()) {
						var[j] = id1;
					}
				}
				//print_var();
				break;
			case LEX_ASSIGN:
				oper2 = st.top();
				st.pop();
				oper1 = st.top();
				st.pop();
				if (oper1 != LEX_IDENT) {
					cout << "runtime assigning to none-ident " << i << endl;
					error();
				}
				id1 = get_arg(oper1);
				id2 = get_arg(oper2);
				if (id2 != "") {
					if (id1.get_type() != LEX_STRUCT) {
						check_init(id2);
						id1.set_value(id2.get_value());
						st.push(oper1);
						for (int j = 0; j < var.size(); j++) {
							if (var[j] == id1.get_name()) {
								var[j] = id1;
							}
						}
						break;
					}
					if (id1.get_struct() != id2.get_struct()) {
						cout << "error not matching struct types " << i << endl;
						error();
					}
					n1 = id1.get_name();
					n2 = id2.get_name();
					for (auto &it : var) {
						b = true;
						string it_name = it.get_name();
						int k;
						//cout << n1 << " " << n2 << " " << it_name << endl;
						if (it_name.size() <= n1.size()) {
							//cout << "cont size" << endl;
							continue;
						}
						for (k = 0; k < it_name.size() && k < n1.size(); k++) {
							if (it_name[k] == '.') {
								break;
							}
							if (it_name[k] != n1[k]) {
								b = false;
							}
						}
						if (!b) {
							//cout << "cont cond" << endl;
							continue;
						}
						else {
							//cout << "in else" << endl;
							string n11 = n1 + "." + it_name.substr(k + 1);
							string n21 = n2 + "." + it_name.substr(k + 1);
							//cout << n11 << " " << n21 << endl;
							ident id11 = get_ident(n11);
							ident id21 = get_ident(n21);
							if (id11 == LEX_NULL || id21 == LEX_NULL) {
								cout << "giga error struct = " << i << endl;
								error();
							}
							for (int j = 0; j < var.size(); j++) {
								if (var[j] == id11.get_name()) {
									var[j].set_value(id21.get_value());
								}
							}
						}
					}
					break;
				}
				switch(id1.get_type()) {
				case LEX_INT:
					id1.set_value(stoi(oper2.n_lex));
					break;
				case LEX_REAL:
					id1.set_value(stof(oper2.n_lex));
					break;
				case LEX_STRING:
					id1.set_value(oper2.n_lex);
					break;
				case LEX_BOOL:
					if (oper2.n_lex == "true") id1.set_value(true);
					else                       id1.set_value(false);
					break;
				default:
					cout << "runtime very strange assign error " << i << endl;
					break;
				}
				for (int j = 0; j < var.size(); j++) {
					if (var[j] == id1.get_name()) {
						var[j] = id1;
					}
				}
				st.push(oper1);
				//print_var();
				break;
			/*
			case LEX_IDENT:
				break;
			*/
			default:
				cout << "error unknown poliz element " << i << endl;
				error();
				break;
			}
			i++;
			x = get_lex();
			t = x.t_lex;
		}
	}
};

int main() {
	lexicalAnalyser LA;
	syntSemAnalyserGenerator SSAG;
	interpreter I;
	while(true) {
		cout << "Enter file name or \"stop\" " << endl;
		string name;
		cin >> name;
		if (cin.fail()) {
			cout << "Wrong name" << endl;;
			break;
		}
		if (name == "stop") {
			break;
		}
		f.open(name);
		if (f.is_open()) {
			try {
				LA.analyse();
				cout << "lex:" << endl;
				print_lex(l);
				SSAG.START();
				print_var();
				print_mark();
				cout << "pol:" << endl;
				print_lex(pol);
				cout << endl << "interpretation:" << endl;
				I.execute();
				cout << endl << "interpretation finished" << endl;
			}
			catch(int) {}
			f.close();
			clear();
		}
		else {
			cout << "Wrong name" << endl;
		}
	}

	return 0;
}
