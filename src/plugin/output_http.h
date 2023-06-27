#ifndef NETREPLAY_OUTPUT_HTTP_H
#define NETREPLAY_OUTPUT_HTTP_H

#include <mutex>
#include <condition_variable>
#include <boost/beast.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include "plugin_chain.h"

namespace beast = boost::beast;
namespace http = beast::http;

class OutputHttp : public OutPlugin {
public:
    OutputHttp(const std::string &addr, const output_http_config &config);

    ~OutputHttp() = default;

    void write(std::shared_ptr<RawMessage> &msg) override;

private:
    void start_worker();
    
    void send_request(std::shared_ptr<RawMessage> &msg);

    http::request<http::dynamic_body> parse_request(std::vector<unsigned char> &buffer);

private:
    output_http_config config;
    std::deque<std::shared_ptr<RawMessage>> messages;
    std::mutex mtx;
    std::condition_variable cv;
};

#endif
