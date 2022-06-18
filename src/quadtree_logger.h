#ifndef ACA_QUADTREE_LOGGER_H
#define ACA_QUADTREE_LOGGER_H

#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <utility>

#include <spdlog/spdlog.h>
// ConsoleLogger
#include <spdlog/sinks/stdout_color_sinks.h>
// FileLogger
#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"

using color_t = uint8_t;

class QuadtreeLogger {
  public:
    virtual void print_depth(unsigned depth) = 0;
    virtual void print_i_j(unsigned i, unsigned j) = 0;
    virtual void print_n_rows_n_cols(unsigned n_rows, unsigned n_cols) = 0;
    virtual void print_mean(double r, double g, double b) = 0;
    virtual void print_sq_mean(double r, double g, double b) = 0;
    virtual void print_stdev(double r, double g, double b) = 0;
    virtual void print_split() = 0;
    virtual void print_leaf() = 0;
};

class QuadtreeSpdLogger : public QuadtreeLogger {
  public:
    explicit QuadtreeSpdLogger(std::shared_ptr<spdlog::logger> logger) : logger(std::move(logger)){};

    void print_depth(unsigned int depth) override {
        logger->info("depth: " + std::to_string(depth));
    }
    void print_i_j(unsigned int i, unsigned int j) override {
        logger->info("i: " + std::to_string(i) + ", j: " + std::to_string(j));
    }
    void print_n_rows_n_cols(unsigned int n_rows, unsigned int n_cols) override {
        logger->info("rows: " + std::to_string(n_rows) + ", cols: " + std::to_string(n_cols));
    }
    void print_mean(double r, double g, double b) override {
        logger->info("mean: " + std::to_string(r) + ' ' + std::to_string(+g) + ' ' + std::to_string(+b));
    }
    void print_sq_mean(double r, double g, double b) override {
        logger->info("sq_mean: " + std::to_string(r) + ' ' + std::to_string(g) + ' ' + std::to_string(b));
    }
    void print_stdev(double r, double g, double b) override {
        logger->info("stdev: " + std::to_string(r) + ' ' + std::to_string(g) + ' ' + std::to_string(b));
    }
    void print_split() override {
        logger->info("split");
    }
    void print_leaf() override {
        logger->info("leaf");
    }

  private:
    std::shared_ptr<spdlog::logger> logger;
};

class QuadtreeMuteLogger final : public QuadtreeLogger {
  public:
    QuadtreeMuteLogger() : QuadtreeLogger(){};

    void print_depth(unsigned int depth) override {
    }
    void print_i_j(unsigned int i, unsigned int j) override {
    }
    void print_n_rows_n_cols(unsigned int n_rows, unsigned int n_cols) override {
    }
    void print_mean(double r, double g, double b) override {
    }
    void print_sq_mean(double r, double g, double b) override {
    }
    void print_stdev(double r, double g, double b) override {
    }
    void print_split() override {
    }
    void print_leaf() override {
    }
};

/**
 * Multi-threaded logger that prints to stdout
 */
class QuadtreeConsoleLogger final : public QuadtreeSpdLogger {
  public:
    QuadtreeConsoleLogger()
        : QuadtreeSpdLogger(spdlog::stdout_color_mt("console_logger")) {}
};

/**
 * Multi-threaded, asynchronous logger that prints to file
 */
class QuadtreeFileLogger final : public QuadtreeSpdLogger {
  public:
    explicit QuadtreeFileLogger(const std::string& filename)
        : QuadtreeSpdLogger(spdlog::basic_logger_mt<spdlog::async_factory>("file_logger", filename)) {}
};

#endif // ACA_QUADTREE_LOGGER_H
