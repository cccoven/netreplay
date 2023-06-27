#include <iostream>
#include <vector>
#include <thread>

#include "output_http.h"
#include "../util/util.hpp"

namespace net = boost::asio;
using tcp = net::ip::tcp;

OutputHttp::OutputHttp(const std::string &addr, const output_http_config &conf) {
    config = conf;
    config.url = addr;

    for (int i = 0; i < config.works; ++i) {
        std::thread t(&OutputHttp::start_worker, this);
        t.detach();
    }
}

void OutputHttp::start_worker() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [this] { return !messages.empty(); });

        auto msg = messages.front();
        messages.pop_front();
        lock.unlock();

        send_request(msg);
    }
}

void OutputHttp::write(std::shared_ptr<RawMessage> &msg) {
    std::unique_lock<std::mutex> lock(mtx);
    messages.push_back(msg);
    lock.unlock();
    cv.notify_one();
}

http::request<http::dynamic_body> OutputHttp::parse_request(std::vector<unsigned char> &buffer) {
    beast::error_code ec;
    http::request_parser<http::dynamic_body> parser;
    parser.eager();
    parser.put(boost::asio::buffer(buffer.data(), buffer.size()), ec);

    http::request<http::dynamic_body> request = parser.get();
    return request;
}

void OutputHttp::send_request(std::shared_ptr<RawMessage> &msg) {
    http::request<http::dynamic_body> request = parse_request(msg->data);

    net_url url = Util::parse_url(config.url);
    std::pair<std::string, std::uint16_t> hp = Util::split_host_port(url.host);
    std::string host = hp.first;
    std::uint16_t port = hp.second;
    if (port == 0) {
        port = 80;
    }
    request.base().set(http::field::host, url.host);

    net::io_context ioc;
    tcp::resolver resolver(ioc);
    beast::tcp_stream stream(ioc);
    auto const results = resolver.resolve(host, std::to_string(port));
    stream.connect(results);

    http::request<http::dynamic_body> req(std::move(request));
    http::write(stream, req);

    beast::flat_buffer buffer;
    http::response<http::dynamic_body> res;
    http::read(stream, buffer, res);

    beast::error_code rec;
    stream.socket().shutdown(tcp::socket::shutdown_both, rec);
}
