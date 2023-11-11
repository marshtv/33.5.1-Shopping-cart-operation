#include <iostream>
#include <string>
#include <map>
#include <exception>

class KeyAlreadyExistException : public std::exception {
	const char* what() const noexcept override {
		return "Key Already exist in DB";
	}
};

class KeyNotExistException : public std::exception {
	const char* what() const noexcept override {
		return "Key Not Exist in DB";
	}
};

class NotEnoughQuantityException : public std::exception {
	const char* what() const noexcept override {
		return "Not enough quantity in DB";
	}
};

void add_to_DB(const std::string& _article, const int& _quantity,
						std::map<std::string, int> &dict) {
	if (dict.count(_article) > 0) {
		throw KeyAlreadyExistException();
	}
	if (_quantity < 0) {
		throw std::invalid_argument("quantity");
	}
	dict.insert(std::make_pair(_article, _quantity));
}

template<typename T>
T input_value() {
	T _value;
	std::cin >> _value;
	return _value;
}

void add_to_cart(const std::string& _str, const int& _quantity,
				 std::map<std::string, int>& _dict,
				 std::map<std::string, int>& _cart) {
	if (_dict.count(_str) == 0) {
		throw KeyNotExistException();
	} else if (_quantity <= 0) {
		throw std::invalid_argument("quantity");
	} else if (_quantity > _dict[_str]) {
		throw NotEnoughQuantityException();
	} else {
		_dict[_str] -= _quantity;
		if (_dict[_str] == 0) {
			std::cout << _str << " next time won't be available in DB." << std::endl;
			_dict.erase(_str);
		} else
			std::cout << _str << " left: " << _dict[_str] << std::endl;
		if (!_cart.empty()) {
			if (_cart.count(_str) > 0) {
				_cart[_str] += _quantity;
			} else
				_cart.insert(std::make_pair(_str, _quantity));
		} else
			_cart.insert(std::make_pair(_str, _quantity));
	}
}

int main() {
	std::map<std::string, int> oGoods, oCart;

	std::cout << "Input Article (or \"-1\" for exit) & Quantity of Goods:" << std::endl;
	std::cout << "--------------------------------------------" << std::endl;
	bool input_1 = true;
	while (input_1) {
		bool input_2 = true;
		while (input_2) {
			std::cout << "Article:";
			auto _article = input_value<std::string>();
			if (_article == "-1") {
				input_1 = false;
				break;
			}
			std::cout << "Quantity:";
			auto _quantity = input_value<int>();
			try {
				add_to_DB(_article, _quantity, oGoods);
				input_2 = false;
			}
			catch(const std::invalid_argument& ex) {
				std::cerr << "Invalid argument supplied: " << ex.what() << std::endl;
			}
			catch(const KeyAlreadyExistException& ex) {
				std::cerr << "Key already exist" << std::endl;
			}
			catch(...) {
				std::cerr << "Some kind of exception happened..." << std::endl;
			}
		}
	}
	std::cout << "\nDataBase complete." << std::endl << std::endl;

	for (auto it = oGoods.begin(); it != oGoods.end(); ++it) {
		std::cout << it->first << " -> " << it->second << std::endl;
	}
	std::cout << "--------------------------------------------" << std::endl;

	std::cout << "Now you can pay any goods \nby input Article (or \"-1\" for exit) & Quantity to cart:" << std::endl;
	input_1 = true;
	while (input_1) {
		bool input_2 = true;
		while (input_2) {
			std::cout << "Article:";
			auto _article = input_value<std::string>();
			if (_article == "-1") {
				input_1 = false;
				break;
			}
			std::cout << "Quantity:";
			auto _quantity = input_value<int>();
			try {
				add_to_cart(_article, _quantity, oGoods, oCart);
				input_2 = false;
			}
			catch(const std::invalid_argument& ex) {
				std::cerr << "Invalid argument supplied: " << ex.what() << std::endl;
			}
			catch(const KeyNotExistException& ex) {
				std::cerr << "Key not exist in DB" << std::endl;
			}
			catch(const NotEnoughQuantityException& ex) {
				std::cerr <<"Not enough quantity" << std::endl;
			}
			catch(...) {
				std::cerr << "Some kind of exception happened..." << std::endl;
			}
		}
	}
	std::cout << "Cart complete!" << std::endl;

	if (!oCart.empty()) {
		std::cout << "You Pay:" << std::endl;
		for (auto it = oCart.begin(); it != oCart.end(); ++it) {
			std::cout << it->first << " -> " << it->second << std::endl;
		}
	} else
		std::cout << "Cart is EMPTY." << std::endl;
	std::cout << "Goods left in DB:" << std::endl;
	if (!oGoods.empty()) {
		for (auto it = oGoods.begin(); it != oGoods.end(); ++it) {
			std::cout << it->first << " -> " << it->second << std::endl;
		}
	} else
		std::cout << "DB is EMPTY." << std::endl;
	std::cout << "\nSee You! Good by!" << std::endl;
}
