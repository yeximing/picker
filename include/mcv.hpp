
#pragma once

#include <bits/stdc++.h>
#include <sys/time.h>
#include <chrono>
#include <regex>
#include <set>
#include "codegen/cpp.hpp"
#include "parser/sv.hpp"

namespace mcv
{
    extern bool is_debug;
#define OUTPUT(o, fmt, ...)             \
    {                                   \
        fprintf(o, fmt, ##__VA_ARGS__); \
    }
#define MESSAGE(fmt, ...)                   \
    {                                       \
        OUTPUT(stdout, fmt, ##__VA_ARGS__); \
        OUTPUT(stdout, "%s\n", "");         \
    }
#define ERROR(fmt, ...)                    \
    {                                      \
        OUTPUT(stderr, fmt, ##__VA_ARGS__) \
    }
#define DEBUG(fmt, ...)                         \
    {                                           \
        if (is_debug)                           \
        {                                       \
            OUTPUT(stderr, "%s", "debug> ");    \
            OUTPUT(stderr, fmt, ##__VA_ARGS__); \
            OUTPUT(stderr, "%s\n", "")          \
        }                                       \
    }
#define FATAL(fmt, ...)                     \
    {                                       \
        OUTPUT(stderr, fmt, ##__VA_ARGS__); \
        exit(-1);                           \
    }

    inline void vassert(bool c, std::string msg = "")
    {
        if (!c)
        {
            FATAL("Assert error: %s\n", msg.c_str());
        }
    }

    inline long vtime()
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec * 1000000 + tv.tv_usec;
    }

    inline std::string fmtnow(std::string fmt = "%Y-%m-%d %H:%M:%S")
    {
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        std::stringstream buffer;
        buffer << std::put_time(&tm, fmt.c_str());
        return buffer.str();
    }

    inline std::string exec(const char *cmd)
    {
        std::array<char, 128> buffer;
        std::string result;
        std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
        if (!pipe)
        {
            throw std::runtime_error("popen() failed!");
        }
        while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr)
        {
            result += buffer.data();
        }
        return result;
    }

    inline std::string trim(std::string s, std::string p)
    {
        if (s.empty())
        {
            return s;
        }
        s.erase(0, s.find_first_not_of(p));
        s.erase(s.find_last_not_of(p) + p.size());
        return s;
    }

    inline std::string trim(std::string s)
    {
        if (s.empty())
        {
            return s;
        }
        s.erase(0, s.find_first_not_of(" "));
        s.erase(s.find_last_not_of(" ") + 1);
        s.erase(0, s.find_first_not_of("\r"));
        s.erase(s.find_last_not_of("\r") + 1);
        s.erase(0, s.find_first_not_of("\n"));
        s.erase(s.find_last_not_of("\n") + 1);
        return s;
    }

    inline std::vector<std::string> strsplit(std::string str, std::string s = " ")
    {
        std::vector<std::string> ret;
        int start = 0;
        int end = str.find(s);
        while (end != -1)
        {
            auto sub = str.substr(start, end - start);
            trim(sub);
            ret.push_back(sub);
            start = end + s.size();
            end = str.find(s, start);
        }
        auto sub = str.substr(start, end - start);
        trim(sub);
        ret.push_back(sub);
        return ret;
    }

    inline std::string strsplit(std::string str, std::initializer_list<std::string> s, bool front = true)
    {
        auto ret = str;
        for (auto &v : s)
        {
            if (front)
            {
                ret = strsplit(ret, v).front();
            }
            else
            {
                ret = strsplit(ret, v).back();
            }
        }
        return ret;
    }

    inline bool strconatins(std::string str, std::string subs)
    {
        return str.find(subs) != std::string::npos;
    }

    inline std::string streplace(std::string str, std::string s, std::string t = "")
    {
        std::string input = str;
        size_t index = 0;
        while (true)
        {
            index = input.find(s, index);
            if (index == std::string::npos)
                break;
            input.replace(index, s.length(), t);
            index += t.length();
        }
        return input;
    }

    inline std::string streplace(std::string str, std::initializer_list<std::string> s, std::string t = "")
    {
        auto ret = str;
        for (auto &v : s)
        {
            ret = streplace(ret, v, t);
        }
        return ret;
    }

    template <typename T>
    inline bool contians(std::vector<T> v, T a)
    {
        for (T x : v)
        {
            if (x == a)
                return true;
        }
        return false;
    }

    inline bool file_exists(std::string f)
    {
        return access(f.c_str(), F_OK) == 0;
    }

    inline std::string exec_result(std::string cmd, int index)
    {
        auto result = strsplit(exec(cmd.c_str()));
        if (result.size() <= index)
        {
            return "";
        }
        return result[index];
    }

    inline std::string lower_case(std::string input)
    {
        auto str = input;
        std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c)
                       { return std::tolower(c); });
        return str;
    }

    inline std::string first_upercase(std::string input)
    {
        std::string ret = input;
        ret[0] = std::toupper(input[0]);
        return ret;
    }

    inline std::string base_name(std::string const &path, bool with_subfix = true)
    {
        auto fname = path.substr(path.find_last_of("/\\") + 1);
        if (with_subfix)
        {
            return fname;
        }
        return strsplit(fname, ".")[0];
    }

    inline std::string get_path(std::string str)
    {
        auto found = str.find_last_of("/\\");
        return str.substr(0, found);
    }

    inline std::string path_join(std::initializer_list<std::string> args)
    {
        std::string path, value;
        auto size = args.size();
        for (auto p : args)
        {
            if (path.empty())
            {
                path = p;
                path.erase(path.find_last_not_of("/\\") + 1);
                continue;
            }
            auto v = trim(p, "/");
            v = trim(v, "\\");
            path.append("/" + v);
        }
        return path;
    }

    inline std::string current_path()
    {
        return std::string(get_current_dir_name());
    }

    template <typename... Args>
    std::string sfmt(const std::string &format, Args... args)
    {
        int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
        if (size_s <= 0)
        {
            throw std::runtime_error("Error during formatting.");
        }
        auto size = static_cast<size_t>(size_s);
        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, format.c_str(), args...);
        return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    }

