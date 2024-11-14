#include <gnuplot-iostream.h>

#include <ALFI.h>

struct PointLine {
	std::string title;
	alfi::dist::Base<double> x_points;
};

void plot_data(const std::vector<PointLine>& lines) {
	static const std::vector<std::string> colors = {
		"blue",        // blue        #0000ff =   0   0 255
		"red",         // red         #ff0000 = 255   0   0
		"green",       // green       #00ff00 =   0 255   0
		"orange",      // orange      #ffa500 = 255 165   0
		"purple",      // purple      #c080ff = 192 128 255
		"brown",       // brown       #a52a2a = 165  42  42
		"magenta",     // magenta     #ff00ff = 255   0 255
		"dark-blue",   // dark-blue   #00008b =   0   0 139
		"coral",       // coral       #ff7f50 = 255 127  80
		"dark-violet", // dark-violet #9400d3 = 148   0 211
		"salmon",      // salmon      #fa8072 = 250 128 114
		"dark-red",    // dark-red    #8b0000 = 139   0   0
		"web-blue",    // web-blue    #0080ff =   0 128 255
		"web-green",   // web-green   #00c000 =   0 192   0
		"dark-orange"  // dark-orange #c04000 = 192  64   0
	};

	const double upper_y = 0;
	const double lower_y = -static_cast<double>(lines.size()) - 1;

	Gnuplot gp;

	gp << "set terminal wxt size 1100,600\n";

	gp << "set xrange [0:1]\n";
	gp << "set yrange [" << lower_y << ":" << upper_y << "]\n";

	gp << "plot ";
	for (size_t i = 0; i < lines.size(); ++i) {
		if (i > 0)
			gp << ", ";
		gp << "'-' with points pt 7 ps 1.5 lc rgb '"
			<< colors[i % colors.size()] << "' title '"
			<< lines[i].title << "'";
	}
	gp << '\n';

	double current_layer = upper_y - 1;
	for (const auto& line : lines) {
		for (const auto& x : line.x_points) {
			gp << x << " " << current_layer << "\n";
		}
		gp << "e\n";
		--current_layer;
	}
}

int main() {
	const size_t n = 9;
	const double a = 0;
	const double b = 1;

	const double ratio = 2;
	const double sigmoid_steepness = 8;
	const double erf_steepness = 3;

	// ReSharper disable CppPossiblyUnintendedObjectSlicing
	plot_data({
		{"Uniform", alfi::dist::Uniform<>(n, a, b)},
		{"Chebyshev", alfi::dist::Chebyshev<>(n, a, b)},
		{"Stretched Chebyshev", alfi::dist::ChebyshevStretched<>(n, a, b)},
		{"Chebyshev Ellipse", alfi::dist::ChebyshevEllipse<>(n, a, b, ratio)},
		{"Stretched Chebyshev Ellipse", alfi::dist::ChebyshevEllipseStretched<>(n, a, b, ratio)},
		{"Circle Projection", alfi::dist::CircleProj<>(n, a, b)},
		{"Ellipse Projection", alfi::dist::EllipseProj<>(n, a, b, ratio)},
		{"Sigmoid", alfi::dist::Sigmoid<>(n, a, b, sigmoid_steepness)},
		{"Stretched Sigmoid", alfi::dist::SigmoidStretched<>(n, a, b, sigmoid_steepness)},
		{"Error function", alfi::dist::Erf<>(n, a, b, erf_steepness)},
		{"Stretched Error function", alfi::dist::ErfStretched<>(n, a, b, erf_steepness)},
	});

	return 0;
}