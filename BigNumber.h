#ifndef BIGNUMBER_H
#define BIGNUMBER_H

#include <iostream>
#include <string>
#include <array>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <sstream>

namespace Big_Number
{

	class DivisionByZero: public std::invalid_argument
	{
		public:
			explicit DivisionByZero(const std::string& msg): std::invalid_argument(msg) {}
	};

	class VeryLongString: public std::invalid_argument
	{
		public:
			explicit VeryLongString(const std::string& msg): std::invalid_argument(msg) {}
	};

	class NegativePower: public std::invalid_argument
	{
		public:
			explicit NegativePower(const std::string& msg): std::invalid_argument(msg) {}
	};

	template <std::size_t k, typename = typename std::enable_if<k != 0>::type>
	class BigNumber
	{
		std::array<unsigned char, k> digits;
		std::size_t end;
		bool sign;

		public:
			static constexpr std::size_t size = k;
			BigNumber(): end(1), sign(true) { digits[0] = 0; }
			BigNumber(const std::string&);
			BigNumber(const char* ca): BigNumber(std::string(ca)) {}
			BigNumber(int n): BigNumber(std::to_string(n)) {} //This is required for "BigNumber(0)"
			/*All of the following integral constructors are required (not just one of them) because otherwise calls to BigNumber constructor with
			certain integral types would have been ambiguous.*/
			BigNumber(unsigned n): BigNumber(std::to_string(n)) {}
			BigNumber(long n): BigNumber(std::to_string(n)) {}
			BigNumber(unsigned long n): BigNumber(std::to_string(n)) {}
			BigNumber(long long n): BigNumber(std::to_string(n)) {}
			BigNumber(unsigned long long n): BigNumber(std::to_string(n)) {}

			BigNumber(const BigNumber&);
			
			unsigned char& operator[](std::size_t i) { return digits[i]; }
			const unsigned char& operator[](std::size_t i) const { return digits[i]; }
			unsigned char& at(std::size_t);
			const unsigned char& at(std::size_t) const;
			BigNumber& operator=(const BigNumber&);
			BigNumber& operator+=(const BigNumber&);
			BigNumber& operator<<=(std::size_t);
			BigNumber& operator*=(const BigNumber&);
			BigNumber& operator>>=(std::size_t);
			BigNumber& operator/=(const BigNumber&);
			BigNumber operator-() const;
			BigNumber& operator-=(const BigNumber&);
			BigNumber& operator%=(const BigNumber&);
			BigNumber& operator^=(const BigNumber&);
			BigNumber& operator++() { return *this += big1; }
			BigNumber& operator--() { return *this -= big1; }
			BigNumber operator++(int);
			BigNumber operator--(int);

		private:
			static const BigNumber big0;
			static const BigNumber big1;
			static const BigNumber big2;

			BigNumber mult_digit(unsigned char) const; //accepts only a digit.
			void discard_leading_zeros();

		//Friends are defined here to enable users of the class to write expressions like 'BigNumber<20>("3849") + 67'.
		//But this has a consequence: These friend functions can only be found using argument dependent lookup.
		//For this class, this problem is not important, as you will probably never write expressions like 'std::string("3849") / std::string("345")'!
		friend BigNumber operator<<(const BigNumber& lhs, std::size_t n)
		{
			BigNumber ret = lhs;
			ret <<= n;
			return ret;
		}

		friend BigNumber operator+(const BigNumber& lhs, const BigNumber& rhs)
		{
			BigNumber temp = lhs;
			temp += rhs;
			return temp;
		}

		friend BigNumber operator*(const BigNumber& lhs, const BigNumber& rhs)
		{
			BigNumber ret = lhs;
			ret *= rhs;
			return ret;
		}

		friend BigNumber operator>>(const BigNumber& lhs, std::size_t n)
		{
			BigNumber ret = lhs;
			ret >>= n;
			return ret;
		}

		friend BigNumber operator/(const BigNumber& lhs, const BigNumber& rhs)
		{
			BigNumber ret = lhs;
			ret /= rhs;
			return ret;
		}

		friend BigNumber operator-(const BigNumber& lhs, const BigNumber& rhs)
		{
			BigNumber ret = lhs;
			ret -= rhs;
			return ret;
		}

		friend BigNumber operator%(const BigNumber& lhs, const BigNumber& rhs)
		{
			BigNumber ret = lhs;
			ret %= rhs;
			return ret;
		}

		friend BigNumber abs(const BigNumber& bn)
		{
			if (bn.sign)
				return bn;
			else
				return -bn;
		}

