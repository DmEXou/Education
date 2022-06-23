#include <string>
#include <string_view>
#include <sstream>
#include <iostream>

class HaserStringStream
{
public:
	HaserStringStream(std::stringstream& ss)
		:ss_(ss)
	{
		final_hash_ = Haser();
	}

	int Give_Hash() {
		return final_hash_;
	}

private:
	int Haser() {
		int hash = 1;
		for (const auto& ch : ss_.str()) {
			char32_t cht(ch);
			hash *= halper(cht);
		}
		if (hash < 0) {
			hash = hash * -1;
		}
		return hash;
	}

	int halper(int val) {
		if (val <= 100) {
			val += 100;
		}
		if (val % 10 > 5) {
			val *= (val * 52) % 1000;
		}
		else {
			val *= (val * 48) % 1000;
		}
		return val;
	}
private:
	int final_hash_ = 0;
	std::stringstream& ss_;
};

void StringValidityTest(std::string_view strv) {
	if (strv.length() > 16) {
		throw std::invalid_argument("to long string");
	}
	else if(strv.length() <= 4) {
		throw std::invalid_argument("to short string");
	}
}

int main(){
	using namespace std::literals;
	std::string hash_variable;
	std::cin >> hash_variable;
	try {
		StringValidityTest(hash_variable);
	} catch (std::invalid_argument& ia) {
		std::cerr << "No valid" << ' ' << ia.what() << std::endl;
		return 0;
	}
	std::stringstream str(hash_variable);
	HaserStringStream hash(str);
	std::cout << hash.Give_Hash();
	std::stringstream stra("12345"s);
	std::stringstream strb("12346"s);
	HaserStringStream a(stra);
	HaserStringStream b(strb);
	std::cout << (a.Give_Hash() == b.Give_Hash());
}