#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <memory>

class Curve {
public:
	virtual ~Curve() = default;
	virtual double getRadius() const = 0;
	virtual void calculatePointAndDerivative(double t, double& x, double& y, double& z, double& dx, double& dy, double& dz) const = 0;
};

class Circle : public Curve {
private:
	double radius;

public:
	Circle(double r) : radius(r) {
		if (radius <= 0) {
			throw std::invalid_argument("Radius must be positive");
		}
	}

	double getRadius() const override {
		return radius;
	}

	void calculatePointAndDerivative(double t, double& x, double& y, double& z, double& dx, double& dy, double& dz) const override {
		x = radius * std::cos(t);
		y = radius * std::sin(t);
		z = 0;
		dx = -radius * std::sin(t);
		dy = radius * std::cos(t);
		dz = 0;
	}
};

class Ellipse : public Curve {
private:
	double RadiusX;
	double RadiusY;

public:
	Ellipse(double RX, double RY) : RadiusX(RX), RadiusY(RY) {
		if (RadiusX <= 0 || RadiusY <= 0) {
			throw std::invalid_argument("Radius must be positive");
		}
	}

	double getRadius() const override {
		return std::sqrt((RadiusX * RadiusX + RadiusY * RadiusY) / 2);
	}

	void calculatePointAndDerivative(double t, double& x, double& y, double& z, double& dx, double& dy, double& dz) const override {
		x = RadiusX * std::cos(t);
		y = RadiusY * std::sin(t);
		z = 0;
		dx = -RadiusX * std::sin(t);
		dy = RadiusY * std::cos(t);
		dz = 0;
	}
};

class Spiral : public Curve {
private:
	double radius;
	double height;

public:
	Spiral(double r, double h): radius(r), height(h) {
		if (radius <= 0) {
			throw std::invalid_argument("Radius must be positive");
		}
	}

	double getRadius() const override {
		return radius;
	}

	void calculatePointAndDerivative(double t, double& x, double& y, double& z, double& dx, double& dy, double& dz) const override {
		double teta = t * 2 * atan(1) * 4;
		x = radius * std::cos(teta);
		y = radius * std::sin(teta);
		z = height * t;
		dx = -radius * std::sin(teta);
		dy = radius * std::cos(teta);
		dz = height;
	}
};

int main() {
	std::vector<std::unique_ptr<Curve>>curves;
	std::vector<Circle*> circles;

	curves.push_back(std::make_unique<Circle>(5.0));
	curves.push_back(std::make_unique<Ellipse>(10.0, 25.0));
	curves.push_back(std::make_unique<Spiral>(15.0, 3.0));

	double t = atan(1) * 4 / 4;
	std::cout << "Coordinates and Derivatives at t = " << t << std::endl << std::endl;
	for (const auto& curve : curves) {
		double x, y, z, dx, dy, dz;
		curve->calculatePointAndDerivative(t, x, y, z, dx, dy, dz);
		std::cout << "Point: (" << x << "," << y << "," << z << ")" << std::endl;
		std::cout << "Derivative: (" << dx << "," << dy << "," << dz << ")" << std::endl << std::endl;
	}
	
	for (const auto& curve : curves) {
		if (const auto circle = dynamic_cast <Circle*>(curve.get())) {
			circles.push_back(circle);
		}
	}

	std::cout << "Sorted circles by radius:" << std::endl;
	std::sort(curves.begin(), curves.end(), [](const std::unique_ptr<Curve>& c1, const std::unique_ptr<Curve>& c2) {
		return c1->getRadius() < c2->getRadius();
		});
	for (const auto& curve : curves) {
		if (const auto circle = dynamic_cast<Circle*>(curve.get())) {
			std::cout << "Radius: " << circle->getRadius() << std::endl;
		}
	}

	double sumofradius = 0.0;
	for (const auto& curve : curves) {
		if (const auto circle = dynamic_cast<Circle*>(curve.get())) {
			sumofradius += circle->getRadius();
		}
	}
	std::cout << "Sum of radius: " << sumofradius << std::endl;

	return 0;
}