		friend BigNumber max(const BigNumber& bn1, const BigNumber& bn2)
		{
			if (bn1 < bn2)
				return bn2;
			else
				return bn1;
		}

		friend BigNumber min(const BigNumber& bn1, const BigNumber& bn2)
		{
			if (bn1 < bn2)
				return bn1;
			else
				return bn2;
		}

		friend bool is_even(const BigNumber& bn)
		{
			return bn.digits[0] % 2 == 0;
		}

		friend bool is_odd(const BigNumber& bn)
		{
			return !is_even(bn);
		}

		friend bool is_positive(const BigNumber& bn)
		{
			return bn.sign;
		}

		friend bool is_negative(const BigNumber& bn)
		{
			return !is_positive(bn);
		}

		friend BigNumber operator^(const BigNumber& lhs, const BigNumber& rhs)
		{
			BigNumber ret = lhs;
			ret ^= rhs;
			return ret;
		}

		friend bool operator<(const BigNumber& lhs, const BigNumber& rhs)
		{
			if (!lhs.sign && rhs.sign)
				return true;
			else if (lhs.sign && !rhs.sign)
				return false;
			else if (!lhs.sign && !rhs.sign)
				return -rhs < -lhs;
			else
			{
				if (lhs.end < rhs.end)
					return true;
				else if (lhs.end > rhs.end)
					return false;
				else
				{
					for (std::size_t i = lhs.end; i-- != 0; )
					{
						if (lhs.digits[i] < rhs.digits[i])
							return true;
						else if (rhs.digits[i] < lhs.digits[i])
							return false;
					}
					return false;
				}
			}
		}

		friend bool operator>(const BigNumber& lhs, const BigNumber& rhs)
		{
			return rhs < lhs;
		}

		friend bool operator<=(const BigNumber& lhs, const BigNumber& rhs)
		{
			return !(rhs < lhs);
		}

		friend bool operator>=(const BigNumber& lhs, const BigNumber& rhs)
		{
			return !(lhs < rhs);
		}

		friend bool operator==(const BigNumber& lhs, const BigNumber& rhs)
		{
			return !(lhs < rhs) && !(rhs < lhs);
		}

		friend bool operator!=(const BigNumber& lhs, const BigNumber& rhs)
		{
			return !(lhs == rhs);
		}

		friend std::ostream& operator<<(std::ostream& os, const BigNumber& bn)
		{
			if (!bn.sign)
				os<<'-';
			for (std::size_t i = bn.end; i-- != 0;)
				os<<(unsigned)(bn.digits[i]);
			return os;
		}

		friend std::istream& operator>>(std::istream& is, BigNumber& bn)
		{
			std::string s;
			is>>s;
			bn = BigNumber(s);
			return is;
		}
	};

	template<std::size_t k, typename T>
	constexpr std::size_t BigNumber<k, T>::size;

	template<std::size_t k, typename T>
	const BigNumber<k, T> BigNumber<k, T>::big0;

	template<std::size_t k, typename T>
	const BigNumber<k, T> BigNumber<k, T>::big1("1");

	template<std::size_t k, typename T>
	const BigNumber<k, T> BigNumber<k, T>::big2("2");

	template <std::size_t k, typename T>
	BigNumber<k, T>::BigNumber(const std::string& s): sign(true)
	{
		if (s.empty() || (s.size() == 1 && (s[0] == '-' || s[0] == '+')))
		{
			end = 1;
			digits[0] = 0;
			return;
		}
		std::size_t pos = 0;
		if (s[0] == '-')
		{
			end = s.size() - 1;
			sign = false;
			pos = 1;
		}
		else if (s[0] == '+')
		{
			end = s.size() - 1;
			pos = 1;
		}
		else
			end = s.size();
		if (end > size)
			throw VeryLongString("Error: Input string contains more than " + std::to_string(size) + " digits.");
		for (std::size_t i = s.size(); i-- != pos; )
			digits[s.size() - i - 1] = s[i] - '0';
		discard_leading_zeros();
	}

	template <std::size_t k, typename T>
	inline BigNumber<k, T>::BigNumber(const BigNumber& rhs): end(rhs.end), sign(rhs.sign)
	{
		std::copy(rhs.digits.begin(), rhs.digits.begin() + rhs.end, digits.begin());
	}

	template <std::size_t k, typename T>
	inline unsigned char& BigNumber<k, T>::at(std::size_t i)
	{
		if (i >= end)
			throw std::out_of_range("Error: Index " + std::to_string(i) + " is larger than the largest possible index for this number (" + std::to_string(end - 1) + ").");
		return digits[i];
	}