    inline std::string version()
    {
        return "v0.1";
    }

    inline std::string find_file(std::vector<std::string> fs)
    {
        for (auto f : fs)
        {
            if (file_exists(f))
            {
                DEBUG("find file: %s", f.c_str());
                return f;
            }
        }
        vassert(false, "search file fail");
        return "";
    }

    inline std::string mcv_file(std::string fname)
    {
        std::vector<std::string> search_path{".", "~/.mcv", "/etc/mcv"};
        std::vector<std::string> search_file;
        for (auto &p : search_path)
        {
            search_file.push_back(path_join({p, fname}));
        }
        return find_file(search_file);
    }

    inline std::string template_rander(std::string tmp_file, std::map<std::string, std::string> &kw)
    {
        inja::Environment env;
        kw["mcv_version"] = version();
        kw["mcv_time"] = fmtnow();
        return env.render_file(tmp_file, kw);
    }

    inline bool write_file(std::string fname, std::string text);
    inline bool write_file(std::string fname, std::string text)
    {
        MESSAGE("Gen file: %s", fname.c_str());
        std::ofstream ofile(fname, std::ios::trunc);
        vassert(ofile.is_open(), "write file(" + fname + ") error");
        ofile << text;
        ofile.close();
        return true;
    }

    inline std::string read_params(std::string fname);
    inline std::string read_params(std::string fname)
    {
        MESSAGE("Read params from: %s", fname.c_str());
        std::ifstream ifile(fname);
        std::string ret;
        if (ifile.is_open())
        {
            std::string line_txt;
            while (std::getline(ifile, line_txt))
            {
                line_txt = trim(strsplit(line_txt, "#").front());
                if (line_txt.empty())
                {
                    continue;
                }
                if (ret.empty())
                {
                    ret = line_txt;
                }
                else
                {
                    ret = ret + " " + line_txt;
                }
            }
        }
        return ret;
    };

    // constant
    const std::string CFG_VERILATOR_INCLUDE = "CFG_VERILATOR_INCLUDE";
    const std::string CFG_VERILATOR_VERSION = "CFG_VERILATOR_VERSION";
    const std::string CFG_MODEL_NAME = "CFG_MODEL_NAME";
    const std::string CFG_CPP_FLAGS = "CFG_CPP_FLAGS";
    const std::string CFG_VERILOG_IO_VARS = "CFG_VERILOG_IO_VARS";

    // struct
    struct CMember
    {
        std::string pclass;
        std::string type; // inc, func, class or var
        std::string name;
        std::string define;
    };

    // export functions
    // gen python interface
    void gen_python(inja::json &cfg, std::vector<CMember> &var_and_fucs, std::string target_dir);
    void gen_java(inja::json &cfg, std::vector<CMember> &var_and_fucs, std::string target_dir);
    void gen_golang(inja::json &cfg, std::vector<CMember> &var_and_fucs, std::string target_dir);
    void gen_cpp(inja::json &cfg, std::vector<CMember> &var_and_fucs, std::string target_dir);

    void print_cmembers(std::ostream &out, std::vector<CMember> &member);
    std::map<std::string, std::string> get_verilog_inoutput_type(std::string fname);
    std::map<std::string, std::string> gen_cpp_so(inja::json &cfg, std::vector<mcv::CMember> &var_and_fucs,
                                                  std::string target_dir, std::string &subdir);

}