// 1.0

#ifndef CHARTS_H
#define CHARTS_H

#include <filesystem>
#include <iostream>
#include <fstream>
// #include <cstdlib>
#include <map>

struct chartPlotter {
    std::string files_name = "chart";
    std::string title = "chart";
    std::string x_label = "x_label";
    std::string y_label = "y_label";
    std::string marker = "points"; // "lines"
    bool grid = true;
    int png_x_size = 800;
    int png_y_size = 600;

    explicit chartPlotter(std::string directory = "") {
        if (directory == "") {
            main_dir = std::string(std::filesystem::current_path()).append("/charts/");
            system(std::string("mkdir -p " + main_dir).c_str());
        } else {
            if (directory.back() != '/')
                directory.append("/");
            main_dir = directory;
        }
        for (const auto &[s, var]: {
                 std::pair("dat/", &dat_dir), std::pair("plt/", &plt_dir), std::pair("png/", &png_dir)
             }) {
            *var = main_dir + s;
            system(std::string("mkdir -p " + *var).c_str());
        }
    }

    template<typename key_type, typename value_type>
    void plot(const std::map<key_type, value_type> &data, const std::string &estimated_complexity = "") {
        const std::string data_file = dat_dir + files_name + ".dat";
        const std::string script_file = plt_dir + files_name + ".plt";
        const std::string image_file = png_dir + files_name + ".png";
        save_results_to_file(data_file, data);
        generate_gnuplot_script(script_file, data_file, image_file, estimated_complexity);
        execute_gnuplot_script(script_file);
    }

    void set_x_range_min(const float x_min) {
        x_range_min = x_min;
    }

    void set_x_range_max(const float x_max) {
        x_range_max = x_max;
    }

    void set_y_range_min(const float y_min) {
        y_range_min = y_min;
    }

    void set_y_range_max(const float y_max) {
        y_range_max = y_max;
    }

private:
    std::string main_dir;
    std::string dat_dir;
    std::string plt_dir;
    std::string png_dir;
    std::string x_range_min = "";
    std::string x_range_max = "";
    std::string y_range_min = "";
    std::string y_range_max = "";

    template<typename key_type, typename value_type>
    void save_results_to_file(const std::string &filename, const std::map<key_type, value_type> &data) {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Error: Could not open file: " << filename << std::endl;
            return;
        }
        for (const auto &[size, time]: data) {
            file << size << " " << time << std::endl;
        }
        file.close();
    }

    static std::string generate_function_to_fit(const std::string &complexity) {
        std::string function;
        if (complexity == "n^2")
            function += "f(x)=a*x**2+b\n";
        else if (complexity == "n" or complexity == "1")
            function += "f(x)=a*x+b\n";
        else
            return "";
        function += "a = 2.0\n";
        function += "b = 5.0\n";
        return function;
    }


    void generate_gnuplot_script(const std::string &script_name, const std::string &data_file,
                                 const std::string &output_file, const std::string &estimated_complexity) const {
        std::ofstream script(script_name);
        script << "set terminal png size " << png_x_size << "," << png_y_size << "\n";
        script << "set output '" << output_file << "'\n";
        script << "set title '" << title << "'\n";
        script << "set xlabel '" << x_label << "'\n";
        script << "set ylabel '" << y_label << "'\n";
        script << "set xrange[" << x_range_min << ":" << x_range_max << "]\n";
        script << "set yrange[" << y_range_min << ":" << y_range_max << "]\n";
        if (grid) script << "set grid\n";

        if (const std::string function_to_fit = generate_function_to_fit(estimated_complexity); function_to_fit != "") {
            script << function_to_fit << "\n";
            script << "fit f(x) '" << data_file << "' using 1:2 via a,b\n";
            script << "plot '" << data_file << "' using 1:2 with points title 'Data', \\\n"
                    << "     f(x) with lines title 'Fit'\n";
        } else {
            script << "plot '" << data_file << "' using 1:2 with " << marker << " title 'Data'\n";
        }
        script.close();
    }

    static void execute_gnuplot_script(const std::string &script_file) {
        const std::string command = "gnuplot " + script_file;
        system(command.c_str());
    }
};

#endif //CHARTS_H