	template <std::size_t k, typename T>
	inline const unsigned char& BigNumber<k, T>::at(std::size_t i) const
	{
		if (i >= end)
			throw std::out_of_range("Error: Index " + std::to_string(i) + " is larger than the largest possible index for this number (" + std::to_string(end - 1) + ").");
		return digits[i];
	}

	template <std::size_t k, typename T>
	BigNumber<k, T>& BigNumber<k, T>::operator=(const BigNumber& rhs)
	{
		if (&rhs != this)
		{
			std::copy(rhs.digits.begin(), rhs.digits.begin() + rhs.end, digits.begin());
			end = rhs.end;
			sign = rhs.sign;
		}
		return *this;
	}

	template <std::size_t k, typename T>
	BigNumber<k, T>& BigNumber<k, T>::operator+=(const BigNumber& rhs)
	{
		if (this->sign != rhs.sign)
		{
			if (this->sign)
				*this -= -rhs;
			else
				*this = rhs - -*this;
		}
		else
		{
			unsigned char carry = 0;
			for (std::size_t i = 0; i < end && i < rhs.end; ++i)
			{
				digits[i] = digits[i] + rhs.digits[i] + carry;
				carry = digits[i] / 10;
				digits[i] %= 10;
			}
			std::size_t i = end;
			if (end > rhs.end)
			{
				for (i = rhs.end; i < end; ++i)
				{
					digits[i] = digits[i] + carry;
					carry = digits[i] / 10;
					digits[i] %= 10;
				}
			}
			else if (rhs.end > end)
			{
				for (i = end; i < rhs.end; ++i)
				{
					digits[i] = rhs.digits[i] + carry;
					carry = digits[i] / 10;
					digits[i] %= 10;
				}
			}
			if (carry && i < size)
			{
				digits[i] = carry;
				end = i + 1;
			}
			else
				end = i;
		}
		return *this;
	}

	template <std::size_t k, typename T>
	BigNumber<k, T>& BigNumber<k, T>::operator<<=(std::size_t n)
	{
		if (n == 0)
			return *this;
		if (n >= size)
			return *this = big0;
		for (std::size_t i = end + n - 1; i != n - 1; --i)
		{
			if (i < size)
				digits[i] = digits[i - n];
		}
		for (std::size_t i = 0; i < n; ++i)
			digits[i] = 0;
		end = std::min(end + n, size);
		discard_leading_zeros();
		return *this;
	}

	template <std::size_t k, typename T>
	BigNumber<k, T>& BigNumber<k, T>::operator*=(const BigNumber& rhs)
	{
		BigNumber product;
		bool initial_this_sign = this->sign;
		bool initial_rhs_sign = rhs.sign; //because rhs can be *this!
		for (std::size_t i = rhs.end; i-- != 0; )
		{
			product <<= 1;
			product += this->mult_digit(rhs.digits[i]);
		}
		*this = std::move(product);
		if (initial_rhs_sign != initial_this_sign && *this != big0)
			this->sign = false;
		return *this;
	}

	template <std::size_t k, typename T>
	BigNumber<k, T>& BigNumber<k, T>::operator>>=(std::size_t n)
	{
		if (n == 0)
			return *this;
		if (end <= n)
			return *this = big0;
		for (std::size_t i = 0; i < end - n; ++i)
			digits[i] = digits[i + n];
		end -= n;
		return *this;
	}

	template <std::size_t k, typename T>
	BigNumber<k, T>& BigNumber<k, T>::operator/=(const BigNumber& rhs)
	{
		if (rhs == big0)
			throw DivisionByZero("Error: Division by zero.");
		bool initial_this_sign = this->sign;
		bool initial_rhs_sign = rhs.sign; //because rhs can be *this!
		std::size_t initial_shift;
		if (end <= rhs.end)
			initial_shift = 0;
		else
			initial_shift = end - rhs.end;
		BigNumber denom = rhs << initial_shift;
		denom.sign = true;
		BigNumber num = std::move(*this);
		num.sign = true;
		BigNumber q;
		for (std::size_t i = 0; i <= initial_shift; ++i)
		{
			unsigned char q_digit = 0;
			while (num >= denom)
			{
				num -= denom;
				++q_digit;
			}
			q <<= 1;
			q.digits[0] = q_digit;
			denom >>= 1;
		}
		*this = std::move(q);
		if (initial_this_sign != initial_rhs_sign && *this != big0)
			this->sign = false;
		return *this;
	}

