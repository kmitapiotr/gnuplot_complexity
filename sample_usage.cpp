#include <map>
#include <chrono>
#include "chartPlotter.h"

#define MAX_SIZE 3001
#define SIZE_STEP 100
#define SAMPLES 8


using namespace std;

template<typename Func>
long measure_time(Func func) {
    const auto beg = chrono::high_resolution_clock::now();
    func();
    const auto end = chrono::high_resolution_clock::now();
    return chrono::duration_cast<chrono::nanoseconds>(end - beg).count();
}

int o_n(const int n) {
    int sum = 0;
    for (int i = 0; i < n; ++i) {
        if (i % 2 == 0) {
            sum += i;
        } else {
            sum -= i;
        }
    }
    return sum;
}

int o_n_2(const int n) {
    int sum = 0;
    for (int _ = 0; _ < n; ++_) {
        for (int i = 0; i < n; ++i) {
            if (i % 2 == 0) {
                sum += i;
            } else {
                sum -= i;
            }
        }
    }
    return sum;
}

int main() {

    map<int, double> o_n_times;
    map<int, double> o_n_2_times;

    for (int n = 0; n < MAX_SIZE; ++n) {
        for (int _ = 0; _ < SAMPLES; _++){
            o_n_times[n] += measure_time([&] { o_n(n); }) / SAMPLES;
            o_n_2_times[n] += measure_time([&] { o_n_2(n); }) / SAMPLES;}
    }

    chartPlotter plotter("./sample_charts");
    plotter.files_name = "o_n_times";
    plotter.title = "o(n)";
    plotter.x_label = "dataset size";
    plotter.y_label = "avg. runtime";
    plotter.marker = "points";
    plotter.grid = false;
    plotter.plot(o_n_times);

    plotter.files_name = "o_n_2_times";
    plotter.title = "o(n^2)";
    // plotter.marker = "points"; // does not matter when estimated_complexity is stated
    plotter.grid = true;
    plotter.plot(o_n_2_times, "n^2");

    // also available:
    // plotter.png_x_size =  ;
    // plotter.png_y_size =  ;
    // plotter.set_x_range_min( );
    // plotter.set_x_range_max( );
    // plotter.set_y_range_min( );
    // plotter.set_y_range_max( );
}
