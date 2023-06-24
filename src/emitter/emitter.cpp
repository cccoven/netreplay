#include <iostream>
#include <thread>

#include "emitter.h"

Emitter::Emitter(PluginChain &chain) : plugin_chain(chain) {}

void Emitter::start() {
    for (auto &in: plugin_chain.inputs) {
        std::thread t([&] {
            deliver_multiple(in, plugin_chain.outputs);
        });
        t.detach();
    }
}

void Emitter::deliver_multiple(std::shared_ptr<InPlugin> &in, std::vector<std::shared_ptr<OutPlugin>> &outputs) {
    while (true) {
        RawMessage msg = in->read();
        std::cout << "msg.data.size = " << msg.data.size() << std::endl;

        for (auto &out: outputs) {
            out->write(msg);
        }
    }
}

void Emitter::close() {

}