	template <std::size_t k, typename T>
	inline BigNumber<k, T> BigNumber<k, T>::operator-() const
	{
		if (*this == big0)
			return big0;
		BigNumber ret = *this;
		if (ret.sign)
			ret.sign = false;
		else
			ret.sign = true;
		return ret;
	}

	template <std::size_t k, typename T>
	BigNumber<k, T>& BigNumber<k, T>::operator-=(const BigNumber& rhs)
	{
		if (this->sign != rhs.sign)
		{
			bool final_sign = this->sign;
			this->sign = true;
			*this += abs(rhs);
			this->sign = final_sign;
		}
		else
		{
			bool final_sign;
			const BigNumber* mi;
			const BigNumber* ma;
			if (*this >= rhs)
			{
				final_sign = true;
				if (!this->sign)
				{
					mi = this;
					ma = &rhs;
				}
				else
				{
					mi = &rhs;
					ma = this;
				}
			}
			else
			{
				final_sign = false;
				if (this->sign)
				{
					mi = this;
					ma = &rhs;
				}
				else
				{
					mi = &rhs;
					ma = this;
				}
			}
			bool borrow = false;
			for (std::size_t i = 0; i < ma->end; ++i)
			{
				unsigned char sub = borrow;
				if (i < mi->end)
					sub += mi->digits[i];
				if (sub > ma->digits[i])
				{
					borrow = true;
					this->digits[i] = 10 + ma->digits[i] - sub;
				}
				else
				{
					borrow = false;
					this->digits[i] = ma->digits[i] - sub;
				}
			}
			this->end = ma->end;
			discard_leading_zeros();
			this->sign = final_sign;
		}
		return *this;
	}

	template <std::size_t k, typename T>
	inline BigNumber<k, T>& BigNumber<k, T>::operator%=(const BigNumber& rhs)
	{
		*this -= (*this / rhs) * rhs;
		return *this;
	}

	template <std::size_t k, typename T>
	BigNumber<k, T>& BigNumber<k, T>::operator^=(const BigNumber& rhs)
	{
		if (!rhs.sign)
			throw NegativePower("Error: Power is negative.");
		if (rhs == big0)
			return *this = big1;
		if (rhs == big1)
			return *this;
		if (is_even(rhs))
		{
			*this ^= rhs / big2;
			*this *= *this;
		}
		else
		{
			BigNumber temp = *this; //because rhs can be *this!
			*this ^= (rhs - big1) / big2;
			*this *= *this;
			*this *= temp;
		}
		if (is_even(rhs))
			this->sign = true;
		return *this;
	}

	template <std::size_t k, typename T>
	inline BigNumber<k, T> BigNumber<k, T>::operator++(int)
	{
		BigNumber ret = *this;
		++*this;
		return ret;
	}

	template <std::size_t k, typename T>
	inline BigNumber<k, T> BigNumber<k, T>::operator--(int)
	{
		BigNumber ret = *this;
		--*this;
		return ret;
	}

	template <std::size_t k, typename T>
	BigNumber<k, T> BigNumber<k, T>::mult_digit(unsigned char n) const //n should belong to [0, 9]
	{
		if (n == 0)
			return big0;
		BigNumber ret = *this;
		ret.sign = true;
		unsigned char carry = 0;
		for (std::size_t i = 0; i < end; ++i)
		{
			ret.digits[i] = n * digits[i] + carry;
			carry = ret.digits[i] / 10;
			ret.digits[i] %= 10;
		}
		if (carry && ret.end < size)
		{
			ret.digits[ret.end] = carry;
			++ret.end;
		}
		return ret;
	}

	template <std::size_t k, typename T>
	inline void BigNumber<k, T>::discard_leading_zeros() //Assumption: end != 0
	{
		while (end != 0 && digits[end - 1] == 0)
			--end;
		if (end == 0)
		{
			++end;
			sign = true;
		}
	}

} //end of namespace Big_Number

namespace std
{
	template <size_t k, typename T>
	struct hash<Big_Number::BigNumber<k, T>>
	{
		typedef size_t result_type;
		typedef Big_Number::BigNumber<k, T> argument_type;

		size_t operator()(const Big_Number::BigNumber<k, T>&) const;
	};

	template <size_t k, typename T>
	size_t hash<Big_Number::BigNumber<k, T>>::operator()(const Big_Number::BigNumber<k, T>& number) const
	{
		ostringstream oss;
		oss << number;
		return hash<string>()(oss.str());
	}
}

#endif
