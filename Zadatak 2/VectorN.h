#pragma once
#include <iostream>
#include <functional>

namespace Vector {

	class ComparableVectorN;
	class VectorSetN;
	class OrderedVectorSetN;

	class VectorN {

		double* elements{ nullptr };
		size_t dim{ 0 };

	public:
		VectorN(double); 
		VectorN(size_t, double);
		explicit VectorN(size_t n = 1); 
		VectorN(const std::initializer_list<double>);
		VectorN(const VectorN&);
		VectorN& operator=(const VectorN&);
		VectorN(VectorN&&) noexcept;
		VectorN& operator=(VectorN&&) noexcept;
		~VectorN();

		const double& operator[](int) const;
		size_t dimension() const;

		static VectorN zeroVector(size_t);
		bool isZeroVector() const;

		double operator()(double) const;

		operator ComparableVectorN() const;
		operator VectorSetN() const;

		friend bool operator!=(const VectorN&, const VectorN&);
		friend bool operator==(const VectorN&, const VectorN&);

		friend VectorN operator+(const VectorN&, const VectorN&);  
		friend VectorN operator-(const VectorN&, const VectorN&);   
		VectorN operator*(double) const;                            
		friend double operator*(const VectorN&, const VectorN&);   
		friend VectorN operator-(const VectorN&);                   

		
		VectorN& operator+=(const VectorN&);                       
		VectorN& operator-=(const VectorN&);                      

		static double errorValue;
		constexpr static double ERROR_RETURNED_ELEMENT_VALUE = NAN;

	private:
		void copyVectorN(const VectorN&);
		void moveVectorN(VectorN&&);

		double& setElementAt(int);
		const double& getElementAt(int index) const;

		friend std::ostream& operator<<(std::ostream&, const VectorN&);  
		friend std::istream& operator>>(std::istream&, VectorN&);      

		bool validIndexRange(size_t index) const { return (index < 0 || index >= dim) ? false : true; }
		bool equalDimensions(int otherDim) const { return dim == otherDim; }
		VectorN calculate(const VectorN&, const std::function<double(double, double)>&) const;

		const std::function<double(double, double)> add = [](double i, double j) {return i + j; };
		const std::function<double(double, double)> sub = [](double i, double j) {return i - j; };
		const std::function<double(double, double)> mul = [](double i, double j) {return i * j; };
		const std::function<double(double, double)> div = [](double i, double j) {return i / j; };

		static double EuclidDistance(const VectorN&);
		double sumOfElements() const;

		friend double VectorNSum(const VectorN&);
		friend ComparableVectorN;
		friend OrderedVectorSetN;
	};
}