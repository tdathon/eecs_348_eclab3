#include <iostream>
#include <iomanip>
using namespace std;

class Account {
	// base account class
	public:
		std::string accountNumber;
		std::string accountHolder;
		double balance;

	public:
		Account(const std::string& number, const std::string& holder, double initialBalance)
			: accountNumber(number), accountHolder(holder), balance(initialBalance) {}

		void displayDetails() const {
			std::cout << "Account Details for " << getType() << " (ID: " << accountNumber << "):" << std::endl;
			std::cout << "   Holder: " << accountHolder << std::endl;
			std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
			displayExtraDetails();
			std::cout << std::endl;
		}

		virtual std::string getType() const {
			// used for std::cout later to load type dynamically
			return "Account";
		}

		virtual void displayExtraDetails() const {}
		// will be defined later to show interest rate or overdraft amount

		void deposit(double amount) {
			balance += amount;
		}

		virtual void withdraw(double amount) {
			if (amount <= balance) {
				balance -= amount;
			} else {
				std::cout << "Insufficient funds for withdrawal.\n";
			}
		}

		friend Account operator+(const Account& lhs, const Account& rhs);
};

// overload + operator
Account& operator+(Account& lhs, Account& rhs) {
	double transferAmount = 300.00;
	if (transferAmount <= lhs.balance) {
		rhs.withdraw(transferAmount);
		lhs.deposit(transferAmount);
		std::cout << "Details after transfer:\n";
	} else {
		std::cout << "Unable to complete transfer. Balance too low.\n";
	}
	return rhs;
}

// overload << operator
std::ostream& operator<<(std::ostream& os, const Account& account) {
	account.displayDetails();
	return os;
}

class SavingsAccount : public Account {
	private:
		double interestRate;

	public:
		SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
			: Account(number, holder, initialBalance), interestRate(rate) {}

		std::string getType() const override {
			return "Savings Account";
		}

		void displayExtraDetails() const override {
			std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << interestRate * 100 << "%" << std::endl;
		}

		void withdraw(double amount) override {
			const double minimumBalance = 100; // Assuming a minimum balance of $100 for savings account
			if (balance - amount >= minimumBalance) {
				balance -= amount;
			} else {
				std::cout << "Withdrawal failed. Minimum balance requirement not met." << std::endl;
			}
		}
};

class CurrentAccount : public Account {
	public:
		double overdraftLimit;

	public:
		CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
			: Account(number, holder, initialBalance), overdraftLimit(limit) {}

		std::string getType() const override {
			return "Current Account";
		}

		void displayExtraDetails() const override {
			std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << std::endl;
		}

		void withdraw(double amount) override {
			if (balance + overdraftLimit >= amount) {
				balance -= amount;
			} else {
				std::cout << "Withdrawal failed. Overdraft limit exceeded." << std::endl;
			}
		}

};


int main() {
	SavingsAccount savings("S123", "John Doe", 1000, 0.02);
	CurrentAccount current("C456", "Jane Doe", 2000, 500);

	savings.displayDetails();
	current.displayDetails();

	savings.deposit(500);
	current.withdraw(1000);
	
	savings.displayDetails();
	current.displayDetails();

	// Transfer 300 from savings to current
	current + savings;

	savings.displayDetails();
	current.displayDetails();

	return 0;
}
